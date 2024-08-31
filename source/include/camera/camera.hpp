#pragma once

#include <glm/glm.hpp>
#include "film.hpp"
#include "camera/ray.hpp"

class Camera
{
public:
    Camera(Film &film, const glm::vec3 &pos, const glm::vec3 &lookAt, float fov);

    //  从屏幕空间坐标生成光线，返回世界空间中的光线
    //  将像素坐标从图像空间转换到世界空间
    //  光线的起始位置是相机的位置，方向是从相机位置指向像素坐标的世界空间位置
    Ray GenerateRay(const glm::ivec2 &pixelCoord, const float offset_x = 0.5, const float offset_y = 0.5) const;

    Film &GetFilm() { return m_Film; }
    const Film &GetFilm() const { return m_Film; }

private:
    Film &m_Film;
    
    glm::vec3 m_Pos;
    glm::vec3 m_LookAt;
    glm::vec3 m_Up;
    glm::vec3 m_Right;
    float m_Fov;

    glm::mat4 m_ClipToCamera;   // 从裁剪空间到相机空间的变换矩阵, 就是透视投影矩阵的逆矩阵
    glm::mat4 m_CameraToWorld;  // 从相机空间到世界空间的变换矩阵，就是相机的模型矩阵的逆矩阵，也是LookAt矩阵的逆矩阵
    
};