#include "Scene1.h"
#include <imgui.h>
#include <glm/gtx/quaternion.hpp>

void Scene1::drawEmitter(Renderer& renderer) {

    // compute the emitter's local axes
    glm::quat e_rotation = glm::quat(glm::vec3(pitch, roll, yaw));
    glm::vec3 e_forward = glm::vec3(glm::toMat4(e_rotation) * glm::vec4(0, 0, 1, 0));
    glm::vec3 e_right = glm::vec3(glm::toMat4(e_rotation) * glm::vec4(1, 0, 0, 0));
    glm::vec3 e_up = glm::vec3(glm::toMat4(e_rotation) * glm::vec4(0, 1, 0, 0));

    // draw the emitter
    // renderer.drawCube(glm::vec3(0, 0, 0), e_rotation, glm::vec3(0.3, 0.3, 0.5), glm::vec4(0.1, 0.1, 0.1, 1));
    // renderer.drawCube(e_forward * 0.25f, e_rotation, glm::vec3(0.5, 0.5, 0.1), glm::vec4(0.1, 0.1, 0.1, 1));
    // renderer.drawCube(e_forward * 0.2f, e_rotation, glm::vec3(0.35, 0.35, 0.1), glm::vec4(0.7, 0.1, 0.1, 1));
    renderer.drawCube(e_right * 0.15f, e_rotation, glm::vec3(0.05, 0.3, 0.5), glm::vec4(0.1, 0.1, 0.1, 1));
    renderer.drawCube(e_right * -0.15f, e_rotation, glm::vec3(0.05, 0.3, 0.5), glm::vec4(0.1, 0.1, 0.1, 1));
    renderer.drawCube(e_up * 0.15f, e_rotation, glm::vec3(0.3, 0.05, 0.5), glm::vec4(0.1, 0.1, 0.1, 1));
    renderer.drawCube(e_up * -0.15f, e_rotation, glm::vec3(0.3, 0.05, 0.5), glm::vec4(0.1, 0.1, 0.1, 1));
    renderer.drawCube(e_forward * -0.25f, e_rotation, glm::vec3(0.3, 0.3, 0.05), glm::vec4(0.1, 0.1, 0.1, 1));
    renderer.drawCube(e_forward * 0.2f + e_right * 0.15f, e_rotation, glm::vec3(0.1, 0.35, 0.1), glm::vec4(0.7, 0.1, 0.1, 1));
    renderer.drawCube(e_forward * 0.2f + e_right * -0.15f, e_rotation, glm::vec3(0.1, 0.35, 0.1), glm::vec4(0.7, 0.1, 0.1, 1));
    renderer.drawCube(e_forward * 0.2f + e_up * 0.15f, e_rotation, glm::vec3(0.35, 0.1, 0.1), glm::vec4(0.7, 0.1, 0.1, 1));
    renderer.drawCube(e_forward * 0.2f + e_up * -0.15f, e_rotation, glm::vec3(0.35, 0.1, 0.1), glm::vec4(0.7, 0.1, 0.1, 1));
    renderer.drawCube(e_forward * 0.25f + e_right * 0.2f, e_rotation, glm::vec3(0.2, 0.5, 0.05), glm::vec4(0.1, 0.1, 0.1, 1));
    renderer.drawCube(e_forward * 0.25f + e_right * -0.2f, e_rotation, glm::vec3(0.2, 0.5, 0.05), glm::vec4(0.1, 0.1, 0.1, 1));
    renderer.drawCube(e_forward * 0.25f + e_up * 0.2f, e_rotation, glm::vec3(0.5, 0.2, 0.05), glm::vec4(0.1, 0.1, 0.1, 1));
    renderer.drawCube(e_forward * 0.25f + e_up * -0.2f, e_rotation, glm::vec3(0.5, 0.2, 0.05), glm::vec4(0.1, 0.1, 0.1, 1));

    // draw the emitter's local axes
    renderer.drawLine(glm::vec3(0), e_forward, glm::vec4(1, 0, 0, 1));
    renderer.drawLine(glm::vec3(0), e_right, glm::vec4(0, 1, 0, 1));
    renderer.drawLine(glm::vec3(0), e_up, glm::vec4(0, 0, 1, 1));
}

void Scene1::launchSphere() {

    // only launch if enough time has passed since the last launch
    if (lastLaunch < launch_delay)
        return;
    lastLaunch = 0;

    // compute the new particle's position, velocity and color
    glm::mat4 rotation = glm::toMat4(glm::quat(glm::vec3(pitch, roll, yaw)));
    glm::vec3 forward = glm::vec3(rotation * glm::vec4(0, 0, 1, 0));
    glm::vec3 right = glm::vec3(rotation * glm::vec4(1, 0, 0, 0));
    glm::vec3 up = glm::vec3(rotation * glm::vec4(0, 1, 0, 0));
    glm::vec4 color = glm::vec4(dis(gen), dis(gen), dis(gen), 1);
    float velocityMagnitude = 4.5f + dis(gen);
    glm::vec3 velocity = forward * velocityMagnitude;
    velocity += right * (dis(gen) - 0.5f) * 2.0f;
    velocity += up * (dis(gen) - 0.5f) * 2.0f;

    // add the new particle to the list
    particles.push_back(Particle{glm::vec3(0), velocity, color, 0.0});
}

void Scene1::simulateStep() {

    float realtimeDt = ImGui::GetIO().DeltaTime;

    // update particles properties
    for (auto& particle : particles) {
        particle.position += particle.velocity * realtimeDt;
        particle.lifetime += realtimeDt;
        particle.velocity += gravityAccel * realtimeDt;
    }

    // erase particles after their lifetime exceeds the maximum lifetime
    particles.erase(std::remove_if(particles.begin(), particles.end(), [&](const Particle& particle) {
        return particle.lifetime > maxLifetime;
    }), particles.end());

    // handle keyboard input
    if (ImGui::IsKeyDown(ImGuiKey_Space))
        launchSphere();
    if (ImGui::IsKeyDown(ImGuiKey_W))
        pitch += pitch_increment;
    if (ImGui::IsKeyDown(ImGuiKey_S))
        pitch -= pitch_increment;
    if (ImGui::IsKeyDown(ImGuiKey_A))
        roll += roll_increment;
    if (ImGui::IsKeyDown(ImGuiKey_D))
        roll -= roll_increment;
    if (ImGui::IsKeyDown(ImGuiKey_Q))
        yaw += yaw_increment;
    if (ImGui::IsKeyDown(ImGuiKey_E))
        yaw -= yaw_increment;

    // handle mouse input
    if (ImGui::IsMouseDown(ImGuiMouseButton_Right)) {
        auto drag = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);
        if (drag.x != 0.0 || drag.y != 0.0) {
            auto dx = drag.x * right * mouseStrength;
            auto dy = -drag.y * up * mouseStrength;
            for (auto& particle : particles) {
                particle.velocity += (dx + dy) * realtimeDt;
            }
        }
    }

    lastLaunch++;
}

void Scene1::onDraw(Renderer& renderer) {

    // update the camera matrix and the axes based on the camera position
    cameraMatrix = renderer.camera.viewMatrix;
    fwd = inverse(cameraMatrix) * glm::vec4(0, 0, 1, 0);
    right = inverse(cameraMatrix) * glm::vec4(1, 0, 0, 0);
    up = inverse(cameraMatrix) * glm::vec4(0, 1, 0, 0);

    // draw the white box and the emitter
    renderer.drawWireCube(glm::vec3(0), glm::vec3(5), glm::vec3(1));
    drawEmitter(renderer);

    // draw every particle
    auto cmap = Colormap("viridis");
    for (auto& particle : particles) {
        // renderer.drawSphere(particle.position, 0.1f, glm::vec4(cmap(particle.lifetime / 1.5f), 1));
        renderer.drawEllipsoid(
            particle.position,
            glm::quat(glm::vec3(0)),
            glm::vec3(0.1f) + abs(particle.velocity) / 100.0f,
            glm::vec4(cmap(particle.lifetime / 1.5f),
            1
        ));
    }
}

void Scene1::onGUI() {

    // sliders to control the emitter's movement
    ImGui::SliderFloat("Pitch Increment", &pitch_increment, -0.01f, 0.01f);
    ImGui::SliderFloat("Roll Increment", &roll_increment, -0.01f, 0.01f);
    ImGui::SliderFloat("Yaw Increment", &yaw_increment, -0.01f, 0.01f);
    ImGui::SliderFloat("Mouse Strength", &mouseStrength, 0.01f, 0.1f);
    ImGui::SliderFloat("Max Lifetime", &maxLifetime, 1.0f, 10.0f);

    // button to launch a particle
    auto launch = ImGui::Button("Launch");
    if (launch)
        launchSphere();
}