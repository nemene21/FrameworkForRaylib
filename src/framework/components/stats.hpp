#ifndef stats_hpp
#define stats_hpp

#include <string>
#include <map>

class Stats {
public:
    void newstat(const std::string& name, float value, float multiplier);

    void fetchstat(const std::string& name) const;

    void newvalue(const std::string& name, float value);

    void newmulti(const std::string& name, float multiplier);

private:
    std::map<std::string, float> stat;
    std::map<std::string, float> multiplier;
};

#endif stats_hpp