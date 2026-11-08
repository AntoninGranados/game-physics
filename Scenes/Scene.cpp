#include "Scene.h"
#include <imgui.h>

void Scene::onDraw(Renderer &renderer)
{
    // Get camera information
    cameraMatrix = renderer.camera.viewMatrix;
    fwd = inverse(cameraMatrix) * glm::vec4(0, 0, 1, 0);
    right = inverse(cameraMatrix) * glm::vec4(1, 0, 0, 0);
    up = inverse(cameraMatrix) * glm::vec4(0, 1, 0, 0);

    renderer.drawWireCube(glm::vec3(0), glm::vec3(5), glm::vec3(1));
    renderer.drawCube(
        glm::vec3(0,0,0), // center position
        glm::angleAxis(glm::radians(45.0f), glm::vec3(1.0f / sqrtf(2.f), 1.0f / sqrtf(2.f), 0.0f)), // rotation
        glm::vec3(0.5,0.5,0.5), // scale
        glm::vec4(1,0,0,1)); // color
}

void Scene::simulateStep() {
    // Timestep, takes framerate into account
    float realtimeDt = ImGui::GetIO().DeltaTime;

    // Gravity
    glm::vec3 gravityAccel = glm::vec3(0, 0, -9.81f);
    for (auto& particle : particles){
        particle.position += realtimeDt * particle.velocity;
        particle.lifetime += realtimeDt;
        particle.velocity += gravityAccel * realtimeDt;
    }

    // Movement of the particles
    for (auto& particle : particles){
        particle.position += 0.01f * particle.velocity;
        particle.lifetime += 0.01f;
    }
}
