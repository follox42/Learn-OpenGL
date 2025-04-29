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
<<<<<<< HEAD
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
6. **Program**  
    Just to complete the shader part, when you create a shader you link the source and then link it to a program.  
7. **Index**  
    To avoid repeating ourself the triangles that are connecting, the 2 shared verteces. We don't want to store all the same verteces and use extra memory.  
    For this we will juste store unique verteces so 4 for exemple for a rectangle. And we will use an index buffer to link the verteces together. And refer with their index instead of row verteces.  
    And we will use the glDrawElements instead of glDrawArrays. An exemple: *positions[] = {vertex 0,vertex 1, vertex 2, vertex 3}* and then *indices[] = {0, 1, 2, 3, 0, 2}*.
=======
1. **Install GLFW.**  
   GLFW is a library that will take of creating a window in the os we are currently using so we can focus more on opengl than creating the right window for our os.
   But i'll probably learn how to create my own window with the api of the os later for linux and windows.
>>>>>>> e5f91f47527bafeda956cd6c6cb1f27be399274d

> This is a learning project. Some folders may be unfinished or experimental.
