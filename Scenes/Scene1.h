#include "Scene.h"

struct Point {
    glm::vec3 position;
    glm::vec3 velocity;
    float mass;
    glm::vec4 color;
};

struct Spring {
    Point* point1;
    Point* point2;
    float restLength;
    float stiffness;
};

class Scene1 : public Scene {

    Point point1;
    Point point2;
    Spring spring;

    float deltaTime = 0.1f;

    int step = 0;

    std::vector<glm::vec3> positions1;
    std::vector<glm::vec3> positions2;

    virtual void init() override;
    void oneStepEuler();
    void oneStepMidpoint();
    void printState();
    virtual void onDraw(Renderer& renderer) override;
    virtual void onGUI() override;
};