#ifndef TRIANGLE_AREA_CALCULATOR_H
#define TRIANGLE_AREA_CALCULATOR_H

#define NOMINMAX

class TriangleAreaCalculator {
public:
    TriangleAreaCalculator();
    void Run();
    
private:
    void InitializeConsole();
    void DisplayWelcomeMessage();
    void GetUserInput();
    bool ValidateInput() const;
    void CalculateArea();
    void DisplayResults() const;
    void VisualizeTriangle() const;
    bool AskForAnotherCalculation();
    
    double m_firstVertexX, m_firstVertexY;
    double m_secondVertexX, m_secondVertexY;
    double m_thirdVertexX, m_thirdVertexY;
    double m_calculatedArea;
};

#endif