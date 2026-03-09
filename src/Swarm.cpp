#include "Swarm.h"
#include <iostream>

Swarm::Swarm() {}

void Swarm::createSwarm(int count, const Vec3& origin, double spacing) {
    m_drones.clear();
    m_drones.reserve(count);

    // Place drones in a cluster around the origin
    int cols = static_cast<int>(std::ceil(std::sqrt(count)));
    for (int i = 0; i < count; ++i) {
        int row = i / cols;
        int col = i % cols;
        Vec3 pos = origin + Vec3(col * spacing, 0, row * spacing);
        m_drones.emplace_back(i, pos);
    }
}

void Swarm::update(double dt) {
    for (auto& drone : m_drones) {
        if (drone.isActive()) {
            drone.update(dt);
        }
    }
}

Drone& Swarm::getDrone(int index) {
    return m_drones.at(index);
}

const Drone& Swarm::getDrone(int index) const {
    return m_drones.at(index);
}

int Swarm::getDroneCount() const {
    return static_cast<int>(m_drones.size());
}

int Swarm::getActiveDroneCount() const {
    int count = 0;
    for (const auto& d : m_drones) {
        if (d.isActive()) ++count;
    }
    return count;
}

std::vector<Drone>& Swarm::getDrones() {
    return m_drones;
}

const std::vector<Drone>& Swarm::getDrones() const {
    return m_drones;
}

bool Swarm::allReachedTarget(double tolerance) const {
    for (const auto& d : m_drones) {
        if (d.isActive() && !d.hasReachedTarget(tolerance)) {
            return false;
        }
    }
    return true;
}

void Swarm::printPositions() const {
    for (const auto& d : m_drones) {
        d.printStatus();
    }
}

void Swarm::forEachActive(const std::function<void(Drone&)>& func) {
    for (auto& d : m_drones) {
        if (d.isActive()) {
            func(d);
        }
    }
}
