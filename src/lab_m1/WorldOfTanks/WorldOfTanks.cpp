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
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
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
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Shader *shader = new Shader("Tank");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "WorldOfTanks", "shaders", "Tank.VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "WorldOfTanks", "shaders", "Tank.FS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
    {
        Shader *shader = new Shader("Color");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "WorldOfTanks", "shaders", "Color.VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "WorldOfTanks", "shaders", "Color.FS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
    
    InitTankMeshes();

    InitBuildings();

    InitEnemyTanks();
    
    camera = new ThirdPersonCamera(glm::vec3(0, 1, 2), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
    orthoCamera = new ThirdPersonCamera(glm::vec3(0, 0, 50), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    cameraInput = new ThirdPersonCameraInput(camera);

    glm::vec3 position = GetRandomPosition();
    while (DetectTankBuildingsCollision(position)) position = GetRandomPosition();
    playerTank = new tank::Tank(tank::TIGER_2, position, camera->forward, 10);
    
    auto defaultCameraInput = GetCameraInput();
    defaultCameraInput->SetActive(false);
    window->DisablePointer();
    camera->SetTarget(playerTank);

    // Light & material properties
    {
        lightPosition = glm::vec3(0, 100, 0);
        materialShininess = 30;
        materialKd = 0.5;
        materialKs = 0.5;
    }
    
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

    for (auto &tank : enemyTanks)
    {
        RenderTank(tank);
    }
    
    for (auto projectile : projectiles)
    {
        RenderProjectile(projectile);
    }

    for (auto &building : buildings)
    {
        RenderBuilding(building);
    }
    
    RenderPlane();
    
    glm::mat4 modelMatrix = translate(glm::mat4(1), glm::vec3(1280 / 2, 100, 0));
    modelMatrix = scale(modelMatrix, glm::vec3(100));
    glm::mat4 shellModel = scale(modelMatrix, glm::vec3(0.3f));
    RenderMeshOrtho(meshes["shell1"], shaders["VertexNormal"], shellModel);
    modelMatrix = translate(modelMatrix, glm::vec3(-0.5f, -0.5f, 0));
    RenderMeshOrtho(meshes["square"], shaders["VertexColor"], modelMatrix);
    modelMatrix = scale(modelMatrix, glm::vec3(1, glm::min(playerTank->timeLastShot / tank::TANK_FIRE_INTERVAL, 1.f), 1));
    RenderMeshOrtho(meshes["filled_square"], shaders["VertexColor"], modelMatrix);

    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(miniViewportArea.x, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);
}

void WorldOfTanks::FrameEnd()
{
}

void WorldOfTanks::OnInputUpdate(float deltaTime, int mods)
{
    if (isinf(deltaTime)) return;
    
    if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        playerTank->RotateTurret_OY(deltaTime);
    }
    
    IncrementTankTimes(deltaTime);

    IncrementProjectileTimes(deltaTime);
    
    UpdateTankStates();
    
    TranslateProjectiles();
    
    ExecuteTankActions(deltaTime);
    
    SetAttackStates();

    HandleProjectileTankCollisions();

    HandleTankTankCollisions();

    HandleTanksBuildingsCollisions(deltaTime);

    HandleProjectilesBuildingsCollisions();
    
    DeleteExpiredProjectiles();

    CheckGameEnded();
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
