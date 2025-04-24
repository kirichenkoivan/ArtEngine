#include "../include/engine/Graphics/Texture.h"

std::shared_ptr<Texture> Texture::CreateTexture(const std::string &path, const TextureType type)
{
    if (path == "")
    {
        Logger::GetInstance().Error("Graphics/Texture", "Texture Creation Is Failed - Path Is Null!");
        assert(false && "Texture Creation Is Failed - Path Is Null!");
    }

    std::shared_ptr<Texture> texture(new Texture());
#ifdef DEBUG
    Logger::GetInstance().Warning("Graphics/Texture", path);
#endif
    texture->FromImage(path, type);
#ifdef DEBUG
    Logger::GetInstance().Warning("Graphics/Texture", "Texture Created Succesully!");
#endif
    return texture;
}

void Texture::FromImage(const std::string &path, const TextureType type)
{
    int w, h, bits;

    stbi_set_flip_vertically_on_load(1);

    auto *pixels = stbi_load(path.c_str(), &w, &h, &bits, STBI_rgb_alpha);
    if (!pixels)
    {
        Logger::GetInstance().Error("Texture", "Failed to load texture from path: " + path);
        return;
    }

    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA8,
        w,
        h,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        pixels);

    stbi_image_free(pixels);
}
