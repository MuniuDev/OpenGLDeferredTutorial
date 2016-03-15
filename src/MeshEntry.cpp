/*
* Copyright by Michal Majczak & Krzysztof Taperek, 2016
* Licensed under the MIT license:
* http://www.opensource.org/licenses/mit-license.php
*
* Author: Michal Majczak <michal.majczak92@gmail.com>
*/

#include <MeshData.hpp>

#include <assimp/scene.h>

#include <SOIL.h>

#include <vector>
#include <algorithm>
#include <string>

static const int MIPMAP_AMOUNT = 4;

GLuint create_texture(char const *Filename);

MeshData::MeshEntry::MeshEntry(const std::string &path,
                               aiMesh *mesh,
                               aiMaterial *material) {
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
    glBufferData(GL_ARRAY_BUFFER, 3 * mesh->mNumVertices * sizeof(GLfloat),
                 &vertices[0], GL_STATIC_DRAW);

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
    glBufferData(GL_ARRAY_BUFFER, 2 * mesh->mNumVertices * sizeof(GLfloat),
                 &texCoords[0], GL_STATIC_DRAW);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
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
    glBufferData(GL_ARRAY_BUFFER, 3 * mesh->mNumVertices * sizeof(GLfloat),
                 &normals[0], GL_STATIC_DRAW);

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
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * mesh->mNumFaces * sizeof(GLuint),
                 &indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(3);
  }


  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  LOGD("Loaded mesh entry: {} with {} vertices, {} faces and parameters: "\
       "pos[{}], tex_coord[{}], norm[{}], faces[{}]",
       mesh->mName.C_Str(),
       vertexCount,
       mesh->mNumFaces,
       mesh->HasPositions() ? "on" : "off",
       mesh->HasTextureCoords(0) ? "on" : "off",
       mesh->HasNormals() ? "on" : "off",
       mesh->HasFaces() ? "on" : "off");


  // Material loading

  aiString texPath;
  if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texPath) == AI_SUCCESS) {
    std::string fullPath = path + std::string(texPath.C_Str());

    // replace all '\' to '/', fix for linux machines
    std::replace( fullPath.begin(), fullPath.end(), '\\', '/');

    texID = create_texture(fullPath.c_str());
    if (!texID) {
      LOGE("Failed to load: {}", fullPath);
    } else {
      LOGD("Succeded to load: {}", fullPath);
    }
  }

  // Material params loading
  material->Get(AI_MATKEY_SHININESS_STRENGTH, mtl.specularIntensity);
  material->Get(AI_MATKEY_SHININESS, mtl.specularPower);

  LOGD("Specular: {}, {}", mtl.specularIntensity, mtl.specularPower);
}

MeshData::MeshEntry::~MeshEntry() {
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

void MeshData::MeshEntry::Draw(float dt) {
  glBindVertexArray(vao);
  glBindTexture(GL_TEXTURE_2D, texID);
  glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, NULL);
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindVertexArray(0);
}

GLuint create_texture(char const *Filename) {

  LOGD("SOIL loading: '{}'", Filename);
  int width, height, channels;
  unsigned char *img = SOIL_load_image(Filename,
                                       &width,
                                       &height,
                                       &channels,
                                       SOIL_LOAD_AUTO);

  /* check for an error during the load process */
  if (!img) {
    LOGE("SOIL loading error: '{}'", SOIL_last_result());
    return 0;
  }
  LOGD("SOIL loading succesfull. {}, {}, {}", width, height, channels);

  // Reverse Y axis
  std::vector<unsigned char> img_y_rev(width * height * channels, 0);
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      for (int k = 0; k < channels; ++k) {
        img_y_rev[(height - i - 1)*width * 3 + j * 3 + k]
          = img[i * width * 3 + j * 3 + k];
      }
    }
  }

  GLuint TextureName = 0;
  glEnable(GL_TEXTURE_2D);
  glGenTextures(1, &TextureName);
  glBindTexture(GL_TEXTURE_2D, TextureName);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, MIPMAP_AMOUNT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
               GL_RGB, GL_UNSIGNED_BYTE, &img_y_rev[0]);

  glGenerateMipmap(GL_TEXTURE_2D);

  SOIL_free_image_data(img);

  return TextureName;
}
