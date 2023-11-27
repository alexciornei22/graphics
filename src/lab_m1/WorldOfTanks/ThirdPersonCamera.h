#pragma once
#include "Tank.h"
#include "utils/glm_utils.h"
#include "ThirdPersonCamera.h"

class ThirdPersonCamera
{
public:
    ThirdPersonCamera();
    ThirdPersonCamera(const glm::vec3& position, const glm::vec3& center, const glm::vec3& up);
    ~ThirdPersonCamera();

    void MoveForward(float distance);
    void TranslateForward(float distance);
    void TranslateUpward(float distance);
    void TranslateRight(float distance);

    void RotateFirstPerson_OX(float angle);
    void RotateFirstPerson_OY(float angle);
    void Rotate_OX(float angle);
    void Rotate_OY(float angle);

    void RotateTarget_OY(float angle);

    glm::mat4 GetViewMatrix();
    glm::vec3 GetTargetPosition();

    void SetTarget(tank::Tank* target);
    
    float distanceToTarget = 2;
    tank::Tank *target;
    glm::vec3 position;
    glm::vec3 forward;
    glm::vec3 right;
    glm::vec3 up;
};
