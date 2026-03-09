#pragma once
#include "Simulation.h"
#include <cmath>
#include <vector>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ============================================================
//  Virtual Structure (Sanal Yapi)
//  Dronelar 2D bir sekil olusturup, bu sekli bozmadan
//  birlikte hareket eder. Isik gosterileri ve 3D figur
//  uygulamalarinin basitlestirilmis versiyonu.
//  - Dronelar bir daire/kare/ucgen seklinde dizilir
//  - Tum yapi birlikte otelenip doner
// ============================================================
class VirtualStructure : public Simulation {
public:
    VirtualStructure()
        : m_complete(false)
        , m_shapeRadius(15.0)
        , m_centerPos(0, 15, 0)
        , m_rotationAngle(0)
        , m_translationX(0)
        , m_step(0)
        , m_maxSteps(300)
    {}

    std::string getName() const override {
        return "Sanal Yapi (Virtual Structure)";
    }

    std::string getDescription() const override {
        return "Dronelar 2D sekil olusturup birlikte hareket eder";
    }

    void setup(Swarm& swarm) override {
        m_complete = false;
        m_step = 0;
        m_rotationAngle = 0;
        m_translationX = 0;
        int n = swarm.getDroneCount();

        // Build initial shape: circle formation
        m_localOffsets.clear();
        for (int i = 0; i < n; ++i) {
            double angle = (2.0 * M_PI * i) / n;
            double x = m_shapeRadius * std::cos(angle);
            double z = m_shapeRadius * std::sin(angle);
            m_localOffsets.push_back(Vec3(x, 0, z));
        }

        std::cout << "[YAPI] Daire sekli olusturuluyor (" << n << " drone, R="
                  << m_shapeRadius << "m)\n";
        std::cout << "[HAREKET] Yapi ileriye hareket edip donecek.\n\n";

        // Set initial targets
        applyTransform(swarm);
    }

    void update(Swarm& swarm, double dt) override {
        ++m_step;

        // Animate: rotate + translate the structure
        double rotSpeed = 0.3;   // radians per second
        double moveSpeed = 5.0;  // meters per second

        m_rotationAngle += rotSpeed * dt;
        m_translationX += moveSpeed * dt;

        // Update center position (moving forward on X axis)
        m_centerPos.x = m_translationX;

        // Apply rotation + translation to all drones
        applyTransform(swarm);

        swarm.update(dt);

        // Print structure integrity check periodically
        if (m_step % 30 == 0) {
            checkIntegrity(swarm);
        }

        if (m_step >= m_maxSteps) {
            m_complete = true;
            std::cout << "\n[OK] Sanal yapi " << m_step << " adim boyunca basariyla korundu.\n";
            std::cout << "     Toplam donus: " << static_cast<int>(m_rotationAngle * 180 / M_PI)
                      << " derece\n";
            std::cout << "     Toplam ilerleme: " << static_cast<int>(m_translationX) << "m\n";
        }
    }

    bool isComplete() const override {
        return m_complete;
    }

private:
    void applyTransform(Swarm& swarm) {
        int n = swarm.getDroneCount();
        double cosA = std::cos(m_rotationAngle);
        double sinA = std::sin(m_rotationAngle);

        for (int i = 0; i < n; ++i) {
            if (!swarm.getDrone(i).isActive()) continue;

            // Rotate local offset
            double lx = m_localOffsets[i].x;
            double lz = m_localOffsets[i].z;
            double rx = lx * cosA - lz * sinA;
            double rz = lx * sinA + lz * cosA;

            // Translate to world position
            Vec3 worldPos(
                m_centerPos.x + rx,
                m_centerPos.y + m_localOffsets[i].y,
                m_centerPos.z + rz
            );

            swarm.getDrone(i).setTarget(worldPos);
            swarm.getDrone(i).setMaxSpeed(20.0); // fast tracking
        }
    }

    void checkIntegrity(const Swarm& swarm) {
        int n = swarm.getDroneCount();
        double maxError = 0;
        double avgError = 0;

        for (int i = 0; i < n; ++i) {
            if (!swarm.getDrone(i).isActive()) continue;
            double err = swarm.getDrone(i).getPosition().distanceTo(
                swarm.getDrone(i).getTarget());
            avgError += err;
            if (err > maxError) maxError = err;
        }
        avgError /= n;

        std::cout << "[BUTUNLUK] Ort. sapma: " << static_cast<int>(avgError * 100) / 100.0
                  << "m | Maks. sapma: " << static_cast<int>(maxError * 100) / 100.0 << "m\n";
    }

    bool m_complete;
    double m_shapeRadius;
    Vec3 m_centerPos;
    double m_rotationAngle;
    double m_translationX;
    int m_step;
    int m_maxSteps;
    std::vector<Vec3> m_localOffsets; // shape-local positions
};
