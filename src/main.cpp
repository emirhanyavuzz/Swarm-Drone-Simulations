#include "Simulation.h"
#include "simulations/VFormation.h"
#include "simulations/DiamondFormation.h"
#include "simulations/GridFormation.h"
#include "simulations/LineFormation.h"
#include "simulations/Infiltration.h"
#include "simulations/SaturationAttack.h"
#include "simulations/VirtualStructure.h"
#include "simulations/SurroundPursue.h"
#include "simulations/SelfHealing.h"
#include "simulations/SwarmCAP.h"

#include <iostream>
#include <vector>
#include <memory>
#include <string>

// ============================================================
//  Simulation runner
// ============================================================
void runSimulation(Simulation& sim, int droneCount) {
    Swarm swarm;
    swarm.createSwarm(droneCount);

    std::cout << "\n========================================\n";
    std::cout << "  " << sim.getName() << "\n";
    std::cout << "  " << sim.getDescription() << "\n";
    std::cout << "  Drone sayisi: " << droneCount << "\n";
    std::cout << "========================================\n\n";

    std::cout << "--- Baslangic Pozisyonlari ---\n";
    swarm.printPositions();
    std::cout << "\n";

    sim.setup(swarm);

    const double dt = 0.1;
    const int maxSteps = 500;
    int step = 0;

    while (!sim.isComplete() && step < maxSteps) {
        sim.update(swarm, dt);
        ++step;

        // Print every 10 steps
        if (step % 10 == 0) {
            std::cout << "--- Adim " << step << " ---\n";
            swarm.printPositions();
            std::cout << "\n";
        }
    }

    std::cout << "--- Son Durum (Adim " << step << ") ---\n";
    swarm.printPositions();

    if (sim.isComplete()) {
        std::cout << "\n[OK] Simulasyon tamamlandi! Tum dronelar formasyona ulasti.\n";
    } else {
        std::cout << "\n[!] Maksimum adim sayisina ulasildi.\n";
    }
}

// ============================================================
//  Main menu
// ============================================================
int main() {
    std::vector<std::unique_ptr<Simulation>> simulations;

    // --- Geometrik Formasyonlar ---
    simulations.push_back(std::make_unique<VFormation>());
    simulations.push_back(std::make_unique<DiamondFormation>());
    simulations.push_back(std::make_unique<GridFormation>());
    simulations.push_back(std::make_unique<LineFormation>());

    // --- Taktiksel Ucus Tipleri ---
    simulations.push_back(std::make_unique<Infiltration>());
    simulations.push_back(std::make_unique<SaturationAttack>());
    simulations.push_back(std::make_unique<VirtualStructure>());

    // --- Davranissal Ucus Modelleri ---
    simulations.push_back(std::make_unique<SurroundPursue>());
    simulations.push_back(std::make_unique<SelfHealing>());
    simulations.push_back(std::make_unique<SwarmCAP>());

    std::cout << "============================================\n";
    std::cout << "   SURU DRONE SIMULASYONU\n";
    std::cout << "   Swarm Drone Flight Simulation\n";
    std::cout << "============================================\n\n";

    // Show menu
    std::cout << "Mevcut Simulasyonlar:\n";
    std::cout << "---------------------\n";
    for (size_t i = 0; i < simulations.size(); ++i) {
        std::cout << "  " << (i + 1) << ". " << simulations[i]->getName()
                  << " - " << simulations[i]->getDescription() << "\n";
    }
    std::cout << "  0. Cikis\n\n";

    int choice = -1;
    std::cout << "Secim: ";
    std::cin >> choice;

    if (choice == 0 || choice < 0 || choice > static_cast<int>(simulations.size())) {
        std::cout << "Cikiliyor...\n";
        return 0;
    }

    int droneCount = 9;
    std::cout << "Drone sayisi (varsayilan 9): ";
    std::cin >> droneCount;
    if (droneCount < 1) droneCount = 9;

    runSimulation(*simulations[choice - 1], droneCount);

    return 0;
}
