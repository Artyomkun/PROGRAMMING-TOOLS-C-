#include "geometry3d.hpp"
#include <iostream>
#include <locale>

using namespace Geometry3D;

int main() {
    std::setlocale(LC_ALL, ".65001");
    try {
        std::cout << "=== Система геометрии экспертного уровня ===\n" << std::endl;
        
        AdvancedRectangle<HeapStorage, StrictValidation, JSONSerialization> rect(3.0, 4.0);
        
        std::cout << "1. Продвинутый прямоугольник:" << std::endl;
        std::cout << "   Площадь: " << rect.area() << std::endl;
        std::cout << "   Периметр: " << rect.perimeter() << std::endl;
        std::cout << "   Центр масс: " << rect.centroid() << std::endl;
        std::cout << "   Количество обращений: " << rect.access_count() << std::endl;
        std::cout << "   В формате JSON: " << rect.serialize() << std::endl;
        
        std::cout << "\n2. Генератор точек на поверхности:" << std::endl;
        auto points = rect.generate_points(2.0);
        size_t count = 0;
        for (auto&& point : points) {
            if (count++ < 5) {
                std::cout << "   Точка " << count << ": " << point << std::endl;
            }
        }
        
        std::cout << "\n3. Точки на границе фигуры:" << std::endl;
        size_t point_count = 0;
        for (auto&& point : rect.get_boundary_points(4)) {
            if (point_count++ < 4) {
                std::cout << "   Граничная точка " << point_count << ": " << point << std::endl;
            }
        }
        
        std::cout << "\n4. Параллельный расчет площади: " << rect.parallel_area() << std::endl;
        
        std::cout << "\n5. Соотношение сторон прямоугольника: ";
        auto ratio = rect.aspect_ratio();
        if (ratio.has_value) {
            std::cout << ratio.value << std::endl;
        } else {
            std::cout << "Ошибка: высота равна нулю" << std::endl;
        }
        
        std::cout << "\n6. Сопоставление с образцом:" << std::endl;
        AdvancedRectangle<HeapStorage, StrictValidation, JSONSerialization>::ShapeVariant shape = rect;
        std::cout << "   " << AdvancedRectangle<HeapStorage, StrictValidation, JSONSerialization>::match_shape(shape) << std::endl;
        
        std::cout << "\n7. Асинхронный расчет площади:" << std::endl;
        auto future = rect.async_area();
        std::cout << "   Результат асинхронного расчета: " << future.get() << std::endl;
        
        std::cout << "\n8. Прямоугольники золотого сечения:" << std::endl;
        auto golden_rects = AdvancedRectangle<HeapStorage, StrictValidation, JSONSerialization>::generate_golden_rectangles<3>(100.0);
        
        for (size_t i = 0; i < golden_rects.size(); ++i) {
            std::cout << "   Прямоугольник " << i + 1 << ": площадь = " << golden_rects[i].area() 
                        << ", соотношение = " << golden_rects[i].width() / golden_rects[i].height() << std::endl;
        }
        
        std::cout << "\n9. Тест производительности:" << std::endl;
        auto benchmark_func = [&rect]() {
            volatile double result = rect.area();
            (void)result;
        };

        auto time = Benchmark::measure(benchmark_func, 100000);
        std::cout << "   Среднее время расчета площади: " << time << " мс" << std::endl;
        
        std::cout << "\n10. Экспертное управление параллельными вычислениями:" << std::endl;
        
        ThreadManager thread_manager;
        
        thread_manager.execute_sequentially([&rect](int id) {
            double factor = 1.0 + id * 0.1;
            rect.scale(factor);
        }, 4, "Масштабирование");
        
        std::cout << "   Итоговая площадь после масштабирования: " << rect.area() << std::endl;
        
        thread_manager.execute_with_barrier([](int id, Barrier& barrier) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100 * (id + 1)));
            barrier.arrive_and_wait();
        }, 4, "Синхронизация");
        
        std::cout << "\n11. Демонстрация обработки исключений:" << std::endl;
        try {
            AdvancedRectangle<HeapStorage, StrictValidation, JSONSerialization> invalid_rect(0, -5);
        } catch (const std::exception& e) {
            std::cout << "   Обнаружена ошибка: " << e.what() << std::endl;
        }
        
        std::cout << "\n12. Инспекция свойств прямоугольника:" << std::endl;
        rect.visit_members([](const std::string& name, const auto& value) {
            if (name == "top_left") {
                std::cout << "   Левый верхний угол: " << value << std::endl;
            } else if (name == "width") {
                std::cout << "   Ширина: " << value << std::endl;
            } else if (name == "height") {
                std::cout << "   Высота: " << value << std::endl;
            }
        });
        
        std::cout << "\n13. Демонстрация стирания типов:" << std::endl;
        ShapeFactory::AnyShape any_shape(rect);
        std::cout << "   Площадь (через стирание типов): " << any_shape.area() << std::endl;
        std::cout << "   Периметр (через стирание типов): " << any_shape.perimeter() << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "\nКритическая ошибка: " << e.what() << std::endl;
        return 1;
    }
    
    std::cout << "\n=== Программа успешно завершена ===" << std::endl;
    return 0;
}