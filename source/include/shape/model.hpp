#pragma once

#include "accelerate/bounds.hpp"
#include "shape/triangle.hpp"
#include <filesystem>

class Model : public Shape {
public:
  Model(const std::vector<Triangle> &triangles) : m_Triangles(triangles) {
    Build();
  }

  Model(const std::filesystem::path &file_path);

  std::optional<HitInfo> Intersect(const Ray &ray, float t_min,
                                   float t_max) const override;

  void PrintModelInfo() const;

private:
  void Build();

private:
  Bounds m_Bounds;

  std::vector<Triangle> m_Triangles;

  std::filesystem::path m_FilePath;
};