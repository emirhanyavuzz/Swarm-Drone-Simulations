#pragma once
#include "Simulation.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ============================================================
//  Saturation Attack (Doyurma Saldirisi)
//  Cok sayida drone farkli yonlerden ayni hedefe esmanli
//  yakinsar. Amac: savunma kapasitesini asmak.
//  - Dronelar hedef etrafinda genis bir daire uzerinde baslar
//  - Tumu ayni anda hedefe dogru hareket eder
//  - Time-on-target: hepsi ayni anda varir
// ============================================================
class SaturationAttack : public Simulation {
public:
    SaturationAttack()
        : m_complete(false)
        , m_target(0, 5, 0)
        , m_orbitalRadius(50.0)
        , m_phase(Phase::DEPLOY)
        , m_deployTimer(0)
    {}

    std::string getName() const override {
        return "Doyurma Saldirisi (Saturation Attack)";
    }

    std::string getDescription() const override {
        return "Farkli yonlerden es zamanli hedefe yaklasma - savunma doyurma";
    }

    void setup(Swarm& swarm) override {
        m_complete = false;
        m_phase = Phase::DEPLOY;
        m_deployTimer = 0;
        int n = swarm.getDroneCount();

        std::cout << "[HEDEF] Merkez: " << m_target << "\n";
        std::cout << "[FAZ 1] Dronelar orbital pozisyonlara dagitiliyor...\n\n";

        // Deploy drones in a circle around the target at orbital radius
        for (int i = 0; i < n; ++i) {
            double angle = (2.0 * M_PI * i) / n;
            double x = m_target.x + m_orbitalRadius * std::cos(angle);
            double z = m_target.z + m_orbitalRadius * std::sin(angle);
            double altitude = 10.0 + (i % 3) * 5.0; // varied altitudes

            Vec3 orbitalPos(x, altitude, z);
            swarm.getDrone(i).setTarget(orbitalPos);
            swarm.getDrone(i).setMaxSpeed(12.0); // fast approach
        }
    }

    void update(Swarm& swarm, double dt) override {
        int n = swarm.getDroneCount();

        switch (m_phase) {
            case Phase::DEPLOY: {
                swarm.update(dt);
                if (swarm.allReachedTarget(1.0)) {
                    m_phase = Phase::CONVERGE;
                    std::cout << "[FAZ 2] Tum dronelar orbital pozisyonda!\n";
                    std::cout << "         Es zamanli saldiri baslatiliyor...\n\n";

                    // Calculate time-on-target: all drones converge at same speed
                    // Find max distance to normalize speeds
                    double maxDist = 0;
                    for (int i = 0; i < n; ++i) {
                        double dist = swarm.getDrone(i).getPosition().distanceTo(m_target);
                        if (dist > maxDist) maxDist = dist;
                    }

                    // Set targets and adjust speeds for simultaneous arrival
                    double baseSpeed = 15.0;
                    for (int i = 0; i < n; ++i) {
                        Drone& d = swarm.getDrone(i);
                        double dist = d.getPosition().distanceTo(m_target);
                        double ratio = dist / maxDist;
                        d.setMaxSpeed(baseSpeed * ratio); // slower drones are closer
                        d.setTarget(m_target);
                    }
                }
                break;
            }

            case Phase::CONVERGE: {
                swarm.update(dt);

                // Print convergence status periodically
                if (m_deployTimer % 15 == 0) {
                    double avgDist = 0;
                    for (int i = 0; i < n; ++i) {
                        avgDist += swarm.getDrone(i).getPosition().distanceTo(m_target);
                    }
                    avgDist /= n;
                    std::cout << "[YAKLASMA] Ortalama hedefe mesafe: "
                              << static_cast<int>(avgDist) << "m\n";
                }

                if (swarm.allReachedTarget(1.5)) {
                    m_phase = Phase::IMPACT;
                    std::cout << "\n[ETKI] Tum dronelar hedefe ulasti!\n";
                    std::cout << "       Savunma sistemi doyuruldu. (" << n << " drone)\n";
                }
                break;
            }

            case Phase::IMPACT:
                m_complete = true;
                break;
        }
        ++m_deployTimer;
    }

    bool isComplete() const override {
        return m_complete;
    }

private:
    enum class Phase { DEPLOY, CONVERGE, IMPACT };

    bool m_complete;
    Vec3 m_target;
    double m_orbitalRadius;
    Phase m_phase;
    int m_deployTimer;
};
