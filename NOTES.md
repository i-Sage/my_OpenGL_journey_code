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
