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

private:
  std::shared_ptr<ObjMesh> mesh;

public:
  GLuint diffuse = 0;
  GLuint overlay = 0;
  GLuint opacity = 0;
  vec3 pos = vec3(0.0);
  vec3 rot = vec3(0.0);
  MaterialInfo mat;

  Object(const char *filename) { mesh = ObjMesh::load(filename, true); }

  mat4 matrix() {
    // Compute a model matrix from the position and rotation.
    mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, pos);
    model = glm::rotate(model, glm::radians(rot.x), vec3(1, 0, 0));
    model = glm::rotate(model, glm::radians(rot.y), vec3(0, 1, 0));
    model = glm::rotate(model, glm::radians(rot.z), vec3(0, 0, 1));
    return model;
  }

  void setUniforms(GLSLProgram *prog) {
    // Set the model matrix and textures
    // This is a bit inefficient, since rebinding the
    // textures each frame can be slow.
    // The solution to this would be a caching layer but
    // thats a bit out of scope.
    mat.setUniform(prog, "material");
    if (diffuse > 0) {
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, diffuse);
    }
    if (overlay > 0) {
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, overlay);
    }
    if (opacity > 0) {
      glActiveTexture(GL_TEXTURE2);
      glBindTexture(GL_TEXTURE_2D, opacity);
    }
  }

  void render() { mesh->render(); }
};
