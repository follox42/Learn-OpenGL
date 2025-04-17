# Learn OpenGL
This repository contains my personal experiments and firest approch with openGL in c++.

## Goals

- Understand how the graphics pipeline works.
- Learn to use shaders
- Practice rendering in 2d and 3d
- Use matrix and vectors
> Basically learn how modern graphics computation works nowadays

## Steps
### Setup
1. **Install GLFW**  
    GLFW is a library that will take of creating a window in the os we are currently using so we can focus more on opengl than creating the right window for our os.
    But i'll probably learn how to create my own window with the api of the os later for linux and windows.
2. **Install GLEW**  
	Glew is a library that will take care of loading the opengl functions for us. It will load the functions that are not available in the opengl version we are using.
3. **Buffers**  
    Buffers are basically just an array or any storage. The idea is to first create the buffer with *glGenBuffers(number_of_buffer, &uint buffer)* and it will assicate a buffer and an id.  
    Then link it with a type *glBindBuffer(buffer_type, buffer)*.  
    Finally add the data *glBufferData(gltype_of_data, size_in_bytes, array, gl_hint)* the hint is like dynamix or static to help run faster if we modify the data or not.
    Last information OpenGL work in a context system that mean we dont have an object or anything if we are working on the buffer we can juste now draw it and it will now we are on it,  
    It's like selecting the object with bind.
4. **Attributes and Layout**  
    We have to define now what are called "attributes". An attribute is juste an information define by a vector about our object, it can be the position but also the colors  
    texture, normal. And we use index, for exemple the index 0 are the positions, 1 the colors ect... The size is for how many values we have per vertex for exemple in 2d the position is 2 values.  
    we set the type of the data and the normalize is if we want to let opengl normalize between 0 and 1. Stride is the size in bytes between to vertex in 2d it would be 2 * sizeof(position_type).  
    The pointer is a pointer into the attributes, the pointer is a real pointer to the next attributes in bytes. The line is *glVertexAttribPointer(index, size, gl_type, normalize(true, false), stride, pointer)*  
    And finaly we have to enable the atribute just use *glEnableVertexAttribArray(gl_object, index)*
5. **Shaders**  
    A shader is is like a program that run on the gpu. There is different type of shaders like the vertex shader that we'll calculate the position of our vertex positions, it runs for each vertex position. The fragment shader will render the pixel  
    and fill the triangle or do calculate the colors, it is useful with light, it runs for each pixels.  
    
> This is a learning project. Some folders may be unfinished or experimental.
