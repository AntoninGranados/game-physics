#pragma once
#include "Renderer.h"
#include <random>

// Particles can be in every scene
struct Particle
{
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec4 color;
    float lifetime;
};

struct MassPoint
{
    float mass;
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
};

struct Spring
{
    float stiffness;
    float restLength;
};

/// @brief Scene base class. **Run `cmake . -B build` after adding new files to the scenes folder**
///
/// This class is the base class for all scenes in the application.
/// When creating a new scene, derive from this class.
/// Add the new scene to the SceneIndex.h file.
///
/// Feel free to modify the non virtual functions for functionality shared between scenes.
class Scene
{
public:
    // Camera information
    glm::mat4 cameraMatrix = glm::mat4(1);
    glm::vec3 fwd = glm::vec3(1, 0, 0);
    glm::vec3 right = glm::vec3(0, 1, 0);
    glm::vec3 up = glm::vec3(0, 0, 1);

    // Random generator
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<float> dis;

    // List of particles in the scene
    std::vector<Particle> particles;    // Stores the list of created particles

    /// @brief Initialize the scene. Gets called every time the scene is switched to.
    virtual void init() {};
    /// @brief Simulate a step in the scene. Gets called every frame before onDraw.
    ///
    /// This is where you should update the physics of the scene.
    virtual void simulateStep();
    /// @brief Draw the scene. Gets called every frame after simulateStep.
    ///
    /// This is where you should call the Renderer draw functions.
    virtual void onDraw(Renderer &renderer);
    /// @brief Define the GUI for the scene. Gets called every frame after onDraw.
    virtual void onGUI() {};
    virtual ~Scene() = default;

    Scene() : gen(rd()), dis(0.f, 1.f) {}  // Public constructor to initialize the random generator

};
