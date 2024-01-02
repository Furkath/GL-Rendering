# Project Title

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
cd run
source source_to_run
cd ../postplot
python newplot.py
```
## Demo
![Vmag_demo](https://github.com/Furkath/TVD_Fluid_Simulator/blob/master/results/320_Re1000_UMIST/pic_mag.png)
