#ifndef GEOMETRY3D_HPP
#define GEOMETRY3D_HPP

#include <condition_variable>
#include <type_traits>
#include <filesystem>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <numeric>
#include <memory>
#include <vector>
#include <chrono>
#include <random>
#include <thread>
#include <future>
#include <atomic>
#include <string>
#include <array>
#include <cmath>
#include <mutex>
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
    class AdvancedRectangle;

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
                return PointType(top_left_[0] + width_ / 2.0, top_left_[1] + height_ / 2.0);
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

    struct HeapStorage
    {
        std::unique_ptr<double[]> cache_;
        void cache_result(double value);
        OptionalDouble get_cached() const;
    };

    struct StrictValidation
    {
        void validate(double w, double h) const;
    };

    struct JSONSerialization
    {
        template <typename Rect>
        std::string serialize_impl(const Rect &rect) const {
            std::ostringstream oss;
            oss << "{\"тип\": \"прямоугольник\", \"ширина\": " << rect.width()
                << ", \"высота\": " << rect.height()
                << ", \"площадь\": " << rect.area() << "}";
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

                    AnyShape(const AnyShape &other) : self_(other.self_->clone()) {}
                    
                    double area() const { return self_->area(); }
                    double perimeter() const { return self_->perimeter(); }
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
            void safe_print(int thread_id, const std::string &message);

        public:
            void execute_sequentially(std::function<void(int)> task, int count, const std::string &stage_name);
            void execute_with_barrier(std::function<void(int, Barrier &)> task, int count, const std::string &stage_name);
    };

} // namespace Geometry3D

#endif // GEOMETRY3D_HPP