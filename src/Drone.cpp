#include "Drone.h"
#include <iostream>
#include <iomanip>

Drone::Drone()
    : m_id(-1), m_active(false), m_maxSpeed(10.0) {}

Drone::Drone(int id, const Vec3& startPosition, double maxSpeed)
    : m_id(id)
    , m_position(startPosition)
    , m_velocity(Vec3::zero())
    , m_target(startPosition)
    , m_active(true)
    , m_maxSpeed(maxSpeed) {}

void Drone::update(double dt) {
    if (!m_active) return;

    Vec3 direction = m_target - m_position;
    double distance = direction.length();

    if (distance < 0.01) {
        m_velocity = Vec3::zero();
        return;
    }

    // Seek behavior: accelerate toward target, cap at maxSpeed
    Vec3 desired = direction.normalized() * m_maxSpeed;

    // Smooth arrival: slow down when close to target
    double arrivalRadius = m_maxSpeed * 1.5;
    if (distance < arrivalRadius) {
        desired = desired * (distance / arrivalRadius);
    }

    m_velocity = desired;
    m_position += m_velocity * dt;
}

void Drone::setTarget(const Vec3& target) {
    m_target = target;
}

bool Drone::hasReachedTarget(double tolerance) const {
    return m_position.distanceTo(m_target) < tolerance;
}

void Drone::activate() { m_active = true; }
void Drone::deactivate() { m_active = false; }
bool Drone::isActive() const { return m_active; }

int Drone::getId() const { return m_id; }
Vec3 Drone::getPosition() const { return m_position; }
Vec3 Drone::getVelocity() const { return m_velocity; }
Vec3 Drone::getTarget() const { return m_target; }
double Drone::getMaxSpeed() const { return m_maxSpeed; }

void Drone::setPosition(const Vec3& pos) { m_position = pos; }
void Drone::setMaxSpeed(double speed) { m_maxSpeed = speed; }

void Drone::printStatus() const {
    std::cout << "  Drone #" << std::setw(2) << m_id
              << (m_active ? " [ON] " : " [OFF]")
              << " Pos: " << m_position
              << " -> Target: " << m_target
              << std::endl;
}
