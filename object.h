#pragma once

#include "helper/objmesh.h"
#include "shader.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "scenebasic_uniform.h"

using glm::mat4;
using glm::vec3;

struct Object {
  std::string name;
  std::string diffuse;
  std::string overlay;
  std::string opacity;
  MaterialInfo mat;
  vec3 pos = vec3(0.0);
  vec3 rot = vec3(0.0);

  mat4 matrix() {
    // Compute a model matrix from the position and rotation.
    mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, pos);
    model = glm::rotate(model, glm::radians(rot.x), vec3(1, 0, 0));
    model = glm::rotate(model, glm::radians(rot.y), vec3(0, 1, 0));
    model = glm::rotate(model, glm::radians(rot.z), vec3(0, 0, 1));
    return model;
  }
};
