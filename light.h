#pragma once

#include "helper/glslprogram.h"
#include "helper/json/json.hpp"
#include <glm/glm.hpp>

using json = nlohmann::json;

enum LightKind { POINT, DIRECTIONAL };

struct Light {
  LightKind kind;
  glm::vec3 position = glm::vec3(0.0);
  float intensity = 50.0;

  void setUniform(GLSLProgram *prog, std::string name) {
    prog->setUniform((name + ".position").c_str(), position);
    prog->setUniform((name + ".intensity").c_str(), intensity);
  }

  json serialize() {
    return {
        {"kind", kind},
        {"position", {position.x, position.y, position.z}},
        {"intensity", intensity},
    };
  }

  static Light deserialize(json j) {
    return {
        j["kind"],
        {j["pos"][0], j["pos"][1], j["pos"][2]},
        j["intensity"],
    };
  }
};
