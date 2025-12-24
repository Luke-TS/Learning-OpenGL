#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include "glad/gl.h"

class Texture
{
public:
    unsigned int ID;

    Texture(const char *filename);
    void bind();
};

#endif
