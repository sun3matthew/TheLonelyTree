#pragma once
#include <glm/glm.hpp>

struct AABB {
    glm::vec2 minPoint;  // Bottom-left
    glm::vec2 maxPoint;  // Top-right

    AABB(const glm::vec2& minPoint = glm::vec2(0.0f), const glm::vec2& maxPoint = glm::vec2(0.0f))
        : minPoint(minPoint), maxPoint(maxPoint) {}

    AABB(float minX, float minY, float maxX, float maxY)
        : minPoint(minX, minY), maxPoint(maxX, maxY) {}

    AABB(const glm::vec2& center, float width, float height)
        : minPoint(center - glm::vec2(width, height) * 0.5f),
          maxPoint(center + glm::vec2(width, height) * 0.5f) {}

    glm::vec2 getCenter() const {
        return (minPoint + maxPoint) * 0.5f;
    }

    glm::vec2 getSize() const {
        return maxPoint - minPoint;
    }

    bool contains(const glm::vec2& point) const {
        return (point.x >= minPoint.x && point.x <= maxPoint.x &&
                point.y >= minPoint.y && point.y <= maxPoint.y);
    }

    bool intersects(const AABB& other) const {
        return !(other.minPoint.x > maxPoint.x || other.maxPoint.x < minPoint.x ||
                 other.minPoint.y > maxPoint.y || other.maxPoint.y < minPoint.y);
    }

    void expand(float amount) {
        minPoint -= glm::vec2(amount);
        maxPoint += glm::vec2(amount);
    }

    void move(const glm::vec2& delta) {
        minPoint += delta;
        maxPoint += delta;
    }

    void setCenter(const glm::vec2& center) {
        glm::vec2 size = getSize();
        minPoint = center - size * 0.5f;
        maxPoint = center + size * 0.5f;
    }

    void setSize(const glm::vec2& size) {
        glm::vec2 center = getCenter();
        minPoint = center - size * 0.5f;
        maxPoint = center + size * 0.5f;
    }
};
