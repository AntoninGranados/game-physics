#include "Scene0.h"

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <imgui.h>

void CameraParameters::computeParameters(const glm::mat4 &viewMatrix_) {
    viewMatrix = viewMatrix_;
    forward = inverse(viewMatrix) * glm::vec4(0, 0, 1, 0);
    right = inverse(viewMatrix) * glm::vec4(1, 0, 0, 0);
    up = inverse(viewMatrix) * glm::vec4(0, 1, 0, 0);
}

void Scene0::onDraw(Renderer &renderer) {
    camera.computeParameters(renderer.camera.viewMatrix);

    renderer.drawWireCube(glm::vec3(0), glm::vec3(cubeRadius * 2), glm::vec3(1));

    glm::vec3 position = glm::vec3(0);
    glm::quat rotation = glm::quat(glm::vec3(pitch, roll, yaw));
    renderer.drawCube(
        glm::vec3(0),
        rotation,
        glm::vec3(0.5),
        glm::vec4(1, 0, 1, 1)
    );

    glm::mat4 R = glm::toMat4(rotation);
    glm::vec3 forward = glm::vec3(R * glm::vec4(0, 0, 1, 0));
    glm::vec3 right = glm::vec3(R * glm::vec4(1, 0, 0, 0));
    glm::vec3 up = glm::vec3(R * glm::vec4(0, 1, 0, 0));

    renderer.drawLine(position, position + forward, glm::vec3(1, 0, 0));
    renderer.drawLine(position, position + right, glm::vec3(0, 1, 0));
    renderer.drawLine(position, position + up, glm::vec3(0, 0, 1));

    for (const Particle &particle : particles) {
        renderer.drawSphere(
            particle.pos,
            Particle::radius,
            glm::vec4(cmap(particle.lifetime / Particle::lifeDuration), 1)
        );
    }
}

void Scene0::onGUI() {
    ImGui::SliderFloat("Pitch Velocity", &dPitch, -10.f, 10.f);
    ImGui::SliderFloat("Yaw Velocity", &dYaw, -10.f, 10.f);
    ImGui::SliderFloat("Roll Velocity", &dRoll, -10.f, 10.f);

    ImGui::Separator();

    ImGui::SliderInt("Launch Delay", &launchDelay, 0, 100);
}

void Scene0::simulateStep() {
    const float dt = ImGui::GetIO().DeltaTime;

    // pitch += dt * dPitch;
    // yaw += dt * dYaw;
    // roll += dt * dRoll;

    glm::vec3 force = gravity;
    if (ImGui::IsMouseReleased(ImGuiMouseButton_Right)) {
        auto drag = ImGui::GetMouseDragDelta(1);
        if (drag.x != 0 || drag.y != 0) {
            glm::vec3 dx =  drag.x * camera.right;
            glm::vec3 dy = -drag.y * camera.up;
            force += dt * (dx + dy) * 100.0f;
        }
    }

    for (Particle &particle : particles) {
        particle.vel += dt * force;
        glm::vec3 newPos = particle.pos + dt * particle.vel;

        // Naive bounding box bounce
        for (int i = 0; i < 3; i++) {
            if (
                newPos[i] - Particle::radius <= -cubeRadius ||
                newPos[i] + Particle::radius >= cubeRadius)
            {
                particle.vel[i] *= -1.0f;
                particle.vel *= 0.6f;
            }
        }

        particle.pos += dt * particle.vel;
        particle.lifetime += dt;
    }

    particles.erase(std::remove_if(particles.begin(), particles.end(), [](const Particle& particle){
        return particle.lifetime > Particle::lifeDuration;
    }), particles.end());

    if(ImGui::IsKeyDown(ImGuiKey_Space))
        launchParticle();
    if(ImGui::IsKeyDown(ImGuiKey_W))
        pitch += dt * dPitch;
    if(ImGui::IsKeyDown(ImGuiKey_S))
        pitch -= dt * dPitch;
    if(ImGui::IsKeyDown(ImGuiKey_A))
        roll += dt * dRoll;
    if(ImGui::IsKeyDown(ImGuiKey_D))
        roll -= dt * dRoll;
    if(ImGui::IsKeyDown(ImGuiKey_Q))
        yaw +=  dt * dYaw;
    if(ImGui::IsKeyDown(ImGuiKey_E))
        yaw -= dt * dYaw;
    lastLaunch++;
}

void Scene0::launchParticle() {
    if (lastLaunch < launchDelay) return;
    lastLaunch = 0;

    glm::mat4 rotation = glm::toMat4(glm::quat(glm::vec3(pitch, roll, yaw)));
    glm::vec3 forward = glm::vec3(rotation * glm::vec4(0, 0, 1, 0));
    glm::vec3 right = glm::vec3(rotation * glm::vec4(1, 0, 0, 0));
    glm::vec3 up = glm::vec3(rotation * glm::vec4(0, 1, 0, 0));

    glm::vec4 color = glm::vec4(dis(gen), dis(gen), dis(gen), 1);
    float velocityMagnitude = 4.5f + dis(gen);
    glm::vec3 velocity = forward * velocityMagnitude;

    velocity += right * (dis(gen) - 0.5f) * 2.f;
    velocity += up * (dis(gen) - 0.5f) * 2.f;

    particles.push_back(Particle{glm::vec3(0), velocity, color, .0});
}

