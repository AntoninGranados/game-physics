#pragma once

#include "Scene.h"
#include <map>

#include "Exercice.h"

#include "SceneDemo.h"

#ifdef EXERCICE_0
#include "exercice_0/Scene0.h"
#endif // EXERCICE_0

#ifdef EXERCICE_1
#include "exercice_1/Scene1.h"
#include "exercice_1/Scene2.h"
#include "exercice_1/Scene3.h"
#include "exercice_1/Scene4.h"
#endif // EXERCICE_1

using SceneCreator = std::function<std::unique_ptr<Scene>()>;

template <typename T>
SceneCreator creator()
{
    return []()
    { return std::make_unique<T>(); };
}

std::map<std::string, SceneCreator> scenesCreators = {
    {"Demo Scene", creator<SceneDemo>()},

#ifdef EXERCICE_0
    {PREFIX(0, 0) "Main", creator<ex0::Scene0>()},
#endif // EXERCICE_0

#ifdef EXERCICE_1
    {PREFIX(1, 1) "Single Step", creator<ex1::Scene1>()},
    {PREFIX(1, 2) "Euler Simulation", creator<ex1::Scene2>()},
    {PREFIX(1, 3) "Midpoint Simulation", creator<ex1::Scene3>()},
    {PREFIX(1, 4) "Complex Simulation", creator<ex1::Scene4>()},
#endif // EXERCICE_1

};
