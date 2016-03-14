/*
* Copyright by Michal Majczak & Krzysztof Taperek, 2016
* Licensed under the MIT license:
* http://www.opensource.org/licenses/mit-license.php
*
* Author: Michal Majczak <michal.majczak92@gmail.com>
*/

#include <Mesh.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>

Mesh::Mesh(const std::string &path,
           const std::string &fileName)
  : m_path(path)
  , m_fileName(fileName) {

}

void Mesh::Init() {
  m_meshEntries.clear();

  Assimp::Importer importer;
  const aiScene *scene = importer.ReadFile(m_path + m_fileName,
                         aiProcessPreset_TargetRealtime_Fast);

  if (!scene) {
    LOGE("Error Importing Asset: {}",  importer.GetErrorString());
    return;
  }

  LOGD("Loading model {} sucessfull.", m_path + m_fileName);
  for (int i = 0; i < scene->mNumMeshes; ++i) {
    m_meshEntries.push_back(std::make_unique<MeshEntry>(m_path,
                            scene->mMeshes[i],
                            scene->mMaterials[i]));
  }
}

void Mesh::Draw(float dt) {

  m_shader->SetUniform("u_transform", GetTransformation());

  for (auto &meshEntry : m_meshEntries) {
    m_shader->SetUniform("u_material.specularIntensity", meshEntry->mtl.specularIntensity);
    m_shader->SetUniform("u_material.specularPower", meshEntry->mtl.specularPower);
    meshEntry->Draw(dt);
  }

}

glm::mat4 Mesh::GetTransformation() const {
  return  glm::translate(glm::mat4(1.0f), m_pos) * glm::mat4_cast(m_rot);
}

void Mesh::Move(const glm::vec3 &dir) {
  m_pos += dir;
}

void Mesh::Rotate(const glm::quat &rot) {
  m_rot = glm::normalize(rot) * m_rot;
}

void Mesh::SetPos(const glm::vec3 &pos) {
  m_pos = pos;
}

void Mesh::SetRot(const glm::quat &rot) {
  m_rot = glm::normalize(rot);
}
