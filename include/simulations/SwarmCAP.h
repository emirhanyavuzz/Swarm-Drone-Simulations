#pragma once
#include "Simulation.h"
#include <cmath>
#include <vector>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ============================================================
//  Swarm CAP (Bolgesel Devriye)
//  Belirli bir bolge uzerinde dairesel ve karmasik rotalar
//  cizerek surekli gozetleme yapan "ucan bir ag".
//  - Bolge sektorlere bolunur, her drone bir sektor devriye eder
//  - Dronelar kendi sektorlerinde dairesel yol izler
//  - Sektorler periyodik olarak rotasyona ugrar
// ============================================================
class SwarmCAP : public Simulation {
public:
    SwarmCAP()
        : m_complete(false)
        , m_patrolRadius(8.0)
        , m_regionSize(40.0)
        , m_altitude(15.0)
        , m_step(0)
        , m_maxSteps(400)
        , m_rotationInterval(100)
    {}

    std::string getName() const override {
        return "Bolgesel Devriye (Swarm CAP)";
    }

    std::string getDescription() const override {
        return "Bolge uzerinde dairesel devriye - surekli gozetleme agi";
    }

    void setup(Swarm& swarm) override {
        m_complete = false;
        m_step = 0;
        int n = swarm.getDroneCount();

        // Calculate sector grid
        int cols = static_cast<int>(std::ceil(std::sqrt(n)));
        int rows = static_cast<int>(std::ceil(static_cast<double>(n) / cols));
        double sectorW = m_regionSize / cols;
        double sectorH = m_regionSize / rows;

        m_sectorCenters.clear();
        m_sectorAssignment.clear();

        // Create sector centers
        for (int i = 0; i < n; ++i) {
            int row = i / cols;
            int col = i % cols;
            double cx = col * sectorW + sectorW / 2.0 - m_regionSize / 2.0;
            double cz = row * sectorH + sectorH / 2.0 - m_regionSize / 2.0;
            m_sectorCenters.push_back(Vec3(cx, m_altitude, cz));
            m_sectorAssignment.push_back(i); // drone i → sector i
        }

        std::cout << "[BOLGE] " << m_regionSize << "x" << m_regionSize
                  << "m bolge, " << n << " sektore bolundu.\n";
        std::cout << "[DEVRIYE] Her drone kendi sektorunde dairesel devriye yapacak.\n";
        std::cout << "[ROTASYON] Her " << m_rotationInterval
                  << " adimda sektor atamalari degisecek.\n\n";

        // Set initial positions to sector centers
        for (int i = 0; i < n; ++i) {
            swarm.getDrone(i).setPosition(Vec3(0, m_altitude, 0));
            swarm.getDrone(i).setMaxSpeed(10.0);
        }
    }

    void update(Swarm& swarm, double dt) override {
        ++m_step;
        int n = swarm.getDroneCount();

        // Check for sector rotation
        if (m_step > 1 && m_step % m_rotationInterval == 0) {
            rotateSectors(swarm);
        }

        // Update each drone's patrol waypoint
        double t = m_step * dt;
        for (int i = 0; i < n; ++i) {
            if (!swarm.getDrone(i).isActive()) continue;

            int sector = m_sectorAssignment[i];
            if (sector >= static_cast<int>(m_sectorCenters.size())) continue;

            Vec3 center = m_sectorCenters[sector];

            // Circular patrol within sector, each drone at different phase
            double phase = (2.0 * M_PI * i) / n;
            double patrolAngle = t * 1.5 + phase;

            Vec3 patrolPos(
                center.x + m_patrolRadius * std::cos(patrolAngle),
                m_altitude + 2.0 * std::sin(patrolAngle * 0.5), // slight altitude wave
                center.z + m_patrolRadius * std::sin(patrolAngle)
            );

            swarm.getDrone(i).setTarget(patrolPos);
        }

        swarm.update(dt);

        // Coverage report
        if (m_step % 50 == 0) {
            printCoverageReport(swarm);
        }

        if (m_step >= m_maxSteps) {
            m_complete = true;
            std::cout << "\n[SONUC] Devriye simulasyonu tamamlandi.\n";
            std::cout << "        Toplam adim: " << m_step << "\n";
            std::cout << "        Sektor rotasyonu: "
                      << (m_step / m_rotationInterval) << " kez\n";
        }
    }

    bool isComplete() const override {
        return m_complete;
    }

private:
    void rotateSectors(Swarm& swarm) {
        int n = swarm.getDroneCount();
        std::cout << "[ROTASYON] Adim " << m_step
                  << " - Sektor atamalari degistiriliyor...\n";

        // Shift each drone to the next sector (circular)
        for (int i = 0; i < n; ++i) {
            m_sectorAssignment[i] = (m_sectorAssignment[i] + 1) % static_cast<int>(m_sectorCenters.size());
        }

        std::cout << "[ROTASYON] Yeni atamalar tamamlandi.\n\n";
    }

    void printCoverageReport(const Swarm& swarm) {
        int n = swarm.getDroneCount();
        int activeDrones = 0;
        double totalCovered = 0;

        for (int i = 0; i < n; ++i) {
            if (swarm.getDrone(i).isActive()) {
                ++activeDrones;
                totalCovered += M_PI * m_patrolRadius * m_patrolRadius;
            }
        }

        double regionArea = m_regionSize * m_regionSize;
        double coverageRatio = std::min(totalCovered / regionArea, 1.0) * 100;

        std::cout << "[KAPSAM] Adim " << m_step
                  << " | Aktif: " << activeDrones
                  << " | Tahmini kapsam: %" << static_cast<int>(coverageRatio)
                  << "\n";
    }

    bool m_complete;
    double m_patrolRadius;
    double m_regionSize;
    double m_altitude;
    int m_step;
    int m_maxSteps;
    int m_rotationInterval;
    std::vector<Vec3> m_sectorCenters;
    std::vector<int> m_sectorAssignment;
};
