#ifndef TEXTURE
#define TEXTURE

#include "../Core/ArtCore.h"
#include "../Core/ArtCoreUtils.h"
#include "../../libs/stb_image.h"

enum TextureType {
            ALBEDO = 0,
            NORMAL,
            AMBIENT_OCCULUSION,
            METALLIC,
            REFLECTION
};

class Texture{
    public:
        Texture(const std::string& path, const TextureType type);

        GLuint GetTexture() const { return m_TextureID; }

    private:
        GLuint m_TextureID; 
        TextureType m_Type;
};

#endif