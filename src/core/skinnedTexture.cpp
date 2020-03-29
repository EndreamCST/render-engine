#include "skinnedTexture.hpp"

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>

SkinnedTexture::SkinnedTexture(GLenum TextureTarget, const std::string &FileName, TextureEnum TextureType) {
    m_textureTarget = TextureTarget;
    m_fileName = FileName;
    m_textureType = TextureType;
    Load();

}

SkinnedTexture::~SkinnedTexture() {
}

void SkinnedTexture::Load() {
    glGenTextures(1, &m_textureID);
    glBindTexture(m_textureTarget, m_textureID);
    int width, height, nrComponents;
    unsigned char *data = stbi_load(m_fileName.c_str(), &width, &height, &nrComponents, 0);
    if (data) {
        GLenum internalFormat;
        GLenum dataFormat;
        if (nrComponents == 1) {
            internalFormat = dataFormat = GL_RED;
        } else if (nrComponents == 3) {
            //internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;
            dataFormat = GL_RGB;
        } else if (nrComponents == 4) {
            //internalFormat = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
            dataFormat = GL_RGBA;
        }
        glTexImage2D(m_textureTarget, 0, dataFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, dataFormat == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, dataFormat == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
        std::cout << "Loaded texture: " << m_fileName << std::endl;

    } else {
        std::cout << "Failed to load texture: " << m_fileName << std::endl;
    }
}

void SkinnedTexture::Bind(GLenum TextureUnit) {
    glActiveTexture(TextureUnit);
    glBindTexture(m_textureTarget, m_textureID);
}
