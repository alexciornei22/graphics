#pragma once

#include "Tank.h"
#include "ThirdPersonCamera.h"
#include "ThirdPersonCameraInput.h"
#include "components/simple_scene.h"


namespace m1
{
    class WorldOfTanks : public gfxc::SimpleScene
    {
     public:
        struct ViewportArea
        {
            ViewportArea() : x(0), y(0), width(1), height(1) {}
            ViewportArea(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };

        WorldOfTanks();
        ~WorldOfTanks();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;
        
        void RenderTank(tank::Tank& tank);
        void RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix) override;

        void OnInputUpdate(float deltaTime, int mods) override;
     protected:
        glm::mat4 projectionMatrix = glm::mat4(1.f);
        ThirdPersonCamera *camera;
        ThirdPersonCameraInput *cameraInput;

        tank::Tank *playerTank;
        ViewportArea miniViewportArea;
    };
}   // namespace m1
