#pragma once
#include <glm/glm.hpp>
#include <string>
#include <array>

#include "Projectile.h"

namespace tank
{
    enum Type
    {
        TIGER_1,
        HELLCAT
    };
    constexpr std::array<Type, 2> TYPES = {
        TIGER_1,
        HELLCAT,
    };
    enum class State
    {
        MoveForward,
        MoveBackward,
        RotateLeft,
        RotateRight
    };
    constexpr std::array<State, 4> STATES = {
        State::MoveForward,
        State::MoveBackward,
        State::RotateLeft,
        State::RotateRight
    };
    constexpr float TANK_FIRE_INTERVAL = 1.5f;
    constexpr float TANK_STATE_INTERVAL = 3.f;
    
    std::string GetTypeString(Type type);
    State GetRandomState();
    
    class Tank
    {
    public:
        Tank(Type type, glm::vec3 position, glm::vec3 forward);

        void MoveForward(float distance);
        void RotateHull_OY(float angle);
        void RotateTurretTarget_OY(float angle);
        void RotateTurret_OY(float angle);

        bool CanFire();
        Projectile FireProjectile();
        void IncrementTime(float deltaTime);

        void ExecuteState(float deltaTime);
        void UpdateState();
        
        Type type;
        glm::vec3 position;
        glm::vec3 forward;
        glm::vec3 right;

        float timeLastShot = 0;
        float timeCurrentState = 0;
        
        float turretRotationTarget = 0;
        float turretRotationActive = 0;
        glm::vec3 gunForward;

    private:
        State state = GetRandomState();
    };
}
