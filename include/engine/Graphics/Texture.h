#ifndef TEXTURE
#define TEXTURE

#include "../Core/ArtCore.h"
#include "../Core/ArtCoreUtils.h"
#include "../../libs/stb_image.h"

enum TextureType
{
    ALBEDO = 0,
    NORMAL,
    AMBIENT_OCCULUSION,
    METALLIC,
    REFLECTION
};

class Texture
{
public:
    static std::shared_ptr<Texture> CreateTexture(const std::string &path, const TextureType type = ALBEDO);
    ~Texture() = default;
    GLuint GetTexture() const { return m_TextureID; }
    TextureType GetTextureType() const { return m_Type; }

    void SetTiling(bool tiled) { m_IsTiled = tiled; }
    bool IsTiled() const { return m_IsTiled; }

private:
    void FromImage(const std::string &path, const TextureType type);

private:
    Texture() = default;
    const std::string CATEGORY = "Graphics/Texture";
    GLuint m_TextureID;
    TextureType m_Type;
    bool m_IsTiled = false;
};

#endif