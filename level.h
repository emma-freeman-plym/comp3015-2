#pragma once

#include "helper/json/json.hpp"

#include "light.h"
#include "object.h"

struct Level {
  glm::vec3 orbit;
  float dist;
  std::vector<Object> objects;
  std::vector<Light> lights;

  Level() {
    orbit = glm::vec3(0.0f, 0.0f, 0.0f);
    dist = 5.0f;
    objects = {};
    lights = {};
  }

  Level(glm::vec3 orbit, float dist, std::vector<Object> objects,
        std::vector<Light> lights)
      : orbit(orbit), dist(dist), objects(objects), lights(lights) {}

  json serialize() {
    json j;
    j["orbit"] = {orbit.x, orbit.y, orbit.z};
    j["dist"] = dist;
    for (auto &o : objects) {
      j["objects"].push_back(o.serialize());
    }
    for (auto &l : lights) {
      j["lights"].push_back(l.serialize());
    }
    return j;
  }

  static Level deserialize(json j) {
    glm::vec3 orbit(j["orbit"][0], j["orbit"][1], j["orbit"][2]);
    float dist = j["dist"];
    std::vector<Object> objects;
    for (auto &o : j["objects"]) {
      objects.push_back(Object::deserialize(o));
    }
    std::vector<Light> lights;
    for (auto &l : j["lights"]) {
      lights.push_back(Light::deserialize(l));
    }
    return Level(orbit, dist, objects, lights);
  }
};
