#pragma once
#include "../Scene.h"

#include "MassSpringSystem.h"

#include <vector>

class Renderer;
class Colormap;

namespace ex1 {

class Scene4 : public Scene {
public:
    virtual void init() override;
    virtual void onDraw(Renderer &renderer) override;
    virtual void onGUI() override;
    virtual void simulateStep() override;

private:
    enum Method { Euler, Midpoint, LeapFrog };

    float appTime = 0.0f, physicsTime = 0.0f;
    float dt = 0.002f;
    int method = Midpoint;
    float stiffness = 400.0f;

    const float boxHalfSize = 2.5f;
    glm::vec3 spherePos = glm::vec3(0);
    float sphereRadius = 1.0f;
    float restitution = 0.0f;
    float friction = 0.0f;

    MassSpringSystem system;

    void handleCollisions();
    void resolveCollision(Point& p, const glm::vec3& normal, float penetration);
};

} // namespace ex1
