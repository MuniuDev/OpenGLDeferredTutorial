/*
* Copyright by Michal Majczak & Krzysztof Taperek, 2016
* Licensed under the MIT license:
* http://www.opensource.org/licenses/mit-license.php
*
* Author: Michal Majczak <michal.majczak92@gmail.com>
*/

#include <Mesh.hpp>

#include <assimp/scene.h>

#include <vector>

Mesh::MeshEntry::MeshEntry(aiMesh *mesh) {
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

  LOGD("Loaded mesh entry with {} vertices and parameters: pos[{}], tex_coord[{}], norm[{}], faces[{}]",
       vertexCount,
       mesh->HasPositions() ? "on" : "off",
       mesh->HasTextureCoords(0) ? "on" : "off",
       mesh->HasNormals() ? "on" : "off",
       mesh->HasFaces() ? "on" : "off");
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
  glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, NULL);
  glBindVertexArray(0);
}