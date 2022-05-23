#include "Application.hpp"

int main() {

    Application app;
    if (app.setUp())
        app.run();

    return 0;
}

/*
    Pimpl Idiom: (out-of-line private construction and destruction)
    - [x] Renderer
    - [x] Shader
    - [x] Application
    - [x] Camera
    - [ ] Orthographic Camera
    - [x] Orbital Callbacks
    - [x] Box
    - [x] Plane
    - [x] Vertex Buffer

    Includes cleaned:
    - [x] Renderer
    - [x] Shader
    - [x] Application
    - [x] Camera
    - [ ] Orthographic Camera
    - [x] Orbital Callbacks
    - [x] Box
    - [x] Plane
    - [x] Vertex Buffer
    - [x] Vertex Buffered

    Copy Semantics
    - [x] Shader
    - [x] Camera
    - [ ] OrthographicCamera
    - [x] PerspectiveCamera
    - [x] VertexBuffer
    - [x] VertexBuffered
    - [x] Point
    - [x] Line
    - [x] Plane
    - [x] Box

    Move Semantics
    - [x] Shader
    - [x] Camera
    - [ ] OrthographicCamera
    - [x] PerspectiveCamera
    - [x] VertexBuffer
    - [x] VertexBuffered
    - [x] Point
    - [x] Line
    - [x] Plane
    - [x] Box
*/