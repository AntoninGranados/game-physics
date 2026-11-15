#pragma once

#include <glm/glm.hpp>

#include <vector>

namespace ex1 {

struct Point {
    glm::vec3 pos, vel, force;
    float mass;
    bool fixed = false;
};

struct Spring {
    size_t p0;
    size_t p1;
    float stiffness;
    float restLength;
};

struct MassSpringSystem {
    std::vector<Point> points;
    std::vector<Spring> springs;

    glm::vec3 gravity = glm::vec3(0);
    float damping = 0.0f;

    void eulerStep(float dt);
    void midpointStep(float dt);
    void leapFrogStep(float dt);

private:
    void computeForces(std::vector<Point>& state) const;
};

} // namespace ex1
