#pragma once

#include "helper/json/json.hpp"

#include "light.h"
#include "object.h"

struct Level {
  std::vector<Object> objects;
  std::vector<Light> lights;

  Level() {
    objects = {};
    lights = {};
  }

  Level(std::vector<Object> objects, std::vector<Light> lights)
      : objects(objects), lights(lights) {}

  json serialize() {
    json j;
    for (auto &o : objects) {
      j["objects"].push_back(o.serialize());
    }
    for (auto &l : lights) {
      j["lights"].push_back(l.serialize());
    }
    return j;
  }

  static Level deserialize(json j) {
    std::vector<Object> objects;
    for (auto &o : j["objects"]) {
      objects.push_back(Object::deserialize(o));
    }
    std::vector<Light> lights;
    for (auto &l : j["lights"]) {
      lights.push_back(Light::deserialize(l));
    }
    return Level(objects, lights);
  }
};
