#include "model.hpp"

#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>

// 从文件中加载模型
Model::Model(const std::filesystem::path &file_path)
    : m_FilePath(file_path)
{
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;

    // 1. 读取模型文件
    std::ifstream file(file_path);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << file_path << std::endl;
        return;
    }

    // 2. 解析模型文件
    std::string line;
    char trash;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string_view line_view(line);

        if (line_view.substr(0, 2) == "v ")
        {
            glm::vec3 v;
            iss >> trash >> v.x >> v.y >> v.z;
            vertices.emplace_back(v);
        }
        else if (line_view.substr(0, 2) == "vn")
        {
            glm::vec3 n;
            iss >> trash >> trash >> n.x >> n.y >> n.z;
            normals.emplace_back(n);
        }
        else if (line_view.substr(0, 1) == "f")
        {
            glm::ivec3 idx_v, idx_n;
            iss >> trash;
            iss >> idx_v.x >> trash >> trash >> idx_n.x;
            iss >> idx_v.y >> trash >> trash >> idx_n.y;
            iss >> idx_v.z >> trash >> trash >> idx_n.z;
            m_Triangles.emplace_back(vertices[idx_v.x - 1], vertices[idx_v.y - 1], vertices[idx_v.z - 1],
                                     normals[idx_n.x - 1], normals[idx_n.y - 1], normals[idx_n.z - 1]);
        }
    }

    // 3. 关闭文件
    file.close();

    m_Triangles.shrink_to_fit();

    // 4. 打印模型信息
    PrintModelInfo();
}

// 测试射线与模型中的所有三角形的相交情况，返回最近的相交信息
std::optional<HitInfo> Model::intersect(const Ray &ray, float t_min, float t_max) const
{
    std::optional<HitInfo> closestHit;

    // 遍历所有三角形
    for (const auto &triangle : m_Triangles)
    {
        // 测试射线与当前三角形的相交情况
        if (auto hit = triangle.intersect(ray, t_min, t_max))
        {
            // 如果当前三角形有相交点，且该相交点比之前记录的最近相交点更近，则更新最近相交点
            if (!closestHit || hit->t < closestHit->t)
            {
                closestHit = hit;
            }
        }
    }

    // 返回最近的相交信息，如果没有相交则返回 std::nullopt
    return closestHit;
}

void Model::PrintModelInfo() const
{
    std::cout << "Model Info: " << m_FilePath << std::endl;
    std::cout << "Number of Triangles: " << m_Triangles.size() << std::endl;
}