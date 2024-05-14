# COMP3015 OpenGL Scene Editor

<p align="center">
    <img src="https://github.com/emma-freeman-plym/comp3015-2/blob/main/preview.png?raw=true">
</p>

This project implements a level editor for designing 3D scenes using OpenGL.

## Features

- Object loading and property editing, such as position and scale

- Texture loading and rendering, with diffuse and overlay

- Lighting and material property editing, such as color and shininess

- Camera movement and rotation

- Vertex animation settings

- Debug wireframe view

- Scene saving and loading to a custom JSON format

## Usage

Using the scene editor is simple:

- Add objects and lights to the scene using the left-side panel

- Modify them using the right-side panel

- Save the scene

## Implementation

This project has been developed on Windows 11 using Visual Studio Community 2022 `17.9.3`.

The code is structured using the provided template:

- The `Level` class manages the scene and its objects, including saving to JSON

- The `Object`, `Light` and `Material` classes represent the scene entities

- The `SceneBasic_uniform` class controls rendering


## Dependencies

- [ImGui](https://github.com/ocornut/imgui) for drawing the UI

- [nlohmann/json](https://github.com/nlohmann/json) for loading and saving scenes

- [tinyfiledialogs](https://sourceforge.net/projects/tinyfiledialogs/) for cross-platform file selection dialogs
