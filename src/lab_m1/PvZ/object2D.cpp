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
    float circumradius = scale / 2;
    float inradius = circumradius / 3;

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

Mesh* object2D::CreateHexagon(
    const std::string& name,
    glm::vec3 center,
    float scale, glm::vec3 color)
{
    float radius = scale / 2;

    std::vector<VertexFormat> vertices = { VertexFormat(center, color)};

    for (int n = 0; n < 6; n++) {
        float x = radius * glm::cos(glm::radians(30.0f + 60 * n));
        float y = radius * glm::sin(glm::radians(30.0f + 60 * n));
        vertices.push_back(
            VertexFormat(center + glm::vec3(x, y, 0), color)
        );
    }

    std::vector<unsigned int> indices = {
        0, 1, 2,
        0, 2, 3,
        0, 3, 4,
        0, 4, 5,
        0, 5, 6,
        0, 6, 1,
    };

    Mesh* hexagon = new Mesh(name);
    hexagon->InitFromData(vertices, indices);
    return hexagon;
}

Mesh* object2D::CreateHeart(
    const std::string& name,
    glm::vec3 center,
    float length,
    int circlePoints,
    glm::vec3 color)
{
    float radius = length / 4;
    float angle = 360.f / circlePoints;
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    // first circle
    for (int i = 0; i < circlePoints; i++) {
        float x = radius * glm::cos(glm::radians(angle * i));
        float y = radius * glm::sin(glm::radians(angle * i));
             
        vertices.push_back(
            VertexFormat(center + glm::vec3(x - radius, y + radius, 0), color)
        );

        if (i > 1) {
            indices.push_back(0);
            indices.push_back(i - 1);
            indices.push_back(i);
        }
    }
    // second circle
    for (int i = 0; i < circlePoints; i++) {
        float x = radius * glm::cos(glm::radians(angle * i));
        float y = radius * glm::sin(glm::radians(angle * i));

        vertices.push_back(
            VertexFormat(center + glm::vec3(radius + x, y + radius, 0), color)
        );

        if (i > 1) {
            indices.push_back(circlePoints);
            indices.push_back(circlePoints + i - 1);
            indices.push_back(circlePoints + i);
        }
    }

    // lower tip of the heart
    vertices.push_back(VertexFormat(center + glm::vec3(0, -2 * radius, 0), color));

    int half = circlePoints / 2;
    int delta = circlePoints * 0.6 - half;

    // indices for lower triangle between tip and circles
    indices.push_back(vertices.size() - 1);
    indices.push_back(2 * circlePoints - delta);
    indices.push_back(half + delta);
    // indices for upper triangle to fill space between circle intersection
    // and lower triangle
    indices.push_back(0);
    indices.push_back(2 * circlePoints - delta);
    indices.push_back(half + delta);

    Mesh* heart = new Mesh(name);
    heart->InitFromData(vertices, indices);
    return heart;
}
