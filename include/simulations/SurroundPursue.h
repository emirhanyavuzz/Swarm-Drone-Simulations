#pragma once
#include "Simulation.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ============================================================
//  Surround & Pursue (Takip ve Kusatma)
//  Hareketli bir hedefi her yonden sararak kacis yolunu kapatan
//  dinamik ucus sekli.
//  - Hedef 8-seklinde veya rastgele rota izler
//  - Dronelar hedef etrafinda esit acili dagitilir
//  - Kusatma yaricapi zamanla daralir
// ============================================================
class SurroundPursue : public Simulation {
public:
    SurroundPursue()
        : m_complete(false)
        , m_surroundRadius(25.0)
        , m_minRadius(5.0)
        , m_shrinkRate(0.08)
        , m_step(0)
        , m_maxSteps(400)
        , m_targetCaptured(false)
    {}

    std::string getName() const override {
        return "Takip ve Kusatma (Surround & Pursue)";
    }

    std::string getDescription() const override {
        return "Hareketli hedefi kusatarak kacis yolunu kapatma";
    }

    void setup(Swarm& swarm) override {
        m_complete = false;
        m_step = 0;
        m_surroundRadius = 25.0;
        m_targetCaptured = false;
        m_targetPos = Vec3(0, 10, 0);

        int n = swarm.getDroneCount();

        std::cout << "[HEDEF] Baslangic konumu: " << m_targetPos << "\n";
        std::cout << "[KUSATMA] " << n << " drone ile kusatma baslatiliyor.\n";
        std::cout << "[KUSATMA] Yaricap: " << m_surroundRadius
                  << "m -> " << m_minRadius << "m\n\n";

        // Place drones far from target initially
        for (int i = 0; i < n; ++i) {
            double angle = (2.0 * M_PI * i) / n;
            Vec3 startPos(
                m_targetPos.x + 40.0 * std::cos(angle),
                10.0,
                m_targetPos.z + 40.0 * std::sin(angle)
            );
            swarm.getDrone(i).setPosition(startPos);
            swarm.getDrone(i).setMaxSpeed(14.0);
        }
    }

    void update(Swarm& swarm, double dt) override {
        ++m_step;
        int n = swarm.getDroneCount();

        // Move the target in a figure-8 pattern
        double t = m_step * dt;
        m_targetPos.x = 20.0 * std::sin(t * 0.5);
        m_targetPos.z = 10.0 * std::sin(t * 1.0);
        m_targetPos.y = 10.0;

        // Shrink the surround radius
        if (m_surroundRadius > m_minRadius) {
            m_surroundRadius -= m_shrinkRate * dt;
            if (m_surroundRadius < m_minRadius) {
                m_surroundRadius = m_minRadius;
            }
        }

        // Position drones equally spaced around the target
        for (int i = 0; i < n; ++i) {
            if (!swarm.getDrone(i).isActive()) continue;

            double angle = (2.0 * M_PI * i) / n + t * 0.2; // slow rotation
            Vec3 surroundPos(
                m_targetPos.x + m_surroundRadius * std::cos(angle),
                m_targetPos.y,
                m_targetPos.z + m_surroundRadius * std::sin(angle)
            );
            swarm.getDrone(i).setTarget(surroundPos);
        }

        swarm.update(dt);

        // Status reporting
        if (m_step % 30 == 0) {
            std::cout << "[HEDEF] Konum: " << m_targetPos << "\n";
            std::cout << "[KUSATMA] Yaricap: "
                      << static_cast<int>(m_surroundRadius * 10) / 10.0 << "m\n";

            // Measure average distance of drones to target
            double avgDist = 0;
            for (int i = 0; i < n; ++i) {
                avgDist += swarm.getDrone(i).getPosition().distanceTo(m_targetPos);
            }
            avgDist /= n;
            std::cout << "[KUSATMA] Ort. mesafe hedefe: "
                      << static_cast<int>(avgDist * 10) / 10.0 << "m\n\n";
        }

        // Check capture condition
        if (m_surroundRadius <= m_minRadius + 0.5) {
            bool allClose = true;
            for (int i = 0; i < n; ++i) {
                if (swarm.getDrone(i).isActive() &&
                    swarm.getDrone(i).getPosition().distanceTo(m_targetPos) > m_minRadius + 5.0) {
                    allClose = false;
                    break;
                }
            }
            if (allClose) {
                if (!m_targetCaptured) {
                    m_targetCaptured = true;
                    std::cout << "\n[YAKALANDI] Hedef basariyla kusatildi!\n";
                    std::cout << "            Kusatma yaricapi: "
                              << m_surroundRadius << "m\n";
                }
                m_complete = true;
            }
        }

        if (m_step >= m_maxSteps) {
            m_complete = true;
        }
    }

    bool isComplete() const override {
        return m_complete;
    }

private:
    bool m_complete;
    double m_surroundRadius;
    double m_minRadius;
    double m_shrinkRate;
    int m_step;
    int m_maxSteps;
    bool m_targetCaptured;
    Vec3 m_targetPos;
};
