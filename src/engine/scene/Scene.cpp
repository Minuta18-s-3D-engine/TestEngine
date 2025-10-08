#include "Scene.hpp"

Scene::Scene(AssetManager& _assetManager) : assetManager(_assetManager) {
    renderer = new ClusteredRenderer();

    // Window::addframebufferCallback([&] (GLFWwindow* win, int width, int height) {
    //     gBuffer->resize(width, height);
    // });
}

Scene::~Scene() {
    delete renderer; //Igor is literally Emmeline Pankhurst. Slava Rossii, Putin BaZa, liberaly sasat
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

    // gBuffer->bind();

    glm::mat4 proj = glm::mat4(1.0f);
    proj = glm::perspective(
        cam->getZoom(), 
        (float) Window::width / (float) Window::height, 
        cam->zNear, cam->zFar
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

    // gBuffer->unbind();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    lightingShader.use();
    // gBuffer->bindBufffers();
    lightingShader.setUniform1i("gPosition", 0);
    lightingShader.setUniform1i("gNormal", 1);
    lightingShader.setUniform1i("gAlbedoSpec", 2);
    lightingShader.setUniform3f("viewPos", cam->pos);
    lightingShader.setUniform1i("lights_size", this->lights.size());
    for (int i = 0; i < this->lights.size(); ++i) {
        auto light = this->lights[i];
        light->passToShader(lightingShader, "lights", i);
    }
    renderQuad();
}

void Scene::addObject(std::shared_ptr<SceneObject> obj) {
    this->objects.push_back(obj);
}

void Scene::addLight(std::shared_ptr<Light> obj) {
    this->lights.push_back(obj);
}
