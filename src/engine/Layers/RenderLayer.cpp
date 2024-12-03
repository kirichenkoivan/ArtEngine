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
    glBufferData(GL_UNIFORM_BUFFER, 128 * sizeof(ObjectTransform), nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, color));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, texCoords));

    glEnableVertexAttribArray(3);
    glVertexAttribIPointer(3, 1, GL_INT, sizeof(Vertex), (const void*)offsetof(Vertex, texID));

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

    if (m_IsMeshesArrChanged){
        PackIntoBatches(m_MeshesArr);
    } else {
        UpdateBatches();
    }

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto batch : m_Batches){

        glBindBuffer(GL_ARRAY_BUFFER, m_QuadVB);
        glBufferSubData(GL_ARRAY_BUFFER, 0, batch->vertices.size() * sizeof(Vertex), batch->vertices.data());

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_QuadIB);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, batch->indices.size() * sizeof(uint32_t), batch->indices.data());

        glBindBuffer(GL_UNIFORM_BUFFER, m_QuadUBO);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, batch->modelTransforms.size() * sizeof(ObjectTransform), batch->modelTransforms.data());

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

    if (ImGui::BeginTabBar("Debug Tabs")) {
        if (ImGui::BeginTabItem("Batches")) {
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
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Objects")) {
            static float squarePosX = 0.0f;
            static float trianglePosX = 1.0f;
            static float squarePosY = 0.0f;
            static float trianglePosY = 0.0f;

            if (ImGui::Button("Add Square")) {
                auto obj = CreateMeshFromJson("/FileSystem/Assets/Library/Meshes/Square.json");
                obj->SetPosition(squarePosX, squarePosY, 0.0f);
                m_MeshesArr.push_back(obj);
                m_IsMeshesArrChanged = true;
                squarePosY += 1.0f;
            }
            if (ImGui::Button("Add Triangle")) {
                auto obj = CreateMeshFromJson("/FileSystem/Assets/Library/Meshes/Triangle.json");
                obj->SetPosition(trianglePosX, trianglePosY, 0.0f);
                m_MeshesArr.push_back(obj);
                m_IsMeshesArrChanged = true;
                trianglePosY += 1.0f;
            }
            for (size_t i = 0; i < m_MeshesArr.size(); i++){
                ImGui::Separator();
                ImGui::Text("Name: %s", m_MeshesArr[i]->GetName().c_str());
                float posX = m_MeshesArr[i]->GetPositionX();
                float posY = m_MeshesArr[i]->GetPositionY();

                float sizeX = m_MeshesArr[i]->GetScaleX();
                float sizeY = m_MeshesArr[i]->GetScaleY();

                float rotationX = m_MeshesArr[i]->GetRotationX();
                float rotationY = m_MeshesArr[i]->GetRotationY();
                float rotationZ = m_MeshesArr[i]->GetRotationZ();

                float uvScaleX = m_MeshesArr[i]->GetUvScaleX();
                float uvScaleY = m_MeshesArr[i]->GetUvScaleY();

                float uvPosX = m_MeshesArr[i]->GetUvPositionX();
                float uvPosY = m_MeshesArr[i]->GetUvPositionY();

                float uvRotation = m_MeshesArr[i]->GetUvRotation();

                ImGui::SliderFloat(("Object pos X##" + std::to_string(i)).c_str(), &posX, -10.0f, 10.0f);
                ImGui::SliderFloat(("Object pos Y##" + std::to_string(i)).c_str(), &posY, -10.0f, 10.0f);

                ImGui::SliderFloat(("Object scale X##" + std::to_string(i)).c_str(), &sizeX, -5.0f, 5.0f);
                ImGui::SliderFloat(("Object scale Y##" + std::to_string(i)).c_str(), &sizeY, -5.0f, 5.0f);

                ImGui::SliderFloat(("Object rotation X##" + std::to_string(i)).c_str(), &rotationX, 0.0f, 180.0f);
                ImGui::SliderFloat(("Object rotation Y##" + std::to_string(i)).c_str(), &rotationY, 0.0f, 180.0f);
                ImGui::SliderFloat(("Object rotation Z##" + std::to_string(i)).c_str(), &rotationZ, 0.0f, 180.0f);

                ImGui::SliderFloat(("UV scale X##" + std::to_string(i)).c_str(), &uvScaleX, 0.0f, 10.0f);
                ImGui::SliderFloat(("UV scale Y##" + std::to_string(i)).c_str(), &uvScaleY, 0.0f, 10.0f);

                ImGui::SliderFloat(("UV pos X##" + std::to_string(i)).c_str(), &uvPosX, -5.0f, 10.0f);
                ImGui::SliderFloat(("UV pos Y##" + std::to_string(i)).c_str(), &uvPosY, -5.0f, 10.0f);

                ImGui::SliderFloat(("UV rotation ##" + std::to_string(i)).c_str(), &uvRotation, 0.0f, 360.0f);

                if (m_MeshesArr[i]->GetPositionX() != posX){
                    m_MeshesArr[i]->SetPositionX(posX);
                }
                if (m_MeshesArr[i]->GetPositionY() != posY){
                    m_MeshesArr[i]->SetPositionY(posY);
                }
                if (m_MeshesArr[i]->GetScaleX() != sizeX){
                    m_MeshesArr[i]->SetScaleX(sizeX);
                }
                if (m_MeshesArr[i]->GetScaleY() != sizeY){
                    m_MeshesArr[i]->SetScaleY(sizeY);
                }
                if (m_MeshesArr[i]->GetRotationX() != rotationX){
                    m_MeshesArr[i]->SetRotationX(rotationX);
                }
                if (m_MeshesArr[i]->GetRotationY() != rotationY){
                    m_MeshesArr[i]->SetRotationY(rotationY);
                }
                if (m_MeshesArr[i]->GetRotationZ() != rotationZ){
                    m_MeshesArr[i]->SetRotationZ(rotationZ);
                }
                if(m_MeshesArr[i]->GetUvScaleX() != uvScaleX){
                    m_MeshesArr[i]->SetUvScaleX(uvScaleX);
                }
                if(m_MeshesArr[i]->GetUvScaleY() != uvScaleY){
                    m_MeshesArr[i]->SetUvScaleY(uvScaleY);
                }
                if(m_MeshesArr[i]->GetUvPositionX() != uvPosX){
                    m_MeshesArr[i]->SetUvPositionX(uvPosX);
                }
                if(m_MeshesArr[i]->GetUvPositionY() != uvPosY){
                    m_MeshesArr[i]->SetUvPositionY(uvPosY);
                }
                if(m_MeshesArr[i]->GetUvRotation() != uvRotation){
                    m_MeshesArr[i]->SetUvRotation(uvRotation);
                }
            }
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
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
    batch->meshes = meshes;
    size_t objID = 0;
    GLuint shader = meshes[0]->GetMaterial()->GetShader()->GetRendererID();
    std::vector<std::shared_ptr<Texture>> textures;

    for (const auto& mesh : meshes) {
        
        ObjectTransform transform;
        transform.modelMatrix = mesh->GetMeshMatrix();
        transform.modelUvMatrix = mesh->GetUvMeshMatrix();
        batch->modelTransforms.push_back(transform);

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

        if (mesh->GetMaterial()->isUsingTexture()) {
            auto it = std::find(textures.begin(), textures.end(), mesh->GetMaterial()->GetTextures()[0]);
            if (it == textures.end()){
                textures.push_back(mesh->GetMaterial()->GetTextures()[0]);
            }
        }

        ++objID;
    }

    if (!textures.empty()){
        glUseProgram(shader);

        auto loc = glGetUniformLocation(shader, "u_Textures");
        if (loc == -1) {
            Logger::GetInstance().Error(CATEGORY, "Uniform 'u_Textures' not found in shader.");
        }

        for (int i = 0; i < textures.size(); i++) {
            batch->samplers.push_back(i);
        }

        glUniform1iv(loc, static_cast<GLsizei>(batch->samplers.size()), batch->samplers.data());

        for (int i = 0; i < textures.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + i);   
            glBindTexture(GL_TEXTURE_2D, textures[i]->GetTexture());
            
            if (textures[i]->IsTiled()) {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            }
        }
    }
    
    return batch;
}


void RenderLayer::PackIntoBatches(std::vector<std::shared_ptr<Mesh>> meshes)
{
    #ifdef DEBUG
        Logger::GetInstance().Info(CATEGORY, "Recalculating Batches");
    #endif

    m_Batches.clear();
    std::unordered_map<std::shared_ptr<Shader>, std::vector<std::shared_ptr<Mesh>>> meshesByShaders;

    for (const auto& mesh : meshes){
        meshesByShaders[mesh->GetMaterial()->GetShader()].push_back(mesh);
    } 

    for (const auto& sortedMehes : meshesByShaders){
        m_Batches.push_back(MakeBatch(sortedMehes.second));
    }

    m_IsMeshesArrChanged = false;
}

void RenderLayer::UpdateBatches()
{
    for (int i = 0; i < m_Batches.size(); ++i) {
        if (m_Batches[i]->IsBatchChanged()) {
            std::shared_ptr<Batch> newBatch = MakeBatch(m_Batches[i]->meshes);
            m_Batches.push_back(newBatch);
            m_Batches.erase(m_Batches.begin() + i);
            --i;

            #ifdef DEBUG
                Logger::GetInstance().Info(CATEGORY, "Batch is Updating!!!");
            #endif
        }
    }
}

bool Batch::IsBatchChanged()
{
    for (const auto& mesh : meshes){
        if (mesh->GetIsMeshStateChanged()){
            return true;
        }
    }
    return false;
}
