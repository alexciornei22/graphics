#pragma once

#include "Building.h"
#include "Projectile.h"
#include "Tank.h"
#include "ThirdPersonCamera.h"
#include "ThirdPersonCameraInput.h"
#include "components/simple_scene.h"

namespace m1
{
    constexpr int WORLD_LENGTH = 50;

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
        void InitTankMeshes();
        void InitBuildings();
        void InitEnemyTanks();
        
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;
        
        void RenderTank(tank::Tank& tank);
        void RenderProjectile(tank::Projectile& projectile);
        void RenderBuilding(Building &building);
        void RenderPlane();
        void RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix) override;
        void RenderTankMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, const glm::vec3& color, float healthPercentage);
        void RenderColorMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, const glm::vec3& color);
        void RenderMeshOrtho(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;

        void IncrementTankTimes(float deltaTime);
        void IncrementProjectileTimes(float deltaTime);
        void UpdateTankStates();
        void TranslateProjectiles();
        void ExecuteTankActions(float deltaTime);
        void SetAttackStates();
        void HandleProjectileTankCollisions();
        void HandleTankTankCollisions();
        void HandleTanksCollision(tank::Tank &tank1, tank::Tank &tank2);
        void HandleTanksBuildingsCollisions(float deltaTime);
        bool DetectTankBuildingCollision(glm::vec3 position, Building &building);
        bool DetectTankBuildingsCollision(glm::vec3 position);
        void HandleTankBuildingCollision(tank::Tank &tank, Building &building, float deltaTime);
        void HandleProjectilesBuildingsCollisions();
        void DeleteExpiredProjectiles();
        void CheckGameEnded();
        
        glm::vec3 GetRandomPosition();
        
     protected:
        glm::mat4 perspectiveProjection = glm::perspective(glm::radians(60.f), window->props.aspectRatio, 0.01f, 200.0f);
        glm::mat4 orthoProjection = glm::ortho(0.f, 1280.f, 0.f, 720.f, 0.1f, 200.f);
        ThirdPersonCamera *camera;
        ThirdPersonCamera *orthoCamera;
        ThirdPersonCameraInput *cameraInput;
        tank::Tank *playerTank;
        std::vector<tank::Tank> enemyTanks;
        std::vector<tank::Projectile> projectiles;
        std::vector<Building> buildings;
        
        glm::vec3 lightPosition;
        int materialShininess;
        float materialKd;
        float materialKs;
        
        ViewportArea miniViewportArea;
    };
}   // namespace m1
