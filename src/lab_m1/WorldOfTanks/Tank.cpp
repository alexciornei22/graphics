#include "Tank.h"

#include <iostream>
#include <ostream>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include "utils/math_utils.h"

using namespace tank;

Tank::Tank(Type type, glm::vec3 position, glm::vec3 forward)
{
    this->type = type;
    this->position = position;
    this->forward = normalize(glm::vec3(forward.x, 0, forward.z));
    this->right = cross(forward, glm::vec3(0, 1, 0));
    this->gunForward = normalize(glm::vec3(forward.x, 0, forward.z));
}

void Tank::MoveForward(float distance)
{
    glm::vec3 dir = normalize(glm::vec3(forward.x, 0, forward.z));
    position += dir * distance;
}

void Tank::RotateHull_OY(float angle)
{
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

    float const oldTurretRotation = turretRotationActive;
    if (turretRotationActive < turretRotationTarget)
    {
        turretRotationActive += angle;
        turretRotationActive = MIN(turretRotationActive, turretRotationTarget);
    } else
    {
        turretRotationActive -= angle;
        turretRotationActive = MAX(turretRotationActive, turretRotationTarget);
    }
    glm::vec4 aux = rotate(glm::mat4(1.f), turretRotationActive - oldTurretRotation, glm::vec3(0, 1, 0)) * glm::vec4(gunForward, 1);
    gunForward = normalize(glm::vec3(aux));
}

bool Tank::CanFire()
{
    return timeLastShot >= 3.f;
}

Projectile Tank::FireProjectile()
{
    glm::vec3 projectilePosition = position + glm::vec3(0, 0.8, 0);
    projectilePosition += glm::normalize(gunForward) * 1.5f;
    timeLastShot = 0;
    return Projectile(projectilePosition, gunForward);
}

void Tank::IncrementTime(float deltaTime)
{
    timeLastShot += deltaTime;
    timeCurrentState += deltaTime;
}

void Tank::ExecuteState(float deltaTime)
{
    switch (state)
    {
    case State::MoveForward: MoveForward(deltaTime); break;
    case State::MoveBackward: MoveForward(-deltaTime); break;
    case State::RotateLeft: RotateHull_OY(deltaTime / 3); break;
    case State::RotateRight: RotateHull_OY(-deltaTime / 3);
    }
}

void Tank::UpdateState()
{
    if (timeCurrentState < TANK_STATE_INTERVAL) return;

    state = GetRandomState();
    timeCurrentState = 0;
}

std::string tank::GetTypeString(Type type)
{
    switch (type) {
    case TIGER_1:
        return "tiger1";
    case HELLCAT:
        return "hellcat";
    default:
            return "";
    }
}

State tank::GetRandomState()
{
    const int random = rand() % STATES.size();
    return STATES[random];
}
