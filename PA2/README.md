# PA2: Spinning Cube

## Building

To build this project, run in the PA3 project folder:

```bash
cmake .
make
```


## Running

To run this project after building, run in the PA3 project folder:

```bash
./Tutorial -v vert.glsl -f frag.glsl
```

The flags:
-v: define the filename for vertex shader
-f: define the filename for fragment shader

The shader files are default named vert.glsl and frag.glsl, and are stored in the shaders folder.

## Program Controls

When the program is running, the following keyboard controls are used to control the cube.

Left Arrow Key: Sets planet's rotation to be counterclockwise.

Right Arrow Key: Sets planet's rotation to be clockwise.

Up Arrow Key: Increases planet's rotation speed.

Down Arrow Key: Decreases planet's rotation speed.

A Key: Reverses rotation and spin direction.

Any Mouse Click: Reverses rotation and spin direction.



