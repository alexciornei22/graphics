#include "ThirdPersonCamera.h"

ThirdPersonCamera::ThirdPersonCamera(const glm::vec3& position, const glm::vec3& center, const glm::vec3& up)
{
    this->position = position;
    forward = normalize(center - position);
    right = cross(forward, up);
    this->up = cross(right, forward);
}

void ThirdPersonCamera::MoveTargetForward(float distance)
{
    target->MoveForward(distance);
}


void ThirdPersonCamera::TranslateForward(float distance)
{
    // TODO(student): Translate the camera using the `forward` vector.
    // What's the difference between `TranslateForward()` and
    // `MoveForward()`?
    glm::vec3 dir = normalize(forward);
    position += dir * distance;
}

void ThirdPersonCamera::TranslateUpward(float distance)
{
    // TODO(student): Translate the camera using the `up` vector.
    glm::vec3 dir = normalize(up);
    position += dir * distance;
}

void ThirdPersonCamera::TranslateRight(float distance)
{
    glm::vec3 dir = normalize(right);
    position += dir * distance;
}

void ThirdPersonCamera::RotateFirstPerson_OX(float angle)
{
    glm::vec4 aux = rotate(glm::mat4(1.f), -angle, right) * glm::vec4(forward, 1);
    forward = normalize(glm::vec3(aux));

    up = cross(right, forward);
}

void ThirdPersonCamera::RotateFirstPerson_OY(float angle)
{
    // TODO(student): Compute the new `forward`, `up` and `right`
    // vectors. Use `glm::rotate()`. Don't forget to normalize the
    // vectors!
    glm::vec4 aux = rotate(glm::mat4(1.f), -angle, glm::vec3(0, 1, 0)) * glm::vec4(forward, 1);
    forward = normalize(glm::vec3(aux));

    aux = rotate(glm::mat4(1.f), -angle, glm::vec3(0, 1, 0)) * glm::vec4(right, 1);
    right = normalize(glm::vec3(aux));

    up = cross(right, forward);
}

void ThirdPersonCamera::Rotate_OX(float angle)
{
    // TODO(student): Rotate the camera in third-person mode around
    // the OX axis. Use `distanceToTarget` as translation distance.
    TranslateForward(distanceToTarget);
    RotateFirstPerson_OX(angle);
    TranslateForward(-distanceToTarget);
    if (position.y < 0)
    {
        TranslateForward(distanceToTarget);
        RotateFirstPerson_OX(-angle);
        TranslateForward(-distanceToTarget);
    }
}

void ThirdPersonCamera::Rotate_OY(float angle)
{
    // TODO(student): Rotate the camera in third-person mode around
    // the OY axis.
    TranslateForward(distanceToTarget);
    RotateFirstPerson_OY(angle);
    TranslateForward(-distanceToTarget);
    target->RotateTurretTarget_OY(angle);
}

void ThirdPersonCamera::RotateTarget_OY(float angle)
{
    target->RotateHullTarget_OY(angle);
}

glm::mat4 ThirdPersonCamera::GetViewMatrix()
{
    // Returns the view matrix
    return lookAt(position, position + forward, up);
}

glm::vec3 ThirdPersonCamera::GetTargetPosition()
{
    return position + forward * distanceToTarget;
}

void ThirdPersonCamera::SetTarget(tank::Tank* target)
{
    this->target = target;
}

void ThirdPersonCamera::SetCameraToTarget()
{
    position = target->position + glm::vec3(0, 1, 0);
    position -= forward * distanceToTarget;
}
