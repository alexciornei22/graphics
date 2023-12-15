#include "WorldOfTanks.h"

using namespace m1;

void WorldOfTanks::InitTankMeshes()
{
    for (auto const type : tank::TYPES)
    {
        auto name = GetTypeString(type);
        {
            Mesh* mesh = new Mesh(name + "_hull");
            mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tanks/" + name), "hull.obj");
            meshes[mesh->GetMeshID()] = mesh;
        }
        {
            Mesh* mesh = new Mesh(name + "_turret");
            mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tanks/" + name), "turret.obj");
            meshes[mesh->GetMeshID()] = mesh;
        }
        {
            Mesh* mesh = new Mesh(name + "_gun");
            mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tanks/" + name), "gun.obj");
            meshes[mesh->GetMeshID()] = mesh;
        }
        {
            Mesh* mesh = new Mesh(name + "_tracks");
            mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "tanks/" + name), "tracks.obj");
            meshes[mesh->GetMeshID()] = mesh;
        }
    }
}

void WorldOfTanks::InitBuildings()
{
    int const nrBuildings = rand() % 5 + 5;

    for (int i = 0; i < nrBuildings; i++)
    {
        int positionX = rand() % 25;
        positionX *= (rand() % 2) ? -1 : 1;
        int positionZ = rand() % 25;
        positionZ *= (rand() % 2) ? -1 : 1;
        glm::vec3 position = glm::vec3(positionX, 0, positionZ);

        int length = rand() % 3 + 3;
        int width = rand() % 3 + 3;
        int height = rand() % 2 + 2;

        buildings.emplace_back(position, length, width, height);
    }
}

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

void WorldOfTanks::DetectTankTankCollisions()
{
    for (auto &tank : enemyTanks)
    {
        DetectTanksCollision(tank, *playerTank);

        for (auto &otherTank : enemyTanks)
        {
            DetectTanksCollision(tank, otherTank);
        }
    }
}

void WorldOfTanks::DetectTanksCollision(tank::Tank& tank1, tank::Tank& tank2)
{
    if (&tank1 == &tank2) return;
    
    float const distance = glm::distance(tank1.position, tank2.position);
    if (distance < tank::TANK_COLLISION_SPHERE_RADIUS * 2)
    {
        glm::vec3 const direction = normalize(tank1.position - tank2.position);
        float const overlap = tank::TANK_COLLISION_SPHERE_RADIUS * 2 - distance;
        
        tank1.TranslateByDirection(overlap / 2, direction);
        tank2.TranslateByDirection(-overlap / 2, direction);
    }
}

void WorldOfTanks::DetectTanksBuildingsCollisions(float deltaTime)
{
    for (auto &building : buildings)
    {
        DetectTankBuildingCollision(*playerTank, building, deltaTime);

        for (auto &tank : enemyTanks)
        {
            DetectTankBuildingCollision(tank, building, deltaTime);
        }
    }
}

void WorldOfTanks::DetectTankBuildingCollision(tank::Tank& tank, Building& building, float deltaTime)
{
    glm::vec3 const buildingPosition = building.GetPosition();
    float const length = building.GetLength();
    float const width = building.GetWidth();
    
    if (
        abs(tank.position.x - buildingPosition.x) < tank::TANK_COLLISION_SPHERE_RADIUS + length / 2.f
        && abs(tank.position.z - buildingPosition.z) < tank::TANK_COLLISION_SPHERE_RADIUS + width / 2.f
        )
    {
        glm::vec3 const direction = normalize(tank.position - buildingPosition);
        tank.TranslateByDirection(deltaTime, direction);
    }
}

void WorldOfTanks::DetectProjectilesBuildingsCollisions()
{
    auto iterator = projectiles.begin();
    while (iterator != projectiles.end())
    {
        auto projectile = *iterator;
        for (auto &building : buildings)
        {
            glm::vec3 const buildingPosition = building.GetPosition();
            float const length = building.GetLength();
            float const width = building.GetWidth();
            
            if (
                abs(projectile.position.x - buildingPosition.x) < length / 2.f
                && abs(projectile.position.z - buildingPosition.z) < width / 2.f
                )
            {
                iterator = projectiles.erase(iterator);
                break;
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
