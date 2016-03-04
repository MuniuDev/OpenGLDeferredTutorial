#pragma once

#include <Common.hpp>
#include <INode.hpp>

#include <vector>

class aiMesh;

class Mesh : public INode {
 public:
  Mesh(const std::string &modelFileName,
       const std::string &materialFileName);
  void Init() override;
  void Draw(float dt) override;
  glm::mat4 GetTransformation() const override;

 private:
  struct MeshEntry {
    MeshEntry(aiMesh *mesh);
    ~MeshEntry();

    void Draw(float dt);

    static enum BufferType {
      VERTEX_BUFFER, TEXCOORD_BUFFER, NORMAL_BUFFER, INDEX_BUFFER, TOTAL_BUFFER_COUNT
    };
    GLuint vao;
    GLuint vbo[TOTAL_BUFFER_COUNT];
    unsigned int vertexCount;
  };

  std::vector<std::unique_ptr<MeshEntry>> m_meshEntries;
  std::string m_modelFileName;

  glm::vec3 m_pos;
  glm::quat m_rot;
};