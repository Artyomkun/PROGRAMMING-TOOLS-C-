#ifndef DX12_RAYTRACING_HPP
#define DX12_RAYTRACING_HPP

#include "geometry3d.hpp"
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <d3d12shader.h>
#include <dxgi1_6.h>
#include <windows.h>
#include <d3d12.h>
#include <vector>
#include <string>
#include <chrono>
#include <wrl.h>

using Microsoft::WRL::ComPtr;

class DX12RayTracing
{
    public:
        DX12RayTracing(HWND hwnd, UINT width, UINT height);
        ~DX12RayTracing();

        void Initialize();
        void Render();
        void Resize(UINT width, UINT height);

        double GetFPS() const { 
            return fps_; 
        }

        void UpdateGeometry(const Geometry3D::AdvancedBox<Geometry3D::HeapStorage, Geometry3D::StrictValidation, Geometry3D::JSONSerialization>& box);

    private:
        static void GetHardwareAdapter(IDXGIFactory1* pFactory, IDXGIAdapter1** ppAdapter);
        void CreateDevice();
        void CreateCommandQueue();
        void CreateSwapChain();
        void CreateDescriptorHeaps();
        void CreateRTV();
        void CreateDSV();
        void CreateCommandAllocator();
        void CreateCommandList();
        void CreateFence();
        void CreateRootSignature();
        void CreatePipelineState();
        void CreateAccelerationStructures();
        void CreateShaderTables();
        void CreateConstantBuffer();
        void LoadShaders();
        void UpdateConstantBuffer();
        void PopulateCommandList();
        void WaitForPreviousFrame();

        static const UINT FrameCount = 2;

        ComPtr<ID3D12Device5> device_;
        ComPtr<ID3D12CommandQueue> commandQueue_;
        ComPtr<IDXGISwapChain3> swapChain_;
        ComPtr<ID3D12DescriptorHeap> rtvHeap_;
        ComPtr<ID3D12DescriptorHeap> dsvHeap_;
        ComPtr<ID3D12DescriptorHeap> srvUavHeap_;
        ComPtr<ID3D12CommandAllocator> commandAllocator_;
        ComPtr<ID3D12GraphicsCommandList4> commandList_;
        ComPtr<ID3D12Resource> renderTargets_[FrameCount];
        ComPtr<ID3D12Resource> depthStencil_;
        ComPtr<ID3D12RootSignature> globalRootSignature_;
        ComPtr<ID3D12RootSignature> localRootSignature_;
        ComPtr<ID3D12StateObject> rtStateObject_;
        ComPtr<ID3D12Resource> accelerationStructure_;
        ComPtr<ID3D12Resource> bottomLevelAS_;
        ComPtr<ID3D12Resource> topLevelAS_;
        ComPtr<ID3D12Resource> rayGenShaderTable_;
        ComPtr<ID3D12Resource> missShaderTable_;
        ComPtr<ID3D12Resource> hitGroupShaderTable_;
        ComPtr<ID3DBlob> rayGenShader_;
        ComPtr<ID3DBlob> closestHitShader_;
        ComPtr<ID3DBlob> missShader_;
        ComPtr<ID3D12Resource> constantBuffer_;
        UINT8* constantBufferData_;

        UINT frameIndex_;
        HANDLE fenceEvent_;
        ComPtr<ID3D12Fence> fence_;
        UINT64 fenceValue_;

        HWND hwnd_;
        UINT width_;
        UINT height_;

        mutable double fps_;
        mutable std::chrono::high_resolution_clock::time_point lastTime_;
        mutable int frameCount_;

        Geometry3D::Point<double, 3> vertex1_, vertex2_, vertex3_;

        std::vector<DirectX::XMFLOAT3> vertices_;
        std::vector<UINT> indices_;

        ComPtr<ID3D12Resource> vertexBuffer_;
        ComPtr<ID3D12Resource> indexBuffer_;
        ComPtr<ID3D12Resource> instanceBuffer_;
        UINT8* instanceBufferData_;
        ComPtr<ID3D12Resource> topLevelScratch_;
        D3D12_RAYTRACING_INSTANCE_DESC instanceDesc_;
        std::chrono::high_resolution_clock::time_point startTime_;

        void UpdateTransform();
        void RebuildTLAS();
};

#endif // DX12_RAYTRACING_HPP