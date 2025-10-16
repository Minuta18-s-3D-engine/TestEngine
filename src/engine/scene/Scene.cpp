#include "Scene.hpp"

Scene::Scene(AssetManager& _assetManager) : assetManager(_assetManager) {
    renderer = new ClusteredRenderer(_assetManager);
    firstRender = true;

    // Window::addframebufferCallback([&] (GLFWwindow* win, int width, int height) {
    //     gBuffer->resize(width, height);
    // });
}

Scene::~Scene() {
    delete renderer;
}

void Scene::renderQuad() {
    if (quadVAO == 0) {
        float quadVertices[] = {
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void Scene::drawAll(Camera* cam) {
    renderer->updateLightData(this->lights);
    renderer->updateClusters(cam);

    Shader& lightingShader = assetManager.require<Shader>("shaders/lightingShader"); 
    lightingShader.use();

    if (firstRender) {
        firstRender = false;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glm::mat4 proj = glm::mat4(1.0f);
    proj = glm::perspective(
        cam->getZoom(), 
        (float) Window::width / (float) Window::height, 
        cam->zNear, cam->zFar
    );
    glm::mat4 viewMat = cam->getViewMat();
    glm::mat4 worldModel = glm::mat4(1.0f);

    lightingShader.setUniform4mat("view", viewMat);
    lightingShader.setUniform4mat("projection", proj);

    lightingShader.setUniform1f("zNear", cam->zNear);
    lightingShader.setUniform1f("zFar", cam->zFar);
    lightingShader.setUniform4mat("inverseProjection", glm::inverse(proj));
    lightingShader.setUniform3ui("gridSize", renderer->getClusterGrid());
    lightingShader.setUniform2ui("screenDimensions", 
        Window::width, Window::height);

    lightingShader.setUniform3f("viewPos", cam->pos);
    lightingShader.setUniform1i("numLights", lights.size());

    renderer->bindClusterData();

    for (auto& object : this->objects) {
        glm::mat4 model = glm::translate(worldModel, object.get()->position);

        lightingShader.setUniform4mat("model", model);
        object->draw(lightingShader);
    }
}

void Scene::addObject(std::shared_ptr<SceneObject> obj) {
    this->objects.push_back(obj);
}

void Scene::addLight(std::shared_ptr<Light> obj) {
    this->lights.push_back(obj);
}
