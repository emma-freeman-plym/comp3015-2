#pragma once

#include "helper/glslprogram.h"
#include "helper/json/json.hpp"
#include <glm/glm.hpp>

using json = nlohmann::json;

struct Material {
  glm::vec3 color;
  float rough;
  bool metal;

  Material(glm::vec3 color, float rough, bool metal)
      : color(color), rough(rough), metal(metal) {}

  void setUniform(GLSLProgram *prog, std::string name) {
    prog->setUniform((name + ".color").c_str(), color);
    prog->setUniform((name + ".rough").c_str(), rough);
    prog->setUniform((name + ".metal").c_str(), metal);
  }

  json serialize() {
    return {
        {"color", {color.x, color.y, color.z}},
        {"rough", rough},
        {"metal", metal},
    };
  }

  static Material deserialize(json j) {
    return {
        {j["color"][0], j["color"][1], j["color"][2]},
        j["rough"],
        j["metal"],
    };
  }
};
