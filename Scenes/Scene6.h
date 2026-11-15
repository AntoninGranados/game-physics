#include "Scene.h"
#include <random>

struct Joint {
    glm::vec3 position;
    glm::vec3 velocity;
    bool isFixed;
};

struct Spring6 {
    int joint1;
    int joint2;
};

struct Cloth {
    std::vector<Joint> joints;
    std::vector<Spring6> springs;
    glm::vec4 color;
    int32_t width;
    int32_t height;
};

class Scene6 : public Scene {

    Cloth cloth;

    int32_t width = 10;
    int32_t height = 10;

    float stiffness = 500.0f;
    float length = 0.5f;
    float damping = 0.01f;
    float mass = 10.0f;
    float gravity = -9.81f;

    int32_t selectedJoint = -1;

    bool isDragging = false;

    void initializeCloth();
    virtual void init() override;
    virtual void simulateStep() override;
    virtual void onDraw(Renderer& renderer) override;
    virtual void onGUI() override;
};