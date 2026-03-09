#pragma once
#include "Simulation.h"
#include <cmath>
#include <random>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ============================================================
//  Self-Healing (Kendi Kendini Iyilestirme)
//  Formasyon icindeki dronelar devre disi kaldiginda, kalan
//  dronelar boslugu otomatik doldurarak formasyonu korur.
//  - Grid formasyonunda baslar
//  - Belirli aralklarla rastgele dronelar devre disi kalir
//  - Kalan dronelar yeni pozisyonlara yeniden atanir
// ============================================================
class SelfHealing : public Simulation {
public:
    SelfHealing()
        : m_complete(false)
        , m_spacing(6.0)
        , m_step(0)
        , m_maxSteps(350)
        , m_nextFailStep(40)
        , m_failCount(0)
        , m_maxFails(4)
    {}

    std::string getName() const override {
        return "Kendi Kendini Iyilestirme (Self-Healing)";
    }

    std::string getDescription() const override {
        return "Dronelar devre disi kalinca formasyon otomatik onarilir";
    }

    void setup(Swarm& swarm) override {
        m_complete = false;
        m_step = 0;
        m_failCount = 0;
        m_nextFailStep = 40;
        int n = swarm.getDroneCount();

        std::cout << "[BASLANGIC] " << n << " drone ile Grid formasyonu olusturuluyor.\n";
        std::cout << "[SENARYO] Her ~50 adimda rastgele bir drone devre disi kalacak.\n";
        std::cout << "[SENARYO] Maksimum " << m_maxFails << " ariza uygulanacak.\n\n";

        // Initial grid formation
        reassignFormation(swarm);
    }

    void update(Swarm& swarm, double dt) override {
        ++m_step;
        swarm.update(dt);

        // Periodically disable a random drone
        if (m_step == m_nextFailStep && m_failCount < m_maxFails) {
            disableRandomDrone(swarm);
            m_nextFailStep += 50 + (m_failCount * 10);
        }

        // Check if formation is stable
        if (m_step % 20 == 0) {
            int active = swarm.getActiveDroneCount();
            int total = swarm.getDroneCount();
            bool stable = true;

            for (auto& d : swarm.getDrones()) {
                if (d.isActive() && !d.hasReachedTarget(0.5)) {
                    stable = false;
                    break;
                }
            }

            if (m_step % 40 == 0) {
                std::cout << "[DURUM] Adim " << m_step
                          << " | Aktif: " << active << "/" << total
                          << " | Formasyon: " << (stable ? "KARARLI" : "YENIDEN KONUMLANIYOR")
                          << "\n";
            }
        }

        if (m_step >= m_maxSteps) {
            m_complete = true;
            int active = swarm.getActiveDroneCount();
            int total = swarm.getDroneCount();
            std::cout << "\n[SONUC] Simulasyon tamamlandi.\n";
            std::cout << "        Toplam ariza: " << m_failCount << "\n";
            std::cout << "        Kalan aktif drone: " << active << "/" << total << "\n";
            std::cout << "        Formasyon basariyla korundu.\n";
        }
    }

    bool isComplete() const override {
        return m_complete;
    }

private:
    void disableRandomDrone(Swarm& swarm) {
        // Find a random active drone to disable
        std::mt19937 rng(m_step * 7 + 13);
        std::vector<int> activeIds;
        for (auto& d : swarm.getDrones()) {
            if (d.isActive()) activeIds.push_back(d.getId());
        }

        if (activeIds.size() <= 3) return; // keep minimum 3 drones

        std::uniform_int_distribution<int> dist(0, static_cast<int>(activeIds.size()) - 1);
        int victimId = activeIds[dist(rng)];

        swarm.getDrone(victimId).deactivate();
        ++m_failCount;

        std::cout << "\n[ARIZA] Drone #" << victimId << " devre disi kaldi! "
                  << "(Ariza #" << m_failCount << ")\n";
        std::cout << "[ONARIM] Formasyon yeniden hesaplaniyor...\n";

        // Reassign remaining drones to fill the grid
        reassignFormation(swarm);

        std::cout << "[ONARIM] Yeni formasyon atandi. ("
                  << swarm.getActiveDroneCount() << " aktif drone)\n\n";
    }

    void reassignFormation(Swarm& swarm) {
        // Collect active drones
        std::vector<int> activeIds;
        for (auto& d : swarm.getDrones()) {
            if (d.isActive()) activeIds.push_back(d.getId());
        }

        int n = static_cast<int>(activeIds.size());
        if (n == 0) return;

        // Create a new grid with active drones only
        int cols = static_cast<int>(std::ceil(std::sqrt(n)));
        double offsetX = (cols - 1) * m_spacing / 2.0;
        int rows = static_cast<int>(std::ceil(static_cast<double>(n) / cols));
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
            swarm.getDrone(activeIds[i]).setTarget(target);
        }
    }

    bool m_complete;
    double m_spacing;
    int m_step;
    int m_maxSteps;
    int m_nextFailStep;
    int m_failCount;
    int m_maxFails;
};
