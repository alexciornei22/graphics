#pragma once

#include "utils/glm_utils.h"


namespace transform2D
{
    struct CoordinateSpace
    {
        CoordinateSpace() : x(0), y(0), width(1), height(1) {}
        CoordinateSpace(float x, float y, float width, float height)
            : x(x), y(y), width(width), height(height) {}
        float x;
        float y;
        float width;
        float height;
    };

    // Translate matrix
    inline glm::mat3 Translate(float translateX, float translateY)
    {
        return glm::transpose(
            glm::mat3(1, 0, translateX,
                0, 1, translateY,
                0, 0, 1)
        );
    }

    // Scale matrix
    inline glm::mat3 Scale(float scaleX, float scaleY)
    {
        return glm::transpose(
            glm::mat3(scaleX, 0, 0,
                0, scaleY, 0,
                0, 0, 1)
        );
    }

    // Rotate matrix
    inline glm::mat3 Rotate(float radians)
    {
        return glm::transpose(
            glm::mat3(cos(radians), -sin(radians), 0,
                sin(radians), cos(radians), 0,
                0, 0, 1)
        );
    }

    // 2D visualization matrix
    inline glm::mat3 VisualizationTransf2D(const CoordinateSpace& logicSpace, const CoordinateSpace& viewSpace)
    {
        float sx, sy, tx, ty;
        sx = viewSpace.width / logicSpace.width;
        sy = viewSpace.height / logicSpace.height;
        tx = viewSpace.x - sx * logicSpace.x;
        ty = viewSpace.y - sy * logicSpace.y;

        return glm::transpose(glm::mat3(
            sx, 0.0f, tx,
            0.0f, sy, ty,
            0.0f, 0.0f, 1.0f));
    }

    // 2D visualization matrix for mouse
    inline void updateMouseCoordinates(
        const CoordinateSpace& logicSpace,
        const CoordinateSpace& viewSpace,
        glm::vec3& mouseCoordinates,
        int mouseX,
        int mouseY
    )
    {
        mouseY = viewSpace.height - mouseY;
        mouseCoordinates = glm::vec3(mouseX, mouseY, 0);
        mouseCoordinates = transform2D::VisualizationTransf2D(viewSpace, logicSpace) * mouseCoordinates;
    }

    // Uniform 2D visualization matrix (same scale factor on x and y axes)
    inline glm::mat3 VisualizationTransf2DUnif(const CoordinateSpace& logicSpace, const CoordinateSpace& viewSpace)
    {
        float sx, sy, tx, ty, smin;
        sx = viewSpace.width / logicSpace.width;
        sy = viewSpace.height / logicSpace.height;
        if (sx < sy)
            smin = sx;
        else
            smin = sy;
        tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
        ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

        return glm::transpose(glm::mat3(
            smin, 0.0f, tx,
            0.0f, smin, ty,
            0.0f, 0.0f, 1.0f));
    }
}   // namespace transform2D
