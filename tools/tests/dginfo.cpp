#include <chrono>
#include <dragon/dragon.hpp>

int main() {
    Dragon::Engine engine;
    Dragon::EngineCreateInfo createInfo{};
    printf("HERE");
    Dragon::createEngine(createInfo, &engine);
}
