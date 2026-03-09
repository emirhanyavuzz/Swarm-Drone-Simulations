#pragma once
#include "Simulation.h"
#include <cmath>

// ============================================================
//  Grid (Izgara) Formation
//  Dronelar N x M dikdortgen grid seklinde dizilir.
//  Genis alan taramasi, haritalama ve arama-kurtarma icin idealdir.
// ============================================================
class GridFormation : public Simulation {
public:
    GridFormation() : m_complete(false), m_spacing(5.0) {}

    std::string getName() const override {
        return "Izgara Formasyonu (Grid Formation)";
    }

    std::string getDescription() const override {
        return "Dronelar NxM grid/izgara seklinde dizilir - alan taramasi";
    }

    void setup(Swarm& swarm) override {
        m_complete = false;
        int n = swarm.getDroneCount();

        // Calculate grid dimensions (as close to square as possible)
        int cols = static_cast<int>(std::ceil(std::sqrt(n)));
        int rows = static_cast<int>(std::ceil(static_cast<double>(n) / cols));

        // Center the grid around origin
        double offsetX = (cols - 1) * m_spacing / 2.0;
        double offsetZ = (rows - 1) * m_spacing / 2.0;
        double altitude = 10.0;

        for (int i = 0; i < n; ++i) {
            int row = i / cols;
            int col = i % cols;

            Vec3 target(
                col * m_spacing - offsetX,
                altitude,
                row * m_spacing - offsetZ
            );
            swarm.getDrone(i).setTarget(target);
        }
    }

    void update(Swarm& swarm, double dt) override {
        swarm.update(dt);
        m_complete = swarm.allReachedTarget(0.3);
    }

    bool isComplete() const override {
        return m_complete;
    }

private:
    bool m_complete;
    double m_spacing;
};
