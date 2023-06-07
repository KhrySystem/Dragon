// Define certain overridable things BEFORE including dragon header
#define APP_NAME "Small Test Game"
//std::cout << "(" << __FILE__ << ", " << __LINE__ << ")\n";
#define VMA_DEBUG_LOG_FORMAT
#include <dragon/dragon.hpp>
#include <exception>
#include <vector>
#include <iostream>
#include <csignal>
#include <chrono>

DgEngine engine{};
std::unique_ptr<DgEngine> pEngine = std::make_unique<DgEngine>(engine); // Owned by user
DgWindow window{};
std::shared_ptr<DgWindow> pWindow = std::make_shared<DgWindow>(window); // Owned by engine and user
DgModel model{};
std::shared_ptr<DgModel> pModel = std::make_shared<DgModel>(model); // Owned by window and user
//DgModel m{};
//std::shared_ptr<DgModel> pM = std::make_shared<DgModel>(m); // Owned by window and user
DgCamera camera{};
std::shared_ptr<DgCamera> pCamera = std::make_shared<DgCamera>(camera); // Owned by window and user
const std::vector<DgVertex> vertices = {
    {{ 0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}}, // 0, red
    {{ 0.25f, 0.0f}, {1.0f, 0.6f, 0.0f}}, // 1, orange
    {{-0.0f,  0.2f}, {1.0f, 1.0f, 1.0f}}, // 2, white
    {{-0.25f, 0.0f}, {1.0f, 0.0f, 1.0f}}, // 3, purple
    {{-0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}}, // 4, blue
    {{ 0.0f,  0.5f}, {0.0f, 1.0f, 0.0f}}, // 5, green
    {{ 0.5f,  0.5f}, {1.0f, 1.0f, 0.0f}}, // 6, yellow
    {{ 1.0f,  1.0f}, {1.0f, 0.0f, 1.0f}}, // 7
    {{ 1.0f,  0.0f}, {0.6f, 0.0f, 0.6f}}, // 8
    {{ 1.0f, -1.0f}, {0.0f, 0.0f, 1.0f}}, // 9
    {{ 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f}}, // 10
    {{-1.0f, -1.0f}, {0.6f, 0.6f, 0.0f}}, // 11
    {{-1.0f,  0.0f}, {0.6f, 0.6f, 0.0f}}, // 12
    {{-1.0f,  1.0f}, {1.0f, 0.6f, 0.0f}}, // 13
    {{ 0.0f,  1.0f}, {1.0f, 0.0f, 0.0f}}  // 14
};
const std::vector<uint32_t> indices = {
    1, 2, 0, 
    0, 2, 3,
    3, 2, 4,
    4, 2, 5,
    5, 2, 6,
    6, 2, 1,
    7, 6, 8,
    6, 1, 8,
    1, 0, 9,
    8, 1, 9,
    0, 10, 9,
    0, 11, 10,
    3, 11, 0,
    3, 12, 11,
    4, 13, 12,
    3, 4, 12,
    4, 14, 13,
    14, 4, 5, 
    14, 5, 6,
    14, 6, 7
};
const std::vector<DgVertex> v = {
    {{ 0.0f, -0.5f}, {0.0f, 0.0f, 0.0f}}, // 0
    {{ 0.25f, 0.0f}, {0.0f, 0.0f, 0.0f}}, // 1
    {{-0.0f,  0.2f}, {0.0f, 0.0f, 0.0f}}, // 2
};
const std::vector<uint32_t> i = {
    0, 1, 2
};

void printVkExtLayerData() {
    std::vector<std::string> extensions;
    extensions = dgGetAvailableVulkanExtensions();
    std::cout << "Extensions Available: " << extensions.size() << '\n';
    for(std::string extension : extensions) {
        std::cout << '\t' << extension << '\n';
    }
    std::vector<std::string> layers;
    layers = dgGetAvailableVulkanLayers();
    std::cout << "\nLayers Available: " << layers.size() << '\n';
    for(std::string layer : layers) {
        std::cout  << '\t' << layer << '\n';
    }
    std::cout << '\n';
}

void initEngine() {
    DgEngineCreateInfo createInfo{};
    createInfo.enableVulkanDebug = DG_TRUE;

    DgResult r = dgCreateEngine(createInfo, pEngine);
    if(r == DG_SUCCESS) return;
    printf("Error 0x%x\n", r);
    dgDestroyEngine(pEngine);
    abort();
}

void initWindow() {
    DgWindowCreateInfo createInfo{};
    createInfo.width = 1000;
    createInfo.height = 800;
    createInfo.pTitle = APP_NAME;
    createInfo.renderLayers = 2;
    DgResult r = dgCreateWindow(createInfo, pWindow, pEngine, pCamera);
    if(r == DG_SUCCESS) return;
    printf("Error 0x%x\n", r);
    dgDestroyWindow(pWindow, pEngine);
    dgDestroyEngine(pEngine);
    abort();
}

void initModel1() {
    DgModelCreateInfo createInfo{};
    createInfo.vertices = vertices;
    createInfo.indices = indices;
    createInfo.pWindow = pWindow;
    createInfo.renderLayer = 0;
    DgResult r = dgCreateModel(createInfo, pModel);
    if(r == DG_SUCCESS) return;
    printf("Error 0x%x\n", r);
    dgDestroyModel(pModel, pWindow);
    dgDestroyWindow(pWindow, pEngine);
    dgDestroyEngine(pEngine);
    abort();
}


void initModels() {
    initModel1();
    //initModel2();
}

void init() {
    // Check Extensions and Layers available
    #if !defined(NDEBUG) || defined(_DEBUG)
        printVkExtLayerData();
    #endif
    // engine creation
    initEngine();
    // window creation
    initWindow();
    // model creation
    initModels();
}

int main() {
    auto initStart = std::chrono::high_resolution_clock::now();
    init();
    auto initEnd = std::chrono::high_resolution_clock::now();
    std::cout << "Took " << std::chrono::duration_cast<std::chrono::milliseconds>(initEnd - initStart).count() << "ms to run initialization.\n";
    // loop
    while(!dgCanEngineClose(pEngine)) {
        // inputs (should come from GLFW?)
        // math things
        // move models
        // rotate camera 90 degrees z-axis every second
        //dgM
        // animation
        // update window
        DgResult r = dgUpdateEngine(pEngine);
        if(r != DG_SUCCESS) {
            printf("Error 0x%x\n", r);
            break;
        } 
    }
    // endloop
    auto destroyStart = std::chrono::high_resolution_clock::now();
    // model destruction
    //dgDestroyModel(pM, pWindow);
    dgDestroyModel(pModel, pWindow);
    // window destruction
    dgDestroyWindow(pWindow, pEngine);
    // engine destruction
    dgDestroyEngine(pEngine);
    auto destroyEnd = std::chrono::high_resolution_clock::now();
    std::cout << "Took " << std::chrono::duration_cast<std::chrono::milliseconds>(destroyEnd - destroyStart).count() << "ms to destroy objects.\n";
    return 0;
}