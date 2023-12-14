#include "WorldOfTanks.h"

using namespace m1;

void WorldOfTanks::IncrementTankTimes(float deltaTime)
{
    playerTank->IncrementTime(deltaTime);
    for (auto &tank : enemyTanks)
    {
        tank.IncrementTime(deltaTime);
    }
}

void WorldOfTanks::IncrementProjectileTimes(float deltaTime)
{
    for (auto &projectile : projectiles)
    {
        projectile.IncrementTime(deltaTime);
    }
}

void WorldOfTanks::UpdateTankStates()
{
    for (auto &tank : enemyTanks)
    {
        tank.UpdateState();
    }
}

void WorldOfTanks::TranslateProjectiles()
{
    for (auto& projectile : projectiles)
    {
        projectile.TranslateForward();
    }
}

void WorldOfTanks::ExecuteTankActions(float deltaTime)
{
    for (auto &tank : enemyTanks)
    {
        tank.ExecuteState(deltaTime, playerTank->position);
        tank.RotateTurret_OY(deltaTime);
    }
}

void WorldOfTanks::SetAttackStates()
{
    for (auto &tank : enemyTanks)
    {
        if (distance(playerTank->position, tank.position) < 5.f)
        {
            tank.SetAttackState();
        }
    }
}

void WorldOfTanks::DetectProjectileTankCollisions()
{
    auto iterator = projectiles.begin();
    while (iterator != projectiles.end())
    {
        auto projectile = *iterator;
        for (auto &tank : enemyTanks)
        {
            if (distance(projectile.position, tank.position) < tank::TANK_COLLISION_SPHERE_RADIUS)
            {
                tank.DecreaseHealth(projectile.damage);
                iterator = projectiles.erase(iterator);
            }
        }

        if (iterator != projectiles.end())
            ++iterator;
    }
}

void WorldOfTanks::DeleteExpiredProjectiles()
{
    auto iterator = projectiles.begin();
    while (iterator != projectiles.end())
    {
        auto projectile = *iterator;
        if (projectile.elapsedShotTime > tank::PROJECTILE_MAX_TIME)
        {
            iterator = projectiles.erase(iterator);
            continue;
        }
        ++iterator;
    }
}
