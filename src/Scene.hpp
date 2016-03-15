/*
* Copyright by Michal Majczak & Krzysztof Taperek, 2016
* Licensed under the MIT license:
* http://www.opensource.org/licenses/mit-license.php
*
* Author: Krzysztof Taperek <krzysztoftaperek@gmail.com>
*/

#pragma once

#include <Common.hpp>

#include <vector>

#include <MeshFactory.hpp>
#include <Camera.hpp>

class Scene {
 public:
  Scene();
  ~Scene();

  void Init(std::shared_ptr<Camera> camera);
  void Update(float dt);

  std::shared_ptr<Camera> GetCamera();

  std::shared_ptr<Camera> m_camera;
  std::vector<std::shared_ptr<MeshNode>> m_meshes;
};
