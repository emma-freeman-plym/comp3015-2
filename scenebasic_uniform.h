#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include "helper/glslprogram.h"
#include "helper/objmesh.h"
#include "helper/scene.h"

#include "cache.h"
#include "light.h"
#include "object.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

class SceneBasic_Uniform : public Scene {
private:
  GLSLProgram prog;
  std::vector<Object> objects;
  std::vector<Light> lights;
  TextureCache tex_cache;
  MeshCache mesh_cache;
  int obj_index = -1;
  int light_index = -1;
  Editable *selected = nullptr;

  float time;

  void compile();
  void setMatrices();

public:
  SceneBasic_Uniform();

  void initScene(void *win);
  void update(float t);
  void render();
  void resize(int, int);
};

#endif // SCENEBASIC_UNIFORM_H
