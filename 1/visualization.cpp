#include "visualization.h"
#include "geometry.h"

void printProgramDescription() {
    std::ios_base::sync_with_stdio(false);
    std::wcout.imbue(std::locale("ru_RU.UTF-8"));
    std::wcin.imbue(std::locale("ru_RU.UTF-8"));
    
    setlocale(LC_ALL, "ru_RU.UTF-8");
    
    cout << fmt::format("{:=^50}", " ПРОВЕРКА ПРИНАДЛЕЖНОСТИ ТОЧКИ ОБЛАСТИ ") << endl;
    cout << "Границы области:" << endl;
    cout << "• Парабола: x = y²/4 - 5" << endl;
    cout << "• Окружность: x² + y² = 64" << endl;
    cout << "• Прямые: y = -3 и y = 3" << endl;
    cout << fmt::format("{:=^50}", "") << endl << endl;
}

void printResultMessage(const string& result, double x, double y) {
    setlocale(LC_ALL, "ru_RU.UTF-8");

    string pointStr = fmt::format("({:.2f}, {:.2f})", x, y);
    
    cout << endl << fmt::format("{:=^50}", " РЕЗУЛЬТАТ ") << endl;
    
    if (result == "Да") {
        cout << fmt::format("Точка {} - ВНУТРИ области", pointStr) << endl;
    } else if (result == "На границе") {
        cout << fmt::format("Точка {} - НА ГРАНИЦЕ области", pointStr) << endl;
    } else {
        cout << fmt::format("Точка {} - ВНЕ области", pointStr) << endl;
    }
    
    cout << fmt::format("{:=^50}", "") << endl;
}

void drawCoordinateSystem(double userX, double userY) {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    
    const int gridWidth = 50;
    const int gridHeight = 20;
    const double xMin = -9.0;
    const double xMax = 9.0;
    const double yMin = -4.0;
    const double yMax = 4.0;
    
    double xStep = (xMax - xMin) / gridWidth;
    double yStep = (yMax - yMin) / gridHeight;
    
    cout << "   y" << endl;
    cout << "   ↑" << endl;
    
    for (int row = 0; row <= gridHeight; ++row) {
        double currentY = yMax - row * yStep;
        
        if (row % 4 == 0) {
            cout << fmt::format("{:2.0f} | ", currentY);
        } else {
            cout << "    | ";
        }
        
        for (int column = 0; column <= gridWidth; ++column) {
            double currentX = xMin + column * xStep;
            cout << getGridCharacter(currentX, currentY, userX, userY);
        }
        cout << endl;
    }
    
    cout << "     +";
    for (int j = 0; j < gridWidth; ++j) {
        cout << "--";
    }
    cout << "> x" << endl;
    
    printLegend(userX, userY);
}

void printLegend(double userX, double userY) {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    
    cout << endl << "ЛЕГЕНДА:" << endl;
    cout << "----------------------------------------" << endl;
    cout << "  #  - граница области" << endl;
    cout << "  .  - внутренняя область" << endl;
    cout << fmt::format("  *  - ваша точка ({:.1f}, {:.1f})", userX, userY) << endl;
    cout << "----------------------------------------" << endl;
}

string getGridCharacter(double x, double y, double userX, double userY) {
    if (fabs(x - userX) < 0.5 && fabs(y - userY) < 0.3) {
        return "*";
    }
    
    if (is_point_on_parabola(x, y) || is_point_on_circle(x, y) || is_point_on_horizontal_boundary(y)) {
        return "#";
    }
    
    if (is_point_inside_area(x, y)) {
        return ".";
    }
    
    return " ";
}