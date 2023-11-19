#include "lab_m1/Tema1/Tema1.h"

#include <vector>
#include <iostream>

#include "lab_m1/Tema1/object2D.h"
#include "lab_m1/Tema1/constants.h"
#include "lab_m1/Tema1/animate.h"

using namespace std;
using namespace m1;

/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    logicSpace = transform2D::CoordinateSpace(0, 0, 1280, 720);
    viewSpace = transform2D::CoordinateSpace(0, 0, 1280, 720);

    Mesh* endLine = object2D::CreateRect("endLine", glm::vec3(0), END_WIDTH, END_HEIGHT, glm::vec3(1, 0, 0), true);
    AddMeshToList(endLine);

    Mesh* tableSquare = object2D::CreateSquare("tableSquare", glm::vec3(0), SQUARE_LENGTH, glm::vec3(0, 0.5f, 0), true);
    AddMeshToList(tableSquare);

    Mesh* itemSquare = object2D::CreateSquare("itemSquare", glm::vec3(0), SQUARE_LENGTH, glm::vec3(1, 1, 1), false);
    AddMeshToList(itemSquare);
    for (int i = 0; i < NR_SHOOTERS; i++) {
        itemCoordinates.push_back(
            game::ItemBoxData(SEPARATION + i * (SEPARATION + SQUARE_LENGTH), logicSpace.height - SQUARE_LENGTH - SEPARATION, SQUARE_LENGTH, &shooters[i])
        );
    }

    Mesh* priceStar = object2D::CreateStar("priceStar", glm::vec3(0), PRICE_SIZE, glm::vec3(1, 1, 0));
    AddMeshToList(priceStar);

    Mesh* star = object2D::CreateStar("star", glm::vec3(0, 0, 10), STAR_SIZE, glm::vec3(1));
    AddMeshToList(star);

    Mesh* shooter = object2D::CreateShooter("shooter", glm::vec3(0, 0, 1), SHOOTER_SIZE, glm::vec3(1, 0, 0));
    AddMeshToList(shooter);

    Mesh* heart = object2D::CreateHeart("heart", glm::vec3(0), HEART_SIZE, 32, glm::vec3(1, 0, 0));
    AddMeshToList(heart);

    Mesh* enemyOuter = object2D::CreateHexagon("enemyOuter", glm::vec3(0, 0, 3), ENEMY_SIZE, glm::vec3(0));
    AddMeshToList(enemyOuter);

    Mesh* enemyInner = object2D::CreateHexagon("enemyInner", glm::vec3(0, 0, 4), ENEMY_SIZE * 0.7f, glm::vec3(0));
    AddMeshToList(enemyInner);

    Mesh* projectile = object2D::CreateStar("projectile", glm::vec3(0, 0, 5), PROJECTILE_SIZE, glm::vec3(1, 1, 1));
    AddMeshToList(projectile);
}


void Tema1::FrameStart()
{
}


void Tema1::Update(float deltaTimeSeconds)
{
    SetViewportArea(viewSpace, backgroundColor, true);

    glm::mat3 modelMatrix = visMatrix;

    if (selectedShooter) {
        modelMatrix = visMatrix;
        modelMatrix *= transform2D::Translate(mouseCoordinates.x, mouseCoordinates.y);
        RenderMesh2D(meshes["shooter"], modelMatrix, selectedShooter->color);
    }

    for (auto& star : stars) {
        modelMatrix = visMatrix;
        modelMatrix *= transform2D::Translate(star.coordinates.x, star.coordinates.y);
        RenderMesh2D(meshes["star"], modelMatrix, glm::vec3(1, 0, 1));
    }

    modelMatrix = visMatrix * transform2D::Translate(SEPARATION, SEPARATION);
    RenderMesh2D(meshes["endLine"], shaders["VertexColor"], modelMatrix);

    for (int i = 0; i < 3; i++) {
        modelMatrix = visMatrix * transform2D::Translate(2 * SEPARATION + END_WIDTH, SEPARATION);
        modelMatrix *= transform2D::Translate(0, i * (SQUARE_LENGTH + SEPARATION));

        for (int j = 0; j < 3; j++) {
            game::TableBoxData currentBox = tableCoordinates[i][j];
            RenderMesh2D(meshes["tableSquare"], shaders["VertexColor"], modelMatrix);

            if (currentBox.shooter) {
                glm::mat3 tempModelMatrix = modelMatrix;

                tempModelMatrix *= transform2D::Translate(SQUARE_LENGTH / 3, SQUARE_LENGTH / 2);
                tempModelMatrix *= transform2D::Scale(currentBox.scale, currentBox.scale);
                RenderMesh2D(meshes["shooter"], tempModelMatrix, currentBox.shooter->color);
            }

            modelMatrix *= transform2D::Translate(SQUARE_LENGTH + SEPARATION, 0);
        }
    }

    modelMatrix = visMatrix * transform2D::Translate(SEPARATION, logicSpace.height - SQUARE_LENGTH - SEPARATION);
    for (int i = 0; i < NR_SHOOTERS; i++) {
        RenderMesh2D(meshes["itemSquare"], shaders["VertexColor"], modelMatrix);
        modelMatrix *= transform2D::Translate(SQUARE_LENGTH + SEPARATION, 0);
    }

    for (int i = 0; i < shooters.size(); i++) {
        modelMatrix = visMatrix * transform2D::Translate(SEPARATION + PRICE_SIZE / 2, logicSpace.height - SQUARE_LENGTH - 1.5f * SEPARATION - PRICE_SIZE / 2);
        modelMatrix *= transform2D::Translate(i * (SQUARE_LENGTH + SEPARATION), 0);
        
        for (int j = 0; j < shooters[i].price; j++) {
            RenderMesh2D(meshes["priceStar"], shaders["VertexColor"], modelMatrix);
            modelMatrix *= transform2D::Translate(PRICE_SEPARATION, 0);
        }
    }

    modelMatrix = visMatrix * transform2D::Translate(SEPARATION + SQUARE_LENGTH / 3, logicSpace.height - SQUARE_LENGTH / 2 - SEPARATION);
    for (int i = 0; i < NR_SHOOTERS; i++) {
        RenderMesh2D(meshes["shooter"], modelMatrix, shooters[i].color);
        modelMatrix *= transform2D::Translate(SQUARE_LENGTH + SEPARATION, 0);
    }

    modelMatrix = visMatrix * transform2D::Translate((NR_SHOOTERS + 1) * (SEPARATION + SQUARE_LENGTH), logicSpace.height - SEPARATION - SQUARE_LENGTH / 2);
    for (auto &heart : hearts) {
        modelMatrix *= transform2D::Scale(heart.scale, heart.scale);
        RenderMesh2D(meshes["heart"], shaders["VertexColor"], modelMatrix);
        modelMatrix *= transform2D::Translate(HEART_SIZE + SEPARATION, 0);
        modelMatrix *= transform2D::Scale(1 / heart.scale, 1 / heart.scale);
    }

    for (int i = 0; i < currentStars; i++) {
        modelMatrix = visMatrix * transform2D::Translate(NR_SHOOTERS * (SEPARATION + SQUARE_LENGTH) + SQUARE_LENGTH, 0);
        modelMatrix *= transform2D::Translate(0, logicSpace.height - SQUARE_LENGTH - 1.5f * SEPARATION - PRICE_SIZE / 2);
        modelMatrix *= transform2D::Translate((i % STARS_PER_ROW) * PRICE_SEPARATION, -(i / STARS_PER_ROW) * PRICE_SEPARATION);
        RenderMesh2D(meshes["priceStar"], shaders["VertexColor"], modelMatrix);
    }

    for (auto &enemy : enemies) {
        modelMatrix = visMatrix;
        modelMatrix *= transform2D::Translate(enemy.coordinates.x, enemy.coordinates.y);
        modelMatrix *= transform2D::Rotate(0.3f);
        modelMatrix *= transform2D::Scale(enemy.scale, enemy.scale);
        RenderMesh2D(meshes["enemyOuter"], modelMatrix, shooters[enemy.type].color);
        modelMatrix *= transform2D::Scale(enemy.health / 3.f, enemy.health / 3.f);
        RenderMesh2D(meshes["enemyInner"], modelMatrix, shooters[enemy.type].color / glm::vec3(2));
    }

    for (auto& projectile : projectiles) {
        modelMatrix = visMatrix;
        modelMatrix *= transform2D::Translate(projectile.coordinates.x, projectile.coordinates.y);
        modelMatrix *= transform2D::Rotate(projectile.angularStep);
        RenderMesh2D(meshes["projectile"], modelMatrix, projectile.color);
    }
}


void Tema1::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    animate::scaleDownShooters(tableCoordinates, deltaTime);

    animate::scaleDownEnemies(enemies, deltaTime);

    animate::scaleDownHearts(hearts, deltaTime);

    animate::moveEnemiesLeft(enemies, deltaTime);

    animate::moveProjectilesRight(projectiles, deltaTime);

    game::checkEnemyReachedEnd(enemies, hearts);

    game::checkIfGameEnded(hearts, window);

    game::generateEnemies(enemies, shooters, lastGeneratedEnemies);

    game::generateProjectiles(tableCoordinates, projectiles, enemies);

    game::generateStars(stars, lastGeneratedStars);

    game::checkProjectileEnemyCollisions(projectiles, enemies);

    game::checkShooterEnemyCollisions(tableCoordinates, enemies);

    game::removeInvalidPieces(projectiles, enemies, hearts);
}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    transform2D::updateMouseCoordinates(logicSpace, viewSpace, mouseCoordinates, mouseX, mouseY);
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    transform2D::updateMouseCoordinates(logicSpace, viewSpace, mouseCoordinates, mouseX, mouseY);

    if (button == GLFW_MOUSE_BUTTON_2) {
        game::checkHasCollectedStar(mouseCoordinates, stars, currentStars);

        game::checkHasSelectedShooter(mouseCoordinates, itemCoordinates, selectedShooter, currentStars);
    }

    if (button == GLFW_MOUSE_BUTTON_3) {
        game::checkHasClearedBox(mouseCoordinates, tableCoordinates);
    }
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    transform2D::updateMouseCoordinates(logicSpace, viewSpace, mouseCoordinates, mouseX, mouseY);

    if (button == GLFW_MOUSE_BUTTON_2 && selectedShooter) {
        game::checkHasDroppedShooter(mouseCoordinates, tableCoordinates, selectedShooter, currentStars);
    }
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema1::OnWindowResize(int width, int height)
{
    viewSpace = transform2D::CoordinateSpace(0, 0, width, height);
    SetViewportArea(viewSpace, backgroundColor, true);

    // Compute uniform 2D visualization matrix
    visMatrix = transform2D::VisualizationTransf2D(logicSpace, viewSpace);
}

void Tema1::SetViewportArea(const transform2D::CoordinateSpace& viewSpace, glm::vec3 colorColor, bool clear)
{
    glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    glEnable(GL_SCISSOR_TEST);
    glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
    GetSceneCamera()->Update();
}