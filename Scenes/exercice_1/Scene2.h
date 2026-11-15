#pragma once
#include "../Scene.h"

#include "MassSpringSystem.h"

#include <random>
#include <vector>

class Renderer;
class Colormap;

namespace ex1 {

class Scene2 : public Scene {
public:
    virtual void init() override;
    virtual void onDraw(Renderer &renderer) override;
    virtual void onGUI() override;
    virtual void simulateStep() override;

private:
    float appTime = 0.0f, physicsTime = 0.0f;
    float dt = 0.005f;

    MassSpringSystem system;
};

} // namespace ex1
