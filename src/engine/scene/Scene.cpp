#include "Scene.hpp"

Scene::Scene(AssetManager& _assetManager) : assetManager(_assetManager) {
    gBuffer = new GBuffer(Window::width, Window::height);

    Window::addframebufferCallback([&] (GLFWwindow* win, int width, int height) {
        gBuffer->resize(width, height);
    });
}

Scene::~Scene() {
    delete gBuffer;
}

void Scene::drawAll(Camera* cam) {
    // Shader& mainShader = assetManager.require<Shader>("shaders/main");

    // glm::mat4 proj = glm::mat4(1.0f);
    // proj = glm::perspective(
    //     cam->getZoom(), 
    //     (float) Window::width / (float) Window::height, 
    //     0.1f, 100.0f
    // );


    // glm::mat4 viewMat = cam->getViewMat();
    // glm::mat4 worldModel = glm::mat4(1.0f);
    // for (auto& object : this->objects) {
    //     glm::mat4 model = glm::translate(worldModel, object.get()->position);
        
    //     mainShader.use();
    //     mainShader.setUniform4mat("projection", proj);
    //     mainShader.setUniform4mat("view", viewMat);
    //     mainShader.setUniform3f("viewPos", cam->pos);
    //     mainShader.setUniform4mat("model", model);

    //     lights[0]->passToShader(mainShader, "light");
    //     object->draw(mainShader);
    // }

    Shader& geomShader = assetManager.require<Shader>("shaders/geomShader");
    Shader& lightingShader = assetManager.require<Shader>("shaders/lightingShader");

    gBuffer->bind();

    glm::mat4 proj = glm::mat4(1.0f);
    proj = glm::perspective(
        cam->getZoom(), 
        (float) Window::width / (float) Window::height, 
        0.1f, 100.0f
    );
    glm::mat4 viewMat = cam->getViewMat();
    glm::mat4 worldModel = glm::mat4(1.0f);

    geomShader.use();
    geomShader.setUniform4mat("projection", proj);
    geomShader.setUniform4mat("view", viewMat);
    for (auto& object : this->objects) {
        glm::mat4 model = glm::translate(worldModel, object.get()->position);
        
        geomShader.setUniform4mat("model", model);
        object->draw(geomShader);
    }

    gBuffer->unbind();

    lightingShader.use();
    gBuffer->bindBufffers();
    lightingShader.setUniform3f("viewPos", cam->pos);
    for (auto& light : this->lights) {

    }
}

void Scene::addObject(std::shared_ptr<SceneObject> obj) {
    this->objects.push_back(obj);
}

void Scene::addLight(std::shared_ptr<Light> obj) {
    this->lights.push_back(obj);
}
