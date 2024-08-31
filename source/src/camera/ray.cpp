#include "camera/ray.hpp"


Ray Ray::WorldToObject(const glm::mat4 &object_to_world) const
{
    glm::vec4 origin = object_to_world * glm::vec4(this->origin, 1);
    glm::vec4 direction = object_to_world * glm::vec4(this->direction, 0);
    return Ray{glm::vec3(origin), glm::vec3(direction)};
}