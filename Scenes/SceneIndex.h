#include "Scene.h"
#include <map>

#include "SceneDemo.h"
#include "Scene0.h"

using SceneCreator = std::function<std::unique_ptr<Scene>()>;

template <typename T>
SceneCreator creator()
{
    return []()
    { return std::make_unique<T>(); };
}

std::map<std::string, SceneCreator> scenesCreators = {
    {"Demo Scene", creator<SceneDemo>()},
    {"Exercice 0", creator<Scene0>()}
    // add more Scene types here
};
