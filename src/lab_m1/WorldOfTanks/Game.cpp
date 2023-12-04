#include "WorldOfTanks.h"

using namespace m1;

void WorldOfTanks::TranslateProjectiles()
{
    for (auto& projectile : projectiles)
    {
        projectile.TranslateForward();
    }
}
