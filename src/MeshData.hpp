/*
* Copyright by Michal Majczak & Krzysztof Taperek, 2016
* Licensed under the MIT license:
* http://www.opensource.org/licenses/mit-license.php
*
* Author: Michal Majczak <michal.majczak92@gmail.com>
*/

#pragma once

#include <Common.hpp>
#include <INode.hpp>
#include <ShaderProgram.hpp>


#include <vector>

class aiMesh;
class aiMaterial;

class MeshData {
 public:
  MeshData(const std::string &path,
           const std::string &fileName);
  MeshData(const MeshData &) = delete;

  void Draw(float dt);
  void SetShader(std::shared_ptr<ShaderProgram> shader) { m_shader = shader; }

 private:
  friend class MeshFactory;
  bool Init();

  struct MeshEntry {
    MeshEntry(const std::string &path, aiMesh *mesh, aiMaterial *material);
    ~MeshEntry();

    void Draw(float dt);

    enum BufferType {
      VERTEX_BUFFER,
      TEXCOORD_BUFFER,
      NORMAL_BUFFER,
      INDEX_BUFFER,
      TOTAL_BUFFER_COUNT
    };

    struct Material {
      float specularIntensity;
      float specularPower;
    };

    GLuint vao;
    GLuint vbo[TOTAL_BUFFER_COUNT];
    GLuint texID;
    unsigned int vertexCount;
    Material mtl;
  };

  std::vector<std::unique_ptr<MeshEntry>> m_meshEntries;
  std::string m_path;
  std::string m_fileName;

  std::shared_ptr<ShaderProgram> m_shader;
};
