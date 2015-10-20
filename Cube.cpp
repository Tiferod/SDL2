#ifndef BUFFER_OFFSET

    #define BUFFER_OFFSET(offset) ((char*)NULL + (offset))

#endif

#include "Cube.h"

using namespace glm;

Cube::Cube(bool lr, bool fb, std::string const vertexShader, std::string const fragmentShader) : m_shader(vertexShader, fragmentShader), m_herbe("TextureHerbe.jpg"), m_terre("TextureTerre.jpg"), m_vboID(0), m_tailleVerticesBytes(54 * sizeof(float)), m_tailleCoordTexturesBytes(36 * sizeof(float)), m_vaoID(0)
{
    m_herbe.charger();
    m_terre.charger();

    m_shader.charger();

    float verticesTmp1[] = {1, 0, 1, 1, 0, 0, 1, 1, 0,//DROITE
                           1, 0, 1, 1, 1, 1, 1, 1, 0,//DROITE
                           0, 0, 0, 1, 0, 0, 1, 1, 0,//DEVANT
                           0, 0, 0, 0, 1, 0, 1, 1, 0,//DEVANT
                           0, 1, 1, 1, 1, 1, 1, 1, 0,//HAUT
                           0, 1, 1, 0, 1, 0, 1, 1, 0};//HAUT

    float coordTexture1[] = {0, 1, 0, 0, 1, 0,//DROITE
                            0, 1, 1, 1, 1, 0,//DROITE
                            0, 0, 1, 0, 1, 1,//DEVANT
                            0, 0, 0, 1, 1, 1,//DEVANT
                            0, 1, 1, 1, 1, 0,//HAUT
                            0, 1, 0, 0, 1, 0};//HAUT*/

    if (lr)
    {
        if (fb)
        {
            float verticesTmp[] = {1, 0, 1, 1, 0, 0, 1, 1, 0,//DROITE
                                   1, 0, 1, 1, 1, 1, 1, 1, 0,//DROITE
                                   0, 0, 1, 1, 0, 1, 1, 1, 1,//DERRIERE
                                   0, 0, 1, 0, 1, 1, 1, 1, 1,//DERRIERE
                                   0, 1, 1, 1, 1, 1, 1, 1, 0,//HAUT
                                   0, 1, 1, 0, 1, 0, 1, 1, 0};//HAUT

            float coordTexture[] = {0, 1, 0, 0, 1, 0,//DROITE
                                    0, 1, 1, 1, 1, 0,//DROITE
                                    0, 0, 1, 0, 1, 1,//DERRIERE
                                    0, 0, 0, 1, 1, 1,//DERRIERE
                                    0, 1, 1, 1, 1, 0,//HAUT
                                    0, 1, 0, 0, 1, 0};//HAUT

            copieTableau(verticesTmp, verticesTmp1, 54);
            copieTableau(coordTexture, coordTexture1, 36);
        }
        else
        {
            float verticesTmp[] = {1, 0, 1, 1, 0, 0, 1, 1, 0,//DROITE
                               1, 0, 1, 1, 1, 1, 1, 1, 0,//DROITE
                               0, 0, 0, 1, 0, 0, 1, 1, 0,//DEVANT
                               0, 0, 0, 0, 1, 0, 1, 1, 0,//DEVANT
                               0, 1, 1, 1, 1, 1, 1, 1, 0,//HAUT
                               0, 1, 1, 0, 1, 0, 1, 1, 0};//HAUT

            float coordTexture[] = {0, 1, 0, 0, 1, 0,//DROITE
                                    0, 1, 1, 1, 1, 0,//DROITE
                                    0, 0, 1, 0, 1, 1,//DEVANT
                                    0, 0, 0, 1, 1, 1,//DEVANT
                                    0, 1, 1, 1, 1, 0,//HAUT
                                    0, 1, 0, 0, 1, 0};//HAUT

            copieTableau(verticesTmp, verticesTmp1, 54);
            copieTableau(coordTexture, coordTexture1, 36);
        }
    }
    else
    {
        if (fb)
        {
            float verticesTmp[] = {0, 0, 0, 0, 0, 1, 0, 1, 1,//GAUCHE
                                   0, 0, 0, 0, 1, 0, 0, 1, 1,//GAUCHE
                                   0, 0, 1, 1, 0, 1, 1, 1, 1,//DERRIERE
                                   0, 0, 1, 0, 1, 1, 1, 1, 1,//DERRIERE
                                   0, 1, 1, 1, 1, 1, 1, 1, 0,//HAUT
                                   0, 1, 1, 0, 1, 0, 1, 1, 0};//HAUT

            float coordTexture[] = {0, 0, 0, 1, 1, 1,//GAUCHE
                                    0, 0, 1, 0, 1, 1,//GAUCHE
                                    0, 0, 1, 0, 1, 1,//DERRIERE
                                    0, 0, 0, 1, 1, 1,//DERRIERE
                                    0, 1, 1, 1, 1, 0,//HAUT
                                    0, 1, 0, 0, 1, 0};//HAUT

            copieTableau(verticesTmp, verticesTmp1, 54);
            copieTableau(coordTexture, coordTexture1, 36);
        }
        else
        {
            float verticesTmp[] = {0, 0, 0, 1, 0, 0, 1, 1, 0,//DEVANT
                               0, 0, 0, 0, 1, 0, 1, 1, 0,//DEVANT
                               0, 0, 0, 0, 0, 1, 0, 1, 1,//GAUCHE
                               0, 0, 0, 0, 1, 0, 0, 1, 1,//GAUCHE
                               0, 1, 1, 1, 1, 1, 1, 1, 0,//HAUT
                               0, 1, 1, 0, 1, 0, 1, 1, 0};//HAUT

            float coordTexture[] = {0, 0, 1, 0, 1, 1,//DEVANT
                                    0, 0, 0, 1, 1, 1,//DEVANT
                                    0, 0, 0, 1, 1, 1,//GAUCHE
                                    0, 0, 1, 0, 1, 1,//GAUCHE
                                    0, 1, 1, 1, 1, 0,//HAUT
                                    0, 1, 0, 0, 1, 0};//HAUT

            copieTableau(verticesTmp, verticesTmp1, 54);
            copieTableau(coordTexture, coordTexture1, 36);
        }
    }

    /*float verticesTmp[] = {1, 0, 1, 1, 0, 0, 1, 1, 0,//DROITE
                           1, 0, 1, 1, 1, 1, 1, 1, 0,//DROITE
                           0, 0, 0, 1, 0, 0, 1, 1, 0,//DEVANT
                           0, 0, 0, 0, 1, 0, 1, 1, 0,//DEVANT
                           0, 0, 0, 0, 0, 1, 0, 1, 1,//GAUCHE
                           0, 0, 0, 0, 1, 0, 0, 1, 1,//GAUCHE
                           0, 0, 1, 1, 0, 1, 1, 1, 1,//DERRIERE
                           0, 0, 1, 0, 1, 1, 1, 1, 1,//DERRIERE
                           0, 1, 1, 1, 1, 1, 1, 1, 0,//HAUT
                           0, 1, 1, 0, 1, 0, 1, 1, 0};//HAUT

    float coordTexture[] = {0, 1, 0, 0, 1, 0,//DROITE
                            0, 1, 1, 1, 1, 0,//DROITE
                            0, 0, 1, 0, 1, 1,//DEVANT
                            0, 0, 0, 1, 1, 1,//DEVANT
                            0, 0, 0, 1, 1, 1,//GAUCHE
                            0, 0, 1, 0, 1, 1,//GAUCHE
                            0, 0, 1, 0, 1, 1,//DERRIERE
                            0, 0, 0, 1, 1, 1,//DERRIERE
                            0, 1, 1, 1, 1, 0,//HAUT
                            0, 1, 0, 0, 1, 0};//HAUT*/

    for(int i = 0; i < 54; i++)
    {
        m_vertices[i] = verticesTmp1[i];
        if (i < 36)
            m_coordTexture[i] = coordTexture1[i];
    }
}

Cube::~Cube()
{
    glDeleteBuffers(1, &m_vboID);
    glDeleteVertexArrays(1, &m_vaoID);
}

void Cube::afficher(float x, float y, float z, glm::mat4 &projection, glm::mat4 &modelview)
{
    glUseProgram(m_shader.getProgramID());
        mat4 sauvegardeModelview = modelview;
        modelview = translate(modelview, vec3(x, y, z));
        glBindVertexArray(m_vaoID);
            glUniformMatrix4fv(glGetUniformLocation(m_shader.getProgramID(), "projection"), 1, GL_FALSE, value_ptr(projection));
            glUniformMatrix4fv(glGetUniformLocation(m_shader.getProgramID(), "modelview"), 1, GL_FALSE, value_ptr(modelview));
            glBindTexture(GL_TEXTURE_2D, m_terre.getID());
            glDrawArrays(GL_TRIANGLES, 0, 12);
            glBindTexture(GL_TEXTURE_2D, m_herbe.getID());
            glDrawArrays(GL_TRIANGLES, 12, 18);
            glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
        modelview = sauvegardeModelview;
    glUseProgram(0);
}

void Cube::charger()
{
    if(glIsBuffer(m_vboID) == GL_TRUE)
        glDeleteBuffers(1, &m_vboID);
    glGenBuffers(1, &m_vboID);
    glBindBuffer(GL_ARRAY_BUFFER, m_vboID);

        glBufferData(GL_ARRAY_BUFFER, m_tailleVerticesBytes + m_tailleCoordTexturesBytes, 0, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, m_tailleVerticesBytes, m_vertices);
        glBufferSubData(GL_ARRAY_BUFFER, m_tailleVerticesBytes, m_tailleCoordTexturesBytes, m_coordTexture);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    if(glIsVertexArray(m_vaoID) == GL_TRUE)
        glDeleteVertexArrays(1, &m_vaoID);
    glGenVertexArrays(1, &m_vaoID);
    glBindVertexArray(m_vaoID);

        glBindBuffer(GL_ARRAY_BUFFER, m_vboID);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(m_tailleVerticesBytes));
            glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void Cube::copieTableau(float tableauOriginal[], float tableauCopie[], int tailleTableau)
{
    for(int i = 0 ; i < tailleTableau ; i++)
    {
        * (tableauCopie + i) = * (tableauOriginal + i) ;
    }
}
