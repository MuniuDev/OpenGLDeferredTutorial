#pragma once

#include <Common.hpp>



struct BaseLight {
  BaseLight() : intensity(0) {}
  BaseLight(const glm::vec3 &color, float intensity)
    : color(color), intensity(intensity) {}
  glm::vec3 color;
  float intensity;
};

typedef BaseLight AmbientLight;

struct DirectionalLight {
  DirectionalLight() {}
  DirectionalLight(const glm::vec3 &color, float intensity, const glm::vec3 &direction)
    : base(color, intensity), direction(direction) {}
  BaseLight base;
  glm::vec3 direction;
};

struct PointLight {
  PointLight() {}
  PointLight(const glm::vec3 &color, float intensity, const glm::vec3 &position, float range, float attenuation)
    : base(color, intensity), position(position), range(range), attenuation(attenuation) {}
  BaseLight base;
  glm::vec3 position;
  float range;
  float attenuation;
};