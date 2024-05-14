#pragma once

#include "helper/json/json.hpp"
#include "helper/objmesh.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include "material.h"
#include "scenebasic_uniform.h"

using glm::mat4;
using glm::vec3;
using json = nlohmann::json;

struct Object {
  std::string name;
  std::string mesh;
  std::string diffuse;
  std::string overlay;
  std::string opacity;
  Material mat;
  vec3 pos = vec3(0.0);
  vec3 rot = vec3(0.0);
  vec3 scale = vec3(1.0);

  Object(std::string name, std::string mesh, std::string diffuse,
         std::string overlay, std::string opacity, Material mat, vec3 pos,
         vec3 rot, vec3 scale)
      : name(name), mesh(mesh), diffuse(diffuse), overlay(overlay),
        opacity(opacity), mat(mat), pos(pos), rot(rot), scale(scale) {}

  void properties();

  mat4 matrix() {
    // Compute a model matrix from the position and rotation.
    mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, pos);
    model = glm::scale(model, scale);
    model = glm::rotate(model, glm::radians(rot.x), vec3(1, 0, 0));
    model = glm::rotate(model, glm::radians(rot.y), vec3(0, 1, 0));
    model = glm::rotate(model, glm::radians(rot.z), vec3(0, 0, 1));
    return model;
  }

  json serialize() {
    return {{"name", name},
            {"mesh", mesh},
            {"diffuse", diffuse},
            {"overlay", overlay},
            {"opacity", opacity},
            {"mat", mat.serialize()},
            {"pos", {pos.x, pos.y, pos.z}},
            {"rot", {rot.x, rot.y, rot.z}},
            {"scale", {scale.x, scale.y, scale.z}}};
  }

  static Object deserialize(json j) {
    return Object(j["name"], j["mesh"], j["diffuse"], j["overlay"],
                  j["opacity"], Material::deserialize(j["mat"]),
                  {j["pos"][0], j["pos"][1], j["pos"][2]},
                  {j["rot"][0], j["rot"][1], j["rot"][2]},
                  {j["scale"][0], j["scale"][1], j["scale"][2]});
  }
};
