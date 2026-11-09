#include "Scene.h"
#include <map>

#include "Scene0.h"
#include "Scene1.h"
#include "Scene2.h"

using SceneCreator = std::function<std::unique_ptr<Scene>()>;

template <typename T>
SceneCreator creator()
{
    return []()
    { return std::make_unique<T>(); };
}

std::map<std::string, SceneCreator> scenesCreators = {
    {"Demo Scene", creator<Scene0>()},
    {"Single Step", creator<Scene1>()},
    {"Euler Simulation", creator<Scene2>()},
    // add more Scene types here
};
