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
    LOGE("Couldn't load model {} - Error Importing Asset", m_path + m_fileName);
    return;
  }

  LOGD("Loading model {} sucessfull.", m_path + m_fileName);
  for (int i = 0; i < scene->mNumMeshes; ++i) {
    m_meshEntries.push_back(std::make_unique<MeshEntry>(m_path, scene->mMeshes[i], scene->mMaterials[i]));
  }
}

void Mesh::Draw(float dt) {
  for (auto &meshEntry : m_meshEntries) {
    meshEntry->Draw(dt);
  }
}

glm::mat4 Mesh::GetTransformation() const {
  return glm::mat4_cast(m_rot) * glm::translate(glm::mat4(1.0f), m_pos);
}
