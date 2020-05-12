#ifndef MONITOR_H
#define MONITOR_H

#include <string>
#include <map>

class Monitor{
public:
    std::map<std::string, std::string> init(); //initializes the APIs and SQL connections, returns various data about the computer's devices
	std::map<std::string, int> getData(); //gets the hardware data (usage, temps)
	void exit();
};
#endif //MONITOR_H
