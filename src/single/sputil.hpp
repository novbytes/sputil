/*
Copyright 2025 spUtil

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef SPUTIL_HPP
#define SPUTIL_HPP

#include <iostream>
#include <string>
#include <algorithm>
#include <chrono>
#include <vector>
#include <random>
#include <thread>
#include <functional>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <unordered_map>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <filesystem>
#include <fstream>
#include <cctype>
#include <regex>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <future>
#include <optional>

namespace sputil
{
    // ===== TIME UTILITIES =====
    namespace time
    {
        void sleep(double seconds)
        {
            std::this_thread::sleep_for(std::chrono::duration<double>(seconds));
        }
        
        void sleep_ms(int milliseconds)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
        }
        
        long long timestamp()
        {
            return std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()).count();
        }
        
        std::string format_time(const std::string& format = "%Y-%m-%d %H:%M:%S")
        {
            auto now = std::chrono::system_clock::now();
            auto time_t = std::chrono::system_clock::to_time_t(now);
            std::stringstream ss;
            ss << std::put_time(std::localtime(&time_t), format.c_str());
            return ss.str();
        }
        
        class Timer
        {
        private:
            std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
            
        public:
            Timer() : start_time(std::chrono::high_resolution_clock::now()) {}
            
            void reset()
            {
                start_time = std::chrono::high_resolution_clock::now();
            }
            
            double elapsed() const
            {
                auto end_time = std::chrono::high_resolution_clock::now();
                return std::chrono::duration<double>(end_time - start_time).count();
            }
            
            double elapsed_ms() const
            {
                auto end_time = std::chrono::high_resolution_clock::now();
                return std::chrono::duration<double, std::milli>(end_time - start_time).count();
            }
        };
    }

    // ===== ARRAY/COLLECTION UTILITIES =====
    namespace array
    {
        template <typename T>
        void shuffle(std::vector<T>& data)
        {
            unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
            std::shuffle(data.begin(), data.end(), std::default_random_engine(seed));
        }
        
        template <typename T>
        bool contains(const std::vector<T>& data, const T& value)
        {
            return std::find(data.begin(), data.end(), value) != data.end();
        }
        
        template <typename T>
        void remove_duplicates(std::vector<T>& data)
        {
            std::sort(data.begin(), data.end());
            data.erase(std::unique(data.begin(), data.end()), data.end());
        }
        
        template <typename T>
        std::vector<T> slice(const std::vector<T>& data, int start, int end = -1)
        {
            if (end == -1) end = data.size();
            if (start < 0) start = data.size() + start;
            if (end < 0) end = data.size() + end;
            
            start = std::max(0, std::min(start, (int)data.size()));
            end = std::max(0, std::min(end, (int)data.size()));
            
            return std::vector<T>(data.begin() + start, data.begin() + end);
        }
        
        template <typename T, typename Func>
        std::vector<T> filter(const std::vector<T>& data, Func predicate)
        {
            std::vector<T> result;
            std::copy_if(data.begin(), data.end(), std::back_inserter(result), predicate);
            return result;
        }
        
        template <typename T, typename Func>
        auto map(const std::vector<T>& data, Func transform)
        {
            std::vector<decltype(transform(data[0]))> result;
            result.reserve(data.size());
            std::transform(data.begin(), data.end(), std::back_inserter(result), transform);
            return result;
        }
    }

    // ===== STRING UTILITIES =====
    namespace string
    {
        std::string trim(const std::string& str)
        {
            auto start = str.begin();
            while (start != str.end() && std::isspace(*start)) {
                start++;
            }
            
            auto end = str.end();
            do {
                end--;
            } while (std::distance(start, end) > 0 && std::isspace(*end));
            
            return std::string(start, end + 1);
        }
        
        std::string to_lower(const std::string& str)
        {
            std::string result = str;
            std::transform(result.begin(), result.end(), result.begin(), ::tolower);
            return result;
        }
        
        std::string to_upper(const std::string& str)
        {
            std::string result = str;
            std::transform(result.begin(), result.end(), result.begin(), ::toupper);
            return result;
        }
        
        bool starts_with(const std::string& str, const std::string& prefix)
        {
            return str.size() >= prefix.size() && 
                   str.compare(0, prefix.size(), prefix) == 0;
        }
        
        bool ends_with(const std::string& str, const std::string& suffix)
        {
            return str.size() >= suffix.size() && 
                   str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
        }
        
        std::vector<std::string> split(const std::string& str, const std::string& delimiter)
        {
            std::vector<std::string> result;
            size_t start = 0;
            size_t end = str.find(delimiter);
            
            while (end != std::string::npos)
            {
                result.push_back(str.substr(start, end - start));
                start = end + delimiter.length();
                end = str.find(delimiter, start);
            }
            
            result.push_back(str.substr(start, end));
            return result;
        }
        
        std::string join(const std::vector<std::string>& strings, const std::string& delimiter)
        {
            if (strings.empty()) return "";
            
            std::string result = strings[0];
            for (size_t i = 1; i < strings.size(); i++)
            {
                result += delimiter + strings[i];
            }
            return result;
        }
        
        std::string replace(const std::string& str, const std::string& from, const std::string& to)
        {
            std::string result = str;
            size_t start_pos = 0;
            while ((start_pos = result.find(from, start_pos)) != std::string::npos)
            {
                result.replace(start_pos, from.length(), to);
                start_pos += to.length();
            }
            return result;
        }
    }

    // ===== MATH UTILITIES =====
    // namespace math
    // {
    //     const double PI = 3.14159265358979323846;
    //     const double E = 2.71828182845904523536;
        
    //     template <typename T>
    //     T clamp(T value, T min, T max)
    //     {
    //         return std::max(min, std::min(value, max));
    //     }
        
    //     template <typename T>
    //     T lerp(T a, T b, double t)
    //     {
    //         return a + (b - a) * clamp(t, 0.0, 1.0);
    //     }
        
    //     double degrees_to_radians(double degrees)
    //     {
    //         return degrees * PI / 180.0;
    //     }
        
    //     double radians_to_degrees(double radians)
    //     {
    //         return radians * 180.0 / PI;
    //     }
        
    //     int random_int(int min, int max)
    //     {
    //         static std::random_device rd;
    //         static std::mt19937 gen(rd());
    //         std::uniform_int_distribution<> dis(min, max);
    //         return dis(gen);
    //     }
        
    //     double random_double(double min, double max)
    //     {
    //         static std::random_device rd;
    //         static std::mt19937 gen(rd());
    //         std::uniform_real_distribution<> dis(min, max);
    //         return dis(gen);
    //     }
        
    //     template <typename T>
    //     T min(const std::vector<T>& values)
    //     {
    //         return *std::min_element(values.begin(), values.end());
    //     }
        
    //     template <typename T>
    //     T max(const std::vector<T>& values)
    //     {
    //         return *std::max_element(values.begin(), values.end());
    //     }
        
    //     template <typename T>
    //     double average(const std::vector<T>& values)
    //     {
    //         if (values.empty()) return 0.0;
    //         double sum = 0.0;
    //         for (const auto& value : values) sum += value;
    //         return sum / values.size();
    //     }
    // }

    // ===== FILE SYSTEM UTILITIES =====
    namespace fs
    {
        bool exists(const std::string& path)
        {
            return std::filesystem::exists(path);
        }
        
        bool is_file(const std::string& path)
        {
            return std::filesystem::is_regular_file(path);
        }
        
        bool is_directory(const std::string& path)
        {
            return std::filesystem::is_directory(path);
        }
        
        std::string read_file(const std::string& path)
        {
            std::ifstream file(path);
            if (!file.is_open()) throw std::runtime_error("Cannot open file: " + path);
            
            std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
            return content;
        }
        
        void write_file(const std::string& path, const std::string& content)
        {
            std::ofstream file(path);
            if (!file.is_open()) throw std::runtime_error("Cannot open file: " + path);
            file << content;
        }
        
        std::vector<std::string> list_files(const std::string& path)
        {
            std::vector<std::string> files;
            for (const auto& entry : std::filesystem::directory_iterator(path))
            {
                if (entry.is_regular_file())
                {
                    files.push_back(entry.path().string());
                }
            }
            return files;
        }
        
        bool create_directory(const std::string& path)
        {
            return std::filesystem::create_directories(path);
        }
        
        uintmax_t file_size(const std::string& path)
        {
            return std::filesystem::file_size(path);
        }
    }

    // ===== THREADING UTILITIES =====
    namespace threading
    {
        class ThreadPool
        {
        private:
            std::vector<std::thread> workers;
            std::queue<std::function<void()>> tasks;
            std::mutex queue_mutex;
            std::condition_variable condition;
            std::atomic<bool> stop;
            
        public:
            ThreadPool(size_t threads = std::thread::hardware_concurrency()) : stop(false)
            {
                for (size_t i = 0; i < threads; ++i)
                {
                    workers.emplace_back([this]
                    {
                        while (true)
                        {
                            std::function<void()> task;
                            {
                                std::unique_lock<std::mutex> lock(this->queue_mutex);
                                this->condition.wait(lock, [this]
                                {
                                    return this->stop || !this->tasks.empty();
                                });
                                
                                if (this->stop && this->tasks.empty()) return;
                                
                                task = std::move(this->tasks.front());
                                this->tasks.pop();
                            }
                            task();
                        }
                    });
                }
            }
            
            template<class F, class... Args>
            auto enqueue(F&& f, Args&&... args) -> std::future<decltype(f(args...))>
            {
                using return_type = decltype(f(args...));
                
                auto task = std::make_shared<std::packaged_task<return_type()>>(
                    std::bind(std::forward<F>(f), std::forward<Args>(args)...)
                );
                
                std::future<return_type> res = task->get_future();
                {
                    std::unique_lock<std::mutex> lock(queue_mutex);
                    if (stop) throw std::runtime_error("enqueue on stopped ThreadPool");
                    tasks.emplace([task](){ (*task)(); });
                }
                condition.notify_one();
                return res;
            }
            
            ~ThreadPool()
            {
                {
                    std::unique_lock<std::mutex> lock(queue_mutex);
                    stop = true;
                }
                condition.notify_all();
                for (std::thread& worker : workers)
                    worker.join();
            }
        };
        
        class MutexGuard
        {
        private:
            std::mutex& mutex;
            
        public:
            explicit MutexGuard(std::mutex& m) : mutex(m)
            {
                mutex.lock();
            }
            
            ~MutexGuard()
            {
                mutex.unlock();
            }
            
            MutexGuard(const MutexGuard&) = delete;
            MutexGuard& operator=(const MutexGuard&) = delete;
        };
    }

    // ===== CONCURRENCY UTILITIES =====
    namespace concurrency
    {
        template <typename T>
        class ConcurrentQueue
        {
        private:
            std::queue<T> queue;
            mutable std::mutex mutex;
            std::condition_variable condition;
            
        public:
            void push(T value)
            {
                {
                    std::unique_lock<std::mutex> lock(mutex);
                    queue.push(std::move(value));
                }
                condition.notify_one();
            }
            
            T pop()
            {
                std::unique_lock<std::mutex> lock(mutex);
                condition.wait(lock, [this]{ return !queue.empty(); });
                T value = std::move(queue.front());
                queue.pop();
                return value;
            }
            
            bool try_pop(T& value)
            {
                std::unique_lock<std::mutex> lock(mutex);
                if (queue.empty()) return false;
                value = std::move(queue.front());
                queue.pop();
                return true;
            }
            
            bool empty() const
            {
                std::unique_lock<std::mutex> lock(mutex);
                return queue.empty();
            }
            
            size_t size() const
            {
                std::unique_lock<std::mutex> lock(mutex);
                return queue.size();
            }
        };
        
        class RateLimiter
        {
        private:
            std::chrono::steady_clock::time_point last_call;
            std::chrono::milliseconds min_delay;
            std::mutex mutex;
            
        public:
            RateLimiter(int calls_per_second) : 
                last_call(std::chrono::steady_clock::now()),
                min_delay(std::chrono::milliseconds(1000 / calls_per_second)) {}
            
            void acquire()
            {
                std::unique_lock<std::mutex> lock(mutex);
                auto now = std::chrono::steady_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_call);
                
                if (elapsed < min_delay)
                {
                    std::this_thread::sleep_for(min_delay - elapsed);
                }
                
                last_call = std::chrono::steady_clock::now();
            }
        };
    }

    // ===== ALGORITHM UTILITIES =====
    namespace algorithm
    {
        template <typename Container, typename T>
        bool binary_search(const Container& container, const T& value)
        {
            return std::binary_search(std::begin(container), std::end(container), value);
        }
        
        template <typename Container, typename Predicate>
        void quick_sort(Container& container, Predicate comp)
        {
            std::sort(std::begin(container), std::end(container), comp);
        }
        
        template <typename T>
        std::vector<T> generate_sequence(T start, T end, T step = 1)
        {
            std::vector<T> result;
            for (T i = start; i <= end; i += step)
            {
                result.push_back(i);
            }
            return result;
        }
        
        template <typename T, typename Hash = std::hash<T>>
        class LRUCache
        {
        private:
            size_t capacity;
            std::list<T> items;
            std::unordered_map<T, typename std::list<T>::iterator, Hash> cache_map;
            
        public:
            LRUCache(size_t capacity) : capacity(capacity) {}
            
            void put(const T& key)
            {
                auto it = cache_map.find(key);
                if (it != cache_map.end())
                {
                    items.erase(it->second);
                    cache_map.erase(it);
                }
                
                items.push_front(key);
                cache_map[key] = items.begin();
                
                if (cache_map.size() > capacity)
                {
                    auto last = items.end();
                    last--;
                    cache_map.erase(*last);
                    items.pop_back();
                }
            }
            
            bool get(const T& key)
            {
                auto it = cache_map.find(key);
                if (it == cache_map.end()) return false;
                
                items.splice(items.begin(), items, it->second);
                return true;
            }
            
            bool contains(const T& key) const
            {
                return cache_map.find(key) != cache_map.end();
            }
            
            size_t size() const { return items.size(); }
        };
    }

    // ===== NETWORKING UTILITIES (Basic) =====
    namespace net
    {
        std::string url_encode(const std::string& value)
        {
            std::ostringstream escaped;
            escaped.fill('0');
            escaped << std::hex;
            
            for (char c : value)
            {
                if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~')
                {
                    escaped << c;
                }
                else
                {
                    escaped << '%' << std::setw(2) << int((unsigned char)c);
                }
            }
            
            return escaped.str();
        }
        
        std::string url_decode(const std::string& value)
        {
            std::string result;
            result.reserve(value.size());
            
            for (size_t i = 0; i < value.size(); ++i)
            {
                if (value[i] == '%' && i + 2 < value.size())
                {
                    int hex_value;
                    std::istringstream hex_stream(value.substr(i + 1, 2));
                    if (hex_stream >> std::hex >> hex_value)
                    {
                        result += static_cast<char>(hex_value);
                        i += 2;
                    }
                    else
                    {
                        result += value[i];
                    }
                }
                else if (value[i] == '+')
                {
                    result += ' ';
                }
                else
                {
                    result += value[i];
                }
            }
            
            return result;
        }
        
        std::map<std::string, std::string> parse_query_string(const std::string& query)
        {
            std::map<std::string, std::string> result;
            auto pairs = string::split(query, "&");
            
            for (const auto& pair : pairs)
            {
                auto key_value = string::split(pair, "=");
                if (key_value.size() == 2)
                {
                    result[url_decode(key_value[0])] = url_decode(key_value[1]);
                }
            }
            
            return result;
        }
    }

    // ===== DEBUGGING UTILITIES =====
    namespace debug
    {
        class ScopeTimer
        {
        private:
            std::string name;
            time::Timer timer;
            
        public:
            explicit ScopeTimer(const std::string& func_name) : name(func_name) {}
            
            ~ScopeTimer()
            {
                std::cout << name << " took " << timer.elapsed_ms() << "ms\n";
            }
        };
        
        #define SPTR_SCOPE_TIMER() SPTRutil::debug::ScopeTimer _scope_timer(__FUNCTION__)
        
        template <typename T>
        void printc(const T& container, const std::string& name = "")
        {
            if (!name.empty()) std::cout << name << ": ";
            std::cout << "[";
            for (auto it = container.begin(); it != container.end(); ++it)
            {
                if (it != container.begin()) std::cout << ", ";
                std::cout << *it;
            }
            std::cout << "]\n";
        }
    }

    // ===== FUNCTIONAL PROGRAMMING UTILITIES =====
    namespace functional
    {
        template <typename T>
        class Maybe
        {
        private:
            std::optional<T> value;
            
        public:
            Maybe() = default;
            Maybe(const T& val) : value(val) {}
            Maybe(T&& val) : value(std::move(val)) {}
            
            bool is_just() const { return value.has_value(); }
            bool is_nothing() const { return !value.has_value(); }
            
            T& get() { return value.value(); }
            const T& get() const { return value.value(); }
            
            T get_or(const T& default_val) const
            {
                return value.value_or(default_val);
            }
            
            template <typename Func>
            auto map(Func f) const -> Maybe<decltype(f(value.value()))>
            {
                if (is_nothing()) return Maybe<decltype(f(value.value()))>();
                return Maybe<decltype(f(value.value()))>(f(value.value()));
            }
        };
        
        template <typename T, typename E>
        class Result
        {
        private:
            std::optional<T> success_value;
            std::optional<E> error_value;
            
        public:
            static Result Ok(const T& value) { return Result(value); }
            static Result Err(const E& error) { return Result(error); }
            
            Result(const T& value) : success_value(value) {}
            Result(const E& error) : error_value(error) {}
            
            bool is_ok() const { return success_value.has_value(); }
            bool is_err() const { return error_value.has_value(); }
            
            T& unwrap() { return success_value.value(); }
            const T& unwrap() const { return success_value.value(); }
            
            E& unwrap_err() { return error_value.value(); }
            const E& unwrap_err() const { return error_value.value(); }
            
            T unwrap_or(const T& default_val) const
            {
                return success_value.value_or(default_val);
            }
        };
    }
}

#endif // SPUTIL_HPP