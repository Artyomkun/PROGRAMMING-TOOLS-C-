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
    
    void ThreadManager::safe_print(int thread_id, const std::string& message) {
        std::lock_guard<std::mutex> lock(cout_mutex_);
        std::cout << "   Поток " << thread_id << ": " << message << std::endl;
    }

    void ThreadManager::execute_sequentially(std::function<void(int)> task, int count, const std::string& stage_name) {
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

    void ThreadManager::execute_with_barrier(std::function<void(int, Barrier&)> task, int count, const std::string& stage_name) {
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

} // namespace Geometry3D