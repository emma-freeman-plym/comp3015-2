#pragma once

#include "helper/glslprogram.h"
#include "helper/json/json.hpp"
#include <glm/glm.hpp>

#include "editable.h"

using json = nlohmann::json;

enum LightKind { POINT, DIRECTIONAL };

struct Light : Editable {
  std::string name;
  LightKind kind;
  glm::vec3 position = glm::vec3(0.0);
  glm::vec3 intensity = glm::vec3(50.0);

  Light(std::string name, LightKind kind, glm::vec3 position,
        glm::vec3 intensity)
      : name(name), kind(kind), position(position), intensity(intensity) {}

  void properties();

  void setUniform(GLSLProgram *prog, std::string name) {
    prog->setUniform((name + ".position").c_str(), position);
    prog->setUniform((name + ".intensity").c_str(), intensity);
  }

  json serialize() {
    return {
        {"name", name},
        {"kind", kind},
        {"position", {position.x, position.y, position.z}},
        {"intensity", {intensity.x, intensity.y, intensity.z}},
    };
  }

  static Light deserialize(json j) {
    return Light(j["name"], j["kind"],
                 {j["position"][0], j["position"][1], j["position"][2]},
                 {j["intensity"][0], j["intensity"][1], j["intensity"][2]});
  }
};
