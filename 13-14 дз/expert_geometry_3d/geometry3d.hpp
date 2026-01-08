#ifndef GEOMETRY3D_HPP
#define GEOMETRY3D_HPP

#include <condition_variable>
#include <type_traits>
#include <filesystem>
#include <algorithm>
#include <iostream> 
#include <sstream>
#include <numeric>
#include <fstream>
#include <memory>
#include <vector>
#include <chrono>
#include <random>
#include <thread>
#include <future>
#include <atomic>
#include <string>
#include <mutex>
#include <array>
#include <cmath>
#include <any>

class Barrier
{
    std::mutex mtx_;
    std::condition_variable cv_;
    size_t count_;
    size_t waiting_;
    size_t generation_;

    public:
        explicit Barrier(size_t count);
        void arrive_and_wait();
};

struct OptionalDouble
{
    double value;
    bool has_value;
    OptionalDouble();
    OptionalDouble(double v);
};

namespace Geometry3D
{
    const double PI = std::acos(-1.0);

    template<typename StoragePolicy, typename ValidationPolicy, typename SerializationPolicy>
    class AdvancedBox;

    template <typename T, size_t N>
    class Point;

    template <typename T, size_t N>
    class Point
    {
        std::array<T, N> coords_;

        public:
            constexpr Point() : coords_{} {}

            template <typename... Args>
            constexpr Point(Args... args) : coords_{static_cast<T>(args)...} {}

            template <size_t I>
            constexpr T get() const { return coords_[I]; }

            constexpr T operator[](size_t i) const { return coords_[i]; }
            T& operator[](size_t i) { return coords_[i]; }

            template <typename U>
            auto distance(const Point<U, N> &other) const
            {
                using ResultType = std::common_type_t<T, U>;
                ResultType sum{};
                for (size_t i = 0; i < N; ++i)
                {
                    auto diff = static_cast<ResultType>(coords_[i]) - static_cast<ResultType>(other[i]);
                    sum += diff * diff;
                }
                return std::sqrt(sum);
            }

            friend std::ostream &operator<<(std::ostream &os, const Point &p)
            {
                os << "(";
                for (size_t i = 0; i < N; ++i)
                {
                    if (i > 0) os << ", ";
                    os << p[i];
                }
                os << ")";
                return os;
            }
    };

    template <typename Derived>
    class ShapeCRTP
    {
        public:
            double area() const {
                return static_cast<const Derived*>(this)->area_impl();
            }

            double perimeter() const {
                return static_cast<const Derived*>(this)->perimeter_impl();
            }

            Point<double, 2> centroid() const {
                return static_cast<const Derived*>(this)->centroid_impl();
            }
    };

    template <typename Derived>
    class ShapeCRTP3D
    {
    public:
        double volume() const {
            return static_cast<const Derived*>(this)->volume_impl();
        }

        double surface_area() const {
            return static_cast<const Derived*>(this)->surface_area_impl();
        }

        Point<double, 3> centroid_3d() const {
            return static_cast<const Derived*>(this)->centroid_3d_impl();
        }
    };

    template <typename StoragePolicy, typename ValidationPolicy, typename SerializationPolicy>
    class AdvancedRectangle : 
        public ShapeCRTP<AdvancedRectangle<StoragePolicy, ValidationPolicy, SerializationPolicy>>,
        private StoragePolicy,
        private ValidationPolicy,
        private SerializationPolicy
    {
        using PointType = Point<double, 2>;
        PointType top_left_;
        double width_, height_;
        mutable std::atomic<int> access_count_{0};

        public:
            using value_type = double;
            using point_type = PointType;

            AdvancedRectangle() : top_left_(0, 0), width_(1.0), height_(1.0) {
                this->validate(width_, height_);
            }
            
            template <typename W, typename H>
            AdvancedRectangle(W w, H h) : top_left_(0, 0), 
                width_(static_cast<double>(w)), 
                height_(static_cast<double>(h)) {
                this->validate(width_, height_);
            }

            AdvancedRectangle(const PointType &top_left, double w, double h) 
                : top_left_(top_left), width_(w), height_(h) {
                this->validate(width_, height_);
            }

            AdvancedRectangle(AdvancedRectangle &&other) noexcept
                : top_left_(std::move(other.top_left_)), width_(other.width_), height_(other.height_) {
                other.width_ = other.height_ = 0.0;
            }

            AdvancedRectangle(const AdvancedRectangle &other)
                : top_left_(other.top_left_), width_(other.width_), height_(other.height_) {}

            AdvancedRectangle &operator=(const AdvancedRectangle &other) {
                if (this != &other) {
                    top_left_ = other.top_left_;
                    width_ = other.width_;
                    height_ = other.height_;
                    this->validate(width_, height_);
                }
                return *this;
            }

            double area_impl() const {
                return width_ * height_;
            }
            
            double perimeter_impl() const {
                return 2.0 * (width_ + height_);
            }
            
            PointType centroid_impl() const {
                return PointType(top_left_[0] + width_ / 2.0, 
                                top_left_[1] + height_ / 2.0);
            }

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
            
            std::vector<PointType> generate_points(double step = 1.0) const {
                std::vector<PointType> points;
                for (double x = top_left_.get<0>(); x < top_left_.get<0>() + width_; x += step) {
                    for (double y = top_left_.get<1>(); y < top_left_.get<1>() + height_; y += step) {
                        points.emplace_back(x, y);
                    }
                }
                return points;
            }
            
            std::vector<PointType> get_boundary_points(size_t /*segments*/) const {
                return {
                    top_left_,
                    PointType(top_left_.get<0>() + width_, top_left_.get<1>()),
                    PointType(top_left_.get<0>() + width_, top_left_.get<1>() + height_),
                    PointType(top_left_.get<0>(), top_left_.get<1>() + height_)
                };
            }
            
            double parallel_area() const {
                return area_impl();
            }
            
            template <typename Visitor>
            void visit_members(Visitor &&visitor) {
                visitor("top_left", top_left_);
                visitor("width", width_);
                visitor("height", height_);
            }
            
            std::string serialize() const {
                return this->serialize_impl(*this);
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

            static constexpr double golden_ratio = 1.6180339887498948482;

            template <size_t Count>
            static std::array<AdvancedRectangle, Count> generate_golden_rectangles(double start_area) {
                std::array<AdvancedRectangle, Count> result;
                double area = start_area;
                const double gr = golden_ratio;

                for (size_t i = 0; i < Count; ++i) {
                    double height = std::sqrt(area / gr);
                    double width = area / height;
                    result[i] = AdvancedRectangle(width, height);
                    area *= gr;
                }
                return result;
            }

            struct ShapeVariant
            {

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

            static std::string match_shape(const ShapeVariant &shape) {
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

        private:
            template <typename, typename, typename>
            friend class AdvancedRectangle;
    };

    template <typename StoragePolicy, typename ValidationPolicy, typename SerializationPolicy>
    class AdvancedBox :
        public ShapeCRTP3D<AdvancedBox<StoragePolicy, ValidationPolicy, SerializationPolicy>>,
        private StoragePolicy,
        private ValidationPolicy,
        private SerializationPolicy
    {
        using PointType = Point<double, 3>;
        std::vector<PointType> vertices_;
        std::vector<size_t> indices_;
        mutable std::atomic<int> access_count_{0};

        public:
            using value_type = double;
            using point_type = PointType;

            AdvancedBox() : vertices_{{0,0,0}, {1,0,0}, {0,1,0}}, indices_{0,1,2} {
                std::cout << "LOG: AdvancedBox default constructor called" << std::endl;
            }
            
            AdvancedBox(const PointType& v1, const PointType& v2, const PointType& v3): vertices_{v1, v2, v3}, indices_{0,1,2} 
            {

            }

            AdvancedBox(AdvancedBox &&other) noexcept: vertices_(std::move(other.vertices_)), indices_(std::move(other.indices_)) 
            {

            }

            AdvancedBox(const AdvancedBox &other): vertices_(other.vertices_), indices_(other.indices_) 
            {

            }

            AdvancedBox &operator=(const AdvancedBox &other) {
                if (this != &other) {
                    vertices_ = other.vertices_;
                    indices_ = other.indices_;
                }
                return *this;
            }

            double volume_impl() const {
                return 0.0;
            }

            double surface_area_impl() const {
                auto v1 = vertices_[0];
                auto v2 = vertices_[1];
                auto v3 = vertices_[2];
                double ax = v2[0] - v1[0], ay = v2[1] - v1[1], az = v2[2] - v1[2];
                double bx = v3[0] - v1[0], by = v3[1] - v1[1], bz = v3[2] - v1[2];
                double cx = ay * bz - az * by;
                double cy = az * bx - ax * bz;
                double cz = ax * by - ay * bx;
                return 0.5 * std::sqrt(cx * cx + cy * cy + cz * cz);
            }

            PointType centroid_3d_impl() const {
                return PointType((vertices_[0][0] + vertices_[1][0] + vertices_[2][0]) / 3.0,
                                (vertices_[0][1] + vertices_[1][1] + vertices_[2][1]) / 3.0,
                                (vertices_[0][2] + vertices_[1][2] + vertices_[2][2]) / 3.0);
            }

            int access_count() const noexcept {
                return access_count_.load(std::memory_order_relaxed);
            }

            OptionalDouble safe_divide(double numerator) const {
                double area = surface_area_impl();
                if (std::fabs(area) < 1e-10) {
                    return OptionalDouble();
                }
                return OptionalDouble(numerator / area);
            }
            
            std::vector<PointType> generate_points(double /*step*/ = 1.0) const {
                return vertices_;
            }
            
            std::vector<PointType> get_boundary_points(size_t /*segments*/) const {
                std::cout << "Getting boundary points" << std::endl;
                return vertices_;
            }

            const std::vector<size_t>& get_indices() const {
                return indices_;
            }

            std::pair<const std::vector<PointType>&, const std::vector<size_t>&> get_render_data() const {
                return {vertices_, indices_};
            }

            double parallel_volume() const {
                return volume_impl();
            }
            
            template <typename Visitor>
            void visit_members(Visitor &&visitor) {
                visitor("vertices", vertices_);
                visitor("indices", indices_);
            }
            
            std::string serialize() const {
                return this->serialize_impl(*this);
            }

            std::future<double> async_volume() const {
                return std::async(std::launch::async, [this]() {
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    return this->volume_impl();
                });
            }

            void scale(double factor) {
                static std::mutex mtx;
                std::lock_guard<std::mutex> lock(mtx);
                auto centroid = centroid_3d_impl();
                for (auto& v : vertices_) {
                    v[0] = centroid[0] + (v[0] - centroid[0]) * factor;
                    v[1] = centroid[1] + (v[1] - centroid[1]) * factor;
                    v[2] = centroid[2] + (v[2] - centroid[2]) * factor;
                }
            }

            void rotate_x(double angle) {
                double cos_a = std::cos(angle);
                double sin_a = std::sin(angle);
                auto centroid = centroid_3d_impl();
                for (auto& v : vertices_) {
                    double dy = v[1] - centroid[1];
                    double dz = v[2] - centroid[2];
                    v[1] = centroid[1] + dy * cos_a - dz * sin_a;
                    v[2] = centroid[2] + dy * sin_a + dz * cos_a;
                }
            }

            void rotate_y(double angle) {
                double cos_a = std::cos(angle);
                double sin_a = std::sin(angle);
                auto centroid = centroid_3d_impl();
                for (auto& v : vertices_) {
                    double dx = v[0] - centroid[0];
                    double dz = v[2] - centroid[2];
                    v[0] = centroid[0] + dx * cos_a - dz * sin_a;
                    v[2] = centroid[2] + dx * sin_a + dz * cos_a;
                }
            }

            void rotate_z(double angle) {
                double cos_a = std::cos(angle);
                double sin_a = std::sin(angle);
                auto centroid = centroid_3d_impl();
                for (auto& v : vertices_) {
                    double dx = v[0] - centroid[0];
                    double dy = v[1] - centroid[1];
                    v[0] = centroid[0] + dx * cos_a - dy * sin_a;
                    v[1] = centroid[1] + dx * sin_a + dy * cos_a;
                }
            }

            Point<double, 2> project_2d() const {
                auto centroid = centroid_3d_impl();
                double x_2d = centroid[0] + (centroid[2] * 0.5);
                double y_2d = centroid[1] - (centroid[2] * 0.5);
                return Point<double, 2>(x_2d, y_2d);
            }

            struct ShapeVariant3D
            {
                enum Type { 
                    Triangle, 
                    Number, 
                    Str 
                };

                Type type;
                AdvancedBox triangle;
                double num;
                std::string str;

                ShapeVariant3D(const AdvancedBox& t) : type(Triangle), triangle(t) 
                {

                }

                ShapeVariant3D(double n) : type(Number), num(n) 
                {

                }

                ShapeVariant3D(const std::string& s) : type(Str), str(s) 
                {

                }
            };

            static std::string match_shape_3d(const ShapeVariant3D &shape) {
                std::ostringstream oss;
                switch (shape.type) {
                    case ShapeVariant3D::Triangle:
                        oss << "Треугольник с площадью: " << shape.triangle.surface_area_impl();
                        return oss.str();
                    case ShapeVariant3D::Number:
                        oss << "Число: " << shape.num;
                        return oss.str();
                    case ShapeVariant3D::Str:
                        return "Строка: " + shape.str;
                }
                return "";
            }

        private:
            template <typename, typename, typename>
            friend class AdvancedBox;
            friend struct JSONSerialization;
    };

    struct HeapStorage
    {
        std::unique_ptr<double[]> cache_;
        void cache_result(double value);
        OptionalDouble get_cached() const;
    };

    struct StrictValidation
    {
        void validate(double w, double h) const;
        void validate(double w, double h, double d) const;
    };

    struct JSONSerialization
    {
        template <typename Rect, std::enable_if_t<std::is_base_of_v<ShapeCRTP<Rect>, Rect>, int> = 0>
        std::string serialize_impl(const Rect &rect) const {
            std::ostringstream oss;
            oss << "{\"тип\": \"прямоугольник\", \"ширина\": " << rect.width()
                << ", \"высота\": " << rect.height()
                << ", \"площадь\": " << rect.area() << "}";
            return oss.str();
        }

        template <typename Shape3D, std::enable_if_t<std::is_base_of_v<ShapeCRTP3D<Shape3D>, Shape3D>, int> = 0>
        std::string serialize_impl(const Shape3D &box) const {
            std::cout << "Serializing 3D shape" << std::endl;
            std::ostringstream oss;
            oss << "{\"тип\": \"triangle\", \"вершины\": [";
            for (size_t i = 0; i < box.vertices_.size(); ++i) {
                if (i > 0) oss << ", ";
                oss << "[" << box.vertices_[i][0] << ", " << box.vertices_[i][1] << ", " << box.vertices_[i][2] << "]";
            }
            oss << "], \"индексы\": [";
            for (size_t i = 0; i < box.indices_.size(); ++i) {
                if (i > 0) oss << ", ";
                oss << box.indices_[i];
            }
            oss << "], \"площадь\": " << box.surface_area()
                << ", \"центр_масс\": [" << box.centroid_3d()[0] << ", " << box.centroid_3d()[1] << ", " << box.centroid_3d()[2] << "]}";
            return oss.str();
        }
    };

    class ShapeFactory
    {
    public:
        template <typename Shape, typename... Args>
        static std::unique_ptr<Shape> create(Args &&...args)
        {
            return std::make_unique<Shape>(std::forward<Args>(args)...);
        }

        class AnyShape
        {
            struct Concept
            {
                virtual ~Concept() = default;
                virtual double area() const = 0;
                virtual double perimeter() const = 0;
                virtual std::unique_ptr<Concept> clone() const = 0;
            };

            template <typename Shape>
            struct Model : Concept
            {
                Shape shape_;
                Model(Shape shape) : shape_(std::move(shape)) {}
                double area() const override { return shape_.area(); }
                double perimeter() const override { return shape_.perimeter(); }
                std::unique_ptr<Concept> clone() const override
                {
                    return std::make_unique<Model>(*this);
                }
            };

            std::unique_ptr<Concept> self_;

            public:
                template <typename Shape>
                AnyShape(Shape shape) : self_(std::make_unique<Model<Shape>>(std::move(shape))) {}

                AnyShape(const AnyShape &other) : self_(other.self_->clone()) 
                {

                }
                
                double area() const { 
                    return self_->area(); 
                }
                double perimeter() const { 
                    return self_->perimeter(); 
                }
        };

        class AnyShape3D
        {
            struct Concept3D
            {
                virtual ~Concept3D() = default;
                virtual double volume() const = 0;
                virtual double surface_area() const = 0;
                virtual std::unique_ptr<Concept3D> clone() const = 0;
            };

            template <typename Shape>
            struct Model3D : Concept3D
            {
                Shape shape_;
                Model3D(Shape shape) : shape_(std::move(shape)) {}
                double volume() const override { return shape_.volume(); }
                double surface_area() const override { return shape_.surface_area(); }
                std::unique_ptr<Concept3D> clone() const override
                {
                    return std::make_unique<Model3D>(*this);
                }
            };

            std::unique_ptr<Concept3D> self_;

            public:
                template <typename Shape>
                AnyShape3D(Shape shape) : self_(std::make_unique<Model3D<Shape>>(std::move(shape))) {}

                AnyShape3D(const AnyShape3D &other) : self_(other.self_->clone()) 
                {

                }
                
                double volume() const { 
                    return self_->volume(); 
                }
                double surface_area() const { 
                    return self_->surface_area(); 
                }
        };
    };

    class Benchmark
    {
        public:
            template <typename Func>
            static auto measure(Func func, size_t iterations = 1000)
            {
                auto start = std::chrono::high_resolution_clock::now();
                for (size_t i = 0; i < iterations; ++i)
                {
                    func();
                }
                auto end = std::chrono::high_resolution_clock::now();
                return std::chrono::duration<double, std::milli>(end - start).count() / iterations;
            }
    };

    class ThreadManager
    {
        private:
            std::mutex cout_mutex_;
            void safe_print(int thread_id, const std::string &message)
            {
                std::lock_guard<std::mutex> lock(cout_mutex_);
                std::cout << "   Поток " << thread_id << ": " << message << std::endl;
            }

        public:
            void execute_sequentially(std::function<void(int)> task, int count, const std::string &stage_name)
            {
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

            void execute_with_barrier(std::function<void(int, Barrier &)> task, int count, const std::string &stage_name)
            {
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

#endif // GEOMETRY3D_HPP