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
