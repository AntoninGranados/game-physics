#include "Scene4.h"

#include <glm/glm.hpp>

#include <imgui.h>

namespace ex1 {

void Scene4::init() {
    appTime = physicsTime = 0.0f;

    system.points.clear();
    system.springs.clear();
    system.gravity = glm::vec3(0, 0, -9.81f);
    system.damping = 0.5f;

    const int n = 20;
    const float spacing = 4.0 / n;
    const float totalMass = 64.0f;

    for (int j = 0; j < n; j++) {
        for (int i = 0; i < n; i++) {
            system.points.push_back(Point{
                .pos = glm::vec3((i - (n - 1) / 2.0f) * spacing, (j - (n - 1) / 2.0f) * spacing, 2.0f),
                .vel = glm::vec3(0),
                .force = glm::vec3(0),
                .mass = totalMass / (n * n),
            });
        }
    }

    system.points[0].fixed = true;
    system.points[n - 1].fixed = true;

    auto addSpring = [&](size_t p0, size_t p1) {
        system.springs.push_back(Spring{
            .p0 = p0,
            .p1 = p1,
            .stiffness = stiffness,
            .restLength = glm::length(system.points[p0].pos - system.points[p1].pos),
        });
    };

    for (int j = 0; j < n; j++) {
        for (int i = 0; i < n; i++) {
            size_t p = j * n + i;

            if (i + 1 < n) addSpring(p, p + 1);
            if (j + 1 < n) addSpring(p, p + n);
            if (i + 1 < n && j + 1 < n) {
                addSpring(p, p + n + 1);
                addSpring(p + 1, p + n);
            }
        }
    }
}

void Scene4::onDraw(Renderer& renderer) {
    renderer.drawWireCube(glm::vec3(0), glm::vec3(2 * boxHalfSize), glm::vec3(1));
    renderer.drawSphere(spherePos, sphereRadius, glm::vec4(0.2, 0.6, 1, 1));

    for (const Point& p : system.points) {
        renderer.drawSphere(p.pos, 0.02f, p.fixed ? glm::vec4(1, 0.2, 0.2, 1) : glm::vec4(0.6, 1, 0.2, 1));
    }

    for (const Spring& s : system.springs) {
        renderer.drawLine(system.points[s.p0].pos, system.points[s.p1].pos, glm::vec3(0.6, 1, 0.2));
    }
}

void Scene4::onGUI() {
    ImGui::SliderFloat("DeltaTime", &dt, 0.001f, 0.1f, "%.3f", ImGuiSliderFlags_Logarithmic);
    ImGui::Combo("Method", &method, "Euler\0Midpoint\0Leap-Frog\0");
    ImGui::SliderFloat3("Gravity", &system.gravity.x, -20.0f, 20.0f);

    ImGui::SeparatorText("Physics");
    if (ImGui::SliderFloat("Stiffness", &stiffness, 10.0f, 2000.0f, "%.0f", ImGuiSliderFlags_Logarithmic)) {
        for (Spring& s : system.springs) s.stiffness = stiffness;
    }
    ImGui::SliderFloat("Damping", &system.damping, 0.0f, 5.0f);

    ImGui::SeparatorText("Collision");
    ImGui::SliderFloat("Restitution", &restitution, 0.0f, 1.0f);
    ImGui::SliderFloat("Friction", &friction, 0.0f, 1.0f);
}

void Scene4::simulateStep() {
    appTime += ImGui::GetIO().DeltaTime;

    if (ImGui::IsMouseDown(ImGuiMouseButton_Right) && !ImGui::GetIO().WantCaptureMouse) {
        ImVec2 delta = ImGui::GetIO().MouseDelta;
        spherePos += 0.01f * (delta.x * Renderer::camera.right() - delta.y * Renderer::camera.up());

        float limit = boxHalfSize - sphereRadius;
        spherePos = glm::clamp(spherePos, glm::vec3(-limit), glm::vec3(limit));
    }

    while (physicsTime < appTime) {
        switch (method) {
            case Euler: system.eulerStep(dt); break;
            case Midpoint: system.midpointStep(dt); break;
            case LeapFrog: system.leapFrogStep(dt); break;
        }
        handleCollisions();
        physicsTime += dt;
    }
}

void Scene4::handleCollisions() {
    for (Point& p : system.points) {
        if (p.fixed) continue;

        for (int axis = 0; axis < 3; axis++) {
            for (float side : {-1.0f, 1.0f}) {
                float penetration = side * p.pos[axis] - boxHalfSize;
                if (penetration <= 0) continue;

                glm::vec3 normal = glm::vec3(0);
                normal[axis] = -side;
                resolveCollision(p, normal, penetration);
            }
        }

        glm::vec3 d = p.pos - spherePos;
        float l = glm::length(d);
        if (l < sphereRadius && l > 1e-6f) {
            resolveCollision(p, d / l, sphereRadius - l);
        }
    }
}

void Scene4::resolveCollision(Point& p, const glm::vec3& normal, float penetration) {
    p.pos += penetration * normal;

    float vn = glm::dot(p.vel, normal);
    if (vn >= 0) return;

    glm::vec3 vt = p.vel - vn * normal;
    float vtLength = glm::length(vt);
    if (vtLength > 1e-6f) {
        vt *= glm::max(1.0f - friction * (1.0f + restitution) * -vn / vtLength, 0.0f);
    }

    p.vel = vt - restitution * vn * normal;
}

} // namespace ex1
