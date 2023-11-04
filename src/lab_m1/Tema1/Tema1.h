#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema1/game.h"

namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        struct CoordinateSpace
        {
            CoordinateSpace() : x(0), y(0), width(1), height(1) {}
            CoordinateSpace(float x, float y, float width, float height)
                : x(x), y(y), width(width), height(height) {}
            float x;
            float y;
            float width;
            float height;
        };

        Tema1();
        ~Tema1();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        // Sets the logic space and view space
        // logicSpace: { x, y, width, height }
        // viewSpace: { x, y, width, height }
        glm::mat3 VisualizationTransf2D(const CoordinateSpace& logicSpace, const CoordinateSpace& viewSpace);
        glm::mat3 VisualizationTransf2DUnif(const CoordinateSpace& logicSpace, const CoordinateSpace& viewSpace);
        void SetViewportArea(const CoordinateSpace& viewSpace, glm::vec3 colorColor = glm::vec3(0), bool clear = true);

    protected:
        glm::mat3 visMatrix;
        CoordinateSpace viewSpace;
        CoordinateSpace logicSpace;
        glm::vec3 bottomLeftCorner = glm::vec3(0);

        std::vector<game::ItemBoxData> itemCoordinates;
        game::TableBoxData tableCoordinates[3][3] = {
            {
                game::TableBoxData(2 * SEPARATION + END_WIDTH, SEPARATION, SQUARE_LENGTH), 
                game::TableBoxData(2 * SEPARATION + END_WIDTH + 1 * (SQUARE_LENGTH + SEPARATION), SEPARATION, SQUARE_LENGTH),
                game::TableBoxData(2 * SEPARATION + END_WIDTH + 2 * (SQUARE_LENGTH + SEPARATION), SEPARATION, SQUARE_LENGTH)
            },
            {
                game::TableBoxData(2 * SEPARATION + END_WIDTH, SEPARATION + 1 * (SQUARE_LENGTH + SEPARATION), SQUARE_LENGTH),
                game::TableBoxData(2 * SEPARATION + END_WIDTH + 1 * (SQUARE_LENGTH + SEPARATION), SEPARATION + 1 * (SQUARE_LENGTH + SEPARATION), SQUARE_LENGTH),
                game::TableBoxData(2 * SEPARATION + END_WIDTH + 2 * (SQUARE_LENGTH + SEPARATION), SEPARATION + 1 * (SQUARE_LENGTH + SEPARATION), SQUARE_LENGTH)
            },
            {
                game::TableBoxData(2 * SEPARATION + END_WIDTH, SEPARATION + 2 * (SQUARE_LENGTH + SEPARATION), SQUARE_LENGTH),
                game::TableBoxData(2 * SEPARATION + END_WIDTH + 1 * (SQUARE_LENGTH + SEPARATION), SEPARATION + 2 * (SQUARE_LENGTH + SEPARATION), SQUARE_LENGTH),
                game::TableBoxData(2 * SEPARATION + END_WIDTH + 2 * (SQUARE_LENGTH + SEPARATION), SEPARATION + 2 * (SQUARE_LENGTH + SEPARATION), SQUARE_LENGTH)
            },
        };
        glm::vec3 backgroundColor = glm::vec3(0.2f);

        game::Shooter* selectedShooter = nullptr;
        int current_stars = 5;
        glm::vec3 mouseCoordinates;

        std::vector<game::Shooter> shooters = {
            game::Shooter(1, glm::vec3(0, 0, 1)),
            game::Shooter(2, glm::vec3(1, 1, 0)),
            game::Shooter(2, glm::vec3(0, 1, 0)),
            game::Shooter(3, glm::vec3(0, 1, 1)),
        };
        std::vector<game::Enemy> enemies;
        std::vector<game::Projectile> projectiles;
    };
}   // namespace m1
