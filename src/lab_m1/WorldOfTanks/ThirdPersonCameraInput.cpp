#include "ThirdPersonCameraInput.h"

#include <iostream>

#include "core/engine.h"

void ThirdPersonCameraInput::OnInputUpdate(float deltaTime, int mods)
{
    if (window->KeyHold(GLFW_KEY_W))
        camera->MoveForward(deltaTime);
    if (window->KeyHold(GLFW_KEY_A))
        camera->RotateTarget_OY(-deltaTime);
    if (window->KeyHold(GLFW_KEY_S))
        camera->MoveForward(-deltaTime);
    if (window->KeyHold(GLFW_KEY_D))
        camera->RotateTarget_OY(deltaTime);
}

void ThirdPersonCameraInput::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    float sensivityOX = 0.002f;
    float sensivityOY = 0.002f;
    
    camera->Rotate_OY((float) deltaX * sensivityOY);
    camera->Rotate_OX((float) deltaY * sensivityOX);
}

void ThirdPersonCameraInput::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
    const float offsetDistance = static_cast<float>(offsetY) / 2;
    if (
        offsetDistance < 0 && camera->distanceToTarget > 5
        || offsetDistance > 0 && camera->distanceToTarget <= 1
    )
        return;

    camera->TranslateForward(offsetDistance);
    camera->distanceToTarget -= offsetDistance;
    if (camera->position.y < 0)
    {
        camera->TranslateForward(-offsetDistance);
        camera->distanceToTarget += offsetDistance;
    }
}
