#pragma once

namespace ocf {

class Renderer {
public:
    Renderer();
    ~Renderer();

    bool init();

    void beginFrame();

    void endFrame();

    void clear();

    void draw();
};

} // namespace ocf
