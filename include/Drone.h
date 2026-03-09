#pragma once
#include "Vec3.h"
#include <string>

class Drone {
public:
    Drone();
    Drone(int id, const Vec3& startPosition, double maxSpeed = 10.0);

    // Movement
    void update(double dt);
    void setTarget(const Vec3& target);
    bool hasReachedTarget(double tolerance = 0.5) const;

    // State
    void activate();
    void deactivate();
    bool isActive() const;

    // Getters
    int getId() const;
    Vec3 getPosition() const;
    Vec3 getVelocity() const;
    Vec3 getTarget() const;
    double getMaxSpeed() const;

    // Setters
    void setPosition(const Vec3& pos);
    void setMaxSpeed(double speed);

    // Display
    void printStatus() const;

private:
    int m_id;
    Vec3 m_position;
    Vec3 m_velocity;
    Vec3 m_target;
    bool m_active;
    double m_maxSpeed;
};
