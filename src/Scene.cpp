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

  // basic scene lights
  m_ambientLight = AmbientLight(glm::vec3(1, 1, 1), 0.05f);
  m_directionalLight = DirectionalLight(glm::vec3(1, 1, 1), 0.7f, glm::normalize(glm::vec3(1, -1, 1)));

  // some test point lights
  m_pointLights.push_back(PointLight(glm::vec3(1, 0, 0), 1, glm::vec3(0, 2, -1), 100, 1));
  m_pointLights.push_back(PointLight(glm::vec3(0, 1, 0), 1, glm::vec3(0, 2, 1), 100, 1));
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
