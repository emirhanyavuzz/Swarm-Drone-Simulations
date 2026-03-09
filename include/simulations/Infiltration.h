#pragma once
#include "Simulation.h"
#include <cmath>
#include <vector>
#include <random>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ============================================================
//  Infiltration (Sizma) Simulation
//  Dronelar daginik ve dusuk irtifada ucarak, radar algilama
//  bolgelerinden (daire) kacinarak hedef noktaya sizarlar.
//  - Rastgele dagitilmis radar zonlari (daire) olusturulur
//  - Her drone waypoint bazli rota izler
//  - Radar zonlarina giren drone uyari alir
// ============================================================
class Infiltration : public Simulation {
public:
    Infiltration()
        : m_complete(false)
        , m_altitude(2.0)        // dusuk irtifa
        , m_targetZone(80, m_altitude, 0)
        , m_step(0)
    {}

    std::string getName() const override {
        return "Sizma (Infiltration)";
    }

    std::string getDescription() const override {
        return "Dusuk irtifa + radar kacinma ile hedef bolgeye sizma";
    }

    void setup(Swarm& swarm) override {
        m_complete = false;
        m_step = 0;
        int n = swarm.getDroneCount();

        // Random number generator
        std::mt19937 rng(42); // fixed seed for reproducibility
        std::uniform_real_distribution<double> distX(10.0, 70.0);
        std::uniform_real_distribution<double> distZ(-20.0, 20.0);

        // Create radar zones (circles drones must avoid)
        m_radarZones.clear();
        int numRadars = 3 + n / 5;
        for (int i = 0; i < numRadars; ++i) {
            double rx = distX(rng);
            double rz = distZ(rng);
            double radius = 6.0 + (i % 3) * 2.0;
            m_radarZones.push_back({Vec3(rx, 0, rz), radius});
        }

        // Print radar zones
        std::cout << "[RADAR] " << m_radarZones.size() << " radar bolgesi olusturuldu:\n";
        for (size_t i = 0; i < m_radarZones.size(); ++i) {
            std::cout << "  Radar #" << i << " Merkez: " << m_radarZones[i].center
                      << " Yaricap: " << m_radarZones[i].radius << "\n";
        }
        std::cout << "\n";

        // Spread drones randomly at start line (x=0)
        std::uniform_real_distribution<double> spreadZ(-15.0, 15.0);
        for (int i = 0; i < n; ++i) {
            Vec3 startPos(0, m_altitude, spreadZ(rng));
            swarm.getDrone(i).setPosition(startPos);
            swarm.getDrone(i).setMaxSpeed(6.0 + (i % 3)); // varied speeds

            // Initial target: move forward with slight random offset
            Vec3 firstTarget(15.0, m_altitude, spreadZ(rng));
            swarm.getDrone(i).setTarget(firstTarget);
        }
    }

    void update(Swarm& swarm, double dt) override {
        ++m_step;
        int n = swarm.getDroneCount();

        for (int i = 0; i < n; ++i) {
            Drone& d = swarm.getDrone(i);
            if (!d.isActive()) continue;

            // If drone reached current waypoint, assign new one closer to target
            if (d.hasReachedTarget(1.5)) {
                Vec3 pos = d.getPosition();

                if (pos.x >= m_targetZone.x - 5.0) {
                    // Reached the target zone
                    d.setTarget(Vec3(m_targetZone.x, m_altitude, pos.z));
                } else {
                    // Next waypoint: advance forward, dodge radar zones
                    Vec3 nextWP(pos.x + 12.0, m_altitude, pos.z);

                    // Check if next waypoint is inside any radar zone, if so deflect
                    for (const auto& rz : m_radarZones) {
                        double dist2D = std::sqrt(
                            std::pow(nextWP.x - rz.center.x, 2) +
                            std::pow(nextWP.z - rz.center.z, 2)
                        );
                        if (dist2D < rz.radius + 3.0) {
                            // Deflect away from radar center
                            double dz = nextWP.z - rz.center.z;
                            double deflect = (dz >= 0) ? (rz.radius + 4.0) : -(rz.radius + 4.0);
                            nextWP.z = rz.center.z + deflect;
                        }
                    }
                    d.setTarget(nextWP);
                }
            }

            // Check radar detection
            Vec3 pos = d.getPosition();
            for (size_t r = 0; r < m_radarZones.size(); ++r) {
                double dist2D = std::sqrt(
                    std::pow(pos.x - m_radarZones[r].center.x, 2) +
                    std::pow(pos.z - m_radarZones[r].center.z, 2)
                );
                if (dist2D < m_radarZones[r].radius) {
                    if (m_step % 20 == 0) {
                        std::cout << "[UYARI] Drone #" << d.getId()
                                  << " Radar #" << r << " tarafindan tespit edildi!\n";
                    }
                }
            }
        }

        swarm.update(dt);

        // Check if all active drones reached target zone
        bool allReached = true;
        for (int i = 0; i < n; ++i) {
            if (swarm.getDrone(i).isActive() &&
                swarm.getDrone(i).getPosition().x < m_targetZone.x - 5.0) {
                allReached = false;
                break;
            }
        }
        m_complete = allReached;
    }

    bool isComplete() const override {
        return m_complete;
    }

private:
    struct RadarZone {
        Vec3 center;
        double radius;
    };

    bool m_complete;
    double m_altitude;
    Vec3 m_targetZone;
    int m_step;
    std::vector<RadarZone> m_radarZones;
};
