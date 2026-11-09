#include "Scene.h"
#include <map>

#include "Scene5.h"
#include "Scene6.h"

using SceneCreator = std::function<std::unique_ptr<Scene>()>;

template <typename T>
SceneCreator creator() {
    return []() {
        return std::make_unique<T>();
    };
}

std::map<std::string, SceneCreator> scenesCreators = {
    {"Demo Scene", creator<Scene>()},
    {"Magic hat", creator<Scene5>()},
    {"Clothes", creator<Scene6>()},
    // add more Scene types here
};
