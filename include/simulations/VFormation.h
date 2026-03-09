#pragma once
#include "Simulation.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ============================================================
//  V Formation (Vee)
//  Dronelar lider drone'un arkasina V seklinde acilarak dizilir.
//  Enerji tasarrufu ve gorus alani maksimizasyonu saglar.
// ============================================================
class VFormation : public Simulation {
public:
    VFormation() : m_complete(false), m_angle(45.0), m_spacing(5.0) {}

    std::string getName() const override {
        return "V Formasyonu (Vee Formation)";
    }

    std::string getDescription() const override {
        return "Dronelar V seklinde dizilir - enerji tasarrufu ve gorus alani";
    }

    void setup(Swarm& swarm) override {
        m_complete = false;
        int n = swarm.getDroneCount();
        double angleRad = m_angle * M_PI / 180.0;

        // Leader at front center
        Vec3 leaderPos(0, 10, 0);
        swarm.getDrone(0).setTarget(leaderPos);

        // Remaining drones alternate left and right arms of the V
        for (int i = 1; i < n; ++i) {
            int side = (i % 2 == 1) ? 1 : -1;   // odd=right, even=left
            int rank = (i + 1) / 2;               // distance rank from leader

            double dx = side * rank * m_spacing * std::sin(angleRad);
            double dz = -rank * m_spacing * std::cos(angleRad);  // behind leader

            Vec3 target = leaderPos + Vec3(dx, 0, dz);
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
    double m_angle;    // half-angle of the V (degrees)
    double m_spacing;  // distance between consecutive drones on each arm
};
