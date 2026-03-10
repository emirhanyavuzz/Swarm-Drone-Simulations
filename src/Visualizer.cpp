#include "Visualizer.h"
#include <iostream>

Visualizer::Visualizer(int width, int height, const std::string& title) 
    : m_width(width), m_height(height) 
{
    InitWindow(width, height, title.c_str());
    SetTargetFPS(60);
}

Visualizer::~Visualizer() {
    close();
}

void Visualizer::close() {
    if (IsWindowReady()) {
        CloseWindow();
    }
}

bool Visualizer::shouldClose() const {
    return WindowShouldClose();
}

void Visualizer::beginDrawing() {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    
    // Draw grid
    const int gridSize = 40;
    for (int i = 0; i <= m_width / gridSize; i++) {
        DrawLine(i * gridSize, 0, i * gridSize, m_height, LIGHTGRAY);
    }
    for (int i = 0; i <= m_height / gridSize; i++) {
        DrawLine(0, i * gridSize, m_width, i * gridSize, LIGHTGRAY);
    }
    
    // Draw center axes
    DrawLine(m_width / 2, 0, m_width / 2, m_height, GRAY);
    DrawLine(0, m_height / 2, m_width, m_height / 2, GRAY);
}

void Visualizer::endDrawing() {
    EndDrawing();
}

Vector2 Visualizer::worldToScreen(const Vec3& pos) const {
    // Basic mapping: 
    // Origin is center
    // Scale: 1 unit in sim = 20 pixels
    const float scale = 20.0f;
    return {
        m_width / 2.0f + static_cast<float>(pos.x) * scale,
        m_height / 2.0f - static_cast<float>(pos.y) * scale // Y is inverted in screen coords
    };
}

void Visualizer::drawDrones(const Swarm& swarm) {
    auto drones = swarm.getDrones();
    for (const auto& drone : drones) {
        if (!drone.isActive()) continue;
        
        Vector2 screenPos = worldToScreen(drone.getPosition());
        
        // Draw Drone circle (Body)
        DrawCircleV(screenPos, 8.0f, BLUE);
        DrawCircleLines(screenPos.x, screenPos.y, 8.0f, DARKBLUE);
        
        // Draw ID text
        DrawText(TextFormat("%d", drone.getId()), screenPos.x + 10, screenPos.y - 10, 10, BLACK);
    }
}

void Visualizer::drawInfo(const std::string& simName, int step, int maxSteps) {
    DrawRectangle(10, 10, 300, 70, Fade(SKYBLUE, 0.5f));
    DrawRectangleLines(10, 10, 300, 70, BLUE);
    
    DrawText(simName.c_str(), 20, 20, 20, DARKBLUE);
    DrawText(TextFormat("Step: %d / %d", step, maxSteps), 20, 50, 16, BLACK);
}
