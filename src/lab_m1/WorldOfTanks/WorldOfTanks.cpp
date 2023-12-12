#include "WorldOfTanks.h"

#include <complex>
#include <vector>
#include <string>
#include <iostream>
#include <glm/gtx/vector_angle.hpp>
#include "Tank.h"
#include "lab_m1/lab3/object2D.h"

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
        Mesh* mesh = shapes::CreateSquare("square", glm::vec3(0), 1, glm::vec3(0));
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = shapes::CreateSquare("filled_square", glm::vec3(0), 1, glm::vec3(0), true);
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("plane50");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("shell1");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tanks"), "shell1.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
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
    orthoCamera = new ThirdPersonCamera(glm::vec3(0, 0, 50), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    cameraInput = new ThirdPersonCameraInput(camera);
    playerTank = new tank::Tank(tank::TIGER_1, glm::vec3(0, 0, 0), camera->forward);
    
    auto defaultCameraInput = GetCameraInput();
    defaultCameraInput->SetActive(false);
    window->DisablePointer();
    camera->SetTarget(playerTank);
    
    // Sets the resolution of the small viewport
    glm::ivec2 resolution = window->GetResolution();
    miniViewportArea = ViewportArea(50, 50, resolution.x / 5.f, resolution.y / 5.f);
}

void WorldOfTanks::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.3, 0.3, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void WorldOfTanks::Update(float deltaTimeSeconds)
{
    // Sets the screen area where to draw
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);

    RenderTank(*playerTank);
    glm::mat4 modelMatrix(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
    RenderMesh(meshes["sphere"], shaders["VertexNormal"], modelMatrix);
    
    for (auto projectile : projectiles)
    {
        modelMatrix = translate(glm::mat4(1.f), projectile.position);
        modelMatrix = scale(modelMatrix, glm::vec3(0.1f));
        modelMatrix = rotate(modelMatrix, glm::pi<float>() / 2, glm::vec3(1, 0, 0));
        modelMatrix = rotate(modelMatrix, orientedAngle(projectile.forward, glm::vec3(0, 0, 1), glm::vec3(0, 1, 0)), glm::vec3(0, 0, 1));
        RenderMesh(meshes["shell1"], shaders["VertexColor"], modelMatrix);
    }

    RenderMesh(meshes["plane50"], shaders["VertexNormal"], glm::mat4(1.f));
    DrawCoordinateSystem(camera->GetViewMatrix(), perspectiveProjection);
    
    modelMatrix = translate(glm::mat4(1), glm::vec3(1280 / 2, 100, 0));
    modelMatrix = scale(modelMatrix, glm::vec3(100));
    glm::mat4 shellModel = scale(modelMatrix, glm::vec3(0.3f));
    RenderMeshOrtho(meshes["shell1"], shaders["VertexNormal"], shellModel);
    modelMatrix = translate(modelMatrix, glm::vec3(-0.5f, -0.5f, 0));
    RenderMeshOrtho(meshes["square"], shaders["VertexColor"], modelMatrix);
    modelMatrix = scale(modelMatrix, glm::vec3(1, glm::min(playerTank->timeLastShot / 3.f, 1.f), 1));
    RenderMeshOrtho(meshes["filled_square"], shaders["VertexColor"], modelMatrix);

    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(miniViewportArea.x, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);
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

void WorldOfTanks::OnInputUpdate(float deltaTime, int mods)
{
    if (isinf(deltaTime)) return;
    
    if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        playerTank->RotateTurret_OY(deltaTime);
    }
    
    playerTank->IncrementTime(deltaTime);
    
    TranslateProjectiles();
}

void WorldOfTanks::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_2)
    {
        if (playerTank->CanFire())
        {
            projectiles.emplace_back(playerTank->FireProjectile());
        }
    }
}
