#include "Tank.h"

#include <iostream>
#include <ostream>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_transform.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "Projectile.h"
#include "utils/math_utils.h"

using namespace tank;

Tank::Tank(Type type, glm::vec3 position, glm::vec3 forward)
{
    this->type = type;
    this->position = position;
    this->forward = normalize(glm::vec3(forward.x, 0, forward.z));
    this->right = cross(forward, glm::vec3(0, 1, 0));
    this->gunForward = normalize(glm::vec3(forward.x, 0, forward.z));
    this->gunForwardTarget = this->gunForward;
}

void Tank::MoveForward(float distance)
{
    glm::vec3 dir = normalize(glm::vec3(forward.x, 0, forward.z));
    position += dir * distance;
}

void Tank::RotateHull_OY(float angle)
{
    glm::vec4 aux = rotate(glm::mat4(1.f), -angle, glm::vec3(0, 1, 0)) * glm::vec4(forward, 1);
    forward = normalize(glm::vec3(aux));
    
    aux = rotate(glm::mat4(1.f), -angle, glm::vec3(0, 1, 0)) * glm::vec4(right, 1);
    right = normalize(glm::vec3(aux));
}

void Tank::RotateTurretTarget_OY(float angle)
{
    glm::vec4 aux = rotate(glm::mat4(1.f), -angle, glm::vec3(0, 1, 0)) * glm::vec4(gunForwardTarget, 1);
    gunForwardTarget = normalize(glm::vec3(aux));
}

void Tank::RotateTurret_OY(float angle)
{
    if (gunForwardTarget == gunForward)
        return;

    float angleToTarget = orientedAngle(gunForward, gunForwardTarget, glm::vec3(0, 1, 0));

    if (abs(angle) > abs(angleToTarget))
    {
        gunForward = gunForwardTarget;
    }
    else
    {
        if (angleToTarget < 0) angle *= -1;
        
        glm::vec4 aux = rotate(glm::mat4(1.f), angle, glm::vec3(0, 1, 0)) * glm::vec4(gunForward, 1);
        gunForward = normalize(glm::vec3(aux));
    }
}

void Tank::AimGunAt(glm::vec3 target)
{
    gunForwardTarget = normalize(target - position);
}

bool Tank::CanFire()
{
    return timeLastShot >= TANK_FIRE_INTERVAL;
}

Projectile Tank::FireProjectile()
{
    glm::vec3 projectilePosition = position + glm::vec3(0, 0.8, 0);
    projectilePosition += normalize(gunForward) * 1.5f;
    timeLastShot = 0;
    return {projectilePosition, gunForward};
}

void Tank::IncrementTime(float deltaTime)
{
    timeLastShot += deltaTime;
    timeCurrentState += deltaTime;
}

void Tank::ExecuteState(float deltaTime, glm::vec3 playerPosition)
{
    switch (state)
    {
    case State::MoveForward: MoveForward(deltaTime); break;
    case State::MoveBackward: MoveForward(-deltaTime); break;
    case State::RotateLeft: RotateHull_OY(deltaTime / 2); break;
    case State::RotateRight: RotateHull_OY(-deltaTime / 2); break;
    case State::RotateTurretLeft: RotateTurretTarget_OY(deltaTime / 2); break;
    case State::RotateTurretRight: RotateTurretTarget_OY(-deltaTime / 2); break;
    case State::Attack: AimGunAt(playerPosition); break;
    }
}

void Tank::UpdateState()
{
    if (timeCurrentState < TANK_STATE_INTERVAL || state == State::Attack) return;

    state = GetRandomState();
    timeCurrentState = 0;
}

void Tank::SetAttackState()
{
    state = State::Attack;
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
    // -1 because attack state is ignored, only set based on distance
    const int random = rand() % (STATES.size() - 1);
    return STATES[random];
}
