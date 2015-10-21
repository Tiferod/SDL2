#ifndef DEF_SCENEOPENGL
#define DEF_SCENEOPENGL

#ifdef WIN32
#include <GL/glew.h>
#else
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SDL2/SDL.h>
#include <iostream>
#include <string>

#include "Cube.h"
#include "Input.h"
#include "Camera.h"

using namespace glm;
class Chunk;

class SceneOpenGL
{
    public:

    SceneOpenGL(std::string titreFenetre, int largeurFenetre, int hauteurFenetre);
    ~SceneOpenGL();
    bool initialiserFenetre();
    bool initGL();
    void bouclePrincipale();
    float Get2DPerlinNoiseValue(float x, float y, float res);
    static int m_height[2000][2000];
    static int m_dHeight[2000][2000];

    private:

    std::string m_titreFenetre;
    int m_largeurFenetre;
    int m_hauteurFenetre;
    SDL_Window* m_fenetre;
    SDL_GLContext m_contexteOpenGL;
    SDL_Event m_evenements;
    Input m_input;
    int m_dAffichage, m_dChunk;
    static Chunk *m_tableauChunk[50][50];
};

#endif
