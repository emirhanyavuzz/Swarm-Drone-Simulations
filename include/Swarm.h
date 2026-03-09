#pragma once
#include "Drone.h"
#include <vector>
#include <functional>

class Swarm {
public:
    Swarm();

    // Initialization
    void createSwarm(int count, const Vec3& origin = Vec3::zero(), double spacing = 2.0);

    // Update
    void update(double dt);

    // Access
    Drone& getDrone(int index);
    const Drone& getDrone(int index) const;
    int getDroneCount() const;
    int getActiveDroneCount() const;
    std::vector<Drone>& getDrones();
    const std::vector<Drone>& getDrones() const;

    // Utilities
    bool allReachedTarget(double tolerance = 0.5) const;
    void printPositions() const;

    // Apply a function to each active drone
    void forEachActive(const std::function<void(Drone&)>& func);

private:
    std::vector<Drone> m_drones;
};
