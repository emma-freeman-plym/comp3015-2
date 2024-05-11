#pragma once

#include "helper/objmesh.h"
#include "helper/texture.h"
#include <map>
#include <string>

template <class T> class Cache {
  std::map<std::string, T> cache;

public:
  Cache() : cache() {}

  T get(std::string key) {
    if (cache.find(key) == cache.end())
      cache[key] = load(key);
    return cache.at(key);
  }

  virtual T load(std::string key) = 0;
};

class MeshCache : public Cache<std::shared_ptr<ObjMesh>> {
  std::shared_ptr<ObjMesh> load(std::string key) {
    return ObjMesh::load(key.c_str(), true);
  }
};

class TextureCache : public Cache<GLuint> {
  GLuint load(std::string key) { return Texture::loadTexture(key.c_str()); }
};
