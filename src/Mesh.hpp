#pragma once

#include <Common.hpp>
#include <INode.hpp>
#include <ShaderProgram.hpp>


#include <vector>

class aiMesh;
class aiMaterial;

class Mesh : public INode {
 public:
  Mesh(const std::string &path,
       const std::string &fileName);
  void Init() override;
  void Draw(float dt) override;
  void SetShader(std::shared_ptr<ShaderProgram> shader) { m_shader = shader; }
  glm::mat4 GetTransformation() const override;

  void SetPos(const glm::vec3 &pos) override;
  void SetRot(const glm::quat &rot) override;
  void Move(const glm::vec3 &dir) override;
  void Rotate(const glm::quat &rot) override;

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

  glm::vec3 m_pos;
  glm::quat m_rot;
  std::shared_ptr<ShaderProgram> m_shader;
};
