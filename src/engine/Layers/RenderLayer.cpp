/****************************************************************
*                        --- Desc ---                           * 
*                                                               *
*   A layer for rendering the entire graphical part of the game *
*                                                               *
*****************************************************************/

#include "../include/engine/Layers/RenderLayer.h"

RenderLayer::RenderLayer() : Layer("RenderLayer"), m_CameraController(16.0f / 9.0f)
{
    Logger::GetInstance().Info(CATEGORY, "RenderLayer Created");
}


static std::vector<Vertex> CreateQuad(float x, float y, GLuint texID){
    
    float size = 1.0f;
    std::vector<Vertex> res;

    Vertex v0;
    v0.position = {x, y, 0.0f};
    v0.texCoords = {0.0f, 0.0f};
    v0.color = {0.18f, 0.6f, 0.96f, 1.0f};
    v0.texID = texID;
    res.push_back(v0);

    Vertex v1;
    v1.position = { x + size, y, 0.0f };
    v1.texCoords = { 1.0f, 0.0f };
    v1.color = {0.18f, 0.6f, 0.96f, 1.0f};
    v1.texID = texID;
    res.push_back(v1);

    Vertex v2;
    v2.position = { x + size, y + size, 0.0f };
    v2.texCoords = { 1.0f, 1.0f };
    v2.color = {0.18f, 0.6f, 0.96f, 1.0f};
    v2.texID = texID;
    res.push_back(v2);

    Vertex v3;
    v3.position = { x, y + size, 0.0f };
    v3.texCoords = { 0.0f, 1.0f };
    v3.color = {0.18f, 0.6f, 0.96f, 1.0f};
    v3.texID = texID;
    res.push_back(v3);

    return res;
}

void RenderLayer::OnAttach()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_Shader = Shader::FromGLSLTextFile("/FileSystem/Shaders/vertex.glsl", "/FileSystem/Shaders/fragment.glsl");
    glUseProgram(m_Shader->GetRendererID());
    auto loc = glGetUniformLocation(m_Shader->GetRendererID(), "u_Textures");
    int samplers[2] = {0 , 1};
    glUniform1iv(loc, 2, samplers);

    glGenVertexArrays(1, &m_QuadVA);
    glBindVertexArray(m_QuadVA);

    glGenBuffers(1, &m_QuadVB);
    glBindBuffer(GL_ARRAY_BUFFER, m_QuadVB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 1000, nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, color));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, texCoords));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, texID));

    uint32_t indices[] = { 
        0, 1, 2, 0, 2, 3
     };
    glGenBuffers(1, &m_QuadIB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_QuadIB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    m_SquareColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

    m_TextureID = LoadTexture("FileSystem/Textures/image.png");

    Logger::GetInstance().Info(CATEGORY, "RenderLayer Attached");
}

void RenderLayer::OnDetach()
{
    glDeleteVertexArrays(1, &m_QuadVA);
    glDeleteBuffers(1, &m_QuadVB);
    glDeleteBuffers(1, &m_QuadIB);

    Logger::GetInstance().Info(CATEGORY, "RenderLayer Detached");
}

void RenderLayer::OnUpdate(Timestep ts)
{
    m_CameraController.OnUpdate(ts);

    auto q0 = CreateQuad(-1.5f, -0.5f, 0);

    Vertex verts[4];
    memcpy(verts, q0.data(), q0.size() * sizeof(Vertex));

    glBindBuffer(GL_ARRAY_BUFFER, m_QuadVB);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), verts);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(m_Shader->GetRendererID());
    glBindTexture(0, m_TextureID);

    auto vp = m_CameraController.GetCamera().GetViewProjectionMatrix();

    SetUniformMat4(m_Shader->GetRendererID(), "u_ViewProjection", vp);
    SetUniformMat4(m_Shader->GetRendererID(), "u_Model", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));

    glBindVertexArray(m_QuadVA);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void RenderLayer::OnImGuiRender()
{
    ImGui::Begin("Render Debug Menu");
    if(ImGui::ColorEdit4("Square Base Color", glm::value_ptr(m_SquareBaseColor))){
        m_SquareColor = m_SquareBaseColor;
    }
    ImGui::ColorEdit4("Square Alternative Color", glm::value_ptr(m_SquareAlternativeColor));

    ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    ImGui::End();
}

void RenderLayer::SetUniformMat4(uint32_t shader, const char* name, const glm::mat4& matrix) {
    int location = glGetUniformLocation(shader, name);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void RenderLayer::SetUniform4fv(uint32_t shader, const char* name, const glm::vec4& vect){
    int location = glGetUniformLocation(shader, name);
    glUniform4fv(location, 1, glm::value_ptr(vect));
}