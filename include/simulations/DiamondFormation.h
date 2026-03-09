#pragma once
#include "Simulation.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ============================================================
//  Diamond (Elmas) Formation
//  Merkezdeki lider etrafinda elmas/baklava seklinde dizilim.
//  V formasyonunun simetrik, kapali versiyonudur.
// ============================================================
class DiamondFormation : public Simulation {
public:
    DiamondFormation() : m_complete(false), m_spacing(6.0) {}

    std::string getName() const override {
        return "Elmas Formasyonu (Diamond Formation)";
    }

    std::string getDescription() const override {
        return "Dronelar elmas/baklava seklinde simetrik dizilir";
    }

    void setup(Swarm& swarm) override {
        m_complete = false;
        int n = swarm.getDroneCount();
        Vec3 center(0, 10, 0);

        // Diamond shape: build concentric diamond rings
        // Ring 0: center (1 drone)
        // Ring 1: 4 drones (top, right, bottom, left)
        // Ring 2: 8 drones, etc.

        std::vector<Vec3> positions;
        positions.push_back(center); // ring 0

        int ring = 1;
        while (static_cast<int>(positions.size()) < n) {
            // Each ring has 4*ring positions
            for (int i = 0; i < 4 * ring && static_cast<int>(positions.size()) < n; ++i) {
                int side = i / ring;  // 0=top-right, 1=bottom-right, 2=bottom-left, 3=top-left
                int pos = i % ring;

                double x = 0, z = 0;
                switch (side) {
                    case 0: // top-right edge: from top to right
                        x = center.x + pos * m_spacing;
                        z = center.z + (ring - pos) * m_spacing;
                        break;
                    case 1: // right to bottom
                        x = center.x + (ring - pos) * m_spacing;
                        z = center.z - pos * m_spacing;
                        break;
                    case 2: // bottom to left
                        x = center.x - pos * m_spacing;
                        z = center.z - (ring - pos) * m_spacing;
                        break;
                    case 3: // left to top
                        x = center.x - (ring - pos) * m_spacing;
                        z = center.z + pos * m_spacing;
                        break;
                }
                positions.push_back(Vec3(x, center.y, z));
            }
            ++ring;
        }

        for (int i = 0; i < n; ++i) {
            swarm.getDrone(i).setTarget(positions[i]);
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
