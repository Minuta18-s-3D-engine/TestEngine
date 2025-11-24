#include <gtest/gtest.h>
#include "engine/gameobject/GameObject.hpp"
#include "engine/gameobject/Component.hpp"

// Mock component for testing
class TestComponent : public Component {
public:
    int value = 42;
    std::unique_ptr<Component> clone() const {
        auto ptr = std::make_unique<TestComponent>();
        ptr->value = this->value;
        return ptr;
    }
};

class AnotherComponent : public Component {
    std::unique_ptr<Component> clone() const {
        return std::make_unique<AnotherComponent>();
    }
};

TEST(GameObjectTest, ConstructorGeneratesUUID) {
    GameObject obj;
    EXPECT_FALSE(obj.getUuid().is_nil());
}

TEST(GameObjectTest, AddGetComponent) {
    GameObject obj;
    auto component = std::make_unique<TestComponent>();
    TestComponent* rawPtr = component.get();
    
    obj.addComponent(std::move(component));
    
    TestComponent* retrieved = obj.getComponent<TestComponent>();
    EXPECT_EQ(retrieved, rawPtr);
    EXPECT_EQ(retrieved->value, 42);
}

TEST(GameObjectTest, HasComponent) {
    GameObject obj;
    EXPECT_FALSE(obj.hasComponent<TestComponent>());
    
    obj.addComponent(std::make_unique<TestComponent>());
    EXPECT_TRUE(obj.hasComponent<TestComponent>());
}

TEST(GameObjectTest, GetComponentThrowsWhenNotFound) {
    GameObject obj;
    EXPECT_THROW(obj.getComponent<TestComponent>(), std::runtime_error);
}

TEST(GameObjectTest, MoveConstructor) {
    GameObject obj1;
    obj1.addComponent(std::make_unique<TestComponent>());
    
    GameObject obj2 = std::move(obj1);
    
    EXPECT_TRUE(obj2.hasComponent<TestComponent>());
    EXPECT_EQ(obj2.getComponent<TestComponent>()->value, 42);
}

TEST(GameObjectTest, UniqueComponents) {
    GameObject obj;
    obj.addComponent(std::make_unique<TestComponent>());
    
    auto secondComponent = std::make_unique<TestComponent>();
    secondComponent->value = 100;
    obj.addComponent(std::move(secondComponent));
    
    EXPECT_EQ(obj.getComponent<TestComponent>()->value, 100);
}