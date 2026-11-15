#include "Scene1.h"

#include <iostream>

#include <glm/glm.hpp>

#include <imgui.h>

namespace ex1 {

void Scene1::init() {
    float dt = 0.1f;

    initScene();
    printPoint("Masspoint 0", system.points[0]);
    printPoint("Masspoint 1", system.points[1]);
    std::cout << std::endl;

    initScene();
    system.midpointStep(dt);
    std::cout << "After " << dt << "s using Midpoint Method:" << std::endl;
    printPoint("Masspoint 0", system.points[0]);
    printPoint("Masspoint 1", system.points[1]);
    std::cout << std::endl;

    initScene();
    system.eulerStep(dt);
    std::cout << "After " << dt << "s using Euler Method:" << std::endl;
    printPoint("Masspoint 0", system.points[0]);
    printPoint("Masspoint 1", system.points[1]);

}

void Scene1::onDraw(Renderer& renderer) {
    renderer.drawWireCube(glm::vec3(0), glm::vec3(5), glm::vec3(1));
}

void Scene1::onGUI() {

}

void Scene1::simulateStep() {

}

void Scene1::initScene() {
    system.points.clear();
    system.springs.clear();

    system.points.push_back(Point{
        .pos = glm::vec3( 0, 0, 0),
        .vel = glm::vec3(-1, 0, 0),
        .force = glm::vec3(0),
        .mass = 10.0f,
    });

    system.points.push_back(Point{
        .pos = glm::vec3( 0, 2, 0),
        .vel = glm::vec3( 1, 0, 0),
        .force = glm::vec3(0),
        .mass = 10.0f,
    });

    system.springs.push_back(Spring{
        .p0 = 0,
        .p1 = 1,
        .stiffness = 40.0f,
        .restLength = 1.0f,
    });
}

void Scene1::printPoint(const std::string& name, const Point& p) {
    std::cout << name << std::endl;
    std::cout << "position: vec3(" << p.pos.x << ", " << p.pos.y << ", " << p.pos.z << ")" << std::endl;
    std::cout << "velocity: vec3(" << p.vel.x << ", " << p.vel.y << ", " << p.vel.z << ")" << std::endl;
}

} // namespace ex1

