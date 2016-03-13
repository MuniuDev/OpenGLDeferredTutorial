/*
* Copyright by Michal Majczak & Krzysztof Taperek, 2016
* Licensed under the MIT license:
* http://www.opensource.org/licenses/mit-license.php
*
* Author: Michal Majczak <michal.majczak92@gmail.com>
*/

#pragma once

#include <memory>
#include <unordered_map>

#include <Common.hpp>
#include <ShaderProgram.hpp>
#include <Camera.hpp>

class Viewport {
 public:
  Viewport(float width, float height);

  void Init();
  void Resize(float width, float height);
  void Draw(float dt);

 private:
  float m_width;
  float m_height;

  Camera m_camera;
  std::shared_ptr<ShaderProgram> m_shader;
};
