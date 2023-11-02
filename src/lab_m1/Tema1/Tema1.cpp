#include "lab_m1/Tema1/Tema1.h"

#include <vector>
#include <iostream>

#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/object2D.h"
#include "lab_m1/Tema1/constants.h"

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

    logicSpace.x = 0;       // logic x
    logicSpace.y = 0;       // logic y
    logicSpace.width = 1280;   // logic width
    logicSpace.height = 720;  // logic height

    Mesh* endLine = object2D::CreateRect("endLine", glm::vec3(0), END_WIDTH, END_HEIGHT, glm::vec3(1, 0, 0), true);
    AddMeshToList(endLine);

    Mesh* tableSquare = object2D::CreateSquare("tableSquare", glm::vec3(0), SQUARE_LENGTH, glm::vec3(0, 1, 0), true);
    AddMeshToList(tableSquare);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            tableCoordinates.push_back(
                game::TableBoxData(2 * SEPARATION + END_WIDTH + j * (SQUARE_LENGTH + SEPARATION), SEPARATION + i * (SQUARE_LENGTH + SEPARATION), SQUARE_LENGTH)
            );
        }
    }

    Mesh* itemSquare = object2D::CreateSquare("itemSquare", glm::vec3(0), SQUARE_LENGTH, glm::vec3(1, 1, 1), false);
    AddMeshToList(itemSquare);
    for (int i = 0; i < NR_SHOOTERS; i++) {
        itemCoordinates.push_back(
            game::ItemBoxData(SEPARATION + i * (SEPARATION + SQUARE_LENGTH), logicSpace.height - SQUARE_LENGTH - SEPARATION, SQUARE_LENGTH, &shooters[i])
        );
    }

    Mesh* priceStar = object2D::CreateStar("priceStar", glm::vec3(0), PRICE_SIZE, glm::vec3(1, 1, 0));
    AddMeshToList(priceStar);

    Mesh* shooter = object2D::CreateShooter("shooter", glm::vec3(0, 0, 1), SHOOTER_SIZE, glm::vec3(1, 0, 0));
    AddMeshToList(shooter);

    Mesh* heart = object2D::CreateHeart("heart", glm::vec3(0), HEART_SIZE, 32, glm::vec3(1, 0, 0));
    AddMeshToList(heart);
}


void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema1::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->GetResolution();
    viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
    SetViewportArea(viewSpace, backgroundColor, true);

    // Compute uniform 2D visualization matrix
    visMatrix = glm::mat3(1);
    visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);

    glm::mat3 modelMatrix = visMatrix;
    modelMatrix *= transform2D::Translate(SEPARATION, SEPARATION);
    RenderMesh2D(meshes["endLine"], shaders["VertexColor"], modelMatrix);

    for (int i = 0; i < 3; i++) {
        modelMatrix = visMatrix * transform2D::Translate(2 * SEPARATION + END_WIDTH, SEPARATION);
        modelMatrix *= transform2D::Translate(0, i * (SQUARE_LENGTH + SEPARATION));

        for (int j = 0; j < 3; j++) {
            RenderMesh2D(meshes["tableSquare"], shaders["VertexColor"], modelMatrix);

            if (tableCoordinates[3 * i + j].shooter) {
                glm::mat3 tempModelMatrix = modelMatrix;

                tempModelMatrix *= transform2D::Translate(SQUARE_LENGTH / 3, SQUARE_LENGTH / 2);
                RenderMesh2D(meshes["shooter"], tempModelMatrix, tableCoordinates[3 * i + j].shooter->color);
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
        modelMatrix = visMatrix * transform2D::Translate(SEPARATION + PRICE_SIZE, logicSpace.height - SQUARE_LENGTH - 2 * SEPARATION);
        modelMatrix *= transform2D::Translate(i * (SQUARE_LENGTH + SEPARATION), 0);
        
        for (int j = 0; j < shooters[i].price; j++) {
            RenderMesh2D(meshes["priceStar"], shaders["VertexColor"], modelMatrix);
            modelMatrix *= transform2D::Translate(PRICE_SEPARATION, 0);
        }
    }

    modelMatrix = visMatrix;
    modelMatrix *= transform2D::Translate(SEPARATION + SQUARE_LENGTH / 3, logicSpace.height - SQUARE_LENGTH / 2 - SEPARATION);
    for (int i = 0; i < NR_SHOOTERS; i++) {
        RenderMesh2D(meshes["shooter"], modelMatrix, shooters[i].color);
        modelMatrix *= transform2D::Translate(SQUARE_LENGTH + SEPARATION, 0);
    }

    modelMatrix = visMatrix;
    modelMatrix *= transform2D::Translate((NR_SHOOTERS + 1) * (SEPARATION + SQUARE_LENGTH), logicSpace.height - SEPARATION - SQUARE_LENGTH / 2);
    for (int i = 0; i < 3; i++) {
        RenderMesh2D(meshes["heart"], shaders["VertexColor"], modelMatrix);
        modelMatrix *= transform2D::Translate(HEART_SIZE + SEPARATION, 0);
    }

    modelMatrix = visMatrix * transform2D::Translate(NR_SHOOTERS * (SEPARATION + SQUARE_LENGTH) + SQUARE_LENGTH, 0);
    modelMatrix *= transform2D::Translate(SEPARATION + PRICE_SIZE, logicSpace.height - SQUARE_LENGTH - 2 * SEPARATION);
    for (int i = 0; i < current_stars; i++) {
        RenderMesh2D(meshes["priceStar"], shaders["VertexColor"], modelMatrix);
        modelMatrix *= transform2D::Translate(PRICE_SEPARATION, 0);
    }

    if (selectedShooter) {
        modelMatrix = visMatrix;
        modelMatrix *= transform2D::Translate(mouseCoordinates.x, mouseCoordinates.y);
        RenderMesh2D(meshes["shooter"], modelMatrix, selectedShooter->color);
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
    mouseY = logicSpace.height - mouseY;

    mouseCoordinates = glm::vec3(mouseX, mouseY, 0);
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    mouseY = logicSpace.height - mouseY;

    if (button == GLFW_MOUSE_BUTTON_2) {
        for (int i = 0; i < NR_SHOOTERS; i++) {
            game::ItemBoxData currentBox = itemCoordinates[i];
            if (mouseX > currentBox.x &&
                mouseX < currentBox.x + currentBox.length &&
                mouseY > currentBox.y &&
                mouseY < currentBox.y + currentBox.length
                ) {
                selectedShooter = currentBox.shooter;
                mouseCoordinates = glm::vec3(mouseX, mouseY, 0);
                break;
            }
        }
    }
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    mouseY = logicSpace.height - mouseY;

    if (button == GLFW_MOUSE_BUTTON_2) {
        for (int i = 0; i < 9; i++) {
            game::TableBoxData &currentBox = tableCoordinates[i];
            if (mouseX > currentBox.x &&
                mouseX < currentBox.x + currentBox.length &&
                mouseY > currentBox.y &&
                mouseY < currentBox.y + currentBox.length &&
                currentBox.shooter == nullptr
                ) {
                currentBox.shooter = selectedShooter;
                break;
            }
        }

        selectedShooter = nullptr;
    }
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}

// 2D visualization matrix
glm::mat3 Tema1::VisualizationTransf2D(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
    float sx, sy, tx, ty;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    tx = viewSpace.x - sx * logicSpace.x;
    ty = viewSpace.y - sy * logicSpace.y;

    return glm::transpose(glm::mat3(
        sx, 0.0f, tx,
        0.0f, sy, ty,
        0.0f, 0.0f, 1.0f));
}


// Uniform 2D visualization matrix (same scale factor on x and y axes)
glm::mat3 Tema1::VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
    float sx, sy, tx, ty, smin;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    if (sx < sy)
        smin = sx;
    else
        smin = sy;
    tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
    ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

    return glm::transpose(glm::mat3(
        smin, 0.0f, tx,
        0.0f, smin, ty,
        0.0f, 0.0f, 1.0f));
}

void Tema1::SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor, bool clear)
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
