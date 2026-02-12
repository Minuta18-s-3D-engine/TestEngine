#include <gtest/gtest.h>
#include "engine/project/PathResolver.hpp"

class PathResolverTest : public ::testing::Test {
protected:
    std::filesystem::path testRoot = "/home/user/project";
    std::filesystem::path testCoreRoot = "/home/user/TestEng/core";
    
    std::unique_ptr<PathResolver> resolver;

    void SetUp() override {
        resolver = std::make_unique<PathResolver>(testRoot, testCoreRoot);
    }
};

TEST_F(PathResolverTest, ResolvesVirtualPrefix) {
    std::string virtualPath = "fs://textures/grass.png";
    std::filesystem::path expected = testRoot / "textures/grass.png";
    
    EXPECT_EQ(resolver->resolve(virtualPath), expected);
}

TEST_F(PathResolverTest, ResolvesRelativePath) {
    std::string relPath = "scripts/main.lua";
    std::filesystem::path expected = testRoot / "scripts/main.lua";
    
    EXPECT_EQ(resolver->resolve(relPath), expected);
}

TEST_F(PathResolverTest, HandlesAbsolutePath) {
#ifdef _WIN32
    std::string absPath = "C:/Windows/system32";
#else
    std::string absPath = "/etc/configs/settings.json";
#endif
    std::filesystem::path expected(absPath);
    
    EXPECT_EQ(resolver->resolve(absPath), expected);
}

TEST_F(PathResolverTest, HandlesEmptyPath) {
    EXPECT_EQ(resolver->resolve(""), "");
}

TEST_F(PathResolverTest, ResolvesComplexVirtualPath) {
    std::string virtualPath = "fs://assets/models/player/skin.fbx";
    std::filesystem::path expected = testRoot / "assets/models/player/skin.fbx";
    
    EXPECT_EQ(resolver->resolve(virtualPath), expected);
}
