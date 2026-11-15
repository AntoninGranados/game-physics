#include "Scene.h"
#include <map>

#include "Scene1.h"
#include "Scene2.h"
#include "Scene3.h"
#include "Scene4.h"
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
    {"Single step", creator<Scene1>()},
    {"Euler simulation", creator<Scene2>()},
    {"Midpoint simulation", creator<Scene3>()},
    {"Complex simulation", creator<Scene4>()},
    {"Magic hat", creator<Scene5>()},
    {"Clothes", creator<Scene6>()},
    // add more Scene types here
};
