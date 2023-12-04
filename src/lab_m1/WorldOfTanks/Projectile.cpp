#include "Projectile.h"

#include <iostream>
#include <ostream>
#include <glm/geometric.hpp>

void tank::Projectile::TranslateForward()
{
    glm::vec3 const dir = normalize(forward);
    position += dir * speed;
}
