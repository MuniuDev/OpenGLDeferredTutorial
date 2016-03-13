/*
* Copyright by Michal Majczak & Krzysztof Taperek, 2016
* Licensed under the MIT license:
* http://www.opensource.org/licenses/mit-license.php
*
* Author: Michal Majczak <michal.majczak92@gmail.com>
*/

#pragma once

#include <Common.hpp>

#include <string>
#include <map>

// Text for fonts:
// http://nehe.gamedev.net/tutorial/freetype_fonts_in_opengl/24001/

class ShaderProgram {
 public:
  ShaderProgram(const std::string &vertexShader,
                const std::string &fragmentShader);
  void BindProgram() const;
  void CompileProgram();
  void Validate();
  void LoadShader(int type, const std::string &shader);

  GLuint GetProgramHandle() const;

  void RegisterUniform(const std::string& name);
  void SetUniform(const std::string& name, int val);
  void SetUniform(const std::string& name, float val);
  void SetUniform(const std::string& name, const glm::vec3& val);
  void SetUniform(const std::string& name, const glm::mat4& val);

 private:
  std::string LoadShaderCode(const std::string &shader) const;
  std::map<std::string, GLuint> m_uniforms;
  GLuint m_program;
};
