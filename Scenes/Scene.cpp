#include "Scene.h"
#include <imgui.h>

void Scene::onDraw(Renderer& renderer) {

    // white box
    renderer.drawWireCube(glm::vec3(0), glm::vec3(5), glm::vec3(1));
    renderer.drawCube(glm::vec3(0), glm::quat(glm::vec3(0)), glm::vec3(0.5), glm::vec4(1, 0, 0, 1));
    
    // axes based on the camera position
    cameraMatrix = renderer.camera.viewMatrix;
    fwd = inverse(cameraMatrix) * glm::vec4(0, 0, 1, 0);
    right = inverse(cameraMatrix) * glm::vec4(1, 0, 0, 0);
    up = inverse(cameraMatrix) * glm::vec4(0, 1, 0, 0);
}
