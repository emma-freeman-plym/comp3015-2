#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include "helper/glslprogram.h"
#include "helper/objmesh.h"
#include "helper/scene.h"

#include "cache.h"
#include "level.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

enum Select { NONE, OBJECT, LIGHT };

class SceneBasic_Uniform : public Scene {
private:
  GLSLProgram prog;
  Level level;
  TextureCache tex_cache;
  MeshCache mesh_cache;
  Select select;
  unsigned int select_index = 0;
  glm::mat4 viewport;
  bool wireframe;
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
