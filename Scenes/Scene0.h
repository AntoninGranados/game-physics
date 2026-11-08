#include "Scene.h"
#include <random>


class Scene0 : public Scene
{
    // Rotation of the cube
    float pitch = 0.f;
    float roll = 0.f;
    float yaw = 0.f;

    // Increments for the rotation
    float pitch_increment = 0.005f;
    float roll_increment = 0.005f;
    float yaw_increment = 0.005f;

    // Limits the number of particles at the same time
    int32_t launch_delay = 8;
    int32_t lastLaunch = 0;

    // Methods from Scene
    virtual void onDraw(Renderer &renderer) override;
    virtual void simulateStep() override;
    virtual void onGUI() override;

    // Additional methods
    void launchSphere();

};