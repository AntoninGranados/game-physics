#pragma once
#include "../Scene.h"

#include "MassSpringSystem.h"

#include <random>
#include <vector>

class Renderer;
class Colormap;

namespace ex1 {

class Scene1 : public Scene {
public:
    virtual void init() override;
    virtual void onDraw(Renderer &renderer) override;
    virtual void onGUI() override;
    virtual void simulateStep() override;

private:
    void initScene();

    MassSpringSystem system;

    void printPoint(const std::string& name, const Point& p);
};

} // namespace ex1
