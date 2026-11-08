#include "Scene0.h"
#include <glm/gtx/quaternion.hpp>
#include <imgui.h>

void Scene0::onDraw(Renderer &renderer) {
    // Get camera information
    cameraMatrix = renderer.camera.viewMatrix;
    fwd = inverse(cameraMatrix) * glm::vec4(0, 0, 1, 0);
    right = inverse(cameraMatrix) * glm::vec4(1, 0, 0, 0);
    up = inverse(cameraMatrix) * glm::vec4(0, 1, 0, 0);

    // Large white wire cube
    renderer.drawWireCube(glm::vec3(0), glm::vec3(5), glm::vec3(1));
    
    // Smaller cube inside
    renderer.drawCube(
        glm::vec3(0,0,0),
        glm::quat(glm::vec3(pitch, roll, yaw)),     // rotation now given by Euler angle
        glm::vec3(0.5,0.5,0.5),
        glm::vec4(0.8,0.4,0,0.9));

    // The cube's local coordinates
    glm::mat4 rotation = glm::toMat4(glm::quat(glm::vec3(pitch, roll, yaw)));
    glm::vec3 forward = glm::vec3(rotation * glm::vec4(0, 0, 1, 0));
    glm::vec3 right = glm::vec3(rotation * glm::vec4(1, 0, 0, 0));
    glm::vec3 up = glm::vec3(rotation * glm::vec4(0, 1, 0, 0));

    renderer.drawLine(glm::vec3(0), forward, glm::vec4(1, 0, 0, 1));
    renderer.drawLine(glm::vec3(0), right, glm::vec4(0, 1, 0, 1));
    renderer.drawLine(glm::vec3(0), up, glm::vec4(0, 0, 1, 1));

    // Draw the particles, their colors depend on their lifetime
    auto cmap = Colormap("viridis");

    for (auto& particle : particles){
        renderer.drawSphere(particle.position, 0.1f, glm::vec4(cmap(particle.lifetime / 1.5f), 1));
    }
}

// Creates movement / animation
void Scene0::simulateStep() {
    // General actions common to every scene
    Scene::simulateStep();

    // Rotation of the cube
    //pitch += pitch_increment;
    //roll += roll_increment;
    //yaw += yaw_increment;

    // Additional force from the mouse
    if(ImGui::IsMouseReleased(ImGuiMouseButton_Right)){   
        auto drag = ImGui::GetMouseDragDelta(1);
        if(!(drag.x == 0 && drag.y == 0)){
            auto dx = drag.x * right;
            auto dy = -drag.y * up;
            for (auto& particle : particles){
                particle.velocity += (dx + dy) * 0.01f;
            }
        }
    }

    // Removes all particles whose lifetime is more than 1s
    particles.erase(std::remove_if(particles.begin(), particles.end(), [](const Particle& particle){
        return particle.lifetime > 2.5f;
        }), particles.end());

    // Creates particle or rotate the cube when a key is pressed 
    if(ImGui::IsKeyDown(ImGuiKey_Space))
        launchSphere();
    if(ImGui::IsKeyDown(ImGuiKey_W))
        pitch += pitch_increment;
    if(ImGui::IsKeyDown(ImGuiKey_S))
        pitch -= pitch_increment;
    if(ImGui::IsKeyDown(ImGuiKey_A))
        roll += roll_increment;
    if(ImGui::IsKeyDown(ImGuiKey_D))
        roll -= roll_increment;
    if(ImGui::IsKeyDown(ImGuiKey_Q))
        yaw += yaw_increment;
    if(ImGui::IsKeyDown(ImGuiKey_E))
        yaw -= yaw_increment;


    // Time since the last launch
    lastLaunch++;
    
}

// User interface
void Scene0::onGUI() {
    // Sliders
    ImGui::SliderFloat("Pitch Increment", &pitch_increment, -0.01f, 0.01f);
    ImGui::SliderFloat("Roll Increment", &roll_increment, -0.01f, 0.01f);
    ImGui::SliderFloat("Yaw Increment", &yaw_increment, -0.01f, 0.01f);
    ImGui::SliderInt("Launch Delay", &launch_delay, 0, 100);

    //auto launch = ImGui::Button("Launch");  // launch true when the button is pressed
    // Creates a particle when launch is true, without delay
    //if(launch)
    //    launchSphere();

}

// Creates a particle if enough time has passed
void Scene0::launchSphere(){
    if( lastLaunch < launch_delay)
        return;

    lastLaunch = 0;
    // Directions
    glm::mat4 rotation = glm::toMat4(glm::quat(glm::vec3(pitch, roll, yaw)));
    glm::vec3 forward = glm::vec3(rotation * glm::vec4(0, 0, 1, 0));
    glm::vec3 right = glm::vec3(rotation * glm::vec4(1, 0, 0, 0));
    glm::vec3 up = glm::vec3(rotation * glm::vec4(0, 1, 0, 0));

    // Random color
    glm::vec4 color = glm::vec4(dis(gen), dis(gen), dis(gen), 1);
    
    // Initial velocity with some randomness
    float velocityMagnitude = 3.5f + dis(gen);
    glm::vec3 velocity = forward * velocityMagnitude;
    velocity += right * (dis(gen) - 0.5f) * 2.f;
    velocity += up * (dis(gen) - 0.5f) * 2.f;

    // Adds the particle to the list
    particles.push_back(Particle{
        glm::vec3(0),
        velocity,
        color,
        .0 // Particles are created with their own time counter set to 0
    });
}