#include "helper/glslprogram.h"

#include <glm/glm.hpp>

struct LightInfo
{
    glm::vec4 position;
    glm::vec3 ld;
    glm::vec3 la;
    glm::vec3 ls;

    void setUniform(GLSLProgram *prog, std::string name)
    {
        prog->setUniform((name + ".position").c_str(), position);
        prog->setUniform((name + ".ld").c_str(), ld);
        prog->setUniform((name + ".la").c_str(), la);
        prog->setUniform((name + ".ls").c_str(), ls);
    }
};

struct MaterialInfo
{
    glm::vec3 kd;
    glm::vec3 ka;
    glm::vec3 ks;
    float shiny;

    void setUniform(GLSLProgram *prog, std::string name)
    {
        prog->setUniform((name + ".kd").c_str(), kd);
        prog->setUniform((name + ".ka").c_str(), ka);
        prog->setUniform((name + ".ks").c_str(), ks);
        prog->setUniform((name + ".shiny").c_str(), shiny);
    }
};
