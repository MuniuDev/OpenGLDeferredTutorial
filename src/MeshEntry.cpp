/*
* Copyright by Michal Majczak & Krzysztof Taperek, 2016
* Licensed under the MIT license:
* http://www.opensource.org/licenses/mit-license.php
*
* Author: Michal Majczak <michal.majczak92@gmail.com>
*/

#include <Mesh.hpp>

#include <assimp/scene.h>

#include <gli/gli.hpp>

#include <vector>
#include <algorithm>
#include <string>

GLuint create_texture(char const *Filename);

Mesh::MeshEntry::MeshEntry(const std::string &path, aiMesh *mesh, aiMaterial *material) {
  vbo[VERTEX_BUFFER] = 0;
  vbo[TEXCOORD_BUFFER] = 0;
  vbo[NORMAL_BUFFER] = 0;
  vbo[INDEX_BUFFER] = 0;

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  vertexCount = mesh->mNumFaces * 3;

  if (mesh->HasPositions()) {
    auto vertices = std::vector<float>(mesh->mNumVertices * 3, 0);
    for (int i = 0; i < mesh->mNumVertices; ++i) {
      vertices[i * 3] = mesh->mVertices[i].x;
      vertices[i * 3 + 1] = mesh->mVertices[i].y;
      vertices[i * 3 + 2] = mesh->mVertices[i].z;
    }

    glGenBuffers(1, &vbo[VERTEX_BUFFER]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[VERTEX_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, 3 * mesh->mNumVertices * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);
  }


  if (mesh->HasTextureCoords(0)) {
    auto texCoords = std::vector<float>(mesh->mNumVertices * 2, 0);
    for (int i = 0; i < mesh->mNumVertices; ++i) {
      texCoords[i * 2] = mesh->mTextureCoords[0][i].x;
      texCoords[i * 2 + 1] = mesh->mTextureCoords[0][i].y;
    }

    glGenBuffers(1, &vbo[TEXCOORD_BUFFER]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[TEXCOORD_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, 2 * mesh->mNumVertices * sizeof(GLfloat), &texCoords[0], GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);
  }


  if (mesh->HasNormals()) {
    auto normals = std::vector<float>(mesh->mNumVertices * 3, 0);
    for (int i = 0; i < mesh->mNumVertices; ++i) {
      normals[i * 3] = mesh->mNormals[i].x;
      normals[i * 3 + 1] = mesh->mNormals[i].y;
      normals[i * 3 + 2] = mesh->mNormals[i].z;
    }

    glGenBuffers(1, &vbo[NORMAL_BUFFER]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[NORMAL_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, 3 * mesh->mNumVertices * sizeof(GLfloat), &normals[0], GL_STATIC_DRAW);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(2);
  }


  if (mesh->HasFaces()) {
    auto indices = std::vector<unsigned int>(mesh->mNumFaces * 3, 0);
    for (int i = 0; i < mesh->mNumFaces; ++i) {
      indices[i * 3] = mesh->mFaces[i].mIndices[0];
      indices[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
      indices[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
    }

    glGenBuffers(1, &vbo[INDEX_BUFFER]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[INDEX_BUFFER]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * mesh->mNumFaces * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(3);
  }


  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  LOGD("Loaded mesh entry: {} with {} vertices and parameters: pos[{}], tex_coord[{}], norm[{}], faces[{}]",
       mesh->mName.C_Str(),
       vertexCount,
       mesh->HasPositions() ? "on" : "off",
       mesh->HasTextureCoords(0) ? "on" : "off",
       mesh->HasNormals() ? "on" : "off",
       mesh->HasFaces() ? "on" : "off");


  // Material loading

  aiString texPath;
  if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texPath) == AI_SUCCESS) {
    std::string fullPath = path + std::string(texPath.C_Str());
    std::replace( fullPath.begin(), fullPath.end(), '\\', '/'); // replace all '\' to '/'
    texID = create_texture(fullPath.c_str());
    if (!texID) {
      LOGE("Failed to load: {}", fullPath);
    } else {
      LOGD("Succeded to load: {}", fullPath);
    }
  }

}

Mesh::MeshEntry::~MeshEntry() {
  if (vbo[VERTEX_BUFFER]) {
    glDeleteBuffers(1, &vbo[VERTEX_BUFFER]);
  }

  if (vbo[TEXCOORD_BUFFER]) {
    glDeleteBuffers(1, &vbo[TEXCOORD_BUFFER]);
  }

  if (vbo[NORMAL_BUFFER]) {
    glDeleteBuffers(1, &vbo[NORMAL_BUFFER]);
  }

  if (vbo[INDEX_BUFFER]) {
    glDeleteBuffers(1, &vbo[INDEX_BUFFER]);
  }

  glDeleteVertexArrays(1, &vao);
}

void Mesh::MeshEntry::Draw(float dt) {
  glBindVertexArray(vao);
  glBindTexture(GL_TEXTURE_2D, texID);
  glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, NULL);
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindVertexArray(0);
}

GLuint create_texture(char const *Filename) {
  gli::texture Texture = gli::load(Filename);
  if (Texture.empty())
  { return 0; }

  gli::gl GL(gli::gl::PROFILE_GL33);
  gli::gl::format const Format = GL.translate(Texture.format(), Texture.swizzles());
  GLenum Target = GL.translate(Texture.target());

  GLuint TextureName = 0;
  glGenTextures(1, &TextureName);
  glBindTexture(Target, TextureName);
  glTexParameteri(Target, GL_TEXTURE_BASE_LEVEL, 0);
  glTexParameteri(Target, GL_TEXTURE_MAX_LEVEL, static_cast<GLint>(Texture.levels() - 1));
  glTexParameteri(Target, GL_TEXTURE_SWIZZLE_R, Format.Swizzles[0]);
  glTexParameteri(Target, GL_TEXTURE_SWIZZLE_G, Format.Swizzles[1]);
  glTexParameteri(Target, GL_TEXTURE_SWIZZLE_B, Format.Swizzles[2]);
  glTexParameteri(Target, GL_TEXTURE_SWIZZLE_A, Format.Swizzles[3]);
  glTexParameteri(Target, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(Target, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(Target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(Target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  glm::tvec3<GLsizei> const Extent(Texture.extent());
  GLsizei const FaceTotal = static_cast<GLsizei>(Texture.layers() * Texture.faces());

  if (Texture.target() != gli::TARGET_2D) {
    LOGE("Failed loading texture");
    assert(0);
    return TextureName;
  }

  glTexStorage2D(Target, static_cast<GLint>(Texture.levels()), Format.Internal, Extent.x, Extent.y);

  for (std::size_t Layer = 0; Layer < Texture.layers(); ++Layer)
    for (std::size_t Face = 0; Face < Texture.faces(); ++Face)
      for (std::size_t Level = 0; Level < Texture.levels(); ++Level) {
        GLsizei const LayerGL = static_cast<GLsizei>(Layer);
        glm::tvec3<GLsizei> Extent(Texture.extent(Level));

        switch (Texture.target()) {
          case gli::TARGET_2D:
            if (gli::is_compressed(Texture.format())) {
              glCompressedTexSubImage2D(
                Target, static_cast<GLint>(Level),
                0, 0,
                Extent.x,
                Extent.y,
                Format.Internal, static_cast<GLsizei>(Texture.size(Level)),
                Texture.data(Layer, Face, Level));
            } else {
              glTexSubImage2D(
                Target, static_cast<GLint>(Level),
                0, 0,
                Extent.x,
                Extent.y,
                Format.External, Format.Type,
                Texture.data(Layer, Face, Level));
            }
            break;
          default: assert(0); break;
        }
      }
  return TextureName;
}
