#include "Scene.h"
#include <glm/gtx/quaternion.hpp>
#include <random>

struct OtherSpring{
    float stiffness;
    float restLength;
    int ind1;
    int ind2;
};

struct Plane{
    glm::vec3 position;
    glm::vec3 rotationAngles;   // Pitch, roll, yaw
    glm::vec4 color;
};
    

class Scene4 : public Scene
{
    float timeStep = 0.005f;

    // Characteristics of the springs and the mass points
    float mass = 10.f;
    float bigMass = 10000.f;
    float stiffness = 80.f;
    float restLength = 1.f;

    // Size of the particles
    float radius = 0.1f;

    // List of mass points in the scene
    std::vector<MassPoint> massPoints;
    // List of springs in the scene
    std::vector<OtherSpring> springs;

    // Floor
    float floorHeight = -2.5f;
    float scale = 10.f;
    Plane floor = Plane{
        glm::vec3(0,0,floorHeight),
        glm::vec3(0,0,0),
        glm::vec4(0.9, 0.9, 0.9,0.7)
    };

    // Which method is used (0=Euler, 1=Midpoint, 2=Leap-Frog)
    int method = 0;

    virtual void onDraw(Renderer &renderer) override;
    virtual void simulateStep() override;
    virtual void onGUI() override;
    virtual void init() override;

    // Update acceleration for every forces
    void Forces(std::vector<MassPoint> &pointsList);

    // SpringForces computes all the forces concerning springs and adds it to the acceleration of every concerned points
    void SpringForces(std::vector<MassPoint> &pointsList);
    
    void GravityForce(std::vector<MassPoint> &pointsist);
    
    void OneStepEuler();
    void OneStepMidpoint();

    void CheckCollisionFloor();

};