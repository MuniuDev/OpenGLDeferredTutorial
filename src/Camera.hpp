/*
* Copyright by Michal Majczak & Krzysztof Taperek, 2016
* Licensed under the MIT license:
* http://www.opensource.org/licenses/mit-license.php
*
* Author: Michal Majczak <michal.majczak92@gmail.com>
*/

#pragma once

#include <Common.hpp>

class Camera {
 public:
  Camera(float fov, float aspect, float clipNear, float clipFar);

  void Resize(float fov, float aspect, float clipNear, float clipFar);
  void HandleInput(float dt);

  void SetPos(const glm::vec3 &pos);
  void Move(const glm::vec3 &dir);
  void Rotate(const glm::quat &rot);

  glm::vec3 GetUp();
  glm::vec3 GetRight();
  glm::vec3 GetFront();

  glm::mat4 GetMVP();
 private:
  glm::vec3 m_pos;
  glm::quat m_rot;
  glm::mat4 m_MVP;
  glm::mat4 m_perspective;

  float m_fov;
  float m_aspect;
  float m_clipNear;
  float m_clipFar;
};
