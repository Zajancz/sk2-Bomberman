# sk2-Bomberman

## To compile project for godot engine:

following: https://docs.godotengine.org/en/stable/tutorials/scripting/gdnative/gdnative_cpp_example.html

Setup
```shell
git submodule add -b 3.x https://github.com/godotengine/godot-cpp
cd godot-cpp
git submodule update --init --recursive
```

Compile project to a shared object `.so`
```shell
scons platform=linux
```

## To compile project with `cmake` run:

```shell
cmake .
make
```