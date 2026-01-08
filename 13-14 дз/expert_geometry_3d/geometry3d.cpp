#include "geometry3d.hpp"
#include <iostream>
#include <cmath>

Barrier::Barrier(size_t count) : count_(count), waiting_(0), generation_(0) {}

void Barrier::arrive_and_wait() {
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

OptionalDouble::OptionalDouble() : has_value(false) {}
OptionalDouble::OptionalDouble(double v) : value(v), has_value(true) {}

namespace Geometry3D {

    void HeapStorage::cache_result(double value) {
        cache_ = std::unique_ptr<double[]>(new double[1]);
        cache_[0] = value;
    }

    OptionalDouble HeapStorage::get_cached() const {
        if (cache_) return OptionalDouble(cache_[0]);
        return OptionalDouble();
    }

    void StrictValidation::validate(double w, double h) const {
        if (w <= 0.0 || h <= 0.0) {
            throw std::invalid_argument("Размеры должны быть положительными");
        }
        if (w > 1e6 || h > 1e6) {
            throw std::overflow_error("Размеры слишком большие");
        }
    }

    void StrictValidation::validate(double w, double h, double d) const {
        if (w <= 0.0 || h <= 0.0 || d <= 0.0) {
            throw std::invalid_argument("Размеры должны быть положительными");
        }
        if (w > 1e6 || h > 1e6 || d > 1e6) {
            throw std::overflow_error("Размеры слишком большие");
        }
    }

} // namespace Geometry3D