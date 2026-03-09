#pragma once
#include "Simulation.h"

// ============================================================
//  Line Abreast (Cizgi) Formation
//  Tum dronelar tek bir yatay cizgi uzerinde esit aralikla dizilir.
//  Genis cephe taramasi ve sensor verisi toplama icin kullanilir.
// ============================================================
class LineFormation : public Simulation {
public:
    LineFormation() : m_complete(false), m_spacing(4.0) {}

    std::string getName() const override {
        return "Cizgi Formasyonu (Line Abreast)";
    }

    std::string getDescription() const override {
        return "Dronelar tek cizgi halinde dizilir - genis cephe taramasi";
    }

    void setup(Swarm& swarm) override {
        m_complete = false;
        int n = swarm.getDroneCount();

        // Center the line on the X axis
        double totalWidth = (n - 1) * m_spacing;
        double startX = -totalWidth / 2.0;
        double altitude = 10.0;

        for (int i = 0; i < n; ++i) {
            Vec3 target(startX + i * m_spacing, altitude, 0);
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
