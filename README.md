# Nocture Engine
A work-in-progress data-oriented (3D, but not yet!) game engine built from scratch in modern C++20.

Nocturne Engine is a learning project exploring low-level game engine design, featuring a custom Entity Component System and OpenGL rendering pipeline. Built to understand how modern game engines work under the hood.

## Current State
Beautiful orange square :)

<img width="720" height="360" alt="image" src="https://github.com/user-attachments/assets/5e2536ee-8847-4cfe-922d-628d658b8534" />

## Current Features
### Core Systems:
Custom Entity Component System (ECS) architecture
- Entities and Entity Manager/Factory
- Components and Component Manager/Factory
- Rendering System
- Resource Manager (sync loading, for now)

GLFW window management
- No input handling yet

Very simple, naive mesh rendering.

### Architecture Highlights
The engine follows data-oriented design principles
- Contiguous component storage for cache efficiency
- Separation of data (components) and behavior (systems)

## Building
Make sure to download included submodules with:
```sh
git submodule init
git submodule update
```
Then build with cmake for your preferred C++20 compatible compiler :)
```sh
cmake -B build
cmake --build build
```

### Requirements:
- C++20 compatible compiler
- CMake 3.15+
- OpenGL 3.3+ compatible GPU

Tested both on Linux(Arch) and Windows 11 using GCC and clang.
