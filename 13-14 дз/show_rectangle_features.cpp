#include <iostream>
#include <utility>

class Rectangle {
private:
    int side_a;  // сторона a
    int side_b;  // сторона b
    static int instance_count;  // статическое поле для подсчета экземпляров

    // Вспомогательная функция для проверки корректности сторон
    void validate_sides(int a, int b) const {
        if (a <= 0 || b <= 0) {
            throw std::invalid_argument("Стороны прямоугольника должны быть положительными числами");
        }
    }

public:
    // Конструктор без параметров
    Rectangle() : side_a(1), side_b(1) {
        ++instance_count;
    }

    // Конструктор инициализации
    Rectangle(int a, int b) {
        validate_sides(a, b);
        side_a = a;
        side_b = b;
        ++instance_count;
    }

    // Конструктор копирования
    Rectangle(const Rectangle& other) : side_a(other.side_a), side_b(other.side_b) {
        ++instance_count;
    }

    // Оператор присваивания
    Rectangle& operator=(const Rectangle& other) {
        if (this != &other) {
            side_a = other.side_a;
            side_b = other.side_b;
        }
        return *this;
    }

    // Деструктор
    ~Rectangle() {
        --instance_count;
    }

    // Методы для получения информации
    void print_info() const {
        std::cout << "Прямоугольник со сторонами: a = " << side_a 
                  << ", b = " << side_b << std::endl;
    }

    int calculate_perimeter() const {
        return 2 * (side_a + side_b);
    }

    int calculate_area() const {
        return side_a * side_b;
    }

    // Установка сторон с проверкой
    void set_sides(int new_a, int new_b) {
        validate_sides(new_a, new_b);
        side_a = new_a;
        side_b = new_b;
    }

    // Проверка, является ли прямоугольник квадратом
    bool is_square() const {
        return side_a == side_b;
    }

    // Геттеры
    int get_side_a() const { return side_a; }
    int get_side_b() const { return side_b; }

    // Статический метод для получения количества экземпляров
    static int get_instance_count() {
        return instance_count;
    }

    // Перегрузка оператора ++ (префиксная форма)
    Rectangle& operator++() {
        ++side_a;
        ++side_b;
        return *this;
    }

    // Перегрузка оператора ++ (постфиксная форма)
    Rectangle operator++(int) {
        Rectangle temp = *this;
        ++(*this);
        return temp;
    }

    // Перегрузка оператора -- (префиксная форма)
    Rectangle& operator--() {
        // Проверяем, не станут ли стороны неположительными
        if (side_a <= 1 || side_b <= 1) {
            throw std::invalid_argument("Нельзя уменьшить стороны: они станут неположительными");
        }
        --side_a;
        --side_b;
        return *this;
    }

    // Перегрузка оператора -- (постфиксная форма)
    Rectangle operator--(int) {
        Rectangle temp = *this;
        --(*this);
        return temp;
    }

    // Перегрузка оператора * для умножения на скаляр
    Rectangle& operator*(int scalar) {
        if (scalar <= 0) {
            throw std::invalid_argument("Скаляр должен быть положительным числом");
        }
        side_a *= scalar;
        side_b *= scalar;
        return *this;
    }
};

// Инициализация статического поля
int Rectangle::instance_count = 0;

// Свободная функция для демонстрации работы класса
void demonstrate_rectangle_operations() {
    std::cout << "\n=== Демонстрация работы класса Rectangle ===" << std::endl;

    try {
        // Создание прямоугольников
        Rectangle rect1;
        std::cout << "\n1. Прямоугольник по умолчанию:" << std::endl;
        rect1.print_info();
        
        Rectangle rect2(3, 4);
        std::cout << "\n2. Прямоугольник 3x4:" << std::endl;
        rect2.print_info();
        std::cout << "   Периметр: " << rect2.calculate_perimeter() << std::endl;
        std::cout << "   Площадь: " << rect2.calculate_area() << std::endl;
        std::cout << "   Это квадрат? " << (rect2.is_square() ? "Да" : "Нет") << std::endl;

        // Проверка на квадрат
        Rectangle rect3(5, 5);
        std::cout << "\n3. Прямоугольник 5x5:" << std::endl;
        rect3.print_info();
        std::cout << "   Это квадрат? " << (rect3.is_square() ? "Да" : "Нет") << std::endl;

        // Изменение сторон
        std::cout << "\n4. Изменение сторон прямоугольника 3x4 на 6x7:" << std::endl;
        rect2.set_sides(6, 7);
        rect2.print_info();

        // Операции ++
        std::cout << "\n5. Применение ++ к прямоугольнику 3x4:" << std::endl;
        Rectangle rect4(3, 4);
        rect4.print_info();
        ++rect4;
        std::cout << "   После ++rect: ";
        rect4.print_info();
        
        Rectangle rect5(3, 4);
        rect5++;
        std::cout << "   После rect++: ";
        rect5.print_info();

        // Операции --
        std::cout << "\n6. Применение -- к прямоугольнику 5x5:" << std::endl;
        Rectangle rect6(5, 5);
        rect6.print_info();
        --rect6;
        std::cout << "   После --rect: ";
        rect6.print_info();

        // Умножение на скаляр
        std::cout << "\n7. Умножение прямоугольника 2x3 на скаляр 3:" << std::endl;
        Rectangle rect7(2, 3);
        rect7.print_info();
        rect7 * 3;
        std::cout << "   После rect * 3: ";
        rect7.print_info();

        // Демонстрация исключений
        std::cout << "\n8. Демонстрация обработки исключений:" << std::endl;
        
        try {
            Rectangle invalid_rect(0, 5);
        } catch (const std::invalid_argument& e) {
            std::cout << "   Исключение при создании: " << e.what() << std::endl;
        }

        try {
            Rectangle rect8(2, 2);
            --(--rect8);  // Уменьшим дважды
        } catch (const std::invalid_argument& e) {
            std::cout << "   Исключение при --: " << e.what() << std::endl;
        }

        try {
            Rectangle rect9(2, 3);
            rect9 * 0;  // Умножение на 0
        } catch (const std::invalid_argument& e) {
            std::cout << "   Исключение при *: " << e.what() << std::endl;
        }

        // Подсчет экземпляров
        std::cout << "\n9. Количество экземпляров Rectangle: " 
                  << Rectangle::get_instance_count() << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Неожиданная ошибка: " << e.what() << std::endl;
    }
}

int main() {
    system("chcp 65001 > nul");
    
    demonstrate_rectangle_operations();
    std::cout << "\nПрограмма завершена. Оставшиеся экземпляры: " 
              << Rectangle::get_instance_count() << std::endl;
    
    return 0;
}