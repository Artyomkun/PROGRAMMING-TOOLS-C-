#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>
#include <utility>
#include <clocale>

struct Point
{
    double x;
    double y;
};

double calculate_distance(Point first, Point second)
{
    using std::pow;
    using std::sqrt;

    const double delta_x = second.x - first.x;
    const double delta_y = second.y - first.y;

    return sqrt(pow(delta_x, 2) + pow(delta_y, 2));
}

double calculate_triangle_perimeter(Point point_a,
                                    Point point_b,
                                    Point point_c)
{
    return calculate_distance(point_a, point_b) +
            calculate_distance(point_b, point_c) +
            calculate_distance(point_c, point_a);
}

std::pair<std::vector<Point>, double> find_minimum_perimeter_triangle(const std::vector<Point> &points)
{
    const size_t POINT_COUNT = points.size();
    const size_t MIN_POINTS_FOR_TRIANGLE = 3;

    if (POINT_COUNT < MIN_POINTS_FOR_TRIANGLE)
    {
        std::cerr << "Ошибка: недостаточно точек для построения треугольника." << std::endl;
        return std::make_pair(std::vector<Point>(), std::numeric_limits<double>::max());
    }

    std::vector<Point> best_triangle(3);
    double min_perimeter = std::numeric_limits<double>::max();

    for (size_t i = 0; i < POINT_COUNT - 2; ++i)
    {
        for (size_t j = i + 1; j < POINT_COUNT - 1; ++j)
        {
            for (size_t k = j + 1; k < POINT_COUNT; ++k)
            {
                const Point p1 = points[i];
                const Point p2 = points[j];
                const Point p3 = points[k];

                const double current_perimeter = calculate_triangle_perimeter(p1, p2, p3);

                if (current_perimeter < min_perimeter)
                {
                    min_perimeter = current_perimeter;
                    best_triangle[0] = p1;
                    best_triangle[1] = p2;
                    best_triangle[2] = p3;
                }
            }
        }
    }

    return std::make_pair(best_triangle, min_perimeter);
}

void print_triangle_info(const std::vector<Point> &triangle, double perimeter)
{
    if (triangle.empty())
    {
        std::cout << "Треугольник не найден." << std::endl;
        return;
    }

    std::cout << "Треугольник с минимальным периметром:" << std::endl;
    std::cout << "Вершина A: (" << triangle[0].x << ", " << triangle[0].y << ")" << std::endl;
    std::cout << "Вершина B: (" << triangle[1].x << ", " << triangle[1].y << ")" << std::endl;
    std::cout << "Вершина C: (" << triangle[2].x << ", " << triangle[2].y << ")" << std::endl;
    std::cout << "Периметр: " << perimeter << std::endl;
}

void print_all_points(const std::vector<Point> &points)
{
    const size_t POINT_COUNT = points.size();

    std::cout << "Заданное множество точек (" << POINT_COUNT << " точек):" << std::endl;

    for (size_t i = 0; i < POINT_COUNT; ++i)
    {
        std::cout << "Точка " << (i + 1) << ": ("
                    << points[i].x << ", " << points[i].y << ")" << std::endl;
    }
}

std::vector<Point> get_points_from_user()
{
    std::vector<Point> points;
    int point_count;

    std::cout << "Введите количество точек (минимум 3): ";
    std::cin >> point_count;

    if (std::cin.fail())
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Неверный ввод. Используется 3 точки по умолчанию." << std::endl;
        point_count = 3;
    }

    const int FINAL_POINT_COUNT = (point_count < 3) ? 3 : point_count;

    points.reserve(FINAL_POINT_COUNT);

    std::cout << "Введите координаты " << FINAL_POINT_COUNT << " точек (x y):" << std::endl;

    for (int i = 0; i < FINAL_POINT_COUNT; ++i)
    {
        Point current_point;

        std::cout << "Точка " << (i + 1) << ": ";
        std::cin >> current_point.x >> current_point.y;

        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Неверные координаты. Используется (0, 0)." << std::endl;
            current_point.x = 0.0;
            current_point.y = 0.0;
        }

        points.push_back(current_point);
    }

    return points;
}

int main()
{
    #ifdef _WIN32
        system("chcp 65001 > nul");
    #endif

    std::setlocale(LC_ALL, "ru_RU.UTF-8");

    std::cout << "==========================================" << std::endl;
    std::cout << "   Поиск треугольника с минимальным периметром" << std::endl;
    std::cout << "==========================================" << std::endl;

    std::vector<Point> points = get_points_from_user();

    std::cout << std::endl;
    print_all_points(points);

    std::cout << std::endl << "Поиск треугольника с минимальным периметром..." << std::endl;

    std::pair<std::vector<Point>, double> result = find_minimum_perimeter_triangle(points);
    std::vector<Point> result_triangle = result.first;
    double perimeter = result.second;

    std::cout << std::endl;
    print_triangle_info(result_triangle, perimeter);

    std::cout << "==========================================" << std::endl;

    return 0;
}