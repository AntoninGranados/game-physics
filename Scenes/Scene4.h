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

    // List of mass points in the scene
    std::vector<MassPoint> massPoints;
    // List of springs in the scene
    std::vector<OtherSpring> springs;

    virtual void onDraw(Renderer &renderer) override;
    virtual void simulateStep() override;
    virtual void onGUI() override;
    virtual void init() override;

    // SpringForces computes all the forces concerning springs and adds it to the acceleration of every concerned points
    void SpringForces(std::vector<MassPoint> &pointsList);
    
    glm::vec3 PFD(float mass, glm::vec3 force);
    void OneStepEuler();
    void OneStepMidpoint();

};