# How to build

### Install dependencies

```bash
# Fedora/RHEL
sudo dnf update
sudo dnf install cmake clang glfw glfw-devel glm-devel libpng-devel libuuid-devel

# Ubuntu/Debian
sudo apt update
sudo apt-get install cmake clang libglfw3 libglfw3-dev libglm-dev libpng-dev uuid-dev
```

### Install GLAD

1. Go to https://glad.dav1d.de/
2. Go to API section, choose GL version 4.6
3. Press "Generate"
4. Download glad.zip 
5. Unzip it to lib:

```
./lib
└── glad
    ├── include
    └── src
```

### Install Assimp

1. Go to [assimp repo](https://github.com/assimp/assimp)
2. Go to Releases
3. Choose 6.0.2 release and place it into lib folder (just like you did GLAD)

### Install BVH

1. Go to [BVH repo](https://github.com/madmann91/bvh/tree/v1?tab=readme-ov-file)
2. Download the source
3. Rename its folder to "bvh"
4. Put it into /src/lib

### Build using clang

```bash
> export CC=clang
> export CXX=clang++
> cmake .
> cmake --build .
```

# How to build tests

1. Install BVH to tests/lib/bvh
2. Run cmake with `-DBUILD_TESTS=ON`:

```bash
> cmake . -DBUILD_TESTS=ON
> cmake --build .
```
