#include "Scene.h"

struct Point3 {
    glm::vec3 position;
    glm::vec3 velocity;
    float mass;
    glm::vec4 color;
};

struct Spring3 {
    Point3* point1;
    Point3* point2;
    float restLength;
    float stiffness;
};

class Scene3 : public Scene {

    Point3 point1;
    Point3 point2;
    Spring3 spring;

    float deltaTime = 0.005f;

    std::vector<glm::vec3> positions1;
    std::vector<glm::vec3> positions2;

    virtual void init() override;
    virtual void simulateStep() override;
    virtual void onDraw(Renderer& renderer) override;
    virtual void onGUI() override;
};