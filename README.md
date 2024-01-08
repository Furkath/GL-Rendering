# GL Rendering

A c++ OpenGL rendering practice, including various shaders, GLSL programming, illumination models; transparency, shadows; modeling, heterogeneous textures; camera and motion.

## Table of Contents

- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Demo](#demo)
<!-- [Contributing](#contributing) -->
<!-- [License](#license) -->
<!-- [Acknowledgements](#acknowledgements) -->

## Features

- A rendering practice based on OpenGL, including many basic items for classic pipelines:
- GLSL language of GPU programming for implementations of different shaders (vertex, fragment, geometric)
- Shadow mapping for quick stratum and shadow calculation
- Various illumination models (of local) and dual-sensitivity textures
- Semi-opaque light mixing for tansparent objects and perspective adjustment
- Graphic assistance for on-the-fly camera motion and model reading
- Supersampling and bias correction 

## Installation

<!-- Provide instructions on how to install and set up your project. Include any dependencies and steps needed for a successful installation. -->

```bash
# for Ubuntu and Debian-based
sudo apt install pkg-config freeglut3 libglew-dev libglfw3 libsoil-dev libglm-dev
cd build
cmake ../
make
```

## Usage

<!-- Explain how to use your project. Include examples or code snippets if applicable. -->
  
```bash
cp build/CGrender run/
cd run
./CGrender
```
## Demo
![demo1](https://raw.githubusercontent.com/Furkath/GL-Rendering/master/demos/test.png)
<!-- img src="https://github.com/Furkath/GL-Rendering/tree/master/demos/16-41-53.png" alt="demo1" width="360" height="202.5" / -->
<!-- img src="https://github.com/Furkath/GL-Rendering/tree/master/demos/15-24-34.png" alt="demo2" width="360" height="202.5" / -->
