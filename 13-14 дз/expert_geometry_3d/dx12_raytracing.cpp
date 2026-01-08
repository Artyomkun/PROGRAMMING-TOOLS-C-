#include "dx12_raytracing.hpp"
#include "geometry3d.hpp"
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <d3d12shader.h>
#include <dxgi1_6.h>
#include <stdexcept>
#include <dxcapi.h>
#include <iostream>
#include <fstream>
#include <d3d12.h>
#include <vector>
#include <string>
#include <wrl.h>

#define ThrowIfFailed(x) \
    { \
        HRESULT hr__ = (x); \
        if(FAILED(hr__)) { \
            throw std::runtime_error("DirectX error"); \
        } \
    }

UINT safe_cast(size_t value) {
    if (value > UINT_MAX) {
        throw std::runtime_error("Value too large for UINT");
    }
    return static_cast<UINT>(value);
}

struct SceneConstantBuffer
{
    DirectX::XMFLOAT4X4 projectionToWorld;
    DirectX::XMFLOAT4 cameraPosition;
    DirectX::XMFLOAT4 lightPosition;
    DirectX::XMFLOAT4 lightAmbientColor;
    DirectX::XMFLOAT4 lightDiffuseColor;
};

DX12RayTracing::DX12RayTracing(
    HWND hwnd, 
    UINT width, 
    UINT height
): 
    hwnd_(hwnd), 
    width_(width), 
    height_(height), 
    frameIndex_(0), 
    fenceValue_(0), 
    fps_(0.0), 
    frameCount_(0)
{
    lastTime_ = std::chrono::high_resolution_clock::now();
    startTime_ = std::chrono::high_resolution_clock::now();
}

DX12RayTracing::~DX12RayTracing()
{
    WaitForPreviousFrame();
}

void DX12RayTracing::Initialize()
{
    std::cout << "Введите координаты трех вершин треугольника (x y z для каждой):\n";
    std::cout << "Вершина 1: ";
    double x1, y1, z1;
    std::cin >> x1 >> y1 >> z1;
    vertex1_ = Geometry3D::Point<double, 3>(x1, y1, z1);

    std::cout << "Вершина 2: ";
    double x2, y2, z2;
    std::cin >> x2 >> y2 >> z2;
    vertex2_ = Geometry3D::Point<double, 3>(x2, y2, z2);
    std::cout << "Вершина 3: ";
    double x3, y3, z3;
    std::cin >> x3 >> y3 >> z3;
    vertex3_ = Geometry3D::Point<double, 3>(x3, y3, z3);

    CreateDevice();
    CreateCommandAllocator();
    CreateCommandList();
    CreateFence();
    CreateCommandQueue();
    CreateSwapChain();
    CreateDescriptorHeaps();
    CreateRTV();
    CreateDSV();
    CreateRootSignature();
    CreatePipelineState();
    CreateAccelerationStructures();
    CreateShaderTables();
    CreateConstantBuffer();
    LoadShaders();
}

void DX12RayTracing::GetHardwareAdapter(
    IDXGIFactory1* pFactory, 
    IDXGIAdapter1** ppAdapter
)
{
    *ppAdapter = nullptr;
    for (UINT adapterIndex = 0; ; ++adapterIndex)
    {
        IDXGIAdapter1* pAdapter = nullptr;
        if (DXGI_ERROR_NOT_FOUND == pFactory->EnumAdapters1(
            adapterIndex, 
            &pAdapter)
        )
        {
            break;
        }

        if (
            SUCCEEDED(
                D3D12CreateDevice(
                    pAdapter, 
                    D3D_FEATURE_LEVEL_12_0, 
                    __uuidof(ID3D12Device), 
                    nullptr
                )
            )
        )
        {
            *ppAdapter = pAdapter;
            return;
        }
        pAdapter->Release();
    }
}

void DX12RayTracing::CreateDevice()
{
    UINT dxgiFactoryFlags = 0;

    #if defined(_DEBUG)
        ComPtr<ID3D12Debug> debugController;
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
        {
            debugController->EnableDebugLayer();
            dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
        }
    #endif

    ComPtr<IDXGIFactory4> factory;
    ThrowIfFailed(
        CreateDXGIFactory2(
            dxgiFactoryFlags, 
            IID_PPV_ARGS(
                &factory
            )
        )
    );

    ComPtr<IDXGIAdapter1> hardwareAdapter;
    GetHardwareAdapter(
        factory.Get(), 
        &hardwareAdapter
    );

    ThrowIfFailed(
        D3D12CreateDevice(
            hardwareAdapter.Get(),
            D3D_FEATURE_LEVEL_12_0,
            IID_PPV_ARGS(
                &device_
            )
        )
    );

    D3D12_FEATURE_DATA_D3D12_OPTIONS5 options5 = {};

    ThrowIfFailed(
        device_->CheckFeatureSupport(
            D3D12_FEATURE_D3D12_OPTIONS5, 
            &options5, 
            sizeof(options5)
        )
    );
    if (options5.RaytracingTier < D3D12_RAYTRACING_TIER_1_0)
    {
        throw std::runtime_error("Ray tracing не поддерживается на этом устройстве");
    }
}

void DX12RayTracing::CreateCommandQueue()
{
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    ThrowIfFailed(
        device_->CreateCommandQueue(
            &queueDesc, 
            IID_PPV_ARGS(
                &commandQueue_
            )
        )
    );
}

void DX12RayTracing::CreateSwapChain()
{
    ComPtr<IDXGIFactory4> factory;
    ThrowIfFailed(
        CreateDXGIFactory1(
            IID_PPV_ARGS(
                &factory
            )
        )
    );

    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.BufferCount = FrameCount;
    swapChainDesc.Width = width_;
    swapChainDesc.Height = height_;
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.SampleDesc.Count = 1;

    ComPtr<IDXGISwapChain1> swapChain;
    ThrowIfFailed(
        factory->CreateSwapChainForHwnd(
            commandQueue_.Get(),
            hwnd_,
            &swapChainDesc,
            nullptr,
            nullptr,
            &swapChain
        )
    );

    ThrowIfFailed(
        swapChain.As(
            &swapChain_
        )
    );

    frameIndex_ = swapChain_->GetCurrentBackBufferIndex();
}

void DX12RayTracing::CreateDescriptorHeaps()
{
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
    rtvHeapDesc.NumDescriptors = FrameCount;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

    ThrowIfFailed(
        device_->CreateDescriptorHeap(
            &rtvHeapDesc, 
            IID_PPV_ARGS(
                &rtvHeap_
            )
        )
    );

    D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
    dsvHeapDesc.NumDescriptors = 1;
    dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

    ThrowIfFailed(
        device_->CreateDescriptorHeap(
            &dsvHeapDesc, 
            IID_PPV_ARGS(
                &dsvHeap_
            )
        )
    );

    D3D12_DESCRIPTOR_HEAP_DESC srvUavHeapDesc = {};
    srvUavHeapDesc.NumDescriptors = 3;
    srvUavHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    srvUavHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

    ThrowIfFailed(
        device_->CreateDescriptorHeap(
            &srvUavHeapDesc, 
            IID_PPV_ARGS(
                &srvUavHeap_
            )
        )
    );
}

void DX12RayTracing::CreateRTV()
{
    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap_->GetCPUDescriptorHandleForHeapStart();

    for (
        UINT n = 0; 
        n < FrameCount; n++
    )
    {
        ThrowIfFailed(
            swapChain_->GetBuffer(
                n, 
                IID_PPV_ARGS(
                    &renderTargets_[n]
                )
            )
        );

        device_->CreateRenderTargetView(
            renderTargets_[n].Get(), 
            nullptr,
            rtvHandle
        );

        rtvHandle.ptr += device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    }
}

void DX12RayTracing::CreateDSV()
{
    D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilDesc = {};
    depthStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
    depthStencilDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
    depthStencilDesc.Flags = D3D12_DSV_FLAG_NONE;
    D3D12_CLEAR_VALUE depthOptimizedClearValue = {};
    depthOptimizedClearValue.Format = DXGI_FORMAT_D32_FLOAT;
    depthOptimizedClearValue.DepthStencil.Depth = 1.0f;
    depthOptimizedClearValue.DepthStencil.Stencil = 0;

    D3D12_HEAP_PROPERTIES heapProps = { 
        D3D12_HEAP_TYPE_DEFAULT, 
        D3D12_CPU_PAGE_PROPERTY_UNKNOWN, 
        D3D12_MEMORY_POOL_UNKNOWN, 
        1, 
        1 
    };

    D3D12_RESOURCE_DESC resourceDesc = { 
        D3D12_RESOURCE_DIMENSION_TEXTURE2D, 
        0, 
        (UINT64)width_, 
        height_, 1, 1, 
        DXGI_FORMAT_D32_FLOAT, 
        {
            1,
            0
        }, 
        D3D12_TEXTURE_LAYOUT_UNKNOWN, 
        D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL 
    };

    ThrowIfFailed(
        device_->CreateCommittedResource(
            &heapProps,
            D3D12_HEAP_FLAG_NONE,
            &resourceDesc,
            D3D12_RESOURCE_STATE_DEPTH_WRITE,
            &depthOptimizedClearValue,
            IID_PPV_ARGS(
                &depthStencil_
            )
        )
    );

    device_->CreateDepthStencilView(
        depthStencil_.Get(), 
        &depthStencilDesc, 
        dsvHeap_->GetCPUDescriptorHandleForHeapStart()
    );
}

void DX12RayTracing::CreateCommandAllocator()
{
    ThrowIfFailed(
        device_->CreateCommandAllocator(
            D3D12_COMMAND_LIST_TYPE_DIRECT, 
            IID_PPV_ARGS(
                &commandAllocator_
            )
        )
    );
}

void DX12RayTracing::CreateCommandList()
{
    ThrowIfFailed(
        device_->CreateCommandList(
            0, 
            D3D12_COMMAND_LIST_TYPE_DIRECT, 
            commandAllocator_.Get(), 
            nullptr, 
            IID_PPV_ARGS(
                &commandList_
            )
        )
    );

    commandList_->Close();
}

void DX12RayTracing::CreateFence()
{
    ThrowIfFailed(
        device_->CreateFence(
            0, 
            D3D12_FENCE_FLAG_NONE, 
            IID_PPV_ARGS(
                &fence_
            )
        )
    );

    fenceValue_ = 1;
    fenceEvent_ = CreateEvent(
        nullptr, 
        FALSE, 
        FALSE, 
        nullptr
    );

    if (fenceEvent_ == nullptr)
    {
        ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
    }
}

void DX12RayTracing::CreateRootSignature()
{
    D3D12_DESCRIPTOR_RANGE ranges[2];
    ranges[0] = { 
        D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 
        1, 
        0, 
        0, 
        D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND 
    };

    ranges[1] = { 
        D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 
        1, 
        0, 
        0, 
        D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND 
    }; 

    D3D12_ROOT_PARAMETER rootParameters[2];
    rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
    rootParameters[0].DescriptorTable.NumDescriptorRanges = 1;
    rootParameters[0].DescriptorTable.pDescriptorRanges = &ranges[0];
    rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
    rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
    rootParameters[1].DescriptorTable.NumDescriptorRanges = 1;
    rootParameters[1].DescriptorTable.pDescriptorRanges = &ranges[1];
    rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

    D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
    rootSignatureDesc.NumParameters = _countof(rootParameters);
    rootSignatureDesc.pParameters = rootParameters;
    rootSignatureDesc.NumStaticSamplers = 0;
    rootSignatureDesc.pStaticSamplers = nullptr;
    rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_NONE;

    ComPtr<ID3DBlob> signature;
    ComPtr<ID3DBlob> error;
    ThrowIfFailed(
        D3D12SerializeRootSignature(
            &rootSignatureDesc, 
            D3D_ROOT_SIGNATURE_VERSION_1, 
            &signature, 
            &error
        )
    );

    ThrowIfFailed(
        device_->CreateRootSignature(
            0, signature->GetBufferPointer(), 
            signature->GetBufferSize(), 
            IID_PPV_ARGS(
                &globalRootSignature_
            )
        )
    );

    D3D12_ROOT_SIGNATURE_DESC localRootSignatureDesc = {};
    localRootSignatureDesc.NumParameters = 0;
    localRootSignatureDesc.pParameters = nullptr;
    localRootSignatureDesc.NumStaticSamplers = 0;
    localRootSignatureDesc.pStaticSamplers = nullptr;
    localRootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE;
    ComPtr<ID3DBlob> localSignature;
    ComPtr<ID3DBlob> localError;

    ThrowIfFailed(
        D3D12SerializeRootSignature(
            &localRootSignatureDesc, 
            D3D_ROOT_SIGNATURE_VERSION_1, 
            &localSignature, 
            &localError
        )
    );

    ThrowIfFailed(
        device_->CreateRootSignature(
            0, 
            localSignature->GetBufferPointer(), 
            localSignature->GetBufferSize(), 
            IID_PPV_ARGS(
                &localRootSignature_
            )
        )
    );
}

void DX12RayTracing::CreatePipelineState()
{
    std::vector<D3D12_STATE_SUBOBJECT> subobjects;
    D3D12_DXIL_LIBRARY_DESC libDesc = {};
    libDesc.DXILLibrary.pShaderBytecode = rayGenShader_->GetBufferPointer();
    auto shaderSize = rayGenShader_->GetBufferSize();
    libDesc.DXILLibrary.BytecodeLength = safe_cast(shaderSize);

    D3D12_EXPORT_DESC exports[] = {
        {   L"RayGen", 
            nullptr, 
            D3D12_EXPORT_FLAG_NONE
        },
        {   L"ClosestHit", 
            nullptr, 
            D3D12_EXPORT_FLAG_NONE
        },
        {   L"Miss", 
            nullptr, 
            D3D12_EXPORT_FLAG_NONE
        }
    };

    libDesc.NumExports = _countof(exports);
    libDesc.pExports = exports;
    D3D12_STATE_SUBOBJECT libSubobject = {};
    libSubobject.Type = D3D12_STATE_SUBOBJECT_TYPE_DXIL_LIBRARY;
    libSubobject.pDesc = &libDesc;
    subobjects.push_back(libSubobject);
    D3D12_HIT_GROUP_DESC hitGroupDesc = {};
    hitGroupDesc.ClosestHitShaderImport = L"ClosestHit";
    hitGroupDesc.HitGroupExport = L"HitGroup";
    hitGroupDesc.Type = D3D12_HIT_GROUP_TYPE_TRIANGLES;
    D3D12_STATE_SUBOBJECT hitGroupSubobject = {};
    hitGroupSubobject.Type = D3D12_STATE_SUBOBJECT_TYPE_HIT_GROUP;
    hitGroupSubobject.pDesc = &hitGroupDesc;
    subobjects.push_back(hitGroupSubobject);
    D3D12_RAYTRACING_SHADER_CONFIG shaderConfigDesc = {};
    shaderConfigDesc.MaxPayloadSizeInBytes = 4 * sizeof(float);
    shaderConfigDesc.MaxAttributeSizeInBytes = 8;
    D3D12_STATE_SUBOBJECT shaderConfigSubobject = {};
    shaderConfigSubobject.Type = D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_SHADER_CONFIG;
    shaderConfigSubobject.pDesc = &shaderConfigDesc;
    subobjects.push_back(shaderConfigSubobject);
    D3D12_STATE_SUBOBJECT localRootSignatureSubobject = {};
    localRootSignatureSubobject.Type = D3D12_STATE_SUBOBJECT_TYPE_LOCAL_ROOT_SIGNATURE;
    localRootSignatureSubobject.pDesc = localRootSignature_.GetAddressOf();
    subobjects.push_back(localRootSignatureSubobject);
    D3D12_RAYTRACING_PIPELINE_CONFIG pipelineConfigDesc = {};
    pipelineConfigDesc.MaxTraceRecursionDepth = 1;
    D3D12_STATE_SUBOBJECT pipelineConfigSubobject = {};
    pipelineConfigSubobject.Type = D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_PIPELINE_CONFIG;
    pipelineConfigSubobject.pDesc = &pipelineConfigDesc;
    subobjects.push_back(pipelineConfigSubobject);
    D3D12_STATE_SUBOBJECT globalRootSignatureSubobject = {};
    globalRootSignatureSubobject.Type = D3D12_STATE_SUBOBJECT_TYPE_GLOBAL_ROOT_SIGNATURE;
    globalRootSignatureSubobject.pDesc = globalRootSignature_.GetAddressOf();
    subobjects.push_back(globalRootSignatureSubobject);
    D3D12_STATE_OBJECT_DESC raytracingPipeline = {};
    raytracingPipeline.Type = D3D12_STATE_OBJECT_TYPE_RAYTRACING_PIPELINE;
    raytracingPipeline.NumSubobjects = safe_cast(subobjects.size());
    raytracingPipeline.pSubobjects = subobjects.data();
    std::cout << "Creating state object" << std::endl;

    ThrowIfFailed(
        device_->CreateStateObject(
            &raytracingPipeline, 
            IID_PPV_ARGS(
                &rtStateObject_
            )
        )
    );
}

void DX12RayTracing::CreateAccelerationStructures()
{
    ThrowIfFailed(commandList_->Reset(commandAllocator_.Get(), nullptr));

    instanceDesc_.InstanceMask = 1;
    instanceDesc_.InstanceID = 0;
    instanceDesc_.InstanceContributionToHitGroupIndex = 0;
    instanceDesc_.Flags = D3D12_RAYTRACING_INSTANCE_FLAG_NONE;
    instanceDesc_.Transform[0][0] = instanceDesc_.Transform[1][1] = instanceDesc_.Transform[2][2] = 1.0f;
    instanceDesc_.Transform[0][1] = instanceDesc_.Transform[0][2] = instanceDesc_.Transform[0][3] = 0.0f;
    instanceDesc_.Transform[1][0] = instanceDesc_.Transform[1][2] = instanceDesc_.Transform[1][3] = 0.0f;
    instanceDesc_.Transform[2][0] = instanceDesc_.Transform[2][1] = instanceDesc_.Transform[2][3] = 0.0f;

    using TriangleBox = Geometry3D::AdvancedBox<Geometry3D::HeapStorage, 
                        Geometry3D::StrictValidation, 
                        Geometry3D::JSONSerialization>;
    TriangleBox box(vertex1_, vertex2_, vertex3_);
    auto boundary = box.get_boundary_points(1);
    vertices_.resize(boundary.size());
    for (size_t i = 0; i < boundary.size(); ++i) {
        vertices_[i] = { 
            static_cast<float>(boundary[i][0]), 
            static_cast<float>(boundary[i][1]), 
            static_cast<float>(boundary[i][2]) 
        };
    }
    auto indices_size_t = box.get_indices();
    indices_.resize(indices_size_t.size());
    for (size_t i = 0; i < indices_size_t.size(); ++i) {
        indices_[i] = static_cast<UINT>(indices_size_t[i]);
    }

    D3D12_RAYTRACING_GEOMETRY_DESC geometryDesc = {};
    geometryDesc.Type = D3D12_RAYTRACING_GEOMETRY_TYPE_TRIANGLES;
    geometryDesc.Triangles.IndexBuffer = 0;
    geometryDesc.Triangles.IndexCount = safe_cast(indices_.size());
    geometryDesc.Triangles.IndexFormat = DXGI_FORMAT_R32_UINT;
    geometryDesc.Triangles.Transform3x4 = 0;
    geometryDesc.Triangles.VertexBuffer.StartAddress = 0;
    geometryDesc.Triangles.VertexBuffer.StrideInBytes = sizeof(DirectX::XMFLOAT3);
    geometryDesc.Triangles.VertexCount = safe_cast(vertices_.size());
    geometryDesc.Triangles.VertexFormat = DXGI_FORMAT_R32G32B32_FLOAT;
    geometryDesc.Flags = D3D12_RAYTRACING_GEOMETRY_FLAG_OPAQUE;

    D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS bottomLevelInputs = {};
    bottomLevelInputs.Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL;
    bottomLevelInputs.Flags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_TRACE;
    bottomLevelInputs.NumDescs = 1;
    bottomLevelInputs.DescsLayout = D3D12_ELEMENTS_LAYOUT_ARRAY;
    bottomLevelInputs.pGeometryDescs = &geometryDesc;

    D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO bottomLevelPrebuildInfo = {};
    device_->GetRaytracingAccelerationStructurePrebuildInfo(
        &bottomLevelInputs, 
        &bottomLevelPrebuildInfo
    );

    D3D12_HEAP_PROPERTIES heapPropsUpload = { 
        D3D12_HEAP_TYPE_UPLOAD, 
        D3D12_CPU_PAGE_PROPERTY_UNKNOWN, 
        D3D12_MEMORY_POOL_UNKNOWN, 
        1, 
        1 
    };

    D3D12_RESOURCE_DESC resourceDescBuffer = { 
        D3D12_RESOURCE_DIMENSION_BUFFER, 
        0, 
        sizeof(DirectX::XMFLOAT3) * vertices_.size(), 
        1, 
        1, 
        1, 
        DXGI_FORMAT_UNKNOWN, 
        {
            1,
            0
        }, 
        D3D12_TEXTURE_LAYOUT_ROW_MAJOR, 
        D3D12_RESOURCE_FLAG_NONE 
    };

    ThrowIfFailed(
        device_->CreateCommittedResource(
            &heapPropsUpload,
            D3D12_HEAP_FLAG_NONE,
            &resourceDescBuffer,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(
                &vertexBuffer_
            )
        )
    );

    UINT8* pVertexDataBegin;

    D3D12_RANGE readRange = {
        0, 
        0
    };

    ThrowIfFailed(
        vertexBuffer_->Map(
            0, 
            &readRange, 
            reinterpret_cast<void**>(
                &pVertexDataBegin
            )
        )
    );

    memcpy(
        pVertexDataBegin, 
        vertices_.data(), 
        sizeof(DirectX::XMFLOAT3) * vertices_.size()
    );

    vertexBuffer_->Unmap(0, nullptr);

    D3D12_RESOURCE_DESC resourceDescIndex = { 
        D3D12_RESOURCE_DIMENSION_BUFFER, 
        0, 
        sizeof(UINT) * indices_.size(), 
        1, 
        1, 
        1, 
        DXGI_FORMAT_UNKNOWN, 
        {
            1,
            0
        }, 
        D3D12_TEXTURE_LAYOUT_ROW_MAJOR,
        D3D12_RESOURCE_FLAG_NONE 
    };

    ThrowIfFailed(
        device_->CreateCommittedResource(
            &heapPropsUpload,
            D3D12_HEAP_FLAG_NONE,
            &resourceDescIndex,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(
                &indexBuffer_
            )
        )
    );

    UINT8* pIndexDataBegin;

    ThrowIfFailed(
        indexBuffer_->Map(
            0, 
            &readRange, 
            reinterpret_cast<void**>(
                &pIndexDataBegin
            )
        )
    );

    memcpy(
        pIndexDataBegin, 
        indices_.data(), 
        sizeof(UINT) * indices_.size()
    );

    indexBuffer_->Unmap(
        0, 
        nullptr
    );

    D3D12_HEAP_PROPERTIES heapPropsDefault = { 
        D3D12_HEAP_TYPE_DEFAULT, 
        D3D12_CPU_PAGE_PROPERTY_UNKNOWN, 
        D3D12_MEMORY_POOL_UNKNOWN, 
        1, 
        1 
    };
    D3D12_RESOURCE_DESC resourceDescBLAS = { 
        D3D12_RESOURCE_DIMENSION_BUFFER, 
        0, 
        bottomLevelPrebuildInfo.ResultDataMaxSizeInBytes, 
        1, 
        1, 
        1, 
        DXGI_FORMAT_UNKNOWN, 
        {
            1,
            0
        }, 
        D3D12_TEXTURE_LAYOUT_ROW_MAJOR, 
        D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS 
    };

    ThrowIfFailed(
        device_->CreateCommittedResource(
            &heapPropsDefault,
            D3D12_HEAP_FLAG_NONE,
            &resourceDescBLAS,
            D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE,
            nullptr,
            IID_PPV_ARGS(&bottomLevelAS_)
        )
    );

    ComPtr<ID3D12Resource> bottomLevelScratch;

    D3D12_RESOURCE_DESC resourceDescScratchBLAS = { 
        D3D12_RESOURCE_DIMENSION_BUFFER, 
        0, 
        bottomLevelPrebuildInfo.ScratchDataSizeInBytes, 
        1,
        1, 
        1,
        DXGI_FORMAT_UNKNOWN, 
        {
            1,
            0
        }, 
        D3D12_TEXTURE_LAYOUT_ROW_MAJOR, 
        D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS 
    };

    ThrowIfFailed(
        device_->CreateCommittedResource(
            &heapPropsDefault,
            D3D12_HEAP_FLAG_NONE,
            &resourceDescScratchBLAS,
            D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
            nullptr,
            IID_PPV_ARGS(
                &bottomLevelScratch
            )
        )
    );

    geometryDesc.Triangles.IndexBuffer = indexBuffer_->GetGPUVirtualAddress();
    geometryDesc.Triangles.VertexBuffer.StartAddress = vertexBuffer_->GetGPUVirtualAddress();
    D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC bottomLevelBuildDesc = {};
    bottomLevelBuildDesc.Inputs = bottomLevelInputs;
    bottomLevelBuildDesc.DestAccelerationStructureData = bottomLevelAS_->GetGPUVirtualAddress();
    bottomLevelBuildDesc.ScratchAccelerationStructureData = bottomLevelScratch->GetGPUVirtualAddress();

    commandList_->BuildRaytracingAccelerationStructure(
        &bottomLevelBuildDesc, 
        0, 
        nullptr
    );

    instanceDesc_.AccelerationStructure = bottomLevelAS_->GetGPUVirtualAddress();
    D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS topLevelInputs = {};
    topLevelInputs.Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL;
    topLevelInputs.Flags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_ALLOW_UPDATE | 
                                D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_TRACE;
    topLevelInputs.NumDescs = 1;
    topLevelInputs.DescsLayout = D3D12_ELEMENTS_LAYOUT_ARRAY;
    D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO topLevelPrebuildInfo = {};

    device_->GetRaytracingAccelerationStructurePrebuildInfo(
        &topLevelInputs, &topLevelPrebuildInfo
    );

    D3D12_RESOURCE_DESC resourceDescTLAS = { 
        D3D12_RESOURCE_DIMENSION_BUFFER, 
        0, 
        topLevelPrebuildInfo.ResultDataMaxSizeInBytes, 
        1, 
        1, 
        1, 
        DXGI_FORMAT_UNKNOWN, 
        {
            1,
            0
        }, 
        D3D12_TEXTURE_LAYOUT_ROW_MAJOR, 
        D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS 
    };

    ThrowIfFailed(
        device_->CreateCommittedResource(
            &heapPropsDefault,
            D3D12_HEAP_FLAG_NONE,
            &resourceDescTLAS,
            D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE,
            nullptr,
            IID_PPV_ARGS(
                &topLevelAS_
            )
        )
    );

    D3D12_RESOURCE_DESC resourceDescScratchTLAS = { 
        D3D12_RESOURCE_DIMENSION_BUFFER, 
        0, 
        topLevelPrebuildInfo.ScratchDataSizeInBytes, 
        1, 
        1, 
        1,
        DXGI_FORMAT_UNKNOWN, 
        {
            1,0
        }, 
        D3D12_TEXTURE_LAYOUT_ROW_MAJOR, 
        D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS 
    };

    ThrowIfFailed(
        device_->CreateCommittedResource(
            &heapPropsDefault,
            D3D12_HEAP_FLAG_NONE,
            &resourceDescScratchTLAS,
            D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
            nullptr,
            IID_PPV_ARGS(&topLevelScratch_)
        )
    );

    D3D12_RESOURCE_DESC resourceDescInstance = { 
        D3D12_RESOURCE_DIMENSION_BUFFER, 
        0, 
        sizeof(instanceDesc_), 
        1, 
        1, 
        1, 
        DXGI_FORMAT_UNKNOWN, 
        {
            1,
            0
        }, 
        D3D12_TEXTURE_LAYOUT_ROW_MAJOR, 
        D3D12_RESOURCE_FLAG_NONE 
    };

    ThrowIfFailed(
        device_->CreateCommittedResource(
            &heapPropsUpload,
            D3D12_HEAP_FLAG_NONE,
            &resourceDescInstance,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&instanceBuffer_)
        )
    );

    ThrowIfFailed(
        instanceBuffer_->Map(
            0, 
            &readRange, 
            reinterpret_cast<void**>(
                &instanceBufferData_
            )
        )
    );

    memcpy(
        instanceBufferData_, 
        &instanceDesc_, 
        sizeof(
            instanceDesc_
        )
    );

    D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC topLevelBuildDesc = {};
    topLevelBuildDesc.Inputs = topLevelInputs;
    topLevelBuildDesc.Inputs.InstanceDescs = instanceBuffer_->GetGPUVirtualAddress();
    topLevelBuildDesc.DestAccelerationStructureData = topLevelAS_->GetGPUVirtualAddress();
    topLevelBuildDesc.ScratchAccelerationStructureData = topLevelScratch_->GetGPUVirtualAddress();
    commandList_->BuildRaytracingAccelerationStructure(&topLevelBuildDesc, 0, nullptr);
    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
    barrier.UAV.pResource = topLevelAS_.Get();
    commandList_->ResourceBarrier(1, &barrier);
    ThrowIfFailed(commandList_->Close());

    ID3D12CommandList* ppCommandLists[] = { 
        commandList_.Get() 
    };

    commandQueue_->ExecuteCommandLists(
        _countof(
            ppCommandLists
        ), 
        ppCommandLists
    );

    WaitForPreviousFrame();
}

void DX12RayTracing::CreateShaderTables()
{
    D3D12_HEAP_PROPERTIES heapPropsUpload = { 
        D3D12_HEAP_TYPE_UPLOAD, 
        D3D12_CPU_PAGE_PROPERTY_UNKNOWN, 
        D3D12_MEMORY_POOL_UNKNOWN, 
        1, 
        1 
    };

    ComPtr<ID3D12StateObjectProperties> stateObjectProperties;
    ThrowIfFailed(
        rtStateObject_.As(
            &stateObjectProperties
        )
    );

    void* rayGenShaderIdentifier = stateObjectProperties->GetShaderIdentifier(L"RayGen");
    void* missShaderIdentifier = stateObjectProperties->GetShaderIdentifier(L"Miss");
    void* hitGroupShaderIdentifier = stateObjectProperties->GetShaderIdentifier(L"HitGroup");

    UINT shaderRecordSize = D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES;
    D3D12_RESOURCE_DESC resourceDescRGST = { 
        D3D12_RESOURCE_DIMENSION_BUFFER, 
        0, 
        shaderRecordSize, 
        1, 
        1, 
        1, 
        DXGI_FORMAT_UNKNOWN, 
        {
            1,
            0
        }, 
        D3D12_TEXTURE_LAYOUT_ROW_MAJOR, 
        D3D12_RESOURCE_FLAG_NONE 
    };

    ThrowIfFailed(
        device_->CreateCommittedResource(
            &heapPropsUpload,
            D3D12_HEAP_FLAG_NONE,
            &resourceDescRGST,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(
                &rayGenShaderTable_
            )
        )
    );

    UINT8* pData;
    ThrowIfFailed(
        rayGenShaderTable_->Map(
            0, 
            nullptr, 
            reinterpret_cast<void**>(
                &pData
            )
        )
    );

    memcpy(
        pData, 
        rayGenShaderIdentifier, 
        D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES
    );

    rayGenShaderTable_->Unmap(
        0, 
        nullptr
    );

    D3D12_RESOURCE_DESC resourceDescMST = { 
        D3D12_RESOURCE_DIMENSION_BUFFER, 
        0, 
        shaderRecordSize, 
        1, 
        1, 
        1, 
        DXGI_FORMAT_UNKNOWN, 
        {
            1,
            0
        }, 
        D3D12_TEXTURE_LAYOUT_ROW_MAJOR, 
        D3D12_RESOURCE_FLAG_NONE 
    };

    ThrowIfFailed(
        device_->CreateCommittedResource(
            &heapPropsUpload,
            D3D12_HEAP_FLAG_NONE,
            &resourceDescMST,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(
                &missShaderTable_
            )
        )
    );

    ThrowIfFailed(
        missShaderTable_->Map(
            0, 
            nullptr, 
            reinterpret_cast<void**>(
                &pData
            )
        )
    );

    memcpy(
        pData, 
        missShaderIdentifier, 
        D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES
    );
    missShaderTable_->Unmap(
        0, 
        nullptr
    );

    D3D12_RESOURCE_DESC resourceDescHGST = { 
        D3D12_RESOURCE_DIMENSION_BUFFER, 
        0, 
        shaderRecordSize, 
        1, 
        1, 
        1, 
        DXGI_FORMAT_UNKNOWN, 
        {
            1,
            0
        }, 
        D3D12_TEXTURE_LAYOUT_ROW_MAJOR, 
        D3D12_RESOURCE_FLAG_NONE 
    };
    ThrowIfFailed(
        device_->CreateCommittedResource(
            &heapPropsUpload,
            D3D12_HEAP_FLAG_NONE,
            &resourceDescHGST,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(
                &hitGroupShaderTable_
            )
        )
    );

    ThrowIfFailed(
        hitGroupShaderTable_->Map(
            0, 
            nullptr, 
            reinterpret_cast<void**>(
                &pData
            )
        )
    );

    memcpy(
        pData, 
        hitGroupShaderIdentifier, 
        D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES
    );

    hitGroupShaderTable_->Unmap(
        0, 
        nullptr
    );
}

void DX12RayTracing::CreateConstantBuffer()
{
    D3D12_HEAP_PROPERTIES heapPropsUpload = { 
        D3D12_HEAP_TYPE_UPLOAD, 
        D3D12_CPU_PAGE_PROPERTY_UNKNOWN, 
        D3D12_MEMORY_POOL_UNKNOWN, 
        1, 
        1 
    };

    D3D12_RESOURCE_DESC resourceDescCB = { 
        D3D12_RESOURCE_DIMENSION_BUFFER, 
        0, 
        sizeof(SceneConstantBuffer), 
        1, 
        1, 
        1, 
        DXGI_FORMAT_UNKNOWN, 
        {
            1,
            0
        }, 
        D3D12_TEXTURE_LAYOUT_ROW_MAJOR, 
        D3D12_RESOURCE_FLAG_NONE 
    };

    ThrowIfFailed(
        device_->CreateCommittedResource(
            &heapPropsUpload,
            D3D12_HEAP_FLAG_NONE,
            &resourceDescCB,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(
                &constantBuffer_
            )
        )
    );

    D3D12_RANGE readRange = {
        0, 
        0
    };

    ThrowIfFailed(
        constantBuffer_->Map(
            0, 
            &readRange, 
            reinterpret_cast<void**>(
                &constantBufferData_
            )
        )
    );
}

void DX12RayTracing::LoadShaders()
{
    std::ifstream rayGenFile("raygen.cso", std::ios::binary);
    if (!rayGenFile) throw std::runtime_error("Не удалось открыть raygen.cso");

    std::vector<char> rayGenData(
            (std::istreambuf_iterator<char>(rayGenFile)
        ), 
        std::istreambuf_iterator<char>()
    );

    ThrowIfFailed(
        D3DCreateBlob(
            rayGenData.size(), 
            &rayGenShader_
        )
    );

    memcpy(
        rayGenShader_->GetBufferPointer(), 
        rayGenData.data(), 
        rayGenData.size()
    );

    std::ifstream closestHitFile("closesthit.cso", std::ios::binary);
    if (!closestHitFile) throw std::runtime_error("Не удалось открыть closesthit.cso");

    std::vector<char> closestHitData(
        (
            std::istreambuf_iterator<char>(closestHitFile)
        ), 
        std::istreambuf_iterator<char>()
    );

    ThrowIfFailed(
        D3DCreateBlob(
            closestHitData.size(), 
            &closestHitShader_
        )
    );

    memcpy(
        closestHitShader_->GetBufferPointer(), 
        closestHitData.data(), 
        closestHitData.size()
    );

    std::ifstream missFile("miss.cso", std::ios::binary);
    if (!missFile) throw std::runtime_error("Не удалось открыть miss.cso");

    std::vector<char> missData(
        (
            std::istreambuf_iterator<char>(missFile)
        ),
        std::istreambuf_iterator<char>()
    );

    ThrowIfFailed(
        D3DCreateBlob(
            missData.size(), 
            &missShader_
        )
    );
    memcpy(
        missShader_->GetBufferPointer(), 
        missData.data(), 
        missData.size()
    );
}

void DX12RayTracing::UpdateConstantBuffer()
{
    SceneConstantBuffer cb = {};
    DirectX::XMVECTOR eye = DirectX::XMVectorSet(0.0f, 0.0f, -5.0f, 1.0f);
    DirectX::XMVECTOR at = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
    DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(eye, at, up);
    DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, width_ / (FLOAT)height_, 0.1f, 100.0f);
    DirectX::XMMATRIX viewProj = view * proj;
    DirectX::XMMATRIX invViewProj = DirectX::XMMatrixInverse(nullptr, viewProj);
    DirectX::XMStoreFloat4x4(&cb.projectionToWorld, DirectX::XMMatrixTranspose(invViewProj));
    DirectX::XMStoreFloat4(&cb.cameraPosition, eye);
    DirectX::XMStoreFloat4(&cb.lightPosition, DirectX::XMVectorSet(10.0f, 10.0f, -10.0f, 1.0f));
    DirectX::XMStoreFloat4(&cb.lightAmbientColor, DirectX::XMVectorSet(0.2f, 0.2f, 0.2f, 1.0f));
    DirectX::XMStoreFloat4(&cb.lightDiffuseColor, DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f));
    memcpy(constantBufferData_, &cb, sizeof(cb));
}

void DX12RayTracing::Render()
{
    UpdateConstantBuffer();
    UpdateTransform();
    RebuildTLAS();
    PopulateCommandList();

    ID3D12CommandList* ppCommandLists[] = { 
        commandList_.Get() 
    };

    commandQueue_->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

    ThrowIfFailed(
        swapChain_->Present(
            1, 0
        )
    );

    WaitForPreviousFrame();

    frameCount_++;
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto deltaTime = std::chrono::duration<double>(currentTime - lastTime_).count();
    if (deltaTime >= 1.0)
    {
        fps_ = frameCount_ / deltaTime;
        frameCount_ = 0;
        lastTime_ = currentTime;
    }
}

void DX12RayTracing::PopulateCommandList()
{
    ThrowIfFailed(
        commandAllocator_->Reset()
    );
    ThrowIfFailed(
        commandList_->Reset(
            commandAllocator_.Get(), 
            nullptr
        )
    );

    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Transition.pResource = renderTargets_[frameIndex_].Get();
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
    commandList_->ResourceBarrier(1, &barrier);
    commandList_->SetComputeRootSignature(globalRootSignature_.Get());

    ID3D12DescriptorHeap* heaps[] = { 
        srvUavHeap_.Get() 
    };

    commandList_->SetDescriptorHeaps(_countof(heaps), heaps);
    D3D12_DISPATCH_RAYS_DESC dispatchDesc = {};
    dispatchDesc.RayGenerationShaderRecord.StartAddress = rayGenShaderTable_->GetGPUVirtualAddress();
    dispatchDesc.RayGenerationShaderRecord.SizeInBytes = D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES;
    dispatchDesc.MissShaderTable.StartAddress = missShaderTable_->GetGPUVirtualAddress();
    dispatchDesc.MissShaderTable.SizeInBytes = D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES;
    dispatchDesc.MissShaderTable.StrideInBytes = D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES;
    dispatchDesc.HitGroupTable.StartAddress = hitGroupShaderTable_->GetGPUVirtualAddress();
    dispatchDesc.HitGroupTable.SizeInBytes = D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES;
    dispatchDesc.HitGroupTable.StrideInBytes = D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES;
    dispatchDesc.Width = width_;
    dispatchDesc.Height = height_;
    dispatchDesc.Depth = 1;
    commandList_->DispatchRays(&dispatchDesc);
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
    commandList_->ResourceBarrier(1, &barrier);
    ThrowIfFailed(commandList_->Close());
}

void DX12RayTracing::WaitForPreviousFrame()
{
    const UINT64 fence = fenceValue_;
    ThrowIfFailed(
        commandQueue_->Signal(
            fence_.Get(), 
            fence
        )
    );

    fenceValue_++;

    if (fence_->GetCompletedValue() < fence)
    {
        ThrowIfFailed(fence_->SetEventOnCompletion(fence, fenceEvent_));
        WaitForSingleObject(fenceEvent_, INFINITE);
    }

    frameIndex_ = swapChain_->GetCurrentBackBufferIndex();
}

void DX12RayTracing::Resize(UINT width, UINT height)
{
    if (width_ == width && height_ == height) return;

    width_ = width;
    height_ = height;

    WaitForPreviousFrame();

    for (int i = 0; i < FrameCount; ++i)
        renderTargets_[i].Reset();

    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChain_->GetDesc(&swapChainDesc);
    ThrowIfFailed(
        swapChain_->ResizeBuffers(
            FrameCount, 
            width, 
            height, 
            swapChainDesc.BufferDesc.Format, 
            swapChainDesc.Flags
        )
    );

    CreateRTV();
    CreateDSV();
}

void DX12RayTracing::UpdateTransform()
{
    auto currentTime = std::chrono::high_resolution_clock::now();
    double elapsed = std::chrono::duration<double>(currentTime - startTime_).count();
    float angle = static_cast<float>(elapsed * 2.0f);
    DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationY(angle);
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            instanceDesc_.Transform[i][j] = rotation.r[i].m128_f32[j];
        }
    }
    memcpy(instanceBufferData_, &instanceDesc_, sizeof(instanceDesc_));
}

void DX12RayTracing::RebuildTLAS()
{
    ThrowIfFailed(commandList_->Reset(commandAllocator_.Get(), nullptr));

    D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS topLevelInputs = {};
    topLevelInputs.Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL;
    topLevelInputs.Flags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PERFORM_UPDATE | 
                                D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_TRACE;
    topLevelInputs.NumDescs = 1;
    topLevelInputs.DescsLayout = D3D12_ELEMENTS_LAYOUT_ARRAY;

    D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC topLevelBuildDesc = {};
    topLevelBuildDesc.Inputs = topLevelInputs;
    topLevelBuildDesc.Inputs.InstanceDescs = instanceBuffer_->GetGPUVirtualAddress();
    topLevelBuildDesc.DestAccelerationStructureData = topLevelAS_->GetGPUVirtualAddress();
    topLevelBuildDesc.SourceAccelerationStructureData = topLevelAS_->GetGPUVirtualAddress();
    topLevelBuildDesc.ScratchAccelerationStructureData = topLevelScratch_->GetGPUVirtualAddress();

    commandList_->BuildRaytracingAccelerationStructure(&topLevelBuildDesc, 0, nullptr);

    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
    barrier.UAV.pResource = topLevelAS_.Get();
    commandList_->ResourceBarrier(1, &barrier);

    ThrowIfFailed(commandList_->Close());

    ID3D12CommandList* ppCommandLists[] = { commandList_.Get() };
    commandQueue_->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

    WaitForPreviousFrame();
}

void DX12RayTracing::UpdateGeometry(
    const Geometry3D::AdvancedBox<Geometry3D::HeapStorage, 
    Geometry3D::StrictValidation, 
    Geometry3D::JSONSerialization>& box
)
{
    auto boundary = box.get_boundary_points(1);
    vertices_.resize(boundary.size());
    for (size_t i = 0; i < boundary.size(); ++i) {
        vertices_[i] = { 
            static_cast<float>(boundary[i][0]), 
            static_cast<float>(boundary[i][1]), 
            static_cast<float>(boundary[i][2]) 
        };
    }
    auto indices_size_t = box.get_indices();
    indices_.resize(indices_size_t.size());
    for (size_t i = 0; i < indices_size_t.size(); ++i) {
        indices_[i] = static_cast<UINT>(indices_size_t[i]);
    }

    D3D12_HEAP_PROPERTIES heapPropsUpload = { 
        D3D12_HEAP_TYPE_UPLOAD, 
        D3D12_CPU_PAGE_PROPERTY_UNKNOWN, 
        D3D12_MEMORY_POOL_UNKNOWN, 
        1, 
        1 
    };

    D3D12_RESOURCE_DESC resourceDescBuffer = { 
        D3D12_RESOURCE_DIMENSION_BUFFER, 0, 
        sizeof(DirectX::XMFLOAT3) * vertices_.size(), 
        1, 
        1, 
        1, 
        DXGI_FORMAT_UNKNOWN, 
        {
            1,
            0
        }, 
        D3D12_TEXTURE_LAYOUT_ROW_MAJOR, 
        D3D12_RESOURCE_FLAG_NONE 
    };

    ThrowIfFailed(
        device_->CreateCommittedResource(
            &heapPropsUpload,
            D3D12_HEAP_FLAG_NONE,
            &resourceDescBuffer,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(
                &vertexBuffer_
            )
        )
    );

    UINT8* pVertexDataBegin;
    D3D12_RANGE readRange = {
        0, 
        0
    };

    ThrowIfFailed(
        vertexBuffer_->Map(
            0, 
            &readRange, 
            reinterpret_cast<void**>(
                &pVertexDataBegin
            )
        )
    );

    memcpy(
        pVertexDataBegin, 
        vertices_.data(), 
        sizeof(DirectX::XMFLOAT3) * vertices_.size()
    );

    vertexBuffer_->Unmap(
        0, nullptr
    );

    D3D12_RESOURCE_DESC resourceDescIndex = { 
        D3D12_RESOURCE_DIMENSION_BUFFER, 
        0, sizeof(UINT) * indices_.size(), 
        1, 
        1, 
        1, 
        DXGI_FORMAT_UNKNOWN, 
        {
            1,
            0
        }, 
        D3D12_TEXTURE_LAYOUT_ROW_MAJOR, 
        D3D12_RESOURCE_FLAG_NONE 
    };

    ThrowIfFailed(
        device_->CreateCommittedResource(
            &heapPropsUpload,
            D3D12_HEAP_FLAG_NONE,
            &resourceDescIndex,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(
                &indexBuffer_
            )
        )
    );

    UINT8* pIndexDataBegin;
    ThrowIfFailed(
        indexBuffer_->Map(
            0, 
            &readRange, 
            reinterpret_cast<void**>(
                &pIndexDataBegin
            )
        )
    );
    memcpy(
        pIndexDataBegin, 
        indices_.data(), 
        sizeof(UINT) * indices_.size()
    );

    indexBuffer_->Unmap(0, nullptr);

    ThrowIfFailed(commandList_->Reset(commandAllocator_.Get(), nullptr));

    D3D12_RAYTRACING_GEOMETRY_DESC geometryDesc = {};
    geometryDesc.Type = D3D12_RAYTRACING_GEOMETRY_TYPE_TRIANGLES;
    geometryDesc.Triangles.IndexBuffer = indexBuffer_->GetGPUVirtualAddress();
    geometryDesc.Triangles.IndexCount = safe_cast(indices_.size());
    geometryDesc.Triangles.IndexFormat = DXGI_FORMAT_R32_UINT;
    geometryDesc.Triangles.Transform3x4 = 0;
    geometryDesc.Triangles.VertexBuffer.StartAddress = vertexBuffer_->GetGPUVirtualAddress();
    geometryDesc.Triangles.VertexBuffer.StrideInBytes = sizeof(DirectX::XMFLOAT3);
    geometryDesc.Triangles.VertexCount = safe_cast(vertices_.size());
    geometryDesc.Triangles.VertexFormat = DXGI_FORMAT_R32G32B32_FLOAT;
    geometryDesc.Flags = D3D12_RAYTRACING_GEOMETRY_FLAG_OPAQUE;
    D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS bottomLevelInputs = {};
    bottomLevelInputs.Type = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL;
    bottomLevelInputs.Flags = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_TRACE;
    bottomLevelInputs.NumDescs = 1;
    bottomLevelInputs.DescsLayout = D3D12_ELEMENTS_LAYOUT_ARRAY;
    bottomLevelInputs.pGeometryDescs = &geometryDesc;
    D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO bottomLevelPrebuildInfo = {};
    device_->GetRaytracingAccelerationStructurePrebuildInfo(&bottomLevelInputs, &bottomLevelPrebuildInfo);

    // Scratch buffer для BLAS
    ComPtr<ID3D12Resource> bottomLevelScratch;
    D3D12_HEAP_PROPERTIES heapPropsDefault = { 
        D3D12_HEAP_TYPE_DEFAULT, 
        D3D12_CPU_PAGE_PROPERTY_UNKNOWN, 
        D3D12_MEMORY_POOL_UNKNOWN, 
        1, 
        1 
    };

    D3D12_RESOURCE_DESC resourceDescScratchBLAS = { 
        D3D12_RESOURCE_DIMENSION_BUFFER, 
        0, 
        bottomLevelPrebuildInfo.ScratchDataSizeInBytes, 
        1, 
        1, 
        1, 
        DXGI_FORMAT_UNKNOWN, 
        {
            1,
            0
        }, 
        D3D12_TEXTURE_LAYOUT_ROW_MAJOR, 
        D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS 
    };

    ThrowIfFailed(
        device_->CreateCommittedResource(
            &heapPropsDefault,
            D3D12_HEAP_FLAG_NONE,
            &resourceDescScratchBLAS,
            D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
            nullptr,
            IID_PPV_ARGS(
                &bottomLevelScratch
            )
        )
    );

    D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC bottomLevelBuildDesc = {};
    bottomLevelBuildDesc.Inputs = bottomLevelInputs;
    bottomLevelBuildDesc.DestAccelerationStructureData = bottomLevelAS_->GetGPUVirtualAddress();
    bottomLevelBuildDesc.ScratchAccelerationStructureData = bottomLevelScratch->GetGPUVirtualAddress();
    commandList_->BuildRaytracingAccelerationStructure(&bottomLevelBuildDesc, 0, nullptr);
    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
    barrier.UAV.pResource = bottomLevelAS_.Get();
    commandList_->ResourceBarrier(1, &barrier);
    ThrowIfFailed(commandList_->Close());

    ID3D12CommandList* ppCommandLists[] = { 
        commandList_.Get() 
    };

    commandQueue_->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
    WaitForPreviousFrame();
}