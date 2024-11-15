/* 
*****************************************************************
*                        --- Desc ---                           * 
*                                                               *
*   A layer for rendering the entire graphical part of the game *
*                                                               *
*****************************************************************
*/

#ifndef RENDER_LAYER
#define RENDER_LAYER

#include "./engine/Core/ArtCore.h"
#include "./engine/Core/ArtCoreUtils.h"
#include"./engine/Graphics/ArtGraphics.h"

class RenderLayer : public Layer{
    public:
        RenderLayer();
        virtual ~RenderLayer() = default;

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnUpdate(Timestep ts) override;
        virtual void OnImGuiRender() override;

    private:
        static void SetUniformMat4(uint32_t shader, const char* name, const glm::mat4& matrix);
        static void SetUniform4fv(uint32_t shader, const char* name, const glm::vec4& vect);
        
    private:
        const std::string CATEGORY = "Layers/RenderLayer";

        OrthographicCameraController m_CameraController;
        std::shared_ptr<Shader> m_Shader;
        GLuint m_QuadVA, m_QuadVB, m_QuadIB;
        glm::vec4 m_SquareBaseColor = { 0.8f, 0.2f, 0.3f, 1.0f };
        glm::vec4 m_SquareAlternativeColor = { 0.2f, 0.3f, 0.8f, 1.0f };
        glm::vec4 m_SquareColor = m_SquareBaseColor;

        GLuint m_TextureID;
        const size_t MAX_QUAD_COUNT = 1000;
        const size_t MAX_VERTEX_COUNT = MAX_QUAD_COUNT * 4;
        const size_t MAX_INDEX_COUNT = MAX_QUAD_COUNT * 6; 

        std::shared_ptr<Mesh> m_Mesh;
        std::shared_ptr<Material> m_Material;
        std::shared_ptr<Texture> m_Texture;
        
};

#endif