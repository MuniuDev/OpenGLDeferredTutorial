#pragma once

#include <Common.hpp>
#include <MeshData.hpp>

class MeshFactory;

class MeshNode : public INode {
 public:
  MeshNode(std::shared_ptr<MeshData> mesh,
           const std::string &name);
  virtual ~MeshNode();

  void Init() override {}
  void Draw(float dt) override;
  void SetShader(std::shared_ptr<ShaderProgram> shader);
  glm::mat4 GetTransformation() const override;
  glm::mat4 GetRotation() const override;

  void SetPos(const glm::vec3 &pos) override;
  void SetRot(const glm::quat &rot) override;
  void Move(const glm::vec3 &dir) override;
  void Rotate(const glm::quat &rot) override;

 private:

  std::shared_ptr<MeshData> m_mesh;

  glm::vec3 m_pos;
  glm::quat m_rot;
  std::shared_ptr<ShaderProgram> m_shader;
  std::string m_name;
};