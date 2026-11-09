#include "Scene2.h"
#include <imgui.h>
#include <glm/gtx/quaternion.hpp>

void Scene2::initializeCloth() {

    cloth = Cloth();
    cloth.color = glm::vec4(0.7, 0.1, 0.1, 1);
    cloth.width = width;
    cloth.height = height;

    for (int y = 0; y < cloth.height; y++) {
        for (int x = 0; x < cloth.width; x++) {
            Joint joint;
            joint.position = glm::vec3((cloth.width / 2 - x) * length, 0, (cloth.height / 2 - y) * length);
            joint.velocity = glm::vec3(0);
            joint.isFixed = (y == 0);
            cloth.joints.push_back(joint);
        }
    }

    for (int y = 0; y < cloth.height; y++) {
        for (int x = 0; x < cloth.width; x++) {
            int index = y * cloth.width + x;
            if (x < cloth.width - 1) {
                Spring spring;
                spring.joint1 = index;
                spring.joint2 = index + 1;
                cloth.springs.push_back(spring);
            }
            if (y < cloth.height - 1) {
                Spring spring;
                spring.joint1 = index;
                spring.joint2 = index + cloth.width;
                cloth.springs.push_back(spring);
            }
        }
    }
}

void Scene2::init() {
    initializeCloth();
}

void Scene2::simulateStep() {

    float realtimeDt = ImGui::GetIO().DeltaTime;

    for (auto& spring : cloth.springs) {

        auto& joint1 = cloth.joints[spring.joint1];
        auto& joint2 = cloth.joints[spring.joint2];

        float currentLength = glm::length(joint2.position - joint1.position);
        glm::vec3 direction = glm::normalize(joint2.position - joint1.position);

        glm::vec3 force = stiffness * (currentLength - length) * direction / mass;

        if (!joint1.isFixed) {
            joint1.velocity += force * realtimeDt;
            joint1.velocity += glm::vec3(0, 0, gravity) / mass * realtimeDt;
            joint1.velocity *= (1.0f - damping);
            joint1.position += joint1.velocity * realtimeDt;
        }

        if (!joint2.isFixed) {
            joint2.velocity -= force * realtimeDt;
            joint2.velocity += glm::vec3(0, 0, gravity) / mass * realtimeDt;
            joint2.velocity *= (1.0f - damping);
            joint2.position += joint2.velocity * realtimeDt;
        }
    }

    if (!isDragging) {
        glm::vec2 mousePos = glm::vec2(ImGui::GetMousePos().x, ImGui::GetMousePos().y);
        for (auto& joint : cloth.joints) {
            glm::vec4 jointPos = projectionMatrix * cameraMatrix * glm::vec4(joint.position, 1);
            jointPos /= jointPos.w;
            glm::vec2 screenPos = (glm::vec2(1.0f) + glm::vec2(jointPos.x, -jointPos.y)) / 2.0f
                * glm::vec2(ImGui::GetMainViewport()->Size.x, ImGui::GetMainViewport()->Size.y);

            if (glm::length(screenPos - mousePos) < 20.0f) {
                selectedJoint = &joint - &cloth.joints[0];
                break;
            }

            selectedJoint = -1;
        }
    }

    if (!ImGui::IsMouseDown(ImGuiMouseButton_Right)) {
        isDragging = false;
    }

    if (ImGui::IsMouseDown(ImGuiMouseButton_Right) && selectedJoint != -1) {
        isDragging = true;

        const glm::vec2 ndc(
            2.0f * ImGui::GetMousePos().x / ImGui::GetMainViewport()->Size.x - 1.0f,
            1.0f - 2.0f * ImGui::GetMousePos().y / ImGui::GetMainViewport()->Size.y
        );

        glm::vec4 nearH = inverse(projectionMatrix * cameraMatrix) * glm::vec4(ndc, -1.0f, 1.0f);
        glm::vec4 farH  = inverse(projectionMatrix * cameraMatrix) * glm::vec4(ndc,  1.0f, 1.0f);

        const glm::vec3 nearPoint = glm::vec3(nearH) / nearH.w;
        const glm::vec3 farPoint  = glm::vec3(farH) / farH.w;
        const glm::vec3 rayDirection = glm::normalize(farPoint - nearPoint);

        auto& joint = cloth.joints[selectedJoint];
        const glm::vec3 planeNormal = glm::normalize(fwd);
        const float denominator = glm::dot(rayDirection, planeNormal);

        if (std::abs(denominator) > 1e-6f) {
            const float distance =
                glm::dot(joint.position - nearPoint, planeNormal) / denominator;

            if (distance >= 0.0f) {
                joint.position = nearPoint + distance * rayDirection;
                joint.velocity = glm::vec3(0.0f);
            }
        }
    }
}

void Scene2::onDraw(Renderer& renderer) {

    cameraMatrix = renderer.camera.viewMatrix;
    projectionMatrix = renderer.camera.projectionMatrix();
    fwd = inverse(cameraMatrix) * glm::vec4(0, 0, 1, 0);
    right = inverse(cameraMatrix) * glm::vec4(1, 0, 0, 0);
    up = inverse(cameraMatrix) * glm::vec4(0, 1, 0, 0);

    for (auto& spring : cloth.springs) {
        auto& joint1 = cloth.joints[spring.joint1];
        auto& joint2 = cloth.joints[spring.joint2];
        renderer.drawLine(joint1.position, joint2.position, cloth.color);
    }

    for (auto& joint : cloth.joints) {
        renderer.drawSphere(joint.position, 0.05f, cloth.color);
    }

    if (selectedJoint != -1) {
        auto& joint = cloth.joints[selectedJoint];
        renderer.drawSphere(joint.position, 0.1f, glm::vec4(0.1, 0.7, 0.1, 1));
    }
}

void Scene2::onGUI() {
    ImGui::Text("Cloth Simulation Parameters");
    ImGui::SliderInt("Width", &width, 2, 50);
    ImGui::SliderInt("Height", &height, 2, 50);
    ImGui::SliderFloat("Stiffness", &stiffness, 1.0f, 1000.0f);
    ImGui::SliderFloat("Length", &length, 0.1f, 1.0f);
    ImGui::SliderFloat("Damping", &damping, 0.0f, 0.05f);
    ImGui::SliderFloat("Mass", &mass, 0.0f, 10.0f);
    auto initialize = ImGui::Button("Initialize");
    if (initialize)
        initializeCloth();
}