#include "Scene.h"
#include <glm/gtx/quaternion.hpp>
#include <random>

struct OtherSpring{
    float stiffness;
    float restLength;
    int ind1;
    int ind2;
};
    

class Scene4 : public Scene
{
    float timeStep = 0.005f;

    // Characteristics of the springs and the mass points
    float mass = 10.f;
    float bigMass = 100000.f;
    float stiffness = 100.f;
    float restLength = 1.f;

    // List of mass points in the scene
    std::vector<MassPoint> massPoints;
    // List of springs in the scene
    std::vector<OtherSpring> springs;

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

};