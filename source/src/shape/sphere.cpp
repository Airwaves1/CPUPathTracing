#include "shape/sphere.hpp"

std::optional<HitInfo> Sphere::Intersect(const Ray &ray, float t_min, float t_max) const
{
    // 计算从球体中心到光线起点的向量 co
    glm::vec3 co = ray.origin - center; // 球体中心指向光线起点的向量

    // 计算光线方向向量的模的平方 a
    // a = |d|^2, 其中 d 为光线方向向量
    float a = glm::dot(ray.direction, ray.direction); // 光线方向的模的平方

    // 计算光线方向和从球心到光线起点的向量 co 的点积的两倍 b
    // b = 2 * (d ⋅ (o - c)), 其中 d 为光线方向向量, o 为光线起点, c 为球体中心
    float b = 2.0f * glm::dot(co, ray.direction); // 光线方向和光线起点到球心的向量的点积的两倍

    // 计算从球心到光线起点的向量 co 的模的平方减去球体半径的平方 c
    // c = |o - c|^2 - r^2, 其中 r 为球体半径
    float c = glm::dot(co, co) - radius * radius; // 球心到光线起点的向量的模的平方减去球体半径的平方

    // 计算一元二次方程的判别式 delta
    // delta = b^2 - 4 * a * c
    float delta = b * b - 4.0f * a * c; // 一元二次方程的判别式

    // 如果判别式小于 0，表示光线没有与球体相交，返回 std::nullopt
    if (delta < 0.0f)
    {
        return std::nullopt;
    }

    // 计算光线与球体相交的最近点 t
    // t = (-b - sqrt(delta)) / (2 * a)
    float hit_t = (-b - glm::sqrt(delta)) * 0.5f / a;

    // 如果最近点 t 小于 0，表示交点在光线起点之前，检查另一个解
    if (hit_t < 0.0f)
    {
        // t = (-b + sqrt(delta)) / (2 * a)
        hit_t = (-b + glm::sqrt(delta)) * 0.5f / a;
    }

    // 如果 t 大于 0，表示光线与球体相交，返回 t 值
    if (hit_t > t_min && hit_t < t_max)
    {
        glm::vec3 hit_point = ray.Hit(hit_t);
        glm::vec3 normal = glm::normalize(hit_point - center);
        return HitInfo{hit_t, hit_point, normal};
    }

    // 如果光线与球体不相交或相交点在光线起点之前，返回 std::nullopt
    return std::nullopt;
}
