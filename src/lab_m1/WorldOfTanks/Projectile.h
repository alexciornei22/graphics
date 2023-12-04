#pragma once
#include <iostream>
#include <ostream>
#include <glm/vec3.hpp>

namespace tank
{
    class Projectile
    {
    public:
        Projectile(glm::vec3 position, glm::vec3 forward)
            : position(position),
              forward(forward)
        {}

        void TranslateForward();
        
        glm::vec3 position;
        glm::vec3 forward;
        float speed = 1.f;
    };
}
