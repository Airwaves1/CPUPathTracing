#include "shape/triangle.hpp"

// intersect 函数用于测试射线与三角形的相交
// Möller-Trumbore  算法
// - ray: 射线
// - t_min: 最小相交距离
// - t_max: 最大相交距离
// - 如果相交，返回包含相交信息的 HitInfo 结构体；否则返回 std::nullopt

std::optional<HitInfo>
Triangle::Intersect(const Ray &ray, float t_min, float t_max) const
{
    // 1.计算三角形的边向量
    glm::vec3 e1 = v1 - v0;
    glm::vec3 e2 = v2 - v0;

    // 2.计算行列式及其倒数
    glm::vec3 p = glm::cross(ray.direction, e2);
    float inv_det = 1.0f / glm::dot(e1, p);

    // 3.计算重心坐标 b1 & b2 ,需要满足 0 <= b1 <= 1
    glm::vec3 T = ray.origin - v0;
    float b1 = glm::dot(T, p) * inv_det;
    if (b1 < 0.0f || b1 > 1.0f)
        return std::nullopt;

    glm::vec3 Q = glm::cross(T, e1);
    float b2 = glm::dot(ray.direction, Q) * inv_det;
    if (b2 < 0.0f || b1 + b2 > 1.0f)
        return std::nullopt;

    // 4.计算相交距离 hit_t
    float hit_t = glm::dot(e2, Q) * inv_det;
    if (hit_t < 0.0f || hit_t < t_min || hit_t > t_max)
        return std::nullopt;

    // 5.计算相交点的法向量, 将三个顶点的法向量进行插值
    glm::vec3 normal = glm::normalize(n0 * (1.0f - b1 - b2) + n1 * b1 + n2 * b2);
    glm::vec3 hit_point = ray.Hit(hit_t);

    return HitInfo{hit_t, hit_point, normal};
}