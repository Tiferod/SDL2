#include "Texture.h"

Texture::Texture() : m_id(0), m_fichierImage("")
{

}

Texture::Texture(std::string fichierImage) : m_id(0), m_fichierImage(fichierImage)
{

}

Texture::~Texture()
{

}

bool Texture::charger()
{
    SDL_Surface *imageSDL = IMG_Load(m_fichierImage.c_str());
    if(imageSDL == 0)
    {
        std::cout << "Erreur : " << SDL_GetError() << std::endl;
        return false;
    }
    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);
    GLenum formatInterne(0);
    GLenum format(0);
    if(imageSDL->format->BytesPerPixel == 3)
    {
        formatInterne = GL_RGB;
        if(imageSDL->format->Rmask == 0xff)
        {
            format = GL_RGB;
        }
        else
        {
            format = GL_BGR;
        }
    }
    else if(imageSDL->format->BytesPerPixel == 4)
    {
        formatInterne = GL_RGBA;
        if(imageSDL->format->Rmask == 0xff)
        {
            format = GL_RGBA;
        }
        else
        {
            format = GL_BGRA;
        }
    }
    else
    {
        std::cout << "Erreur, format interne de l'image inconnu" << std::endl;
        SDL_FreeSurface(imageSDL);
        return false;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, formatInterne, imageSDL->w, imageSDL->h, 0, format, GL_UNSIGNED_BYTE, imageSDL->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    SDL_FreeSurface(imageSDL);
    return true;
}

GLuint Texture::getID() const
{
    return m_id;
}

void Texture::setFichierImage(const std::string &fichierImage)
{
    m_fichierImage = fichierImage;
}
