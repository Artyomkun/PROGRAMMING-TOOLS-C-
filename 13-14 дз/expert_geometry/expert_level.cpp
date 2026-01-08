#include <condition_variable>
#include <type_traits>
#include <filesystem>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <future>
#include <atomic>
#include <locale>
#include <string>
#include <memory>
#include <vector>
#include <numeric>
#include <chrono>
#include <random>
#include <mutex>
#include <array>
#include <cmath>
#include <any>

const double PI = std::acos(-1.0);

class Barrier {
    std::mutex mtx_;
    std::condition_variable cv_;
    size_t count_;
    size_t waiting_;
    size_t generation_;

    public:
        explicit Barrier(size_t count) : count_(count), waiting_(0), generation_(0) {}

        void arrive_and_wait() {
            std::unique_lock<std::mutex> lock(mtx_);
            size_t gen = generation_;
            ++waiting_;
            if (waiting_ == count_) {
                ++generation_;
                waiting_ = 0;
                cv_.notify_all();
            } else {
                cv_.wait(lock, [this, gen] { return gen != generation_; });
            }
        }
};

struct OptionalDouble {
    double value;
    bool has_value;
    OptionalDouble() : has_value(false) {}
    OptionalDouble(double v) : value(v), has_value(true) {}
};

namespace Geometry3D {

    template<typename Derived>
    class ShapeCRTP {
        public:
            auto area() const { return derived().area_impl(); }
            auto perimeter() const { return derived().perimeter_impl(); }
            auto centroid() const { return derived().centroid_impl(); }
            
        private:
            const Derived& derived() const { 
                return static_cast<const Derived&>(*this); 
            }
    };

    template<typename T, size_t N>
    class Point {
        std::array<T, N> coords_;
        
        public:
            constexpr Point() : coords_{} {}
            
            template<typename... Args>
            constexpr Point(Args... args) : coords_{static_cast<T>(args)...} {}
            
            template<size_t I>
            constexpr T get() const { return coords_[I]; }
            
            constexpr T operator[](size_t i) const { return coords_[i]; }
            
            template<typename U>
            using distance_type = std::conditional_t<std::is_integral_v<T>, double, T>;
            
            template<typename U>
            auto distance(const Point<U, N>& other) const {
                using ResultType = std::common_type_t<T, U>;
                ResultType sum{};
                for (size_t i = 0; i < N; ++i) {
                    auto diff = static_cast<ResultType>(coords_[i]) - static_cast<ResultType>(other[i]);
                    sum += diff * diff;
                }
                return std::sqrt(sum);
            }
            
            friend std::ostream& operator<<(std::ostream& os, const Point& p) {
                os << "(";
                for (size_t i = 0; i < N; ++i) {
                    if (i > 0) os << ", ";
                    os << p[i];
                }
                os << ")";
                return os;
            }
    };

    template<typename StoragePolicy, typename ValidationPolicy, typename SerializationPolicy>
    class AdvancedRectangle : 
        public ShapeCRTP<AdvancedRectangle<StoragePolicy, ValidationPolicy, SerializationPolicy>>,
        private StoragePolicy,
        private ValidationPolicy,
        private SerializationPolicy {
    
            using PointType = Point<double, 2>;
            PointType top_left_;
            double width_, height_;
            mutable std::atomic<int> access_count_{0};
            
            public:
                using value_type = double;
                using point_type = PointType;
                
                AdvancedRectangle() 
                    : top_left_(0, 0),
                    width_(1.0), height_(1.0) {
                    this->validate(width_, height_);
                }
                
                template<typename W, typename H>
                AdvancedRectangle(W w, H h) 
                    : top_left_(0, 0),
                    width_(static_cast<double>(w)), 
                    height_(static_cast<double>(h)) {
                    this->validate(width_, height_);
                }
                
                AdvancedRectangle(const PointType& top_left, double w, double h) 
                    : top_left_(top_left), width_(w), height_(h) {
                    this->validate(width_, height_);
                }
                
                AdvancedRectangle(AdvancedRectangle&& other) noexcept
                    : top_left_(std::move(other.top_left_)),
                    width_(other.width_),
                    height_(other.height_) {
                    other.width_ = other.height_ = 0.0;
                }
                
                AdvancedRectangle(const AdvancedRectangle& other)
                    : top_left_(other.top_left_),
                    width_(other.width_),
                    height_(other.height_) {}
                
                AdvancedRectangle& operator=(const AdvancedRectangle& other) {
                    if (this != &other) {
                        top_left_ = other.top_left_;
                        width_ = other.width_;
                        height_ = other.height_;
                        this->validate(width_, height_);
                    }
                    return *this;
                }
                
                ~AdvancedRectangle() = default;
                
                double width() const noexcept {
                    access_count_.fetch_add(1, std::memory_order_relaxed);
                    return width_;
                }
                
                double height() const noexcept {
                    access_count_.fetch_add(1, std::memory_order_relaxed);
                    return height_;
                }
                
                int access_count() const noexcept {
                    return access_count_.load(std::memory_order_relaxed);
                }
                
                OptionalDouble safe_divide(double numerator) const {
                    if (std::fabs(width_) < 1e-10 || std::fabs(height_) < 1e-10) {
                        return OptionalDouble();
                    }
                    return OptionalDouble(numerator / (width_ * height_));
                }

                OptionalDouble aspect_ratio() const {
                    if (std::fabs(height_) < 1e-10) {
                        return OptionalDouble();
                    }
                    return OptionalDouble(width_ / height_);
                }
                
                std::vector<double> generate_points(double step = 1.0) const {
                    std::vector<double> points;
                    for (double x = top_left_.get<0>(); x < top_left_.get<0>() + width_; x += step) {
                        for (double y = top_left_.get<1>(); y < top_left_.get<1>() + height_; y += step) {
                            points.push_back(x * y);
                        }
                    }
                    return points;
                }
                
                auto get_boundary_points(size_t segments) const {
                    std::vector<PointType> points;
                    for (size_t i = 0; i < segments; ++i) {
                        double angle = 2.0 * PI * i / segments;
                        points.emplace_back(
                            top_left_.get<0>() + width_/2.0 + width_/2.0 * std::cos(angle),
                            top_left_.get<1>() + height_/2.0 + height_/2.0 * std::sin(angle)
                        );
                    }
                    return points;
                }
                
                double parallel_area() const {
                    auto concurrency = std::thread::hardware_concurrency();
                    if (concurrency == 0) concurrency = 1;
                    std::vector<double> areas(concurrency);
                    std::transform(areas.begin(), areas.end(), areas.begin(),
                        [this](auto&) { return this->area_impl(); });
                    return std::accumulate(areas.begin(), areas.end(), 0.0);
                }
                
                template<typename Visitor>
                void visit_members(Visitor&& visitor) {
                    visitor("top_left", top_left_);
                    visitor("width", width_);
                    visitor("height", height_);
                }
                
                std::string serialize() const {
                    return this->serialize_impl(*this);
                }
                
                struct ShapeVariant {

                    enum Type { 
                        Rectangle, 
                        Number, 
                        Str 
                    };

                    Type type;
                    AdvancedRectangle rect;
                    double num;
                    std::string str;
                    ShapeVariant(const AdvancedRectangle& r) : type(Rectangle), rect(r) 
                    {

                    }
                    ShapeVariant(double n) : type(Number), num(n) 
                    {

                    }
                    ShapeVariant(const std::string& s) : type(Str), str(s) 
                    {

                    }
                };
                static std::string match_shape(const ShapeVariant& shape) {
                    std::ostringstream oss;
                    switch (shape.type) {
                        case ShapeVariant::Rectangle:
                            oss << "Прямоугольник с площадью: " << shape.rect.area_impl();
                            return oss.str();
                        case ShapeVariant::Number:
                            oss << "Число: " << shape.num;
                            return oss.str();
                        case ShapeVariant::Str:
                            return "Строка: " + shape.str;
                    }
                    return "";
                }
                
                static constexpr double golden_ratio() {
                    return (1.0 + std::sqrt(5.0)) / 2.0;
                }
                
                template<size_t Count>
                static std::array<AdvancedRectangle, Count> generate_golden_rectangles(double start_area) {
                    std::array<AdvancedRectangle, Count> result;
                    double area = start_area;
                    const double gr = golden_ratio();
                    
                    for (size_t i = 0; i < Count; ++i) {
                        double height = std::sqrt(area / gr);
                        double width = area / height;
                        result[i] = AdvancedRectangle(width, height);
                        area *= gr;
                    }
                    return result;
                }
                
                std::future<double> async_area() const {
                    return std::async(std::launch::async, [this]() {
                        std::this_thread::sleep_for(std::chrono::milliseconds(10));
                        return this->area_impl();
                    });
                }
                
                void scale(double factor) {
                    static std::mutex mtx;
                    std::lock_guard<std::mutex> lock(mtx);
                    width_ *= factor;
                    height_ *= factor;
                    this->validate(width_, height_);
                }
                double area_impl() const {
                    return width_ * height_;
                }
                double perimeter_impl() const {
                    return 2.0 * (width_ + height_);
                }
                PointType centroid_impl() const {
                    return PointType(top_left_.get<0>() + width_ / 2.0, top_left_.get<1>() + height_ / 2.0);
                }
            private:
                template<typename, typename, typename>
                friend class AdvancedRectangle;
    };

    struct HeapStorage {
        std::unique_ptr<double[]> cache_;
        void cache_result(double value) {
            cache_ = std::unique_ptr<double[]>(new double[1]);
            cache_[0] = value;
        }
        OptionalDouble get_cached() const {
            if (cache_) return OptionalDouble(cache_[0]);
            return OptionalDouble();
        }
    };

    struct StrictValidation {
        void validate(double w, double h) const {
            if (w <= 0.0 || h <= 0.0) {
                throw std::invalid_argument("Размеры должны быть положительными");
            }
            if (w > 1e6 || h > 1e6) {
                throw std::overflow_error("Размеры слишком большие");
            }
        }
    };

    struct JSONSerialization {
        template<typename Rect>
        std::string serialize_impl(const Rect& rect) const {
            std::ostringstream oss;
            oss << "{\"тип\": \"прямоугольник\", \"ширина\": " << rect.width()
                << ", \"высота\": " << rect.height()
                << ", \"площадь\": " << rect.area() << "}";
            return oss.str();
        }
    };

    class ShapeFactory {
        public:
            template<typename Shape, typename... Args>
            static std::unique_ptr<Shape> create(Args&&... args) {
                return std::make_unique<Shape>(std::forward<Args>(args)...);
            }
            
            class AnyShape {
                struct Concept {
                    virtual ~Concept() = default;
                    virtual double area() const = 0;
                    virtual double perimeter() const = 0;
                    virtual std::unique_ptr<Concept> clone() const = 0;
                };
                
                template<typename Shape>
                struct Model : Concept {
                    Shape shape_;
                    
                    Model(Shape shape) : shape_(std::move(shape)) {}
                    
                    double area() const override { return shape_.area(); }
                    double perimeter() const override { return shape_.perimeter(); }
                    std::unique_ptr<Concept> clone() const override {
                        return std::make_unique<Model>(*this);
                    }
                };
                
                std::unique_ptr<Concept> self_;
            
                public:
                    template<typename Shape>
                    AnyShape(Shape shape) : self_(std::make_unique<Model<Shape>>(std::move(shape))) {}
                    
                    AnyShape(const AnyShape& other) : self_(other.self_->clone()) {}
                    
                    double area() const { return self_->area(); }
                    double perimeter() const { return self_->perimeter(); }
            };
    };

    class Benchmark {
        public:
            template<typename Func>
            static auto measure(Func func, size_t iterations = 1000) {
                auto start = std::chrono::high_resolution_clock::now();
                for (size_t i = 0; i < iterations; ++i) {
                    func();
                }
                auto end = std::chrono::high_resolution_clock::now();
                return std::chrono::duration<double, std::milli>(end - start).count() / iterations;
            }
    };

    class ThreadManager {
        private:
            std::mutex cout_mutex_;
            
            void safe_print(int thread_id, const std::string& message) {
                std::lock_guard<std::mutex> lock(cout_mutex_);
                std::cout << "   Поток " << thread_id << ": " << message << std::endl;
            }
        
        public:
            void execute_sequentially(std::function<void(int)> task, int count, const std::string& stage_name) {
                std::cout << "\n   Этап \"" << stage_name << "\":" << std::endl;
                
                for (int i = 0; i < count; ++i) {
                    std::thread t([this, i, &task]() {
                        safe_print(i, "начал работу");
                        task(i);
                        safe_print(i, "завершил работу");
                    });
                    
                    t.join();
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                }
                
                std::cout << "   Все потоки завершили этап \"" << stage_name << "\"" << std::endl;
            }
            
            void execute_with_barrier(std::function<void(int, Barrier&)> task, int count, const std::string& stage_name) {
                std::cout << "\n   Этап \"" << stage_name << "\":" << std::endl;
                
                Barrier barrier(count);
                std::vector<std::thread> parallel_threads;
                
                for (int i = 0; i < count; ++i) {
                    parallel_threads.emplace_back([this, i, &barrier, &task]() {
                        safe_print(i, "начал подготовку");
                        std::this_thread::sleep_for(std::chrono::milliseconds(100 * (i + 1)));
                        
                        task(i, barrier);
                        
                        safe_print(i, "завершил работу");
                    });
                }
                
                for (auto& t : parallel_threads) {
                    t.join();
                }
                
                std::cout << "   Все потоки синхронизированы" << std::endl;
            }
    };

} // namespace Geometry3D

int main() {
    std::setlocale(LC_ALL, ".65001");
    using namespace Geometry3D;
    
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
        for (double point : points) {
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
        auto golden_rects = AdvancedRectangle<HeapStorage, StrictValidation, JSONSerialization>
            ::generate_golden_rectangles<3>(100.0);
        
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