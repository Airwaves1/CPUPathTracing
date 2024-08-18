#include <iostream>

#include <chrono>
#include "thread_pool.hpp"
#include "film.hpp"
#include "camera.hpp"
#include "sphere.hpp"

class SimpleTask : public Task
{
public:
    void Run() override
    {
        std::cout << "I am a worker thread" << std::this_thread::get_id() << std::endl;
    }
};

int main()
{
    std::cout << "--------------CPU Ray Tracing--------------" << std::endl;

    // 创建线程池
    ThreadPool pool{};

    std::filesystem::path currentPath = std::filesystem::current_path();
    std::filesystem::path outputPath = currentPath / "../../output.ppm";
    std::cout << "Output path: " << outputPath << std::endl;

    // 创建一个1920x1080的胶片
    Film film{1920, 1080};

    // 开始计时
    auto start = std::chrono::high_resolution_clock::now();

    pool.ParallelFor(1920, 1080, [&](size_t x, size_t y)
                     { film.SetPixel(x, y, {0.5f, 0.2f, 0.6f}); });

    pool.Wait(); // 等待所有线程执行完毕再保存胶片

    // for (size_t x = 0; x < 1920; x++)
    // {
    //     for (size_t y = 0; y < 1080; y++)
    //     {
    //         film.SetPixel(x, y, {0.5f, 0.2f, 0.6f});
    //     }
    // }

    // 结束计时
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "ParallelFor execution time: " << elapsed.count() << " seconds" << std::endl;

    film.Save(outputPath);
    return 0;
}