#include "WorldOfTanks.h"
#include <vector>
#include <string>
#include <iostream>

#include "Tank.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


WorldOfTanks::WorldOfTanks()
{
}


WorldOfTanks::~WorldOfTanks()
{
}


void WorldOfTanks::Init()
{
    {
        Mesh* mesh = new Mesh("tiger1_hull");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tanks/tiger1"), "hull.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("tiger1_turret");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tanks/tiger1"), "turret.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("tiger1_gun");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tanks/tiger1"), "gun.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("tiger1_tracks");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tanks/tiger1"), "tracks.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    camera = new ThirdPersonCamera(glm::vec3(0, 1, 2), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
    cameraInput = new ThirdPersonCameraInput(camera);
    playerTank = new tank::Tank(tank::Type::TIGER_1, glm::vec3(0, 0, 0), camera->forward);
    auto defaultCameraInput = GetCameraInput();
    defaultCameraInput->SetActive(false);
    window->DisablePointer();
    camera->SetTarget(playerTank);
    projectionMatrix = glm::perspective(glm::radians(60.f), window->props.aspectRatio, 0.01f, 200.0f);

    // Sets the resolution of the small viewport
    glm::ivec2 resolution = window->GetResolution();
    miniViewportArea = ViewportArea(50, 50, resolution.x / 5.f, resolution.y / 5.f);
}

void WorldOfTanks::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void WorldOfTanks::Update(float deltaTimeSeconds)
{
    glLineWidth(3);
    glPointSize(5);

    // Sets the screen area where to draw
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);

    RenderTank(*playerTank);
    glm::mat4 modelMatrix(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
    RenderMesh(meshes["sphere"], shaders["VertexNormal"], modelMatrix);
    DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);

    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(miniViewportArea.x, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);
    
    RenderMesh(meshes["tiger1_hull"], shaders["VertexNormal"], modelMatrix);
    RenderMesh(meshes["tiger1_turret"], shaders["VertexNormal"], modelMatrix);
    RenderMesh(meshes["tiger1_gun"], shaders["VertexNormal"], modelMatrix);
    RenderMesh(meshes["tiger1_tracks"], shaders["VertexNormal"], modelMatrix);
    DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}

void WorldOfTanks::FrameEnd()
{
}

void WorldOfTanks::RenderTank(tank::Tank& tank)
{
    const std::string typeString = GetTypeString(tank.type);
    glm::mat4 hullModelMatrix = translate(glm::mat4(1), tank.position);
    glm::mat4 turretModelMatrix = hullModelMatrix;
    hullModelMatrix = rotate(hullModelMatrix, tank.hullRotation, glm::vec3(0, 1, 0));
    turretModelMatrix = rotate(turretModelMatrix, tank.turretRotationActive, glm::vec3(0, 1, 0));
    
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
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}

void WorldOfTanks::OnInputUpdate(float deltaTime, int mods)
{
    if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        playerTank->RotateTurret_OY(deltaTime);
    }
}
