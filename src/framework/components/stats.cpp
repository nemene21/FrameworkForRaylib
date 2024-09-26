#include "stats.hpp"
#include <iostream>

void Stats::newstat(const std::string& name, float value, float multiplier) {
    stat[name] = value;
    this->multiplier[name] = multiplier;
}

void Stats::fetchstat(const std::string& name) const {
    if (stat.find(name) != stat.end()) {
        float value = stat.at(name);
        value *= multiplier.count(name) ? multiplier.at(name) : 1.0f;
        std::cout << "Stat: " << name << "Value: " << value << std::endl;
    }
}

void Stats::newvalue(const std::string& name, float value) {
    if (stat.find(name) != stat.end()) {
        stat[name] = value;
    }
}

void Stats::newmulti(const std::string& name, float multiplierValue) {
    if (multiplier.find(name) != multiplier.end()) {
        multiplier[name] = multiplierValue;
    }
}