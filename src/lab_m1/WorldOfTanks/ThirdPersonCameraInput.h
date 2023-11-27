#pragma once
#include "ThirdPersonCamera.h"
#include "core/window/input_controller.h"

class ThirdPersonCameraInput : public InputController
{
public:
    ThirdPersonCameraInput(ThirdPersonCamera *camera) : camera(camera) {}
    
    void OnInputUpdate(float deltaTime, int mods) override;
    void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
    void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
    
    ThirdPersonCamera *camera;
};
