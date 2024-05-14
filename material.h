#pragma once

#include "helper/glslprogram.h"
#include "helper/json/json.hpp"
#include <glm/glm.hpp>

using json = nlohmann::json;

struct Material {
  glm::vec3 color;
  float rough;
  bool metal;
  float frequency;
  float velocity;
  float amplitude;

  Material(glm::vec3 color, float rough, bool metal, float frequency,
           float velocity, float amplitude)
      : color(color), rough(rough), metal(metal), frequency(frequency),
        velocity(velocity), amplitude(amplitude) {}

  void setUniform(GLSLProgram *prog, std::string name) {
    prog->setUniform((name + ".color").c_str(), color);
    prog->setUniform((name + ".rough").c_str(), rough);
    prog->setUniform((name + ".metal").c_str(), metal);
    prog->setUniform((name + ".frequency").c_str(), frequency);
    prog->setUniform((name + ".velocity").c_str(), velocity);
    prog->setUniform((name + ".amplitude").c_str(), amplitude);
  }

  json serialize() {
    return {
        {"color", {color.x, color.y, color.z}},
        {"rough", rough},
        {"metal", metal},
        {"frequency", frequency},
        {"velocity", velocity},
        {"amplitude", amplitude},
    };
  }

  static Material deserialize(json j) {
    return {
        {j["color"][0], j["color"][1], j["color"][2]},
        j["rough"],
        j["metal"],
        j["frequency"],
        j["velocity"],
        j["amplitude"],
    };
  }
};
