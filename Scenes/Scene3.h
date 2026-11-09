#include "Scene.h"
#include <glm/gtx/quaternion.hpp>

class Scene3 : public Scene
{
    float timeStep = 0.005f;

    MassPoint x0 = MassPoint{
        10.f,
        glm::vec3(0),
        glm::vec3(-1.,0.,0.)
    };
    MassPoint x1 = MassPoint{
        10.f,
        glm::vec3(0.f,2.f,0.f),
        glm::vec3(1.f,0.f,0.f)
    };
    Spring s = Spring{
        40.f,
        1.f
    };

    virtual void onDraw(Renderer &renderer) override;
    virtual void simulateStep() override;
    virtual void onGUI() override;

    glm::vec3 SpringForce(Spring spring, float currentLength, glm::vec3 direction);
    glm::vec3 PFD(float mass, glm::vec3 force);
};