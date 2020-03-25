#pragma once

#include "../Storm/storm.h"

struct StormVMCall {
	std::vector<StormType> typesWanted;
	std::string name;
	uint8_t byte;
};

// Read call
const StormVMCall STORM_READ = {{StormType::STRING, StormType::INTEGER}, "read", 0x40};
// Write call
const StormVMCall STORM_WRITE = {{StormType::STRING, StormType::STRING}, "write", 0x41};

// For calls to system functions (write, read, etc.) add args to data 
void addCallArgsToData(StormVMCall call, std::vector<std::string>::iterator &chunk);
// Execute Call
void StormCall(std::vector<std::string>::iterator &chunk);