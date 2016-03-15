#include <MeshFactory.hpp>
#include <MeshData.hpp>



MeshFactory &MeshFactory::GetInstance() {
  static MeshFactory instance;
  return instance;
}

std::shared_ptr<MeshNode> MeshFactory::LoadMesh(std::string path, std::string file) {
  std::string name = path + file;
  LOGD("Loading mesh: {}", name);
  if (m_meshes.find(name) == m_meshes.end()) {
    LOGD("Assigning mesh memory: {}", name);
    auto meshData = std::make_shared<MeshData>(path, file);
    if (meshData->Init()) {
      m_meshes[name] = meshData;
      m_refCount[name] = 1;
      return std::make_shared<MeshNode>(meshData, name);
    } else {
      LOGE("Failed to load mesh: {}", name);
      return std::shared_ptr<MeshNode>();
    }
  } else {
    auto meshData = m_meshes[name];
    m_refCount[name] += 1;
    return std::make_shared<MeshNode>(meshData, name);
  }
}

void MeshFactory::OnRelease(const std::string &name) {
  LOGD("Lowering mesh ref count: {}", name);
  m_refCount[name] -= 1;
  if (m_refCount[name] == 0) {
    LOGD("Freeing mesh memory: {}", name);
    m_meshes.erase(name);
  }
}