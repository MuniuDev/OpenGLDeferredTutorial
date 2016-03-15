#include <MeshNode.hpp>
#include <MeshFactory.hpp>

MeshNode::MeshNode(std::shared_ptr<MeshData> mesh,
                   const std::string &name)
  : m_mesh(mesh), m_name(name) {
}

MeshNode::~MeshNode() {
  MeshFactory::GetInstance().OnRelease(m_name);
}

void MeshNode::SetShader(std::shared_ptr<ShaderProgram> shader) {
  m_shader = shader;
  m_mesh->SetShader(shader);
}

glm::mat4 MeshNode::GetTransformation() const {
  return  glm::translate(glm::mat4(1.0f), m_pos) * glm::mat4_cast(m_rot);
}

glm::mat4 MeshNode::GetRotation() const {
  return glm::mat4_cast(m_rot);
}

void MeshNode::Move(const glm::vec3 &dir) {
  m_pos += dir;
}

void MeshNode::Rotate(const glm::quat &rot) {
  m_rot = glm::normalize(rot) * m_rot;
}

void MeshNode::SetPos(const glm::vec3 &pos) {
  m_pos = pos;
}

void MeshNode::SetRot(const glm::quat &rot) {
  m_rot = glm::normalize(rot);
}

void MeshNode::Draw(float dt) {
  m_shader->SetUniform("u_transform", GetTransformation());
  m_mesh->Draw(dt);
}