#pragma once

#include "triangle.hpp"
#include <filesystem>

class Model : public Shape
{
public:
    Model(const std::vector<Triangle> &triangles) : m_Triangles(triangles) {}

    Model(const std::filesystem::path &filen_path);

    std::optional<HitInfo> intersect(const Ray &ray, float t_min, float t_max) const override;

    void PrintModelInfo() const;

private:
    std::vector<Triangle> m_Triangles;

    std::filesystem::path m_FilePath;
};