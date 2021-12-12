# PA11: Penguin Bowling

## Team Members:
Justin Howell - has repository with working Project 11
Elizabeth Kish
Noah Doddridge

## CMake and Building

To build this project, run in the PA11/build folder:

```bash
cmake ..
```

Then call the Makefile with:

```bash
make
```

## Running

To run this project after building, run the following command in the PA11/build folder:

```bash
./Tutorial -v vert.glsl -f frag.glsl
```

See below for shader flag instructions

Do not run executable from outside of PA11/build folder, file paths to model and shader folder are hard coded in!

## Bowling Controls
Instructions:
You have 5 rounds, with 2 balls each round to knock as many penguins into your home as possible to eat later.

Controls:
A - Move ball left
D - Move ball right
Q - Turn ball left
E - Turn ball right
W - Hold to launch the ball, the longer held the more speed the ball will launch with
S - After throwing the ball, press S to recall the ball and move to the next stage

## Models
Models (stored as .obj), .mtl, and texture images, should be placed in the models folder in the PA11 project folder.

In the PA11/build folder, a config is used with .obj file names of each object in the scene.

Currently ONLY runs with .obj's with defined .mtl files and texture files, all saved to the /models folder.

## Shaders

Shader flags:
-v: define the filename for vertex shader

-f: define the filename for fragment shader

The shader files are default named vert.glsl and frag.glsl, and are stored in the shaders folder, vert_light.glsl and frag_light.glsl are the fragment lighting shaders, and are currently hard coded in.

