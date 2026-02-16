#include <gtest/gtest.h>
#include "TypedPropertyStorage.hpp"
#include <glm/glm.hpp>
#include <memory>

class TypedPropertyStorageTest : public ::testing::Test {
protected:
    TypedPropertyStorage storage;
};

TEST_F(TypedPropertyStorageTest, SupportsAllBasicTypes) {
    storage.setProperty<int32_t>("int", -100);
    storage.setProperty<uint32_t>("uint", 500);
    storage.setProperty<float>("float", 3.14f);
    storage.setProperty<bool>("bool", true);
    
    EXPECT_EQ(storage.getProperty<int32_t>("int"), -100);
    EXPECT_EQ(storage.getProperty<uint32_t>("uint"), 500);
    EXPECT_FLOAT_EQ(storage.getProperty<float>("float"), 3.14f);
    EXPECT_TRUE(storage.getProperty<bool>("bool"));
}

TEST_F(TypedPropertyStorageTest, SupportsGlmTypes) {
    glm::vec3 pos(1.0f, 2.0f, 3.0f);
    glm::mat4 mat(1.0f);
    
    storage.setProperty<glm::vec3>("position", pos);
    storage.setProperty<glm::mat4>("matrix", mat);
    
    EXPECT_EQ(storage.getProperty<glm::vec3>("position"), pos);
    EXPECT_EQ(storage.getProperty<glm::mat4>("matrix"), mat);
}

TEST_F(TypedPropertyStorageTest, CopyConstructorPerformDeepCopy) {
    storage.setProperty<float>("val", 10.0f);
    
    TypedPropertyStorage copy(storage);
    EXPECT_EQ(copy.getProperty<float>("val"), 10.0f);
    
    copy.setProperty<float>("val", 20.0f);
    EXPECT_EQ(storage.getProperty<float>("val"), 10.0f);
    EXPECT_EQ(copy.getProperty<float>("val"), 20.0f);
}

TEST_F(TypedPropertyStorageTest, CopyAssignmentPerformDeepCopy) {
    storage.setProperty<int>("a", 1);
    TypedPropertyStorage other;
    other.setProperty<int>("a", 2);
    
    other = storage;
    EXPECT_EQ(other.getProperty<int>("a"), 1);
}

TEST_F(TypedPropertyStorageTest, MoveConstructorTransfersOwnership) {
    storage.setProperty<glm::vec3>("vec", {1, 2, 3});
    
    TypedPropertyStorage moved(std::move(storage));
    EXPECT_EQ(moved.getProperty<glm::vec3>("vec").y, 2.0f);
    EXPECT_FALSE(storage.hasProperty("vec")); 
}

TEST_F(TypedPropertyStorageTest, ThrowsOnTypeMismatchSet) {
    storage.setProperty<float>("param", 1.0f);
    EXPECT_THROW(storage.setProperty<int32_t>("param", 2), std::invalid_argument);
}

TEST_F(TypedPropertyStorageTest, ThrowsOnTypeMismatchGet) {
    storage.setProperty<float>("param", 1.0f);
    EXPECT_THROW(storage.getProperty<glm::vec2>("param"), std::invalid_argument);
}

TEST_F(TypedPropertyStorageTest, ThrowsOnMissingProperty) {
    EXPECT_THROW(storage.getProperty<float>("ghost"), std::invalid_argument);
}

TEST_F(TypedPropertyStorageTest, HandlesUnsetPropertyState) {
    storage.setProperty<float>("empty_val");
    
    EXPECT_TRUE(storage.hasProperty("empty_val"));
    EXPECT_FALSE(storage.isPropertySet("empty_val"));
    
    EXPECT_THROW(storage.getProperty<float>("empty_val"), std::invalid_argument);

    storage.setProperty<float>("empty_val", 5.0f);
    EXPECT_TRUE(storage.isPropertySet("empty_val"));
    EXPECT_NO_THROW(storage.getProperty<float>("empty_val"));
}

TEST_F(TypedPropertyStorageTest, DataPersistsAfterRealloc) {
    storage.setProperty<int>("first", 123);

    for(int i = 0; i < 100; ++i) {
        storage.setProperty<glm::mat4>("mat" + std::to_string(i), glm::mat4(float(i)));
    }
    
    EXPECT_EQ(storage.getProperty<int>("first"), 123);
}