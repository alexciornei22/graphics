#include <glm/gtx/vector_angle.hpp>

#include "WorldOfTanks.h"

using namespace m1;

void WorldOfTanks::RenderTank(tank::Tank& tank)
{
    const std::string typeString = GetTypeString(tank.type);
    glm::mat4 hullModelMatrix = translate(glm::mat4(1), tank.position);
    glm::mat4 turretModelMatrix = hullModelMatrix;
    float const hullAngle = -orientedAngle(tank.forward, glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
    hullModelMatrix = rotate(hullModelMatrix, hullAngle, glm::vec3(0, 1, 0));
    float const gunAngle = -orientedAngle(tank.gunForward, glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
    turretModelMatrix = rotate(turretModelMatrix, gunAngle, glm::vec3(0, 1, 0));

    // RenderMesh(meshes["sphere"], shaders["VertexNormal"], glm::scale(hullModelMatrix, glm::vec3(2.f)));
    RenderMesh(meshes[typeString + "_hull"], shaders["Simple"], hullModelMatrix);
    RenderMesh(meshes[typeString + "_turret"], shaders["Simple"], turretModelMatrix);
    RenderMesh(meshes[typeString + "_gun"], shaders["Simple"], turretModelMatrix);
    RenderMesh(meshes[typeString + "_tracks"], shaders["Simple"], hullModelMatrix);
}

void WorldOfTanks::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(perspectiveProjection));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}

void WorldOfTanks::RenderMeshOrtho(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(orthoCamera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(orthoProjection));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}