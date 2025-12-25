#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/common.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"

// materials are defined as a color for each
// phong lighting componenet and a shininess value
class Material {
public:
    glm::vec3 ambient;    // ambient color
    glm::vec3 diffuse;    // diffuse color
    glm::vec3 specular;   // specular color
    float     shininess;  // shininess value
                          // typical range: 32 - 256

    Material(glm::vec3 ambient,
             glm::vec3 diffuse,
             glm::vec3 specular,
             float shininess) : 
        ambient(ambient),
        diffuse(diffuse),
        specular(specular),
        shininess(shininess) {};

    void use(Shader& shader) {
        shader.setVec3("material.ambient", glm::value_ptr(ambient));
        shader.setVec3("material.diffuse", glm::value_ptr(diffuse));
        shader.setVec3("material.specular", glm::value_ptr(specular));
        shader.setFloat("material.shininess", shininess);
    }
};

#endif
