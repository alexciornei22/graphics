#include "Tank.h"

#include <iostream>
#include <ostream>
#include <vector>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_transform.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "Projectile.h"

using namespace tank;

Tank::Tank(Type type, glm::vec3 position, glm::vec3 forward, int health) : initialHealth(health)
{
    this->type = type;
    this->position = position;
    this->forward = normalize(glm::vec3(forward.x, 0, forward.z));
    this->forwardTarget = this->forward;
    this->right = cross(forward, glm::vec3(0, 1, 0));
    this->gunForward = normalize(glm::vec3(forward.x, 0, forward.z));
    this->gunForwardTarget = this->gunForward;
    this->health = health;
}

void Tank::MoveForward(float distance)
{
    if (state == State::Dead) return;

    glm::vec3 dir = normalize(glm::vec3(forward.x, 0, forward.z));
    position += dir * distance * TANK_SPEED;
}

void Tank::TranslateByDirection(float distance, glm::vec3 direction)
{
    position += direction * distance;
}

void Tank::RotateHull_OY(float angle)
{
    if (state == State::Dead) return;

    if (forwardTarget == forward)
        return;

    float angleToTarget = orientedAngle(forward, forwardTarget, glm::vec3(0, 1, 0));

    if (abs(angle) > abs(angleToTarget))
    {
        forward = forwardTarget;
    }
    else
    {
        if (angleToTarget < 0) angle *= -1;
        
        glm::vec4 aux = rotate(glm::mat4(1.f), angle, glm::vec3(0, 1, 0)) * glm::vec4(forward, 1);
        forward = normalize(glm::vec3(aux));
    }
}

void Tank::RotateHullTarget_OY(float angle)
{
    if (state == State::Dead) return;

    glm::vec4 aux = rotate(glm::mat4(1.f), -angle, glm::vec3(0, 1, 0)) * glm::vec4(forwardTarget, 1);
    forwardTarget = normalize(glm::vec3(aux));
}

void Tank::RotateTurretTarget_OY(float angle)
{
    if (state == State::Dead) return;

    glm::vec4 aux = rotate(glm::mat4(1.f), -angle, glm::vec3(0, 1, 0)) * glm::vec4(gunForwardTarget, 1);
    gunForwardTarget = normalize(glm::vec3(aux));
}

void Tank::RotateTurret_OY(float angle)
{
    if (state == State::Dead) return;

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

void Tank::AimHullAt(glm::vec3 target)
{
    forwardTarget = normalize(target - position);
}

void Tank::Attack(glm::vec3 target, std::vector<Projectile>& projectiles, float deltaTime)
{
    AimGunAt(target);
    AimHullAt(target);
    
    if (distance(position, target) > 3.f && angle(forward, forwardTarget) < glm::pi<float>() / 36.f)
    {
        MoveForward(deltaTime);
    }
    
    if (CanFire() && angle(gunForward, gunForwardTarget) < glm::pi<float>() / 36.f)
    {
        projectiles.emplace_back(FireProjectile());
    }
}

bool Tank::CanFire()
{
    return timeLastShot >= TANK_FIRE_INTERVAL;
}

Projectile Tank::FireProjectile()
{
    glm::vec3 projectilePosition = position + glm::vec3(0, 0.5, 0);
    projectilePosition += normalize(gunForward) * 1.5f;
    timeLastShot = 0;
    return {projectilePosition, gunForward};
}

void Tank::IncrementTime(float deltaTime)
{
    if (state == State::Dead) return;

    timeLastShot += deltaTime;
    timeCurrentState += deltaTime;
}

float Tank::GetHealthPercentage()
{
    return (float) health / (float) initialHealth;
}

void Tank::DecreaseHealth(int damage)
{
    if (state == State::Dead) return;
    
    health -= damage;
    if (health <= 0)
        state = State::Dead;
}

void Tank::ExecuteState(float deltaTime, glm::vec3 playerPosition, std::vector<Projectile>& projectiles)
{
    switch (state)
    {
    case State::MoveForward: MoveForward(deltaTime); break;
    case State::MoveBackward: MoveForward(-deltaTime); break;
    case State::RotateLeft: RotateHullTarget_OY(deltaTime / 2); break;
    case State::RotateRight: RotateHullTarget_OY(-deltaTime / 2); break;
    case State::RotateTurretLeft: RotateTurretTarget_OY(deltaTime / 2); break;
    case State::RotateTurretRight: RotateTurretTarget_OY(-deltaTime / 2); break;
    case State::Attack: Attack(playerPosition, projectiles, deltaTime); break;
    case State::Dead: break;
    }
}

void Tank::UpdateState()
{
    if (timeCurrentState < TANK_STATE_INTERVAL) return;
    if (state == State::Attack) return;
    if (state == State::Dead) return;
    
    state = GetRandomState();
    timeCurrentState = 0;
}

void Tank::SetAttackState()
{
    if (state == State::Dead) return;

    state = State::Attack;
}

bool Tank::IsDead()
{
    return health <= 0;
}

std::string tank::GetTypeString(Type type)
{
    switch (type) {
    case TIGER_1:
        return "tiger1";
    case HELLCAT:
        return "hellcat";
    case IS_1:
        return "is1";
    case TIGER_2:
        return "tiger2";
    }
}

State tank::GetRandomState()
{
    // -2 because attack/dead is ignored
    const int random = rand() % (STATES.size() - 2);
    return STATES[random];
}
