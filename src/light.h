#ifndef LIGHT_H
#define LIGHT_H 

#include <glm/common.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"

// materials are defined as a color for each
// phong lighting componenet and a shininess value
class Light {
public:
    glm::vec3 ambient;    // ambient strength
    glm::vec3 diffuse;    // diffuse strength
    glm::vec3 specular;   // specular strength

    Light(glm::vec3 ambient,
             glm::vec3 diffuse,
             glm::vec3 specular) :
        ambient(ambient),
        diffuse(diffuse),
        specular(specular) {};

    void use(Shader& shader) {
        shader.setVec3("light.ambient", glm::value_ptr(ambient));
        shader.setVec3("light.diffuse", glm::value_ptr(diffuse));
        shader.setVec3("light.specular", glm::value_ptr(specular));
    }
};

#endif
