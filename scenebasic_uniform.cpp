#include "scenebasic_uniform.h"

#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>

#include "helper/glutils.h"
#include <glm/gtc/matrix_transform.hpp>

using glm::mat4;
using glm::vec3;
using std::string;

SceneBasic_Uniform::SceneBasic_Uniform() : torus(0.7f, 0.3f, 30, 30) {}

void SceneBasic_Uniform::initScene()
{
    // Compile shaders
    compile();
    // Enable depth buffers
    glEnable(GL_DEPTH_TEST);
    // Set up projection matrices
    model = mat4(1.0f);
    model = glm::rotate(model, glm::radians(-35.0f), vec3(1.0f, 0.0f, 0.0f));
    view = glm::lookAt(vec3(0.0f, 0.0f, 2.0f), vec3(0.0f), vec3(0.0f, 1.0f, 0.0f));
    projection = mat4(1.0f);

    // Set lighting information
    prog.setUniform("light.position", view * glm::vec4(5.0f, 5.0f, 2.0f, 1.0f));
    prog.setUniform("light.ld", vec3(1.0f));
    prog.setUniform("light.la", vec3(0.4f));
    prog.setUniform("light.ls", vec3(1.0f));

    // Set material information
    prog.setUniform("material.kd", vec3(0.2f, 0.55f, 0.9f));
    prog.setUniform("material.ka", vec3(0.2f, 0.55f, 0.9f));
    prog.setUniform("material.ks", vec3(0.8f));
    prog.setUniform("material.shiny", 100.0f);
}

void SceneBasic_Uniform::compile()
{
    try
    {
        prog.compileShader("shader/basic_uniform.vert");
        prog.compileShader("shader/basic_uniform.frag");
        prog.link();
        prog.use();
    }
    catch (GLSLProgramException &e)
    {
        std::cerr << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
}

void SceneBasic_Uniform::update(float t) {}

void SceneBasic_Uniform::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render torus by setting matrix uniforms and calling method.
    setMatrices();
    torus.render();
}

void SceneBasic_Uniform::resize(int w, int h)
{
    // When the window resizes, recalculate the projection matrix to
    // use the new aspect ratio.
    width = w;
    height = h;
    glViewport(0, 0, w, h);
    projection = glm::perspective(glm::radians(70.0f), (float)w / h, 0.3f, 100.0f);
}

void SceneBasic_Uniform::setMatrices()
{
    // Set shader uniforms to model view matrices.
    mat4 mv = view * model;
    prog.setUniform("model_view_matrix", mv);
    prog.setUniform("normal_matrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
    prog.setUniform("model_view_projection", projection * mv);
}