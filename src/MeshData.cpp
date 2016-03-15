/*
* Copyright by Michal Majczak & Krzysztof Taperek, 2016
* Licensed under the MIT license:
* http://www.opensource.org/licenses/mit-license.php
*
* Author: Michal Majczak <michal.majczak92@gmail.com>
*/

#include <MeshData.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>

MeshData::MeshData(const std::string &path,
                   const std::string &fileName)
  : m_path(path)
  , m_fileName(fileName) {

}

bool MeshData::Init() {
  m_meshEntries.clear();

  Assimp::Importer importer;
  const aiScene *scene = importer.ReadFile(m_path + m_fileName,
                         aiProcessPreset_TargetRealtime_Fast);

  if (!scene) {
    LOGE("Error Importing Asset: {}",  importer.GetErrorString());
    return false;
  }

  LOGD("Loading model {} sucessfull.", m_path + m_fileName);
  for (int i = 0; i < scene->mNumMeshes; ++i) {
    m_meshEntries.push_back(std::make_unique<MeshEntry>(m_path,
                            scene->mMeshes[i],
                            scene->mMaterials[i]));
  }
  return true;
}

void MeshData::Draw(float dt) {
  for (auto &meshEntry : m_meshEntries) {
    m_shader->SetUniform("u_material.specularIntensity", meshEntry->mtl.specularIntensity);
    m_shader->SetUniform("u_material.specularPower", meshEntry->mtl.specularPower);
    m_shader->SetUniform("u_material.specularColor", meshEntry->mtl.specularColor);
    meshEntry->Draw(dt);
  }
}
