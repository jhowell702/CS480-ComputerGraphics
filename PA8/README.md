# PA8: Bullet

## Team Members:
Justin Howell - has repository with working Project 8
Elizabeth Kish
Noah Doddridge

## CMake and Building

To build this project, run in the PA8/build folder:

```bash
cmake ..
```

Then call the Makefile with:

```bash
make
```

## Running

To run this project after building, run the following command in the PA6/build folder:

```bash
./Tutorial -v vert.glsl -f frag.glsl
```

See below for shader flag instructions

Do not run executable from outside of PA8/build folder, file paths to model and shader folder are hard coded in!

## Pinball Controls
Tapping the WASD keys will apply a force to the cube in given direction. The sphere will move, but the cylinder is a static object.

## Models
Models (stored as .obj), .mtl, and texture images, should be placed in the models folder in the PA8 project folder.

In the PA8/build folder, a config is used with .obj file names of each object in the scene.

Currently ONLY runs with .obj's with defined .mtl files and texture files, all saved to the /models folder.

## Shaders

Shader flags:
-v: define the filename for vertex shader

-f: define the filename for fragment shader

The shader files are default named vert.glsl and frag.glsl, and are stored in the shaders folder.

### For further documentation of the code, primary edits and documentation were made to graphics.cpp, engine.cpp, object.cpp, and camera.cpp since the previous project.
