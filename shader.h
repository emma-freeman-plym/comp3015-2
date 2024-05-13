#pragma once

#include "helper/glslprogram.h"

#include <glm/glm.hpp>

struct LightInfo {
  glm::vec4 position;
  float intensity;

  void setUniform(GLSLProgram *prog, std::string name) {
    prog->setUniform((name + ".position").c_str(), position);
    prog->setUniform((name + ".intensity").c_str(), intensity);
  }
};

struct MaterialInfo {
  glm::vec3 color;
  float rough;
  bool metal;

  void setUniform(GLSLProgram *prog, std::string name) {
    prog->setUniform((name + ".color").c_str(), color);
    prog->setUniform((name + ".rough").c_str(), rough);
    prog->setUniform((name + ".metal").c_str(), metal);
  }
};
