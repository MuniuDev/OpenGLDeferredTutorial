/*
* Copyright by Michal Majczak & Krzysztof Taperek, 2016
* Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
*
* Author: Michal Majczak <michal.majczak92@gmail.com>
*/

#pragma once

#include <Common.hpp>

class Camera {
public:
  Camera(float fov, float aspect, float clipNear, float clipFar);

  void Resize(float fov, float aspect, float clipNear, float clipFar);
  void HandleInput();

  void SetPos(const glm::vec3& pos) { m_pos = pos; }
  void Move(const glm::vec3& pos) { m_pos += glm::vec3(glm::mat4_cast(m_rot) * glm::vec4(pos,1)); }
  void Rotate(const glm::quat& rot) { m_rot *=  rot; }

  glm::vec3 GetUp() {return glm::vec3(glm::mat4_cast(m_rot) * glm::vec4(0,1,0,1));}
  glm::vec3 GetRight() {return glm::vec3(glm::mat4_cast(m_rot) * glm::vec4(1,0,0,1));}
  glm::vec3 GetFront() {return glm::vec3(glm::mat4_cast(m_rot) * glm::vec4(0,0,1,1));}

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
