#include "Scene2.h"

#include <glm/glm.hpp>

#include <imgui.h>

namespace ex1 {

void Scene2::init() {
    appTime = physicsTime = 0.0f;

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

void Scene2::onDraw(Renderer& renderer) {
    renderer.drawWireCube(glm::vec3(0), glm::vec3(5), glm::vec3(1));

    for (const Point& p : system.points) {
        renderer.drawSphere(p.pos, 0.1f, glm::vec4(1, 0, 1, 1));
    }

    for (const Spring& s : system.springs) {
        renderer.drawLine(system.points[s.p0].pos, system.points[s.p1].pos, glm::vec3(0, 1, 0));
    }
}

void Scene2::onGUI() {
    ImGui::SliderFloat("DeltaTime", &dt, 0.001f, 0.1f, "%.3f", ImGuiSliderFlags_Logarithmic);
}

void Scene2::simulateStep() {
    appTime += ImGui::GetIO().DeltaTime;

    while (physicsTime < appTime) {
        system.eulerStep(dt);
        physicsTime += dt;
    }
}

} // namespace ex1

