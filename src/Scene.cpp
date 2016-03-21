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

  m_meshes.push_back(MeshFactory::GetInstance().LoadMesh(GetPath("res/model-ground/"), "ground.fbx"));

  // basic scene lights
  m_ambientLight = AmbientLight(glm::vec3(1, 1, 1), 0.05f);
  m_directionalLight = DirectionalLight(glm::vec3(1, 1, 1), 0.7f, glm::normalize(glm::vec3(1, -1, 1)));
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

void Scene::AddMesh(std::shared_ptr<MeshNode> mesh) {
  m_meshes.push_back(mesh);
}

void Scene::AddPointLight(PointLight light) {
  m_pointLights.push_back(light);
}
