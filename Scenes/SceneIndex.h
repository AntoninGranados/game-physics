#pragma once
#include "Scene.h"
#include <map>

#include "SceneDemo.h"
#include "exercice_0/Scene0.h"

#include "exercice_1/Scene1.h"
#include "exercice_1/Scene2.h"
#include "exercice_1/Scene3.h"
#include "exercice_1/Scene4.h"

using SceneCreator = std::function<std::unique_ptr<Scene>()>;

template <typename T>
SceneCreator creator()
{
    return []()
    { return std::make_unique<T>(); };
}

std::map<std::string, SceneCreator> scenesCreators = {
    {"Demo Scene", creator<SceneDemo>()},

    {"Exercice 0", creator<ex0::Scene0>()},

    {"Single Step", creator<ex1::Scene1>()},
    {"Euler Simulation", creator<ex1::Scene2>()},
    {"Midpoint Simulation", creator<ex1::Scene3>()},
    {"Complex Simulation", creator<ex1::Scene4>()},
    // add more Scene types here
};
