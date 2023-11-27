#include "Tank.h"

#include <iostream>
#include <ostream>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_transform.hpp>

#include "utils/math_utils.h"

using namespace tank;

void Tank::MoveForward(float distance)
{
    glm::vec3 dir = normalize(glm::vec3(forward.x, 0, forward.z));
    position += dir * distance;
}

void Tank::RotateHull_OY(float angle)
{
    hullRotation -= angle;
    if (turretRotationActive != turretRotationTarget)
    {
        turretRotationActive -= angle;
        glm::vec4 aux = rotate(glm::mat4(1.f), -angle, glm::vec3(0, 1, 0)) * glm::vec4(gunForward, 1);
        gunForward = normalize(glm::vec3(aux));
    }
    glm::vec4 aux = rotate(glm::mat4(1.f), -angle, glm::vec3(0, 1, 0)) * glm::vec4(forward, 1);
    forward = normalize(glm::vec3(aux));
    
    aux = rotate(glm::mat4(1.f), -angle, glm::vec3(0, 1, 0)) * glm::vec4(right, 1);
    right = normalize(glm::vec3(aux));
}

void Tank::RotateTurretTarget_OY(float angle)
{
    turretRotationTarget -= angle;
}

void Tank::RotateTurret_OY(float angle)
{
    if (turretRotationActive == turretRotationTarget)
        return;
    
    if (turretRotationActive < turretRotationTarget)
    {
        turretRotationActive += angle;
        turretRotationActive = MIN(turretRotationActive, turretRotationTarget);
    } else
    {
        turretRotationActive -= angle;
        turretRotationActive = MAX(turretRotationActive, turretRotationTarget);
    }
    glm::vec4 aux = rotate(glm::mat4(1.f), -angle, glm::vec3(0, 1, 0)) * glm::vec4(gunForward, 1);
    gunForward = normalize(glm::vec3(aux));
}

std::string tank::GetTypeString(Type type)
{
    switch (type) {
    case Type::TIGER_1:
        return "tiger1";
    default:
            return "";
    }
}
