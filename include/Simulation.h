#pragma once
#include "Swarm.h"
#include <string>

class Simulation {
public:
    virtual ~Simulation() = default;

    // Simulation identity
    virtual std::string getName() const = 0;
    virtual std::string getDescription() const = 0;

    // Lifecycle
    virtual void setup(Swarm& swarm) = 0;
    virtual void update(Swarm& swarm, double dt) = 0;
    virtual bool isComplete() const = 0;
};
