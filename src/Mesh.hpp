#pragma once

#include <Common.hpp>
#include <INode.hpp>

#include <vector>

class aiMesh;
class aiMaterial;

class Mesh : public INode {
 public:
  Mesh(const std::string &path,
       const std::string &fileName);
  void Init() override;
  void Draw(float dt) override;
  glm::mat4 GetTransformation() const override;

 private:
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
    GLuint vao;
    GLuint vbo[TOTAL_BUFFER_COUNT];
    GLuint texID;
    unsigned int vertexCount;
  };

  std::vector<std::unique_ptr<MeshEntry>> m_meshEntries;
  std::string m_path;
  std::string m_fileName;

  glm::vec3 m_pos;
  glm::quat m_rot;
};
