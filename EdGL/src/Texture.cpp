#include "Texture.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include <stb/stb_image.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace edgl {

std::unordered_map<std::string, Ref<Texture>> Texture::m_Cache = std::unordered_map<std::string, Ref<Texture>>{};

//Texture::Texture(const Texture& obj) : 
//    m_Id(obj.m_Id),
//    m_Width(obj.m_Width),
//    m_Height(obj.m_Height),
//    m_NrChannels(obj.m_NrChannels),
//    m_Type(obj.m_Type)
//{
//    m_Texture = new unsigned char[obj.m_Height * obj.m_Width * obj.m_NrChannels];
//    memcpy(m_Texture, obj.m_Texture, obj.m_Height * obj.m_Width * obj.m_NrChannels);
//}

Texture::Texture(TextureType targetType, const std::string& textureName) :
    m_Type(targetType), m_CacheName(textureName)
{
    // Generating texture
    glGenTextures(1, &m_Id);
    glBindTexture(m_Type, m_Id);
    //glTexParameteri(m_Type, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(m_Type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(m_Type, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(m_Type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(m_Type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(m_Type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenerateMipmap(m_Type);

    glBindTexture(m_Type, 0);
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_Id);
}

void Texture::Bind(int unit) const
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(m_Type, m_Id);
}

void Texture::Unbind() const
{
    glBindTextureUnit(0, 0);
}


Ref<Texture> Texture2D::FlatColor(const glm::vec3& color, int width, int height, const std::string& textureName)
{
    if (m_Cache.find(textureName) != m_Cache.end())
        return m_Cache[textureName];
    auto texture = Ref<Texture2D>(new Texture2D(color, width, height, textureName));
    m_Cache[textureName] = texture;
    return texture;
}

Ref<Texture> Texture2D::FlatColor(const glm::vec4& color, int width, int height, const std::string& textureName)
{
    if (m_Cache.find(textureName) != m_Cache.end())
        return m_Cache[textureName];
    auto texture = Ref<Texture2D>(new Texture2D(color, width, height, textureName));
    m_Cache[textureName] = texture;
    return texture;
}

Ref<Texture> Texture2D::FromFile(const std::string& texturePath, const std::string& textureName)
{
    if (m_Cache.find(textureName) != m_Cache.end())
        return m_Cache[textureName];
    auto texture = Ref<Texture2D>(new Texture2D(texturePath, textureName));
    m_Cache[textureName] = texture;
    return texture;
}

Ref<Texture> Texture2D::FromFile(const std::string& texturePath)
{
    return FromFile(texturePath, texturePath);
}

Texture2D::Texture2D(const std::string& texturePath, const std::string& textureName) : Texture(TEXTURE_2D, textureName)
{
    stbi_set_flip_vertically_on_load(true);
    m_Data = Scope<uint8_t>(stbi_load(texturePath.c_str(), &m_Width, &m_Height, &m_NrChannels, 4));
    if (m_Data == nullptr) throw FailedToLoadTextureException(texturePath);

    int openGLFormat = m_NrChannels == 3 ? GL_RGB : GL_RGBA;
    glBindTexture(m_Type, m_Id);
    glTexImage2D(m_Type, 0, openGLFormat, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_Data.get());
    glBindTexture(m_Type, 0);
}

Texture2D::Texture2D(const glm::vec3& color, int width, int height, const std::string& textureName) :
    Texture(TEXTURE_2D, textureName), m_Width(width), m_Height(height), m_NrChannels(3)
{
    long dataSize = static_cast<long>(width * height * m_NrChannels);
    m_Data = Scope<uint8_t>(new uint8_t[dataSize]);
    if (m_Data == nullptr) throw FailedToCreateTexture();

    auto convertedColor = glm::clamp(color * 255.f, 0.f, 255.f);
    for (uint8_t* it = m_Data.get(); it < m_Data.get() + dataSize; it += m_NrChannels)
    {
        *it = convertedColor.r;
        *(it + 1) = convertedColor.g;
        *(it + 2) = convertedColor.b;
    }
    glBindTexture(m_Type, m_Id);
    glTexImage2D(m_Type, 0, GL_RGB, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_Data.get());
    glBindTexture(m_Type, 0);
}

Texture2D::Texture2D(const glm::vec4& color, int width, int height, const std::string& textureName) :
    Texture(TEXTURE_2D, textureName), m_Width(width), m_Height(height), m_NrChannels(4)
{
    long dataSize = static_cast<long>(width * height * m_NrChannels);
    m_Data = Scope<uint8_t>(new uint8_t[dataSize]);
    if (m_Data == nullptr) throw FailedToCreateTexture();

    auto convertedColor = glm::clamp(color * 255.f, 0.f, 255.f);
    for (uint8_t* it = m_Data.get(); it < m_Data.get() + dataSize; it += m_NrChannels)
    {
        *it = convertedColor.r;
        *(it + 1) = convertedColor.g;
        *(it + 2) = convertedColor.b;
        *(it + 3) = convertedColor.a;
    }
    glBindTexture(m_Type, m_Id);
    glTexImage2D(m_Type, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_Data.get());
    glBindTexture(m_Type, 0);
}

}