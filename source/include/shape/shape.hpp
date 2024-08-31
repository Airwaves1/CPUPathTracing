#pragma once

#include <optional>

#include "camera/ray.hpp"

struct Shape
{

    virtual std::optional<HitInfo> Intersect(const Ray &ray,
                                           float t_min,
                                           float t_max) const = 0;

                                           
};
