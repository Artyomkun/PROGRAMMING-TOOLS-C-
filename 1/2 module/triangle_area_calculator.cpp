#include "triangle_area_calculator.h"
#include "system_info.h"
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <cmath>

#ifdef _WIN32
    #include <windows.h>
#endif

TriangleAreaCalculator::TriangleAreaCalculator() 
    : m_firstVertexX(0.0), m_firstVertexY(0.0)
    , m_secondVertexX(0.0), m_secondVertexY(0.0)
    , m_thirdVertexX(0.0), m_thirdVertexY(0.0)
    , m_calculatedArea(0.0) {
}

void TriangleAreaCalculator::InitializeConsole() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
}

void TriangleAreaCalculator::DisplayWelcomeMessage() {
    SystemInfo::PrintSystemInfo();

    std::cout << "==============================================" << std::endl;
    std::cout << "            TRIANGLE CALCULATOR               " << std::endl;
    std::cout << "==============================================" << std::endl;
    std::cout << std::endl;
}

void TriangleAreaCalculator::GetUserInput() {
    std::cout << "Please enter the coordinates of three vertices:" << std::endl;
    std::cout << "----------------------------------------------" << std::endl;
    
    std::cout << "First vertex (x y): ";
    std::cin >> m_firstVertexX >> m_firstVertexY;
    
    std::cout << "Second vertex (x y): ";
    std::cin >> m_secondVertexX >> m_secondVertexY;
    
    std::cout << "Third vertex (x y): ";
    std::cin >> m_thirdVertexX >> m_thirdVertexY;
    
    std::cout << std::endl;
}

bool TriangleAreaCalculator::ValidateInput() const {
    if (std::cin.fail()) {
        std::cout << "ERROR: Invalid input. Please enter numeric values only." << std::endl;
        return false;
    }
    
    double determinant = (m_secondVertexX - m_firstVertexX) * (m_thirdVertexY - m_firstVertexY) 
                       - (m_thirdVertexX - m_firstVertexX) * (m_secondVertexY - m_firstVertexY);
    
    if (std::abs(determinant) < 1e-10) {
        std::cout << "ERROR: Points are collinear - cannot form a triangle." << std::endl;
        return false;
    }
    
    return true;
}

void TriangleAreaCalculator::CalculateArea() {
    const double determinant = (m_secondVertexX - m_firstVertexX) * (m_thirdVertexY - m_firstVertexY) 
                             - (m_thirdVertexX - m_firstVertexX) * (m_secondVertexY - m_firstVertexY);
    
    m_calculatedArea = 0.5 * std::abs(determinant);
}

void TriangleAreaCalculator::DisplayResults() const {
    std::cout << "==============================================" << std::endl;
    std::cout << "               CALCULATION RESULTS" << std::endl;
    std::cout << "==============================================" << std::endl;
    
    std::cout << "Vertex coordinates:" << std::endl;
    std::cout << "  A: (" << m_firstVertexX << ", " << m_firstVertexY << ")" << std::endl;
    std::cout << "  B: (" << m_secondVertexX << ", " << m_secondVertexY << ")" << std::endl;
    std::cout << "  C: (" << m_thirdVertexX << ", " << m_thirdVertexY << ")" << std::endl;
    std::cout << std::endl;
    
    std::cout << "Calculated area: " << std::fixed << std::setprecision(2) 
              << m_calculatedArea << " square units" << std::endl;
    
    std::cout << "==============================================" << std::endl;
}

void TriangleAreaCalculator::VisualizeTriangle() const {
    std::cout << std::endl << "TRIANGLE VISUALIZATION:" << std::endl;
    std::cout << "----------------------------------------------" << std::endl;
    
    const int width = 60;
    const int height = 30;
    
    char grid[height][width];
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            grid[i][j] = ' ';
        }
    }
    
    double minX = (std::min)((std::min)(m_firstVertexX, m_secondVertexX), m_thirdVertexX);
    double maxX = (std::max)((std::max)(m_firstVertexX, m_secondVertexX), m_thirdVertexX);
    double minY = (std::min)((std::min)(m_firstVertexY, m_secondVertexY), m_thirdVertexY);
    double maxY = (std::max)((std::max)(m_firstVertexY, m_secondVertexY), m_thirdVertexY);
    
    double paddingX = (maxX - minX) * 0.1;
    double paddingY = (maxY - minY) * 0.1;
    if (paddingX == 0) paddingX = 1.0;
    if (paddingY == 0) paddingY = 1.0;
    
    minX -= paddingX;
    maxX += paddingX;
    minY -= paddingY;
    maxY += paddingY;
    
    auto scaleX = [&](double x) -> int {
        return static_cast<int>((x - minX) / (maxX - minX) * (width - 1));
    };
    
    auto scaleY = [&](double y) -> int {
        return static_cast<int>((y - minY) / (maxY - minY) * (height - 1));
    };
    
    int x1 = scaleX(m_firstVertexX);
    int y1 = scaleY(m_firstVertexY);
    int x2 = scaleX(m_secondVertexX);
    int y2 = scaleY(m_secondVertexY);
    int x3 = scaleX(m_thirdVertexX);
    int y3 = scaleY(m_thirdVertexY);
    
    auto getLineSymbol = [](int dx, int dy) -> char {
        if (dx == 0) return '|'; 
        if (dy == 0) return '-'; 
        
        double ratio = std::abs(static_cast<double>(dy) / std::abs(dx));
        if (ratio < 0.3) return '-';  
        if (ratio > 3.0) return '|';    
        if ((dx > 0 && dy < 0) || (dx < 0 && dy > 0)) return '/';
        return '\\';
    };
    
    auto drawLine = [&](int x0, int y0, int x1, int y1) {
        int dx = std::abs(x1 - x0);
        int dy = std::abs(y1 - y0);
        int sx = (x0 < x1) ? 1 : -1;
        int sy = (y0 < y1) ? 1 : -1;
        int err = dx - dy;
        
        char symbol = getLineSymbol(x1 - x0, y1 - y0);
        
        int currentX = x0;
        int currentY = y0;
        
        while (true) {
            if (currentX >= 0 && currentX < width && currentY >= 0 && currentY < height) {
                if (!(currentX == x0 && currentY == y0) && 
                    !(currentX == x1 && currentY == y1)) {
                    grid[currentY][currentX] = symbol;
                }
            }
            
            if (currentX == x1 && currentY == y1) break;
            
            int e2 = 2 * err;
            if (e2 > -dy) {
                err -= dy;
                currentX += sx;
            }
            if (e2 < dx) {
                err += dx;
                currentY += sy;
            }
        }
    };
    
    drawLine(x1, y1, x2, y2);
    drawLine(x2, y2, x3, y3);
    drawLine(x3, y3, x1, y1);
    
    if (x1 >= 0 && x1 < width && y1 >= 0 && y1 < height) grid[y1][x1] = 'A';
    if (x2 >= 0 && x2 < width && y2 >= 0 && y2 < height) grid[y2][x2] = 'B';
    if (x3 >= 0 && x3 < width && y3 >= 0 && y3 < height) grid[y3][x3] = 'C';
    
    for (int i = height - 1; i >= 0; --i) {
        for (int j = 0; j < width; ++j) {
            std::cout << grid[i][j];
        }
        std::cout << std::endl;
    }
    
    std::cout << "----------------------------------------------" << std::endl;
}

bool TriangleAreaCalculator::AskForAnotherCalculation() {
    char choice;
    
    while (true) {
        std::cout << std::endl;
        std::cout << "Do you want to perform another calculation? (y/n): ";
        std::cin >> choice;
        std::cin.ignore(10000, '\n');
        
        if (choice == 'y' || choice == 'Y') {
            return true;
        }
        else if (choice == 'n' || choice == 'N') {
            return false;
        }
        else {
            std::cout << "Invalid input. Please enter 'y' for yes or 'n' for no." << std::endl;
        }
    }
}

void TriangleAreaCalculator::Run() {
    InitializeConsole();
    
    bool continueCalculations = true;
    
    while (continueCalculations) {
        DisplayWelcomeMessage();
        GetUserInput();
        
        if (!ValidateInput()) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }
        
        CalculateArea();
        VisualizeTriangle(); 
        DisplayResults();
        
        continueCalculations = AskForAnotherCalculation();
    }
    
    std::cout << std::endl;
    std::cout << "Thank you for using Triangle Calculator!" << std::endl;
}