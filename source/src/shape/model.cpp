#include "shape/model.hpp"

#include <iostream>
#include <optional>

#include <rapidobj/rapidobj.hpp>

#include "util/profile.hpp"
// 从文件中加载模型
Model::Model(const std::filesystem::path &file_path) : m_FilePath(file_path) {

  PROFILE("Load Model " + file_path.string());

  auto result =
      rapidobj::ParseFile(file_path, rapidobj::MaterialLibrary::Ignore());

  for (const auto &shape : result.shapes) {
    size_t index_offset = 0;
    for (size_t num_face_vectex : shape.mesh.num_face_vertices) {
      if (num_face_vectex == 3) {
        auto index = shape.mesh.indices[index_offset];
        glm::vec3 pos0{
            result.attributes.positions[index.position_index * 3 + 0],
            result.attributes.positions[index.position_index * 3 + 1],
            result.attributes.positions[index.position_index * 3 + 2]};
        index = shape.mesh.indices[index_offset + 1];
        glm::vec3 pos1{
            result.attributes.positions[index.position_index * 3 + 0],
            result.attributes.positions[index.position_index * 3 + 1],
            result.attributes.positions[index.position_index * 3 + 2]};
        index = shape.mesh.indices[index_offset + 2];
        glm::vec3 pos2{
            result.attributes.positions[index.position_index * 3 + 0],
            result.attributes.positions[index.position_index * 3 + 1],
            result.attributes.positions[index.position_index * 3 + 2]};

        if (index.normal_index >= 0) {
          index = shape.mesh.indices[index_offset];
          glm::vec3 normal0{
              result.attributes.normals[index.normal_index * 3 + 0],
              result.attributes.normals[index.normal_index * 3 + 1],
              result.attributes.normals[index.normal_index * 3 + 2]};
          index = shape.mesh.indices[index_offset + 1];
          glm::vec3 normal1{
              result.attributes.normals[index.normal_index * 3 + 0],
              result.attributes.normals[index.normal_index * 3 + 1],
              result.attributes.normals[index.normal_index * 3 + 2]};
          index = shape.mesh.indices[index_offset + 2];
          glm::vec3 normal2{
              result.attributes.normals[index.normal_index * 3 + 0],
              result.attributes.normals[index.normal_index * 3 + 1],
              result.attributes.normals[index.normal_index * 3 + 2]};
          m_Triangles.push_back(
              Triangle{pos0, pos1, pos2, normal0, normal1, normal2});
        } else {
          m_Triangles.push_back(Triangle{pos0, pos1, pos2});
        }
      }
      index_offset += num_face_vectex;
    }
  }

  Build();

  // PrintModelInfo();
}

// 测试射线与模型中的所有三角形的相交情况，返回最近的相交信息
std::optional<HitInfo> Model::Intersect(const Ray &ray, float t_min,
                                        float t_max) const {

  // 如果射线与模型的包围盒没有相交，则直接返回 std::nullopt
  if (!m_Bounds.HasIntersection(ray, t_min, t_max)) {
    return std::nullopt;
  }

  std::optional<HitInfo> closestHit{};

  // 遍历所有三角形
  for (const auto &triangle : m_Triangles) {
    // 测试射线与当前三角形的相交情况
    auto hitInfo = triangle.Intersect(ray, t_min, t_max);
    if (hitInfo.has_value()) {
      t_max = hitInfo->t;
      closestHit = hitInfo;
    }
  }

  // 返回最近的相交信息，如果没有相交则返回 std::nullopt
  return closestHit;
}

void Model::Build() {
  // 计算模型的包围盒
  for (const auto &triangle : m_Triangles) {
    m_Bounds.Expand(triangle.v0);
    m_Bounds.Expand(triangle.v1);
    m_Bounds.Expand(triangle.v2);
  }
}

void Model::PrintModelInfo() const {
  std::cout << "Model Info: " << m_FilePath << std::endl;
  std::cout << "Number of Triangles: " << m_Triangles.size() << std::endl;
}

/*
  手动加载obj文件
  // 从文件中加载模型
Model::Model(const std::filesystem::path &file_path) : m_FilePath(file_path) {

  PROFILE("Load Model " + file_path.string());

  std::vector<glm::vec3> vertices;
  std::vector<glm::vec3> normals;

  // 1. 读取模型文件
  std::ifstream file(file_path);
  if (!file.is_open()) {
    std::cerr << "Failed to open file: " << file_path << std::endl;
    return;
  }

  // 2. 解析模型文件
  std::string line;
  char trash;
  while (std::getline(file, line)) {
    std::istringstream iss(line);
    std::string_view line_view(line);

    if (line_view.substr(0, 2) == "v ") {
      glm::vec3 v;
      iss >> trash >> v.x >> v.y >> v.z;
      vertices.emplace_back(v);
    } else if (line_view.substr(0, 2) == "vn") {
      glm::vec3 n;
      iss >> trash >> trash >> n.x >> n.y >> n.z;
      normals.emplace_back(n);
    } else if (line_view.substr(0, 1) == "f") {
      glm::ivec3 idx_v, idx_n;
      iss >> trash;
      iss >> idx_v.x >> trash >> trash >> idx_n.x;
      iss >> idx_v.y >> trash >> trash >> idx_n.y;
      iss >> idx_v.z >> trash >> trash >> idx_n.z;
      m_Triangles.emplace_back(vertices[idx_v.x - 1], vertices[idx_v.y - 1],
                               vertices[idx_v.z - 1], normals[idx_n.x - 1],
                               normals[idx_n.y - 1], normals[idx_n.z - 1]);
    }
  }

  // 3. 关闭文件
  file.close();

  m_Triangles.shrink_to_fit(); // 释放多余的内存

  // 4. 计算模型的包围盒
  Build();

  // PrintModelInfo();
}
*/