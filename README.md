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
    
> This is a learning project. Some folders may be unfinished or experimental.
