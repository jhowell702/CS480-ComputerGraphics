# PA4: Spinning Cube

## Team Members:
Justin Howell - has repository with working Project 5
Elizabeth Kish
Noah Doddridge

## CMake and Building

To build this project, run in the PA4/build folder:

```bash
cmake ..
```

Then call the Makefile with:

```bash
make
```

## Running

To run this project after building, run the following command in the PA4/build folder:

```bash
./Tutorial -v vert.glsl -f frag.glsl -m {replace_me_with_model_filename.obj}
```

See below for shader and model instructions

Do not run executable from outside of PA4/build folder, file paths to model and shader folder are hard coded in!

## Models
Models should be placed in the models folder in the PA4 project folder.

At runtime run the program using the flag -m followed by the file name of the model to be loaded.

By default, I've included bunny.obj, teapot.obj, and box.obj in the models folder, and can be safely passed to command line as parameters.

## Shaders

Shader flags:
-v: define the filename for vertex shader

-f: define the filename for fragment shader

The shader files are default named vert.glsl and frag.glsl, and are stored in the shaders folder.
