/*
* Copyright by Michal Majczak & Krzysztof Taperek, 2016
* Licensed under the MIT license:
* http://www.opensource.org/licenses/mit-license.php
*
* Author: Krzysztof Taperek <krzysztoftaperek@gmail.com>
*/

#include <Scene.hpp>


Scene::Scene()
  : m_camera(nullptr) {

#ifdef __WIN32__
  m_meshes.push_back(MeshFactory::GetInstance().LoadMesh("res\\model-tank\\", "tank.fbx"));
  m_meshes.push_back(MeshFactory::GetInstance().LoadMesh("res\\model-ground\\", "ground.fbx"));
#else
  m_meshes.push_back(MeshFactory::GetInstance().LoadMesh("res/model-tank/", "tank.fbx"));
  m_meshes.push_back(MeshFactory::GetInstance().LoadMesh("res/model-ground/", "ground.fbx"));
#endif
}

Scene::~Scene() {

}

void Scene::Init(std::shared_ptr<Camera> camera) {

  m_camera = camera;

  m_camera->SetPos(glm::vec3(-8.04383, 4.87507, -6.82812));
  m_camera->SetRotate(glm::quat(-0.48016, 0.0973405, 0.854395, 0.173216));
}

void Scene::Update(float dt) {
  m_camera->HandleInput(dt);
}

std::shared_ptr<Camera> Scene::GetCamera() {
  return m_camera;
}
