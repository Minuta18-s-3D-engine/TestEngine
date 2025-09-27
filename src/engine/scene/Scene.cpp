#include "Scene.hpp"

Scene::Scene(AssetManager& _assetManager) : assetManager(_assetManager) {}

void Scene::drawAll(Camera* cam) {
    Shader* mainShader = assetManager.get<Shader>("shaders/main");

    glm::mat4 proj = glm::mat4(1.0f);
    proj = glm::perspective(
        cam->getZoom(), 
        (float) Window::width / (float) Window::height, 
        0.1f, 100.0f
    );

    lights[0]->passToShader(*mainShader, "light");

    glm::mat4 viewMat = cam->getViewMat();
    glm::mat4 worldModel = glm::mat4(1.0f);
    for (auto& object : this->objects) {
        glm::mat4 model = glm::translate(worldModel, object.get()->position);
        
        mainShader->use();
        mainShader->setUniform4mat("projection", proj);
        mainShader->setUniform4mat("view", viewMat);
        mainShader->setUniform3f("viewPos", cam->pos);
        mainShader->setUniform4mat("model", model);

        object.get()->draw(*mainShader);
    }
}

void Scene::addObject(std::shared_ptr<SceneObject> obj) {
    this->objects.push_back(obj);
}

void Scene::addLight(std::shared_ptr<Light> obj) {
    this->lights.push_back(obj);
}
