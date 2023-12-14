#include "Projectile.h"

#include <glm/geometric.hpp>

void tank::Projectile::TranslateForward()
{
    glm::vec3 const dir = normalize(forward);
    position += dir * speed;
}

void tank::Projectile::IncrementTime(float deltaTime)
{
    elapsedShotTime += deltaTime;
}
