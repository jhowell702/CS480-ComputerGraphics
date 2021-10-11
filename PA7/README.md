# PA6: Spinning Cube

## Team Members:
Justin Howell - has repository with working Project 6
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

To run this project after building, run the following command in the PA6/build folder:

```bash
./Tutorial -v vert.glsl -f frag.glsl -m {replace_me_with_model_filename.obj}
```

The following command uses the provided buddha.obj, .mtl, and texture files and should work as a demonstration.
```bash
./Tutorial -v vert.glsl -f frag.glsl -m buddha.obj
```

See below for shader and model flag instructions

Do not run executable from outside of PA6/build folder, file paths to model and shader folder are hard coded in!

## Models
Models should be placed in the models folder in the PA6 project folder.

At runtime run the program using the flag -m followed by the file name of the model to be loaded.

By default, I've included buddha.obj in the models folder, and can be safely passed to command line as parameters.

Currently ONLY runs with .obj's with defined .mtl files and texture files, all saved to the /models folder.

## Shaders

Shader flags:
-v: define the filename for vertex shader

-f: define the filename for fragment shader

The shader files are default named vert.glsl and frag.glsl, and are stored in the shaders folder.

### For further documentation of the code, primary edits and documentation were made to graphics.cpp and object.cpp since Project 5
