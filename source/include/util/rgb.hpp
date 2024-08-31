#pragma once

#include <glm/glm.hpp>

class RGB {

public:
  RGB(int r, int g, int b) : r(r), g(g), b(b) {}

  // 进行颜色的gamma校正， 将线性颜色转换为sRGB颜色空间。
  RGB(const glm::vec3 &color) {
    r = glm::clamp(glm::pow(color.r, 1.0f / 2.2f) * 255.0f, 0.0f, 255.0f);
    g = glm::clamp(glm::pow(color.g, 1.0f / 2.2f) * 255.0f, 0.0f, 255.0f);
    b = glm::clamp(glm::pow(color.b, 1.0f / 2.2f) * 255.0f, 0.0f, 255.0f);
  }

  // 逆gamma校正，将sRGB颜色空间转换为线性颜色空间
  operator glm::vec3() const {
    return glm::vec3(glm::pow(r / 255.0f, 2.2f), glm::pow(g / 255.0f, 2.2f),
                     glm::pow(b / 255.0f, 2.2f));
  }


  int r, g, b;
};
