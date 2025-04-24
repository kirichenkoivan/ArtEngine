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

    glGenBuffers(1, &m_LightsUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, m_LightsUBO);
    glBufferData(GL_UNIFORM_BUFFER, 128 * sizeof(LightInfo), nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glGenBuffers(1, &m_MaterialsUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, m_MaterialsUBO);
    glBufferData(GL_UNIFORM_BUFFER, 128 * sizeof(MaterialInfo), nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *)offsetof(Vertex, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *)offsetof(Vertex, color));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *)offsetof(Vertex, texCoords));

    glEnableVertexAttribArray(3);
    glVertexAttribIPointer(3, 1, GL_INT, sizeof(Vertex), (const void *)offsetof(Vertex, texID));

    glEnableVertexAttribArray(4);
    glVertexAttribIPointer(4, 1, GL_INT, sizeof(Vertex), (const void *)offsetof(Vertex, objID));

    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *)offsetof(Vertex, normal));

    Logger::GetInstance().Info(CATEGORY, "RenderLayer Attached");
}

void RenderLayer::OnDetach()
{
    glDeleteVertexArrays(1, &m_QuadVA);
    glDeleteBuffers(1, &m_QuadVB);
    glDeleteBuffers(1, &m_QuadIB);
    glDeleteBuffers(1, &m_QuadUBO);
    glDeleteBuffers(1, &m_LightsUBO);
    glDeleteBuffers(1, &m_MaterialsUBO);

    Logger::GetInstance().Info(CATEGORY, "RenderLayer Detached");
}

void RenderLayer::OnUpdate(Timestep ts)
{
    m_CameraController.OnUpdate(ts);
    m_DrawCallsCounter = 0;

    if (CheckLightsChanged() || m_IsLightsArrChanged)
    {
        PackLightSources(m_LightsArr);
    }
    if (m_IsMeshesArrChanged)
    {
        PackIntoBatches(m_MeshesArr);
    }
    else
    {
        UpdateBatches();
    }

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto batch : m_Batches)
    {

        glBindBuffer(GL_ARRAY_BUFFER, m_QuadVB);
        glBufferSubData(GL_ARRAY_BUFFER, 0, batch->vertices.size() * sizeof(Vertex), batch->vertices.data());

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_QuadIB);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, batch->indices.size() * sizeof(uint32_t), batch->indices.data());

        glBindBuffer(GL_UNIFORM_BUFFER, m_QuadUBO);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, batch->modelTransforms.size() * sizeof(ObjectTransform), batch->modelTransforms.data());

        glBindBuffer(GL_UNIFORM_BUFFER, m_LightsUBO);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, m_Lights.size() * sizeof(LightInfo), m_Lights.data());

        glBindBuffer(GL_UNIFORM_BUFFER, m_MaterialsUBO);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, batch->materialsInfo.size() * sizeof(MaterialInfo), batch->materialsInfo.data());

        GLuint shaderProgramID = batch->shader->GetRendererID();

        GLuint blockIndex = glGetUniformBlockIndex(shaderProgramID, "ModelMatrices");
        if (blockIndex != GL_INVALID_INDEX)
        {
            glUniformBlockBinding(shaderProgramID, blockIndex, 0);
            glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_QuadUBO);
        }
        else
        {
            Logger::GetInstance().Error(CATEGORY, "Uniform block 'ModelMatrices' not found in shader.");
        }

        blockIndex = glGetUniformBlockIndex(shaderProgramID, "Lights");
        if (blockIndex != GL_INVALID_INDEX)
        {
            glUniformBlockBinding(shaderProgramID, blockIndex, 1);
            glBindBufferBase(GL_UNIFORM_BUFFER, 1, m_LightsUBO);
            SetUniformInt(shaderProgramID, "u_LightsCount", m_Lights.size());
        }
        else
        {
            Logger::GetInstance().Error(CATEGORY, "Uniform block 'Lights' not found in shader.");
        }

        blockIndex = glGetUniformBlockIndex(shaderProgramID, "Materials");
        if (blockIndex != GL_INVALID_INDEX)
        {
            glUniformBlockBinding(shaderProgramID, blockIndex, 2);
            glBindBufferBase(GL_UNIFORM_BUFFER, 2, m_MaterialsUBO);
        }
        else
        {
            Logger::GetInstance().Error(CATEGORY, "Uniform block 'Materials' not found in shader.");
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

    if (ImGui::BeginTabBar("Debug Tabs"))
    {
        if (ImGui::BeginTabItem("Batches"))
        {
            for (size_t i = 0; i < m_Batches.size(); ++i)
            {
                auto &batch = m_Batches[i];
                ImGui::Separator();
                ImGui::Text("Batch %zu", i);
                ImGui::Text("Shader: %s", batch->shader ? batch->shader->GetName().c_str() : "None");
                ImGui::Text("Vertices: %lu", batch->vertices.size());
                ImGui::Text("Indices: %lu", batch->indices.size());

                if (ImGui::TreeNode(("Indices##" + std::to_string(i)).c_str()))
                {
                    for (size_t j = 0; j < batch->indices.size(); ++j)
                    {
                        ImGui::Text("%lu: %u", j, batch->indices[j]);
                    }
                    ImGui::TreePop();
                }
            }
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Objects"))
        {
            static float squarePosX = 0.0f;
            static float trianglePosX = 0.0f;
            static float squarePosY = 0.0f;
            static float trianglePosY = 0.0f;

            if (ImGui::Button("Add Square"))
            {
                auto obj = CreateMeshFromJson("/FileSystem/Assets/Library/Meshes/Square.json");
                obj->SetPosition(squarePosX, squarePosY, 0.0f);
                m_MeshesArr.push_back(obj);
                m_IsMeshesArrChanged = true;
                squarePosY += 1.0f;
            }
            if (ImGui::Button("Add Triangle"))
            {
                auto obj = CreateMeshFromJson("/FileSystem/Assets/Library/Meshes/Triangle.json");
                obj->SetPosition(trianglePosX, trianglePosY, 0.0f);
                m_MeshesArr.push_back(obj);
                m_IsMeshesArrChanged = true;
                trianglePosY += 1.0f;
            }
            if (ImGui::Button("Add Cube"))
            {
                auto obj = CreateMeshFromJson("/FileSystem/Assets/Library/Meshes/Cube.json");
                obj->SetPosition(squarePosX, squarePosY, 0.0f);
                m_MeshesArr.push_back(obj);
                m_IsMeshesArrChanged = true;
                squarePosY += 1.0f;
            }

            for (size_t i = 0; i < m_MeshesArr.size(); i++)
            {
                ImGui::Separator();
                ImGui::Text("Name: %s", m_MeshesArr[i]->GetName().c_str());
                float posX = m_MeshesArr[i]->GetPositionX();
                float posY = m_MeshesArr[i]->GetPositionY();
                float posZ = m_MeshesArr[i]->GetPositionZ();

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
                ImGui::SliderFloat(("Object pos Z##" + std::to_string(i)).c_str(), &posZ, -10.0f, 10.0f);

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

                if (m_MeshesArr[i]->GetPositionX() != posX)
                {
                    m_MeshesArr[i]->SetPositionX(posX);
                }
                if (m_MeshesArr[i]->GetPositionY() != posY)
                {
                    m_MeshesArr[i]->SetPositionY(posY);
                }
                if (m_MeshesArr[i]->GetPositionZ() != posZ)
                {
                    m_MeshesArr[i]->SetPositionZ(posZ);
                }
                if (m_MeshesArr[i]->GetScaleX() != sizeX)
                {
                    m_MeshesArr[i]->SetScaleX(sizeX);
                }
                if (m_MeshesArr[i]->GetScaleY() != sizeY)
                {
                    m_MeshesArr[i]->SetScaleY(sizeY);
                }
                if (m_MeshesArr[i]->GetRotationX() != rotationX)
                {
                    m_MeshesArr[i]->SetRotationX(rotationX);
                }
                if (m_MeshesArr[i]->GetRotationY() != rotationY)
                {
                    m_MeshesArr[i]->SetRotationY(rotationY);
                }
                if (m_MeshesArr[i]->GetRotationZ() != rotationZ)
                {
                    m_MeshesArr[i]->SetRotationZ(rotationZ);
                }
                if (m_MeshesArr[i]->GetUvScaleX() != uvScaleX)
                {
                    m_MeshesArr[i]->SetUvScaleX(uvScaleX);
                }
                if (m_MeshesArr[i]->GetUvScaleY() != uvScaleY)
                {
                    m_MeshesArr[i]->SetUvScaleY(uvScaleY);
                }
                if (m_MeshesArr[i]->GetUvPositionX() != uvPosX)
                {
                    m_MeshesArr[i]->SetUvPositionX(uvPosX);
                }
                if (m_MeshesArr[i]->GetUvPositionY() != uvPosY)
                {
                    m_MeshesArr[i]->SetUvPositionY(uvPosY);
                }
                if (m_MeshesArr[i]->GetUvRotation() != uvRotation)
                {
                    m_MeshesArr[i]->SetUvRotation(uvRotation);
                }
            }
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Lights"))
        {
            static glm::vec4 lightColor = {1.0f, 1.0f, 1.0f, 1.0f};
            if (ImGui::Button("Add PointLight"))
            {
                auto obj = PointLight::CreatePointLight(lightColor, {0.0f, 0.0f, 1.0f});
                m_LightsArr.push_back(obj);
                m_IsLightsArrChanged = true;
            }

            if (ImGui::Button("Add DirectLight"))
            {
                auto obj = DirectLight::CreateDirectLight({0.0f, 0.0f, -1.0f}, lightColor);
                m_LightsArr.push_back(obj);
                m_IsLightsArrChanged = true;
            }

            if (ImGui::Button("Add SpotLight"))
            {
                auto obj = SpotLight::CreateSpotLight(lightColor, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, 1.0f, 0.0f);
                m_LightsArr.push_back(obj);
                m_IsLightsArrChanged = true;
            }

            for (size_t i = 0; i < m_LightsArr.size(); i++)
            {
                ImGui::Separator();
                if (m_LightsArr[i]->GetType() == POINT_LIGHT)
                {
                    float posX = m_LightsArr[i]->GetPositionX();
                    float posY = m_LightsArr[i]->GetPositionY();
                    float posZ = m_LightsArr[i]->GetPositionZ();

                    float lightStrenght = m_LightsArr[i]->GetColorA();

                    ImGui::SliderFloat(("Light pos X##" + std::to_string(i)).c_str(), &posX, -10.0f, 10.0f);
                    ImGui::SliderFloat(("Light pos Y##" + std::to_string(i)).c_str(), &posY, -10.0f, 10.0f);
                    ImGui::SliderFloat(("Light pos Z##" + std::to_string(i)).c_str(), &posZ, -10.0f, 10.0f);
                    ImGui::SliderFloat(("Light strenght##" + std::to_string(i)).c_str(), &lightStrenght, -1.0f, 1.0f);

                    if (m_LightsArr[i]->GetPositionX() != posX)
                    {
                        m_LightsArr[i]->SetPositionX(posX);
                    }
                    if (m_LightsArr[i]->GetPositionY() != posY)
                    {
                        m_LightsArr[i]->SetPositionY(posY);
                    }
                    if (m_LightsArr[i]->GetPositionZ() != posZ)
                    {
                        m_LightsArr[i]->SetPositionZ(posZ);
                    }
                    if (m_LightsArr[i]->GetColorA() != lightStrenght)
                    {
                        m_LightsArr[i]->SetColorA(lightStrenght);
                    }
                }
                if (m_LightsArr[i]->GetType() == LightType::DIRECTIONAL_LIGHT)
                {
                    float posX = m_LightsArr[i]->GetPositionX();
                    float posY = m_LightsArr[i]->GetPositionY();
                    float posZ = m_LightsArr[i]->GetPositionZ();

                    float lightStrenght = m_LightsArr[i]->GetColorA();

                    ImGui::SliderFloat(("Light dir X##" + std::to_string(i)).c_str(), &posX, -10.0f, 10.0f);
                    ImGui::SliderFloat(("Light dir Y##" + std::to_string(i)).c_str(), &posY, -10.0f, 10.0f);
                    ImGui::SliderFloat(("Light dir Z##" + std::to_string(i)).c_str(), &posZ, -10.0f, 10.0f);
                    ImGui::SliderFloat(("Light strenght##" + std::to_string(i)).c_str(), &lightStrenght, -1.0f, 1.0f);

                    if (m_LightsArr[i]->GetPositionX() != posX)
                    {
                        m_LightsArr[i]->SetPositionX(posX);
                    }
                    if (m_LightsArr[i]->GetPositionY() != posY)
                    {
                        m_LightsArr[i]->SetPositionY(posY);
                    }
                    if (m_LightsArr[i]->GetPositionZ() != posZ)
                    {
                        m_LightsArr[i]->SetPositionZ(posZ);
                    }
                    if (m_LightsArr[i]->GetColorA() != lightStrenght)
                    {
                        m_LightsArr[i]->SetColorA(lightStrenght);
                    }
                }
                if (m_LightsArr[i]->GetType() == LightType::SPOT_LIGHT)
                {
                    float posX = m_LightsArr[i]->GetPositionX();
                    float posY = m_LightsArr[i]->GetPositionY();
                    float posZ = m_LightsArr[i]->GetPositionZ();

                    float dirX = m_LightsArr[i]->GetDirectionX();
                    float dirY = m_LightsArr[i]->GetDirectionY();
                    float dirZ = m_LightsArr[i]->GetDirectionZ();

                    float cutOff = m_LightsArr[i]->GetCutOff();
                    float exponent = m_LightsArr[i]->GetExponent();
                    float lightStrenght = m_LightsArr[i]->GetColorA();

                    ImGui::SliderFloat(("Light pos X##" + std::to_string(i)).c_str(), &posX, -10.0f, 10.0f);
                    ImGui::SliderFloat(("Light pos Y##" + std::to_string(i)).c_str(), &posY, -10.0f, 10.0f);
                    ImGui::SliderFloat(("Light pos Z##" + std::to_string(i)).c_str(), &posZ, -10.0f, 10.0f);

                    ImGui::SliderFloat(("Light dir X##" + std::to_string(i)).c_str(), &dirX, -10.0f, 10.0f);
                    ImGui::SliderFloat(("Light dir Y##" + std::to_string(i)).c_str(), &dirY, -10.0f, 10.0f);
                    ImGui::SliderFloat(("Light dir Z##" + std::to_string(i)).c_str(), &dirZ, -10.0f, 10.0f);

                    ImGui::SliderFloat(("Light cutoff##" + std::to_string(i)).c_str(), &cutOff, 0.0f, 90.0f);
                    ImGui::SliderFloat(("Light exponent##" + std::to_string(i)).c_str(), &exponent, 0.0f, 5.0f);
                    ImGui::SliderFloat(("Light strenght##" + std::to_string(i)).c_str(), &lightStrenght, -1.0f, 1.0f);

                    if (m_LightsArr[i]->GetPositionX() != posX)
                    {
                        m_LightsArr[i]->SetPositionX(posX);
                    }
                    if (m_LightsArr[i]->GetPositionY() != posY)
                    {
                        m_LightsArr[i]->SetPositionY(posY);
                    }
                    if (m_LightsArr[i]->GetPositionZ() != posZ)
                    {
                        m_LightsArr[i]->SetPositionZ(posZ);
                    }
                    if (m_LightsArr[i]->GetDirectionX() != dirX)
                    {
                        m_LightsArr[i]->SetDirectionX(dirX);
                    }
                    if (m_LightsArr[i]->GetDirectionY() != dirY)
                    {
                        m_LightsArr[i]->SetDirectionY(dirY);
                    }
                    if (m_LightsArr[i]->GetDirectionZ() != dirZ)
                    {
                        m_LightsArr[i]->SetDirectionZ(dirZ);
                    }
                    if (m_LightsArr[i]->GetColorA() != lightStrenght)
                    {
                        m_LightsArr[i]->SetColorA(lightStrenght);
                    }
                    if (m_LightsArr[i]->GetCutOff() != cutOff)
                    {
                        m_LightsArr[i]->SetCutOff(cutOff);
                    }
                    if (m_LightsArr[i]->GetExponent() != exponent)
                    {
                        m_LightsArr[i]->SetExponent(exponent);
                    }
                }
            }
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Camera"))
        {

            float rotationX = m_CameraController.GetCamera().GetRotationX();
            float rotationY = m_CameraController.GetCamera().GetRotationY();
            float rotationZ = m_CameraController.GetCamera().GetRotationZ();

            ImGui::SliderFloat("Rotation X", &rotationX, -180.0f, 180.0f);
            ImGui::SliderFloat("Rotation Y", &rotationY, -180.0f, 180.0f);
            ImGui::SliderFloat("Rotation Z", &rotationZ, -180.0f, 180.0f);

            if (m_CameraController.GetCamera().GetRotationX() != rotationX)
            {
                m_CameraController.GetCamera().SetRotationX(rotationX);
            }
            if (m_CameraController.GetCamera().GetRotationY() != rotationY)
            {
                m_CameraController.GetCamera().SetRotationY(rotationY);
            }
            if (m_CameraController.GetCamera().GetRotationZ() != rotationZ)
            {
                m_CameraController.GetCamera().SetRotationZ(rotationZ);
            }

            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    ImGui::End();
}

void RenderLayer::SetUniformMat4(uint32_t shader, const char *name, const glm::mat4 &matrix)
{
    int location = glGetUniformLocation(shader, name);
    if (location == -1)
    {
        Logger::GetInstance().Error(CATEGORY, "Uniform location not found: " + std::string(name));
        return;
    }
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void RenderLayer::SetUniform4fv(uint32_t shader, const char *name, const glm::vec4 &vect)
{
    int location = glGetUniformLocation(shader, name);
    if (location == -1)
    {
        Logger::GetInstance().Error(CATEGORY, "Uniform location not found: " + std::string(name));
        return;
    }
    glUniform4fv(location, 1, glm::value_ptr(vect));
}

void RenderLayer::SetUniformInt(uint32_t shader, const char *name, const int value)
{
    int location = glGetUniformLocation(shader, name);
    if (location == -1)
    {
        Logger::GetInstance().Error(CATEGORY, "Uniform location not found: " + std::string(name));
        return;
    }
    glUniform1i(location, value);
}

std::shared_ptr<Batch> RenderLayer::MakeBatch(std::vector<std::shared_ptr<Mesh>> meshes)
{
    std::shared_ptr<Batch> batch(new Batch());
    batch->meshes = meshes;
    size_t objID = 0;
    GLuint shader = meshes[0]->GetMaterial()->GetShader()->GetRendererID();
    std::vector<std::shared_ptr<Texture>> textures;

    for (const auto &mesh : meshes)
    {

        ObjectTransform transform;
        transform.modelMatrix = mesh->GetMeshMatrix();
        transform.modelUvMatrix = mesh->GetUvMeshMatrix();
        batch->modelTransforms.push_back(transform);

        size_t vertexOffset = batch->vertices.size();

        for (const auto &vertex : mesh->GetVertices())
        {
            Vertex batchedVertex = vertex;
            batchedVertex.objID = objID;
            batch->vertices.push_back(batchedVertex);
        }

        for (const auto &index : mesh->GetIndices())
        {
            batch->indices.push_back(index + static_cast<uint32_t>(vertexOffset));
        }

        if (batch->shader == nullptr)
        {
            batch->shader = mesh->GetMaterial()->GetShader();
        }
        else if (batch->shader != mesh->GetMaterial()->GetShader())
        {
            Logger::GetInstance().Error(CATEGORY, "Meshes with different shaders are in the same batch. This is not supported.");
        }

        if (mesh->GetMaterial()->isUsingTexture())
        {
            auto it = std::find(textures.begin(), textures.end(), mesh->GetMaterial()->GetTextures()[0]);
            if (it == textures.end())
            {
                textures.push_back(mesh->GetMaterial()->GetTextures()[0]);
            }
        }

        // TODO - need to add check for materials, that is already saved
        batch->materialsInfo.push_back(mesh->GetMaterial()->GetMaterialInfo());

        ++objID;
    }

    if (!textures.empty())
    {
        glUseProgram(shader);

        auto loc = glGetUniformLocation(shader, "u_Textures");
        if (loc == -1)
        {
            Logger::GetInstance().Error(CATEGORY, "Uniform 'u_Textures' not found in shader.");
        }

        for (int i = 0; i < textures.size(); i++)
        {
            batch->samplers.push_back(i);
        }

        glUniform1iv(loc, static_cast<GLsizei>(batch->samplers.size()), batch->samplers.data());

        for (int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, textures[i]->GetTexture());

            if (textures[i]->IsTiled())
            {
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

    for (const auto &mesh : meshes)
    {
        meshesByShaders[mesh->GetMaterial()->GetShader()].push_back(mesh);
    }

    for (const auto &sortedMehes : meshesByShaders)
    {
        m_Batches.push_back(MakeBatch(sortedMehes.second));
    }

    m_IsMeshesArrChanged = false;
}

void RenderLayer::PackLightSources(std::vector<std::shared_ptr<LightSource>> lights)
{
    m_Lights.clear();
    for (const auto &light : lights)
    {
        m_Lights.push_back(light->GetLightInfo());
    }
    m_IsLightsArrChanged = false;
}

void RenderLayer::UpdateBatches()
{
    for (int i = 0; i < m_Batches.size(); ++i)
    {
        if (m_Batches[i]->IsBatchChanged())
        {
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

bool RenderLayer::CheckLightsChanged()
{
    for (const auto &light : m_LightsArr)
    {
        if (light->GetIsLightSourceStateChanged())
        {
            return true;
        }
    }
    return false;
}

bool Batch::IsBatchChanged()
{
    for (const auto &mesh : meshes)
    {
        if (mesh->GetIsMeshStateChanged())
        {
            return true;
        }
    }
    return false;
}
