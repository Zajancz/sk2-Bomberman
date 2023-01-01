# sk2-Bomberman

## To compile project for godot engine:

following: https://docs.godotengine.org/en/stable/tutorials/scripting/gdnative/gdnative_cpp_example.html

Setup
```shell
# git submodule add -b 3.2 https://github.com/godotengine/godot-cpp
git submodule update --init --recursive
scons platform=linux generate_bindings=yes
```

> **Note** If you experience compilation errors, make sure the godot headers are under `godot-cpp/godot-headers`. In some versions this headers might be under `godot-cpp/godot_headers`. You may need to adjust version of the submodule or fix the `SConstruct`.

Compile project to a shared object `.so`
```shell
scons platform=linux
```

## To compile project with `cmake` run:

```shell
cmake .
make
```