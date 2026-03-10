#pragma once
#include "Swarm.h"
#include <raylib.h>
#include <string>

class Visualizer {
public:
    Visualizer(int width = 800, int height = 800, const std::string& title = "Swarm Drone Simulation");
    ~Visualizer();

    void beginDrawing();
    void endDrawing();
    bool shouldClose() const;
    void close();

    void drawDrones(const Swarm& swarm);
    void drawInfo(const std::string& simName, int step, int maxSteps);

private:
    int m_width;
    int m_height;
    
    // Helper to transform simulation coords (Vec3) to screen coords (Vector2)
    Vector2 worldToScreen(const Vec3& pos) const;
};
