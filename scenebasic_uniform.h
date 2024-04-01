#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include "helper/scene.h"
#include "helper/torus.h"
#include "helper/glslprogram.h"

#include <glm/glm.hpp>
#include <glad/glad.h>

class SceneBasic_Uniform : public Scene
{
private:
    Torus torus;
    GLSLProgram prog;

    void compile();
    void setMatrices();

public:
    SceneBasic_Uniform();

    void initScene();
    void update(float t);
    void render();
    void resize(int, int);
};

#endif // SCENEBASIC_UNIFORM_H
