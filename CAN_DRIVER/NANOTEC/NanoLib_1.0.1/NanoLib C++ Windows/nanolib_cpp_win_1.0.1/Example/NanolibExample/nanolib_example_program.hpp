#pragma once

#include "nanolib_helper.hpp"

#include <optional>

class NanoLibExampleProgram {
public:
	NanoLibExampleProgram();
	~NanoLibExampleProgram();

public:

	int run();

private:

	static char readCharFromTheKeyboard(bool echo, bool newline);

	static bool askTheUser(const char *question);

private:	

	void processObjectDictionaryAccessExamples();
	void processObjectDictionaryAccessExamplesViaDictionaryInterface();
	void processProfinetDCPExample();

private:
	NanoLibHelper nanolibHelper;
	std::optional<nlc::BusHardwareId> openedBusHardware;
	std::optional<nlc::DeviceHandle> connectedDeviceHandle;
};
