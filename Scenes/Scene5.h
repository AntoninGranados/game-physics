#include "Scene.h"
#include <random>

struct Particle {
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec4 color;
    float lifetime;
};

class Scene5 : public Scene {

    // emitter's movement parameters
    float pitch = 0.f;
    float roll = 0.f;
    float yaw = 0.f;
    float pitch_increment = 0.005f;
    float roll_increment = 0.005f;
    float yaw_increment = 0.005f;

    // other parameters
    float mouseStrength = 0.1f;
    float maxLifetime = 5.0f;

    // time management for particle launching
    int32_t launch_delay = 8;
    int32_t lastLaunch = 0;

    // random number generators for particle properties
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<float> dis;

    // gravity acceleration vector
    glm::vec3 gravityAccel = glm::vec3(0, 0, -9.81f);

    // particles
    std::vector<Particle> particles;

    void drawEmitter(Renderer& renderer);
    void launchSphere();
    virtual void simulateStep() override;
    virtual void onDraw(Renderer& renderer) override;
    virtual void onGUI() override;

public:

    Scene5() : gen(rd()), dis(0.f, 1.f) {}
};