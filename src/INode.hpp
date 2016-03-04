/*
* Copyright by Michal Majczak & Krzysztof Taperek, 2016
* Licensed under the MIT license:
* http://www.opensource.org/licenses/mit-license.php
*
* Author: Michal Majczak <michal.majczak92@gmail.com>
*/

#pragma once

#include <Common.hpp>

class INode {
 public:
  virtual void Init() = 0;
  virtual void Draw(float dt) = 0;
  virtual glm::mat4 GetTransformation() = 0;
}
