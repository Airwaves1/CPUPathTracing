#include "film.hpp"
#include <fstream>
#include <iostream>

Film::Film(size_t width, size_t height) : width(width), height(height)
{
    pixels.resize(width * height, glm::vec3(0.0f));
}

void Film::Save(const std::filesystem::path &path)
{
    std::ofstream file(path, std::ios::binary);

    file << "P6\n";
    file << width << " " << height << "\n";
    file << "255\n"; 

    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < width; x++)
        {
            const glm::vec3 &color = GetPixel(x, y);
            glm::ivec3 icolor = glm::clamp(color * 255.0f, 0.0f, 255.0f);
            file << static_cast<uint8_t>(icolor.r) << static_cast<uint8_t>(icolor.g) << static_cast<uint8_t>(icolor.b);
        }
    }

    file.close();

    std::cout << "Save image to " << path << std::endl;
}
