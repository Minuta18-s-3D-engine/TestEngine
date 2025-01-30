# How to build

### Install dependencies

```bash
# Fedora/RHEL
sudo dnf update
sudo dnf install cmake clang glfw glfw-devel

# Ubuntu/Debian
sudo apt update
sudo apt-get install cmake clang libglfw3 libglfw3-dev
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

### Build using clang

```bash
> export CC=clang
> export CXX=clang++
> cmake .
> cmake --build .
```
