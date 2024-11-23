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

void RenderLayer::OnAttach()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float lastYPos = 0.0f;
    for (int j = 0; j < 3; ++j){
        float lastPos = 0.0f;
        for (int i = 0; i <= 20; ++i){
            auto obj = CreateMeshFromJson();
            obj->SetPosition(lastPos, lastYPos, 0.0f);
            m_MeshesArr.push_back(obj);
            lastPos += 0.5f;
        }
        lastYPos += 0.75;
    }

    glGenVertexArrays(1, &m_QuadVA);
    glBindVertexArray(m_QuadVA);

    glGenBuffers(1, &m_QuadVB);
    glBindBuffer(GL_ARRAY_BUFFER, m_QuadVB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 1000, nullptr, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &m_QuadIB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_QuadIB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * 5000, nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glGenBuffers(1, &m_QuadUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, m_QuadUBO);
    glBufferData(GL_UNIFORM_BUFFER, 128 * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, color));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, texCoords));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, texID));

    glEnableVertexAttribArray(4);
    glVertexAttribIPointer(4, 1, GL_INT, sizeof(Vertex), (const void*)offsetof(Vertex, objID));

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
    m_DrawCallsCounter = 0;
    PackIntoBatches(m_MeshesArr);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto batch : m_Batches){

        glBindBuffer(GL_ARRAY_BUFFER, m_QuadVB);
        glBufferSubData(GL_ARRAY_BUFFER, 0, batch->vertices.size() * sizeof(Vertex), batch->vertices.data());

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_QuadIB);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, batch->indices.size() * sizeof(uint32_t), batch->indices.data());

        glBindBuffer(GL_UNIFORM_BUFFER, m_QuadUBO);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, batch->modelMatrices.size() * sizeof(glm::mat4), batch->modelMatrices.data());

        GLuint shaderProgramID = batch->shader->GetRendererID();
        GLuint blockIndex = glGetUniformBlockIndex(shaderProgramID, "ModelMatrices");
        if (blockIndex != GL_INVALID_INDEX) {
            glUniformBlockBinding(shaderProgramID, blockIndex, 0);
            glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_QuadUBO);
        } else {
            Logger::GetInstance().Error(CATEGORY, "Uniform block 'ModelMatrices' not found in shader.");
        }

        glUseProgram(shaderProgramID);

        auto vp = m_CameraController.GetCamera().GetViewProjectionMatrix();

        SetUniformMat4(shaderProgramID, "u_ViewProjection", vp);

        glBindVertexArray(m_QuadVA);
        glDrawElements(GL_TRIANGLES, batch->indices.size(), GL_UNSIGNED_INT, nullptr);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        m_DrawCallsCounter++;
    }
}

void RenderLayer::OnImGuiRender()
{
    ImGui::Begin("Render Debug Menu");

    ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    ImGui::Text("Batches: %lu", m_Batches.size());
    ImGui::Text("Draw Calls: %d", m_DrawCallsCounter);

    for (size_t i = 0; i < m_Batches.size(); ++i) {
        auto& batch = m_Batches[i];
        ImGui::Separator();
        ImGui::Text("Batch %zu", i);
        ImGui::Text("Shader: %s", batch->shader ? batch->shader->GetName().c_str() : "None");
        ImGui::Text("Vertices: %lu", batch->vertices.size());
        ImGui::Text("Indices: %lu", batch->indices.size());

        if (ImGui::TreeNode(("Indices##" + std::to_string(i)).c_str())) {
            for (size_t j = 0; j < batch->indices.size(); ++j) {
                ImGui::Text("%lu: %u", j, batch->indices[j]);
            }
            ImGui::TreePop();
        }
    }
    ImGui::End();
}

void RenderLayer::SetUniformMat4(uint32_t shader, const char* name, const glm::mat4& matrix) {
    int location = glGetUniformLocation(shader, name);
    if (location == -1) {
        Logger::GetInstance().Error("Renderer/", "Uniform location not found: " + std::string(name));
        return;
    }
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void RenderLayer::SetUniform4fv(uint32_t shader, const char* name, const glm::vec4& vect){
    int location = glGetUniformLocation(shader, name);
    if (location == -1) {
        Logger::GetInstance().Error("Renderer", "Uniform location not found: " + std::string(name));
        return;
    }
    glUniform4fv(location, 1, glm::value_ptr(vect));
}

std::shared_ptr<Batch> RenderLayer::MakeBatch(std::vector<std::shared_ptr<Mesh>> meshes) {
    std::shared_ptr<Batch> batch(new Batch());
    size_t objID = 0;

    for (const auto& mesh : meshes) {
        batch->modelMatrices.push_back(mesh->GetMeshMatrix());

        size_t vertexOffset = batch->vertices.size();

        for (const auto& vertex : mesh->GetVertices()) {
            Vertex batchedVertex = vertex;
            batchedVertex.objID = objID;
            batch->vertices.push_back(batchedVertex);
        }

        for (const auto& index : mesh->GetIndices()) {
            batch->indices.push_back(index + static_cast<uint32_t>(vertexOffset));
        }

        if (batch->shader == nullptr) {
            batch->shader = mesh->GetMaterial()->GetShader();
        } else if (batch->shader != mesh->GetMaterial()->GetShader()) {
            Logger::GetInstance().Error(CATEGORY, "Meshes with different shaders are in the same batch. This is not supported.");
        }

        ++objID;
    }

    return batch;
}

void RenderLayer::PackIntoBatches(std::vector<std::shared_ptr<Mesh>> meshes)
{
    m_Batches.clear();
    std::unordered_map<std::shared_ptr<Shader>, std::vector<std::shared_ptr<Mesh>>> meshesByShaders;

    for (const auto& mesh : meshes){
        meshesByShaders[mesh->GetMaterial()->GetShader()].push_back(mesh);
    } 

    for (const auto& sortedMehes : meshesByShaders){
        m_Batches.push_back(MakeBatch(sortedMehes.second));
    }
}