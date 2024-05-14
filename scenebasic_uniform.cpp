#include "scenebasic_uniform.h"

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include "helper/glutils.h"
#include "helper/texture.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "helper/imgui/backends/imgui_impl_glfw.h"
#include "helper/imgui/backends/imgui_impl_opengl3.h"
#include "helper/imgui/imgui.h"
#include "helper/imgui/misc/cpp/imgui_stdlib.h"
#include "helper/tinyfiledialogs/tinyfiledialogs.h"

#include "light.h"
#include "material.h"
#include "object.h"

using glm::mat4;
using glm::vec3;
using std::string;

SceneBasic_Uniform::SceneBasic_Uniform() : tex_cache(), mesh_cache() {}

void SceneBasic_Uniform::initScene(void *win) {

  // Compile shaders
  compile();
  glEnable(GL_DEPTH_TEST);

  level = Level();
  level.lights.push_back(
      Light("Default light", POINT, vec3(5.0f, 5.0f, 2.0f), vec3(50.0)));
  level.background = vec3(0.192f, 0.212f, 0.247f);

  // Set up projection matrices
  model = mat4(1.0f);
  projection = mat4(1.0f);

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
    prog.compileShader("shader/basic_uniform.gs");
    prog.link();
    prog.use();
  } catch (GLSLProgramException &e) {
    std::cerr << e.what() << std::endl;
    exit(EXIT_FAILURE);
  }
}

void SceneBasic_Uniform::update(float t) {
  time = t;

  // Update ImGui, handle input etc
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  {
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(200, 0));
    ImGui::Begin("Scene");

    ImVec2 full = ImVec2(-FLT_MIN, 0.0f);
    const char *filter[1] = {"*.json"};

    if (ImGui::Button("Save", full)) {

      char const *file = NULL;
      try {
        file = tinyfd_saveFileDialog("Save Scene", NULL, 1, filter, NULL);
      } catch (...) {
      }

      if (file) {
        std::ofstream out(file);
        out << level.serialize() << std::endl;
        out.close();
      }
    }

    if (ImGui::Button("Load", full)) {
      char const *file = NULL;
      try {
        file = tinyfd_openFileDialog("Load Scene", NULL, 1, filter, NULL, 0);
      } catch (...) {
      }

      if (file) {
        std::ifstream in(file);
        json j;
        in >> j;
        in.close();
        level = Level::deserialize(j);
      }
    }

    ImGui::SeparatorText("Camera");
    ImGui::DragFloat3("Orbit", glm::value_ptr(level.orbit), 1.0, -360, 360);
    ImGui::DragFloat("Dist", &level.dist, 0.1, 0.1, 20.0);
    ImGui::ColorEdit3("Color", glm::value_ptr(level.background));

    ImGui::SeparatorText("Debug");
    ImGui::Checkbox("Wireframe", &wireframe);

    ImGui::SeparatorText("Objects");

    float w = ImGui::GetWindowWidth() - ImGui::GetStyle().ItemSpacing.x * 3;
    ImVec2 half = ImVec2(w / 2, 0.0f);

    if (ImGui::Button("Add##objects", half)) {
      level.objects.push_back(Object("New object", "media/cube.obj", "", "", "",
                                     Material(vec3(0.5), 0.5, false, 0, 0, 1),
                                     vec3(0.0), vec3(0.0), vec3(1.0)));
    }
    ImGui::SameLine();
    if (ImGui::Button("Remove##objects", half)) {
      if (select == OBJECT) {
        level.objects.erase(level.objects.begin() + select_index);
        select = NONE;
      }
    }

    if (ImGui::BeginListBox("##objects", ImVec2(-FLT_MIN, 120))) {
      auto i = 0;
      for (auto &obj : level.objects) {
        auto sel = select == OBJECT && select_index == i;
        auto name = obj.name + "##" + std::to_string(i);
        if (ImGui::Selectable(name.c_str(), sel)) {
          select = OBJECT;
          select_index = i;
        }
        if (sel)
          ImGui::SetItemDefaultFocus();
        i++;
      }
      ImGui::EndListBox();
    }

    ImGui::SeparatorText("Lights");

    if (ImGui::Button("Add##lights", half)) {
      level.lights.push_back(Light("New light", POINT,
                                   view * glm::vec4(5.0f, 5.0f, 2.0f, 1.0f),
                                   vec3(50.0)));
    }
    ImGui::SameLine();
    if (ImGui::Button("Remove##lights", half)) {
      if (select == LIGHT) {
        level.lights.erase(level.lights.begin() + select_index);
        select = NONE;
      }
    }

    if (ImGui::BeginListBox("##lights", ImVec2(-FLT_MIN, 150))) {
      auto i = 0;
      for (auto &light : level.lights) {
        auto sel = select == LIGHT && select_index == i;
        auto name = light.name + "##" + std::to_string(i);
        if (ImGui::Selectable(name.c_str(), sel)) {
          select = LIGHT;
          select_index = i;
        }
        if (sel)
          ImGui::SetItemDefaultFocus();
        i++;
      }
      ImGui::EndListBox();
    }

    ImGui::End();
  }

  {
    ImGui::SetNextWindowPos(ImVec2(1200 - 200, 0));
    ImGui::SetNextWindowSize(ImVec2(200, 450));
    ImGui::Begin("Properties");

    if (select == OBJECT) {
      Object *obj = &level.objects[select_index];

      ImVec2 size = ImVec2(-FLT_MIN, 0);

      const char *mesh_filters[2] = {"*.obj", "*.fbx"};
      const char *tex_filters[2] = {"*.jpg", "*.png"};

      ImGui::InputText("Name", &obj->name);

      if (ImGui::Button("Mesh", size)) {
        char const *file = NULL;
        try {
          file = tinyfd_openFileDialog("Select file", NULL, 2, mesh_filters,
                                       NULL, 0);
        } catch (...) {
        }
        if (file)
          obj->mesh = std::string(file);
      }

      ImGui::SeparatorText("Textures");

      if (ImGui::Button("Diffuse", size)) {
        char const *file = NULL;
        try {
          file = tinyfd_openFileDialog("Select file", NULL, 2, tex_filters,
                                       NULL, 0);
        } catch (...) {
        }
        if (file)
          obj->diffuse = std::string(file);
        else
          obj->diffuse = "";
      }
      if (ImGui::Button("Overlay", size)) {
        char const *file = NULL;
        try {
          file = tinyfd_openFileDialog("Select file", NULL, 2, tex_filters,
                                       NULL, 0);
        } catch (...) {
        }
        if (file)
          obj->overlay = std::string(file);
        else
          obj->overlay = "";
      }
      if (ImGui::Button("Opacity", size)) {
        char const *file = NULL;
        try {
          file = tinyfd_openFileDialog("Select file", NULL, 2, tex_filters,
                                       NULL, 0);
        } catch (...) {
        }
        if (file)
          obj->opacity = std::string(file);
        else
          obj->opacity = "";
      }

      ImGui::SeparatorText("Transform");

      ImGui::DragFloat3("Position", glm::value_ptr(obj->pos), 1.0, -5, 5);
      ImGui::DragFloat3("Rotation", glm::value_ptr(obj->rot), 1.0, -360, 360);
      ImGui::DragFloat3("Scale", glm::value_ptr(obj->scale), 1.0, 0.05, 5);

      ImGui::SeparatorText("Material");

      ImGui::ColorEdit3("Color", glm::value_ptr(obj->mat.color));
      ImGui::SliderFloat("Roughness", &obj->mat.rough, 0.0, 1.0);
      ImGui::Checkbox("Metal", &obj->mat.metal);

      ImGui::SeparatorText("Animation");

      ImGui::DragFloat("Frequency", &obj->mat.frequency, 0.1, 0.1, 10.0);
      ImGui::DragFloat("Velocity", &obj->mat.velocity, 0.1, 0.1, 10.0);
      ImGui::DragFloat("Amplitude", &obj->mat.amplitude, 0.1, 0.1, 10.0);

    } else if (select == LIGHT) {
      Light *light = &level.lights[select_index];
      ImVec2 size = ImVec2(-FLT_MIN, 0);
      const char *kinds[] = {"Point", "Directional"};

      ImGui::InputText("Name", &light->name);
      ImGui::Combo("combo", (int *)&light->kind, kinds, 2);
      ImGui::DragFloat3("Position", glm::value_ptr(light->position), 1.0, -5,
                        5);
      ImGui::DragFloat3("Intensity", glm::value_ptr(light->intensity), 1.0, 0,
                        100);
    }

    ImGui::End();
  }
}

void SceneBasic_Uniform::render() {
  glClearColor(level.background.x, level.background.y, level.background.z,
               1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Set uniforms
  prog.setUniform("time", time);
  prog.setUniform("wireframe", wireframe);

  // Set up view matrix
  mat4 v = glm::lookAt(vec3(0.0f, 0.0f, level.dist), vec3(0.0f),
                       vec3(0.0f, 1.0f, 0.0f));
  v = glm::rotate(v, glm::radians(level.orbit.x), vec3(1.0f, 0.0f, 0.0f));
  v = glm::rotate(v, glm::radians(level.orbit.y), vec3(0.0f, 1.0f, 0.0f));
  v = glm::rotate(v, glm::radians(level.orbit.z), vec3(0.0f, 0.0f, 1.0f));
  view = v;

  unsigned int num_lights = level.lights.size();

  for (int i = 0; i < num_lights; i++) {
    level.lights[i].setUniform(&prog, "lights[" + std::to_string(i) + "]");
  }
  prog.setUniform("num_lights", num_lights);

  for (auto &obj : level.objects) {
    model = obj.matrix();
    setMatrices();
    glActiveTexture(GL_TEXTURE0);
    if (obj.diffuse.length() > 0) {
      prog.setUniform("material.diffuse", true);
      glBindTexture(GL_TEXTURE_2D, tex_cache.get(obj.diffuse));
    } else {
      prog.setUniform("material.diffuse", false);
      glBindTexture(GL_TEXTURE_2D, 0);
    }

    glActiveTexture(GL_TEXTURE1);
    if (obj.overlay.length() > 0) {
      prog.setUniform("material.overlay", true);
      glBindTexture(GL_TEXTURE_2D, tex_cache.get(obj.overlay));
    } else {
      prog.setUniform("material.overlay", false);
      glBindTexture(GL_TEXTURE_2D, 0);
    }

    glActiveTexture(GL_TEXTURE2);
    if (obj.opacity.length() > 0) {
      glBindTexture(GL_TEXTURE_2D, tex_cache.get(obj.opacity));
    } else {
      glBindTexture(GL_TEXTURE_2D, 0);
    }

    obj.mat.setUniform(&prog, "material");
    mesh_cache.get(obj.mesh)->render();
  }

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

  float w2 = w / 2.0f;
  float h2 = h / 2.0f;
  viewport = mat4(
      glm::vec4(w2, 0.0f, 0.0f, 0.0f), glm::vec4(0.0f, h2, 0.0f, 0.0f),
      glm::vec4(0.0f, 0.0f, 1.0f, 0.0f), glm::vec4(w2 + 0, h2 + 0, 0.0f, 1.0f));
}

void SceneBasic_Uniform::setMatrices() {
  // Set shader uniforms to model view matrices.
  mat4 mv = view * model;
  prog.setUniform("model_view_matrix", mv);
  prog.setUniform("normal_matrix",
                  glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
  prog.setUniform("model_view_projection", projection * mv);
  prog.setUniform("viewport_matrix", viewport);
}
