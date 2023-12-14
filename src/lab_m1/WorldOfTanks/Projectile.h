#pragma once
#include <iostream>
#include <ostream>
#include <glm/vec3.hpp>

namespace tank
{
    constexpr float PROJECTILE_MAX_TIME = 5.f;
    
    class Projectile
    {
    public:
        Projectile(glm::vec3 position, glm::vec3 forward)
            : position(position),
              forward(forward)
        {}

        void TranslateForward();
        void IncrementTime(float deltaTime);
        
        glm::vec3 position;
        glm::vec3 forward;

        int damage = 1;
        float speed = 0.5f;
        float elapsedShotTime = 0;
    };
}
