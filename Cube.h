#ifndef CUBE_H
#define CUBE_H

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

class Cube
{
    public:

    Cube(bool lr, bool fb, std::string const vertexShader, std::string const fragmentShader);
    ~Cube();
    void afficher(float x, float y, float z, glm::mat4 &projection, glm::mat4 &modelview);
    void charger();
    void copieTableau(float tableauOriginal[], float tableauCopie[], int tailleTableau);

    private:

    Shader m_shader;
    float m_vertices[54];
    float m_coordTexture[36];
    Texture m_herbe, m_terre;

    GLuint m_vboID;
    int m_tailleVerticesBytes;
    int m_tailleCoordTexturesBytes;
    GLuint m_vaoID;
};

#endif
