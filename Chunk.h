#ifndef CHUNK_H
#define CHUNK_H

#ifdef WIN32
#include <GL/glew.h>
#else
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Texture.h"
#include <iostream>

class Chunk
{
    public:

    Chunk(int pX, int pY, std::string const vertexShader, std::string const fragmentShader);
    ~Chunk();
    void afficher(glm::mat4 &projection, glm::mat4 &modelview);
    void charger();

    private:

    Shader m_shader;
    float m_vertices[46080];
    float m_coordTexture[30720];
    Texture m_herbe, m_terre;

    GLuint m_vboID;
    int m_tailleVerticesBytes;
    int m_tailleCoordTexturesBytes;
    GLuint m_vaoID;
    int m_nbrCubes;
    bool m_loaded;
};

#endif // CHUNK_H
