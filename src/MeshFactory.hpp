#pragma once

#include <map>
#include <string>

#include <Common.hpp>
#include <MeshNode.hpp>

class MeshData;

class MeshFactory {
 public:
  static MeshFactory &GetInstance();

  std::shared_ptr<MeshNode> LoadMesh(std::string path, std::string file);
 private:
  friend class MeshNode;

  void OnRelease(const std::string &name);

  MeshFactory() {}
  MeshFactory(const MeshFactory &) = delete;
  MeshFactory &operator=(const MeshFactory &) = delete;

  std::map<std::string, std::shared_ptr<MeshData>> m_meshes;
  std::map<std::string, uint_t> m_refCount;
};