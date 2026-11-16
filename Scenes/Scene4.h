#include "Scene.h"

struct Point4 {
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 position_tmp;
    glm::vec3 velocity_tmp;
    glm::vec3 acceleration;
    float radius;
    bool isFixed;
};

struct Spring4 {
    Point4* point1;
    Point4* point2;
};

class Scene4 : public Scene {

    std::vector<Point4> pointsEuler;
    std::vector<Spring4> springsEuler;

    std::vector<Point4> pointsMidpoint;
    std::vector<Spring4> springsMidpoint;

    Point4 collider;

    int width = 10;
    int height = 10;

    float stiffness = 10000.0f;
    float restLength = 0.5f;
    float damping = 10.0f;
    float mass = 1.0f;
    float gravity = -9.81f;
    float deltaTime = 0.001f;
    float maxVelocity = 10.0f;

    bool useMidpoint = false;

    virtual void init() override;
    void oneStepEuler();
    void oneStepMidpoint();
    virtual void simulateStep() override;
    virtual void onDraw(Renderer& renderer) override;
    virtual void onGUI() override;
};