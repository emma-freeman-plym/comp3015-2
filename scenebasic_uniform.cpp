#include "scenebasic_uniform.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

#include "helper/glutils.h"
#include "helper/texture.h"
#include "shader.h"
#include <glm/gtc/matrix_transform.hpp>

#include "helper/imgui/backends/imgui_impl_glfw.h"
#include "helper/imgui/backends/imgui_impl_opengl3.h"
#include "helper/imgui/imgui.h"

using glm::mat4;
using glm::vec3;
using std::string;

SceneBasic_Uniform::SceneBasic_Uniform() {}

void SceneBasic_Uniform::initScene(void *win) {
  // Compile shaders
  compile();
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.192f, 0.212f, 0.247f, 1.0f);

  // Load mesh
  // This is placed here rather than in the constructor so that
  // shader errors occur before model loading, which speeds up
  // development time.
  mesh = ObjMesh::load("media/chess_piece.obj", true);
  rotation = 0;

  // Set up projection matrices
  model = mat4(1.0f);
  view =
      glm::lookAt(vec3(0.0f, 3.0f, 0.3f), vec3(0.0f), vec3(0.0f, 1.0f, 0.0f));
  projection = mat4(1.0f);

  // Set lighting uniforms
  vec3 v = vec3(0.2f, 0.3f, 0.3f);
  LightInfo lights[] = {
      {view * glm::vec4(5.0f, 5.0f, 2.0f, 1.0f), vec3(0.5f), vec3(0.4f),
       vec3(0.5f)},
      {view * glm::vec4(0.0f, -1.0f, 2.0f, 1.0f), v, v, v},
  };
  unsigned int num_lights = sizeof(lights) / sizeof(lights[0]);

  for (int i = 0; i < num_lights; i++) {
    lights[i].setUniform(&prog, "lights[" + std::to_string(i) + "]");
  }
  prog.setUniform("num_lights", num_lights);

  // Set material uniform
  MaterialInfo mat{vec3(0.2f, 0.55f, 0.9f), vec3(0.2f, 0.55f, 0.9f), vec3(1.0f),
                   100.0f};
  mat.setUniform(&prog, "material");

  // Load textures
  GLuint diffuse = Texture::loadTexture("media/Substance_Graph_BaseColor.jpg");
  GLuint overlay = Texture::loadTexture(
      "media/Surface_Imperfections_Cracks_001_basecolor.jpg");
  GLuint opacity = Texture::loadTexture(
      "media/Surface_Imperfections_Cracks_001_opacity.jpg");
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, diffuse);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, overlay);
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, opacity);

  // Setup ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  // Due to compilation issues, `scene.h` can't see `GLFW/glfw3.h`.
  // Instead, window is passed as a void pointer and cast.
  // Yes, this is bad.
  // If I had more time, I'd debug it, but for now it works.
  ImGui_ImplGlfw_InitForOpenGL((GLFWwindow *)win, true);
  ImGui_ImplOpenGL3_Init();
}

void SceneBasic_Uniform::compile() {
  try {
    prog.compileShader("shader/basic_uniform.vert");
    prog.compileShader("shader/basic_uniform.frag");
    prog.link();
    prog.use();
  } catch (GLSLProgramException &e) {
    std::cerr << e.what() << std::endl;
    exit(EXIT_FAILURE);
  }
}

void SceneBasic_Uniform::update(float t) {
  rotation += 0.04f;
  time = t;

  // Update ImGui, handle input etc
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  ImGui::ShowDemoWindow();
}

void SceneBasic_Uniform::render() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Set time
  prog.setUniform("time", time);

  // Rotate the 0th light
  glm::vec4 base = glm::vec4(5.0f, 5.0f, 2.0f, 1.0f);
  prog.setUniform("lights[0].position",
                  base * glm::rotate(mat4(1.0f), glm::radians(rotation),
                                     vec3(0.0f, 1.0f, 0.0f)));

  // Render mesh by setting matrix uniforms and calling method.
  model = glm::rotate(mat4(1.0f), glm::radians(-35.0f), vec3(1.0f, 0.0f, 0.0f));
  model =
      glm::rotate(model, glm::radians(rotation / 6), vec3(0.0f, 1.0f, 0.0f));
  setMatrices();
  mesh->render();

  // Render ImGui
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void SceneBasic_Uniform::resize(int w, int h) {
  // When the window resizes, recalculate the projection matrix to
  // use the new aspect ratio.
  width = w;
  height = h;
  glViewport(0, 0, w, h);
  projection =
      glm::perspective(glm::radians(70.0f), (float)w / h, 0.3f, 100.0f);
}

void SceneBasic_Uniform::setMatrices() {
  // Set shader uniforms to model view matrices.
  mat4 mv = view * model;
  prog.setUniform("model_view_matrix", mv);
  prog.setUniform("normal_matrix",
                  glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
  prog.setUniform("model_view_projection", projection * mv);
}
