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
        RotateRight,
        RotateTurretLeft,
        RotateTurretRight,
        Attack,
        Dead
    };
    constexpr std::array<State, 8> STATES = {
        State::MoveForward,
        State::MoveBackward,
        State::RotateLeft,
        State::RotateRight,
        State::RotateTurretLeft,
        State::RotateTurretRight,
        State::Attack,
        State::Dead
    };
    constexpr float TANK_FIRE_INTERVAL = 1.5f;
    constexpr float TANK_STATE_INTERVAL = 3.f;
    constexpr float TANK_COLLISION_SPHERE_RADIUS = 1.25f;
    
    std::string GetTypeString(Type type);
    State GetRandomState();
    
    class Tank
    {
    public:
        Tank(Type type, glm::vec3 position, glm::vec3 forward, int health);

        void MoveForward(float distance);
        void RotateHull_OY(float angle);
        void RotateTurretTarget_OY(float angle);
        void RotateTurret_OY(float angle);
        void AimGunAt(glm::vec3 target);
        
        bool CanFire();
        Projectile FireProjectile();
        void IncrementTime(float deltaTime);

        void DecreaseHealth(int damage);
        void ExecuteState(float deltaTime, glm::vec3 playerPosition);
        void UpdateState();
        void SetAttackState();
        
        Type type;
        glm::vec3 position;
        glm::vec3 forward;
        glm::vec3 right;

        float timeLastShot = 0;
        float timeCurrentState = 0;
        
        glm::vec3 gunForward;
        glm::vec3 gunForwardTarget;

    private:
        int health;
        State state = GetRandomState();
    };
}
