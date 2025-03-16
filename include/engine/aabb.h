#pragma once
#include <glm/glm.hpp>

struct AABB {
    glm::vec2 min;  // Bottom-left
    glm::vec2 max;  // Top-right

    AABB(const glm::vec2& min = glm::vec2(0.0f), const glm::vec2& max = glm::vec2(0.0f))
        : min(min), max(max) {}

    AABB(float minX, float minY, float maxX, float maxY)
        : min(minX, minY), max(maxX, maxY) {}

    AABB(const glm::vec2& center, float width, float height)
        : min(center - glm::vec2(width, height) * 0.5f),
          max(center + glm::vec2(width, height) * 0.5f) {}

    glm::vec2 getCenter() const {
        return (min + max) * 0.5f;
    }

    glm::vec2 getSize() const {
        return max - min;
    }

    bool contains(const glm::vec2& point) const {
        return (point.x >= min.x && point.x <= max.x &&
                point.y >= min.y && point.y <= max.y);
    }

    bool intersects(const AABB& other) const {
        return !(other.min.x > max.x || other.max.x < min.x ||
                 other.min.y > max.y || other.max.y < min.y);
    }

    void expand(float amount) {
        min -= glm::vec2(amount);
        max += glm::vec2(amount);
    }

    void move(const glm::vec2& delta) {
        min += delta;
        max += delta;
    }

    void setCenter(const glm::vec2& center) {
        glm::vec2 size = getSize();
        min = center - size * 0.5f;
        max = center + size * 0.5f;
    }

    void setSize(const glm::vec2& size) {
        glm::vec2 center = getCenter();
        min = center - size * 0.5f;
        max = center + size * 0.5f;
    }
};
