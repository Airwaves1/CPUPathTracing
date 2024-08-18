#pragma once

#include <cstddef>
#include <filesystem>
#include <vector>

#include <glm/glm.hpp>

// 用于存储最终渲染结果的类，类似电影胶片

class Film
{
public:
    Film(size_t width, size_t height);
    ~Film() = default;


    size_t GetWidth() const { return width; }
    size_t GetHeight() const { return height; }
    float GetAspect() const { return static_cast<float>(width) / static_cast<float>(height); }


    const glm::vec3& GetPixel(size_t x, size_t y) { return pixels[y * width + x]; }
    void SetPixel(size_t x, size_t y, const glm::vec3 &color) { pixels[y * width + x] = color; }
    
    void Save(const std::filesystem::path &path);

private:
    size_t width;
    size_t height;
    std::vector<glm::vec3> pixels;
};