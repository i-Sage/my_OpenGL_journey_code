# OpenGL

OpenGl is mainly considered an API that provides us with a
large set of function that we can use to manipulate graphics
and images. However, NOTE that OpenGL by itself is not an API,
but merely a specification, developed and maintained by that
the [khronos](www.khronos.org)

The OpenGL specification specifies exactly what the resulting output of each function should be and how it should perform. It is then up to you the developer implementing this specification to come up with a solution of how this function should operate.

The people actually developing the actual OpenGL libraries are usually the graphics card manufactures. Each graphic card that you buy supports a specific version of OpenGL which are the versions of OpenGL developed specifically for that card (series). When using an Apple system, the OpenGL library is maintained by Apple themselves and under Linux, there exist a combination of graphics suppliers versions and hobbyist adaptions of these libraries.

> Since most implementations are build by graphics manufacturers. When there is a bug in the implementation this is usually solved by updating your video card drivers; those drives include the newest versions of OpenGL that your supports.This is one of the reasons why it's always advised to occassionally update your graphics card


OpenGL's core-profile: This is the division of OpenGL's specifications that removed all old deprecated functionality.

## State Machine

**OpenGL is by itself a large state machine**:
A collection of variables that define how OpenGL should
currently operate. The state of OpenGL is commonly referred
to as the *OpenGL Context*. **When using OpenGL, we often change its state by setting some options, Manipulating some buffers and then rendering the current context.**

Whenever we tell OpenGL that we now want to draw lines instead
of triangles for example, we change the state of OpenGL by changing some context variable that sets how OpenGL should draw lines, the next drawing commands will now draw lines instead of triangles.

## Objects

An **object** in OpenGL is a collection of options that represent a subset of OpenGL's state. For example, we could have an object that represents the settings of the drawing window; we could then set its size, how many colors it supports
and so on. One could visualize an object as a C-like struct

    ```C
        struct Object_name {
            float  option1;
            int    option2;
            char[] name;
        }
    ```
Whenever we want to use the objects it generally looks something like this (with OpenGL's contex visualized as a large struct):

    ```C
        struct OpenGL_Context {
            // ...
            object_name *objet_Window_Target;
            // ...
        }
    ```

    ```C
        // creat object
        unsigned int objectId = 0;
        glGenObject(1, &objectId);
        // bind/assign object to context
        glBindObject(GL_WINDOW_TARGET, objectID);
        // set options of object currently bound
        // to GL_WINDOW_TARGET
        glSetObjectOption(GL_WINDOW_TARGET, GL_OPTION_WINDOW_WIDTH, 800);
        glSetObjectOption(GL_WIDOW_TARGET, GL_OPTION_WIDOW_HEIGHT, 600);
        // set context target back to default
        glBindObject(GL_WINDOW_TARGET, 0);
    ```

> The code samples provided so far are only apprximations of
> how OpenGL operates.

The great thing about these objects is tha we can ca define more than one object in our application, set their option and whenever we start an operation that uses OpenGL's state, we bind the object with our preferred settings.


## GLFW

GLFW is a library written in C, specifically targeted at OpenGL. GLFW gives us the bare neccessities required for rendering goodies to the screen. It allows us to creat an OpenGL context, define window parameters, and handle user input.

## GLAD

the GLAD file is generally referred to as the OpenGL loading library which generally loads the various pointers, to the various types of the OpenGL functins.


## OVERVIEW OF THE PIPELINE

---

### Input 

As input to the graphics pipeline we pass in a list of three 3D coordinates that should 
form a triangle in an array here called **Vertex Data**; this vertex data is a collection of vertices. A vertex is a collection of data per 3D coordinate. This vertex's data is represented using **Vertex attributes** that can contain any data we'd like, but for simplicity's sake, let's assume that each vertex consist of just a 3D position and some color value.

In order for OpenGL to know what to make of your collection of coordinates and color values OpenGL requires you to hint what kind of render types you want to form with the data. Do we want the data rendered as a collection of points, a collection of triangles or perhaps just one long line? Those hints are called primitives and are given to OpenGL while calling any of the drawing commands. Some of these hints are **GL_POINTS**, **GL_TRIANGLES** and **GL_LINE_STRIP**.


### Vertex Shader

The first part of the  pipe line is the **vertex shader** that takes as input a single vertex. The vertex shader is run for each vertex passed into the pipe line. The main purpose of the vertex shader is to **transform 3D coordinates to other 3D coordinates**, the vertex shader also allows us to perform some basic processing on vertex attributes.

### Primittive Assembly

The primitive assembly stage takes as input all the vertices (or vertex if GL_POINTS is chosen) from the vertex shader that form a primitive and assembles all the point(s) in theprimitive shape given; in this case a triangle.

### Geometry Shader.

The output of the primitive assembly stage is passed to the geometry shader. The geometry shader takes as input a collection of vertices from a primitive and has the ability to generate other shapes by emitting new vertices to from new (or other) primitives(s).

### Rasterizer

The output of the geometry shader is then passed onto the Rasterization stage where it maps the resulting primitive(s) to othe corresponding pixels on the final screen, resulting in fragments for the fragment shader to use. Before the fragment shaders run, clipping is performed. Clipping discards all fragments that are outside your view, increasing performance.

> A fragment in OpenGL is all the data required for OpenGL to render a single pixel.

### Fragment Shader 

 The main purpose of the fragment shader is to calculate the final color of a pixel and this is usually the stage where all the advanced OpenGL effects occur. Usually, the fragment shader contains data about the 3D scene that it can use to calculate the final pixel color (like lights, shadows, color of the light and so on).


## SHADERS 

Shaders are little programs that rest on the GPU. These programs are run for each specific section of the graphics pipeline. In a basic sense, shaders are nothing more than programs that transform inputs to outputs. Shaders are also very isolated programs in that they're not allowed to communicate with each other; the only communication they have is via their inputs and outputs.

### GLSL 

Shaders are written in a C-like language GLSL. GLSL is tailored for use with graphics and contain useful specifically targeted at vector and matrix multiplication.

Shader always begin with a **version declaration**, followed by a **list of input and output variables, uniforms and its main function** where we process any input variables and output the result in its ouput variables.

A typical shader has the following structure:

```glsl
#version version_number

in type in_variable_name;
in type in_varialble_name;

out type out_variable_name;

uniform type uniform_name;

void main() {
    // process input(s) and do some weird graphics stuff
    //---
    // output processed stuff to output variables 
    out_variable_name = weird_stuff_we_processed;
}
```

When we're talking specifically about the vertex shader each input variable is also known as a **vertex attribute**. There are a maximum number of vertex attributes we're allowed to declare limited by hardware. OpenGL guarantees there are always at least 16 4-component vertex attributes available, but some hardware may allow for more which you can retrieve by querying **GL_MAX_VERTEX_ATTRIBS**:

```cpp
int num_of_attributes;
glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &num_of_attributes);
std::cout << "Max num of attributes supported: " << num_of_attributes << std::endl;
```

### Types

- int
- float 
- double
- uint 
- bool.

GLSL also features two container types, namely: 
- vectors
- matrices

#### Vectors

A vector in GLSL is a 1, 2, 3 or 4 component container for any of the basic types just mentioned. They can take the following from (n represents, the number of components):
- vecn : the default vector with n floats
- bvecn: a vector of n booleans
- ivecn: a vector of n integers
- uvecn: a vector of n unsigned integers
- dvecn: a vector of n double components

Components of a vector can be accessed via vec.x where x is the first component of the vector. You can use .x, .y, .z and .2 to access their first, second, third and fourth component respectively. GLSL also allows you to use rgba for colors or stpq for texture coordinates, accessing the same component.

The vector datatype allows for some interesting and flexible component selection called **swizzling**. Swizzling allows us to use syntax like this:

```glsl
vec2 some_vec;
vec4 different_vec = some_vec.xyxx;
vec3 another_vec   = different_vec.zyw;
vec4 other_vec     = some_.xxxx + another_vec.yxzy;
```

We can also pass vectors as arguments to different vector constructors calls, reducing the number of arguments required:

```glsl
vec2 vect = vec2(0.5, 0.7);
vec4 result = vec4(vect, 0.0, 0.0);
vec4 other_result = vec4(result.xyz, 1.0);
```

#### ins and outs

GLSL defines **in** and **out** keywords specifically for the purpose of moving stuff around. Each shader can specify inputs and outputs using those keywords and whenever an output variable matches with an input variable of the next shader stage they're passed along. The vertex and fragment shader differ a bit though.

The vertex shader **should** receive some form of input otherwise it would be pretty ineffective. The vertex shader differs in its input, in that it receives its input straight from the vertex data. To define how the vertex data is organized we specify the input variables with location metadata so we can configure the vertex attributes on the CPU. We've seen this in the previous chapters as **layout (location = 0)**. The vertex shader thus requires an extra layout specification for its inputs so we can link it with the vertex data.

The other exception is that the fragment shader requires a vec4 color output variable, since the fragment shader needs to generate a final output color. If you fail to specify an output color in your fragment shader, the color buffer output for those fragments will be undefined(which means OpenGL will render them either black or white).

SUMMARY: so if we want to send data from one shader to the other we'd have to declare an output in the sending shader and an input in the receiving shader. When the types and the names are equal on both sides, OpenGL will link those variables together and then it is possible to send data between shaders (this is done when linking a program object)

EXAMPLE: 

- vertex shader: 

```glsl
#version 450 core
layout (location = 0) in vec3 a_pos;   // positon has attribute position 0.

out vec4 vertex_color; // specify a color output to the fragment shader.

void main() {
    gl_Position = vec4(a_pos, 1.0) // we give a vec3 to vec4's construction.
    vertex_color = vec4(0.5, 0.0, 0.0, 1.0); // ouput variable to dark-red
}
```

- fragment shader:

```glsl
#version 450 core
out vec4 frag_color;

in vec4 vertex_color; // input variable form vertex shader (same name and type)

void main() {
    frag_color = vertex_color;
}
```

#### Uniforms

Uniforms are anther way to pass data from out application on the CPU to shader on the GPU. Uniforms are however slightly different compared to vertex attributes. First of all, uniforms are **global**. Global, meaning that a uniform variable is unique per shader program object, and can be accessed from any shader at any stage in the shader program. Second, whatever you set the unifrom value to , uniforms will keep their value until they are either reset or updated.

To declare a uniform in GLSL, we simply add the **uniform** keyword to a shader with a type and a name. From that point on we can use the newly declared uniform in the shader.

NOTE: if you declare a unifrom that is not used anywhere in your GLSL code the compiler will silently remove the variable from the compiled version which is the cause for several frustrating errors; keep this in mind!
