#pragma once
#include <glm/glm.hpp>
#include <string>

namespace tank
{
    enum class Type
    {
        TIGER_1
    };

    class Tank
    {
    public:
        Tank(Type type, glm::vec3 position, glm::vec3 forward) :
        type(type),
        position(position),
        forward(forward),
        right(cross(forward, glm::vec3(0, 1, 0))),
        gunForward(forward) {}
        
        void MoveForward(float distance);
        void RotateHull_OY(float angle);
        void RotateTurretTarget_OY(float angle);
        void RotateTurret_OY(float angle);
        
        Type type;
        glm::vec3 position;
        glm::vec3 forward;
        glm::vec3 right;
        float hullRotation = 0;
        
        float turretRotationTarget = 0;
        float turretRotationActive = 0;
        glm::vec3 gunForward;
    };

    std::string GetTypeString(Type type);
}
