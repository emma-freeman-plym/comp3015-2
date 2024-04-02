# COMP3015 OpenGL Scene

<p align="center">
    <img src="https://github.com/emma-freeman-plym/comp3015-2/blob/main/preview.png?raw=true">
</p>

This project implements a basic scene in OpenGL using shaders.

The scene contains a custom 3d model of a chess piece, along with textures and lighting.

## Features

- Custom modelled chess piece object with UV unwrapping.

- Diffusion marble texture from [here](https://3dtextures.me/2022/06/20/marble-white-008/).

- Overlay and opacity crack texture from [here](https://3dtextures.me/2021/10/23/surface-imperfections-cracks-001/).

- Multiple lights in the fragment shader.

- Animated rotating model and lights.

## Implementation

This project has been developed on Windows 11 using Visual Studio Community 2022 `17.9.3`.

The code is structured using the provided template:

- The `initScene` function compiles the shaders, sets the lighting and material uniforms, then loads textures.

- The `update` function increments a `rotation` variable, used for animation.

- The `render` handles animating the light by setting its uniform, as well as rotating and rendring the chess piece mesh.

- The `shader.h` file contains abstractions for lighting and material information structs, as well as methods to store their data into shader uniforms.

Within the fragment shader, the `phong` function calculates lighting information.
It is called with each `LightInfo` struct, and the results are summed.
The function mixes the base and opacity textures, then calculates the ambient, diffuse, and specular components to produce the final color.

