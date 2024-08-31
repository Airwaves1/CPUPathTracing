#include "camera/camera.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

Camera::Camera(Film &film, const glm::vec3 &pos, const glm::vec3 &lookAt, float fov)
    : m_Film(film), m_Pos(pos), m_LookAt(lookAt), m_Fov(fov)
{
    m_Up = glm::vec3(0, 1, 0);

    m_ClipToCamera = glm::inverse(glm::perspective(glm::radians(m_Fov), m_Film.GetAspect(), 1.0f, 2.0f));

    m_CameraToWorld = glm::inverse(glm::lookAt(m_Pos, m_LookAt, m_Up));
}

Ray Camera::GenerateRay(const glm::ivec2 &pixelCoord, const float offset_x, const float offset_y) const
{
    glm::vec2 ndc = (glm::vec2(pixelCoord) + glm::vec2(offset_x, offset_y)) / glm::vec2(m_Film.GetWidth(), m_Film.GetHeight());
    ndc.y = 1.0f - ndc.y;
    ndc = ndc * 2.0f - 1.0f;

    glm::vec4 clipPos{ndc.x, ndc.y, 0.f, 1.0f};
    glm::vec3 world = glm::vec3(m_CameraToWorld * m_ClipToCamera * clipPos);

    return Ray{m_Pos, glm::normalize(world - m_Pos)};
}
