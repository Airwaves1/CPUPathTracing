#pragma once

#include <optional>

#include "ray.hpp"

struct Shape
{

    virtual std::optional<HitInfo> intersect(const Ray &ray,
                                           float t_min = 1e-6,
                                           float t_max = std::numeric_limits<float>::infinity()) const = 0;

                                           
};
