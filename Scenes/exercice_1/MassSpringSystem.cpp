#include "MassSpringSystem.h"

namespace ex1 {

void MassSpringSystem::computeForces(std::vector<Point>& state) const {
    for (Point& p : state) p.force = p.mass * gravity - damping * p.vel;

    for (const Spring& s : springs) {
        glm::vec3 d = state[s.p0].pos - state[s.p1].pos;
        float l = glm::length(d);
        if (l < 1e-6f) continue;
        d /= l;

        glm::vec3 F = -s.stiffness * (l - s.restLength) * d;

        state[s.p0].force += F;
        state[s.p1].force -= F;
    }
}

void MassSpringSystem::eulerStep(float dt) {
    computeForces(points);

    for (Point& p : points) {
        if (p.fixed) continue;
        p.pos += dt * p.vel;
        p.vel += dt / p.mass * p.force;
    }
}

void MassSpringSystem::midpointStep(float dt) {
    computeForces(points);

    std::vector<Point> intermediatePoints = points;
    for (Point& p : intermediatePoints) {
        if (p.fixed) continue;
        p.pos += dt * 0.5f * p.vel;
        p.vel += dt * 0.5f / p.mass * p.force;
    }

    computeForces(intermediatePoints);

    for (size_t i = 0; i < points.size(); i++) {
        if (points[i].fixed) continue;
        points[i].pos += dt * intermediatePoints[i].vel;
        points[i].vel += dt / points[i].mass * intermediatePoints[i].force;
    }
}

void MassSpringSystem::leapFrogStep(float dt) {
    computeForces(points);

    for (Point& p : points) {
        if (p.fixed) continue;
        p.vel += dt / p.mass * p.force;
        p.pos += dt * p.vel;
    }
}

} // namespace ex1
