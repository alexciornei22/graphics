#include "object2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object2D::CreateSquare(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object2D::CreateRect(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    float width,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, width, 0), color),
        VertexFormat(corner + glm::vec3(0, width, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object2D::CreateShooter(
    const std::string& name,
    glm::vec3 center,
    float scale,
    glm::vec3 color)
{
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(center + glm::vec3(0, 6, 0) * scale, color),
        VertexFormat(center + glm::vec3(-3, 0, 0) * scale, color),
        VertexFormat(center + glm::vec3(3, 0, 0) * scale, color),
        VertexFormat(center + glm::vec3(0, -6, 0) * scale, color),
        VertexFormat(center + glm::vec3(2.5, 1, 0) * scale, color),
        VertexFormat(center + glm::vec3(7, 1, 0) * scale, color),
        VertexFormat(center + glm::vec3(7, -1, 0) * scale, color),
        VertexFormat(center + glm::vec3(2.5, -1, 0) * scale, color),
    };

    std::vector<unsigned int> indices = {
        0, 1, 2,
        1, 3, 2,
        4, 6, 5,
        4, 7, 6
    };

    Mesh* shooter = new Mesh(name);
    shooter->InitFromData(vertices, indices);
    return shooter;
}

Mesh* object2D::CreateStar(
    const std::string& name,
    glm::vec3 center,
    float scale,
    glm::vec3 color)
{
    float circumradius = scale;
    float inradius = scale / 3;

    std::vector<VertexFormat> vertices;

    for (int n = 0; n < 5; n++) {
        float x = circumradius * glm::cos(glm::radians(90.0f + 72 * n));
        float y = circumradius * glm::sin(glm::radians(90.0f + 72 * n));
        vertices.push_back(
            VertexFormat(center + glm::vec3(x, y, 0), color)
        );

        x = inradius * glm::cos(glm::radians(126.0f + 72 * n));
        y = inradius * glm::sin(glm::radians(126.0f + 72 * n));
        vertices.push_back(
            VertexFormat(center + glm::vec3(x, y, 0), color)
        );
    }

    std::vector<unsigned int> indices = {
        0, 4, 7,
        2, 5, 8,
        0, 3, 6
    };

    Mesh* star = new Mesh(name);
    star->InitFromData(vertices, indices);
    return star;
}

Mesh* object2D::CreateEnemy(
    const std::string& name,
    glm::vec3 center,
    float scale,
    glm::vec3 outerColor,
    glm::vec3 innerColor)
{
    float radius = scale;
    float innerRadius = scale * 0.6;

    std::vector<VertexFormat> vertices = { VertexFormat(center, innerColor) };

    // vertices for outer hexagon
    for (int n = 0; n < 6; n++) {
        float x = radius * glm::cos(glm::radians(30.0f + 60 * n));
        float y = radius * glm::sin(glm::radians(30.0f + 60 * n));
        vertices.push_back(
            VertexFormat(center + glm::vec3(x, y, 0), outerColor)
        );
    }

    // vertices for inner hexagon
    for (int n = 0; n < 6; n++) {
        float x = innerRadius * glm::cos(glm::radians(30.0f + 60 * n));
        float y = innerRadius * glm::sin(glm::radians(30.0f + 60 * n));
        vertices.push_back(
            VertexFormat(center + glm::vec3(x, y, 1), innerColor)
        );
    }

    std::vector<unsigned int> indices = {
        // outer hexagon
        0, 1, 2,
        0, 2, 3,
        0, 3, 4,
        0, 4, 5,
        0, 5, 6,
        0, 6, 1,

        // inner hexagon
        0, 7, 8,
        0, 8, 9,
        0, 9, 10,
        0, 10, 11,
        0, 11, 12,
        0, 12, 7
    };

    Mesh* star = new Mesh(name);
    star->InitFromData(vertices, indices);
    return star;
}
