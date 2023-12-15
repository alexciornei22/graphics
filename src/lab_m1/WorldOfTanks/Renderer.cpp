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

    glm::vec3 turretColor {0, 1, 0};
    glm::vec3 hullColor {0, 0.75f, 0};
    glm::vec3 gunColor {0.8f, 0.8f, 0.8f};
    glm::vec3 tracksColor {0.7f, 0.9f, 0.7f};

    float healthPercentage = tank.GetHealthPercentage();
    // RenderMesh(meshes["sphere"], shaders["VertexNormal"], glm::scale(hullModelMatrix, glm::vec3(tank::TANK_COLLISION_SPHERE_RADIUS * 2)));
    RenderTankMesh(meshes[typeString + "_hull"], shaders["Tank"], hullModelMatrix, hullColor, healthPercentage);
    RenderTankMesh(meshes[typeString + "_turret"], shaders["Tank"], turretModelMatrix, turretColor, healthPercentage);
    RenderTankMesh(meshes[typeString + "_gun"], shaders["Tank"], turretModelMatrix, gunColor, healthPercentage);
    RenderTankMesh(meshes[typeString + "_tracks"], shaders["Tank"], hullModelMatrix, tracksColor, healthPercentage);
}

void WorldOfTanks::RenderProjectile(tank::Projectile& projectile)
{
    glm::mat4 modelMatrix = translate(glm::mat4(1.f), projectile.position);
    modelMatrix = scale(modelMatrix, glm::vec3(0.1f));
    modelMatrix = rotate(modelMatrix, glm::pi<float>() / 2, glm::vec3(1, 0, 0));
    modelMatrix = rotate(modelMatrix, orientedAngle(projectile.forward, glm::vec3(0, 0, 1), glm::vec3(0, 1, 0)), glm::vec3(0, 0, 1));
    RenderMesh(meshes["shell1"], shaders["VertexColor"], modelMatrix);
}

void WorldOfTanks::RenderBuilding(Building& building)
{
    glm::mat4 modelMatrix {1.f};
    modelMatrix = glm::translate(modelMatrix, building.GetPosition());
    glm::vec3 scaleVector = glm::vec3(building.GetLength(), building.GetHeight() * 2, building.GetWidth());
    modelMatrix = glm::scale(modelMatrix, scaleVector);

    RenderTankMesh(meshes["box"], shaders["Tank"], modelMatrix, glm::vec3(0.8f), 1.f);
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

void WorldOfTanks::RenderTankMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, float healthPercentage)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->GetUniformLocation("View"), 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->GetUniformLocation("Projection"), 1, GL_FALSE, glm::value_ptr(perspectiveProjection));
    glUniformMatrix4fv(shader->GetUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

    glUniform3fv(shader->GetUniformLocation("light_position"), 1, glm::value_ptr(lightPosition));
    glm::vec3 eyePosition = camera->position;
    glUniform3fv(shader->GetUniformLocation("eye_position"), 1, glm::value_ptr(eyePosition));
    glUniform1i(shader->GetUniformLocation("material_shininess"), materialShininess);
    glUniform1f(shader->GetUniformLocation("material_kd"), materialKd);
    glUniform1f(shader->GetUniformLocation("material_ks"), materialKs);
    glUniform3fv(shader->GetUniformLocation("object_color"), 1, glm::value_ptr(color));
    glUniform1f(shader->GetUniformLocation("health_percentage"), healthPercentage);

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
