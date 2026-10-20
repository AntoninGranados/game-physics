#pragma once
#include "Scene.h"

#include <random>
#include <vector>

class Renderer;
class Colormap;

struct Particle {
    glm::vec3 pos;
    glm::vec3 vel;
    glm::vec4 color;
    float lifetime;

    static constexpr float lifeDuration = 4.0f;
    static constexpr float radius = 0.1f;
};

struct CameraParameters {
    glm::mat4 viewMatrix;
    glm::vec3 forward;
    glm::vec3 right;
    glm::vec3 up;

    void computeParameters(const glm::mat4 &viewMatrix);
};

class Scene0 : public Scene {
public:
    Scene0(): gen(rd()), dis(0.f, 1.f) {}

    virtual void onDraw(Renderer &renderer) override;
    virtual void onGUI() override;
    virtual void simulateStep() override;

private:
    // Cube
    const float cubeRadius = 2.5f;
    float pitch = 0.f, dPitch = 1.f;
    float yaw = 0.f, dYaw = 2.f;
    float roll = 0.f, dRoll = 3.f;

    // Random number generation
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<float> dis;

    // Particles
    std::vector<Particle> particles;
    const glm::vec3 gravity = glm::vec3(0.0, 0.0, -9.81);
    Colormap cmap = Colormap("inferno");
    int32_t launchDelay = 8;
    int32_t lastLaunch = 0;
    void launchParticle();

    // Camera
    CameraParameters camera;
};
