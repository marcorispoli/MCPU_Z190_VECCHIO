#include "nanolib_example_program.hpp"
#include "nano_lib_hw_strings.hpp"
#include "nanolib_sampler_example.hpp"
#include "od_types_helper.hpp"

#include <iostream>

#include <memory.h>
#include <termios.h>
#include <unistd.h>

static int _getch() {

	struct termios tios;

	memset(&tios, 0, sizeof(tios));

	if (tcgetattr(STDIN_FILENO, &tios) < 0)
		return EOF;

	tios.c_lflag &= ~(ICANON | ECHO);

	if (tcsetattr(STDIN_FILENO, TCSANOW, &tios) < 0)
		return EOF;

	setbuf(stdin, NULL);

	char ch = getchar();

	tios.c_lflag |= (ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &tios);

	return ch;
}

char NanoLibExampleProgram::readCharFromTheKeyboard(bool echo, bool newline) {

	const char ch = static_cast<char>(_getch());

	if (echo)
		std::cout << ch;
	if (newline)
		std::cout << std::endl;

	return ch;
}

bool NanoLibExampleProgram::askTheUser(const char *question) {

	std::cout << std::endl << question << " Press 'Y'es / 'N'o: ";

	const char ch = readCharFromTheKeyboard(true, true);

	return (ch == 'Y') || (ch == 'y');
}

NanoLibExampleProgram::NanoLibExampleProgram() {
}

NanoLibExampleProgram::~NanoLibExampleProgram() {

	std::cout << std::endl;

	// Always finalize connected hardware

	if (connectedDeviceHandle.has_value()) {
		std::cout << "Disconnecting the device." << std::endl;
		nanolibHelper.disconnectDevice(*connectedDeviceHandle);
	}

	if (openedBusHardware.has_value()) {
		std::cout << "Closing the hardware bus." << std::endl;
		nanolibHelper.closeBusHardware(*openedBusHardware);
	}

	std::cout << std::endl << "Press any key to exit...";
	readCharFromTheKeyboard(false, true);
}

int NanoLibExampleProgram::run() {

	try {

		// its possible to set the logging level to a different level
		nanolibHelper.setLoggingLevel(nlc::LogLevel::Off);

		// list all hardware available
		std::vector<nlc::BusHardwareId> busHardwareIds = nanolibHelper.getBusHardware();

		if (busHardwareIds.empty()) {
			std::cout << "No hardware buses found." << std::endl;
			return EXIT_SUCCESS;
		}

		std::cout << std::endl << "Available hardware buses and protocols:" << std::endl;

		unsigned lineNum = 0;
		// print out available hardware
		for (const nlc::BusHardwareId &busHwId : busHardwareIds) {
			std::cout << lineNum << ". " << busHwId.getName()
					  << " protocol: " << busHwId.getProtocol() << std::endl;
			lineNum++;
		}

		if (lineNum > 1) {

			std::cout << std::endl
					  << "Type bus hardware number (0-" << (lineNum - 1) << ") and press Enter:";
			std::cin >> lineNum;
			std::cout << std::endl;

			if (lineNum >= busHardwareIds.size()) {
				std::cerr << "Invalid bus hardware number." << std::endl;
				return EXIT_FAILURE;
			}

		} else {
			lineNum = 0;
			std::cout << std::endl;
		}

		// Use the selected bus hardware
		nlc::BusHardwareId busHwId = busHardwareIds[lineNum];

		// create bus hardware options for opening the hardware
		nlc::BusHardwareOptions busHwOptions = nanolibHelper.createBusHardwareOptions(busHwId);

		std::cout << "Opening bus hardware: " << busHwId.getName() << std::endl;

		// now able to open the hardware itself
		nanolibHelper.openBusHardware(busHwId, busHwOptions);
		openedBusHardware = busHwId;

		// Scan the bus for available devices
		std::cout << "Scanning bus for devices..." << std::endl;

		std::vector<nlc::DeviceId> deviceIds = nanolibHelper.scanBus(busHwId);

		if (deviceIds.empty()) {
			std::cout << std::endl << "No devices found." << std::endl;
			return EXIT_SUCCESS;
		}

		std::cout << std::endl << "Available devices:" << std::endl << std::endl;
		lineNum = 0;
		// print out available devices
		for (const nlc::DeviceId &deviceId : deviceIds) {
			std::cout << lineNum << ". " << deviceId.getDescription()
					  << " [device id: " << deviceId.getDeviceId()
					  << ", hardware: " << deviceId.getBusHardwareId().getName() << "]"
					  << std::endl;
			lineNum++;
		}

		if (lineNum > 1) {

			std::cout << std::endl
					  << "Type device number (0-" << (lineNum - 1) << ") and press Enter:";
			std::cin >> lineNum;
			std::cout << std::endl;

			if (lineNum >= deviceIds.size()) {
				std::cerr << "Invalid device number." << std::endl;
				return EXIT_FAILURE;
			}

		} else {
			lineNum = 0;
			std::cout << std::endl;
		}

		// Use selected device. 
		// We could also create device id manually, as follows: 
		// nlc::DeviceId deviceId = nlc::DeviceId(busHwId, 1, "");
		nlc::DeviceId deviceId = deviceIds[lineNum];

		// Register the device id
		nlc::DeviceHandle deviceHandle = nanolibHelper.addDevice(deviceId);

		// Establishing a connection with the device
		std::cout << "Connecting to device " << deviceId.getDescription() << std::endl;
		nanolibHelper.connectDevice(deviceHandle);
		connectedDeviceHandle = deviceHandle;

		// Now we are ready to work with the controller. 
		
		// Performing some examples with access to the object dictionary
		processObjectDictionaryAccessExamples();
		processObjectDictionaryAccessExamplesViaDictionaryInterface();
		
		// We can use Profinet DCP only with Ethernet bus hardware.
		if (busHwId.getBusHardware() == nlc::BUS_HARDWARE_ID_NETWORK) {

			// Check service availability - Npcap/WinPcap driver required
			if (!nanolibHelper->getProfinetDCP().isServiceAvailable(busHwId).hasError()) {

				if (askTheUser("Do you wish to proceed with Profinet DCP example?"))
					processProfinetDCPExample();
			}
		}

		if (askTheUser("Do you wish to proceed with sampler examples?")) {

			NanoLibSamplerExample samplerExample(nanolibHelper, deviceHandle);

			samplerExample.process();
		}

	} catch (const nanolib_exception &e) {
		std::cerr << "Error occurred: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

void NanoLibExampleProgram::processObjectDictionaryAccessExamples() {

	std::cout << std::endl << "Object Dictionary Access Examples:" << std::endl;

	std::cout << std::endl << "Reading subindex 0 of index 0x6040" << std::endl;
	uint16_t controlWord = static_cast<uint16_t>(
		nanolibHelper.readInteger(*connectedDeviceHandle, nlc::OdIndex(0x6040, 0x00)));
	std::cout << "Result: " << controlWord << std::endl;

	std::cout << std::endl << "Motor Stop (0x6040-0)" << std::endl;
	nanolibHelper.writeInteger(*connectedDeviceHandle, 6, nlc::OdIndex(0x6040, 0x00), 16);

	std::cout << std::endl << "Reading Nanotec home page string" << std::endl;
	std::cout << "Homepage of Nanotec Electronic GmbH & Co. KG is: "
			  << nanolibHelper.readString(*connectedDeviceHandle, nlc::OdIndex(0x6505, 0x00))
			  << std::endl;

	std::cout << std::endl << "Reading device error stack:" << std::endl;
	std::vector<std::int64_t> errorStack = nanolibHelper.readArray(*connectedDeviceHandle, 0x1003);
	std::cout << "The error stack has " << std::to_string(errorStack.at(0)) << " element"
			  << std::endl;
}

void NanoLibExampleProgram::processObjectDictionaryAccessExamplesViaDictionaryInterface() {

	std::cout << std::endl << "Object Dictionary Access Examples via Dictionary Interface:" << std::endl;

	const nlc::OdIndex odControlWord(0x6040, 0x00);
	const nlc::OdIndex odHomePage(0x6505, 0x00);

	nlc::ResultObjectDictionary resultObjectDictionary
		= NanoLibHelper::checkedResult("getAssignedObjectDictionary", nanolibHelper->getAssignedObjectDictionary(*connectedDeviceHandle));

	nlc::ObjectDictionary &objectDictionary
		= const_cast<nlc::ObjectDictionary&>(resultObjectDictionary.getResult());

	std::cout << std::endl << "Reading subindex 0 of index 0x6040" << std::endl;
	uint16_t controlWord = static_cast<uint16_t>(
		NanoLibHelper::checkedResult("ObjectDictionary::readNumber",
									 objectDictionary.readNumber(odControlWord))
			.getResult());
	std::cout << "Result: " << controlWord << std::endl;

	std::cout << std::endl << "Motor Stop (0x6040-0)" << std::endl;
	NanoLibHelper::checkResult("ObjectDictionary::writeNumber", objectDictionary.writeNumber(odControlWord, 6));
	
	std::cout << std::endl << "Reading Nanotec home page string" << std::endl;
	std::cout << "Homepage of Nanotec Electronic GmbH & Co. KG is: "
			  << NanoLibHelper::checkedResult("ObjectDictionary::readString",
											  objectDictionary.readString(odHomePage))
					 .getResult()
			  << std::endl;

	std::cout << std::endl;
		
	std::cout << "Some ObjectEntry properties:" << std::endl;
		
	const nlc::ObjectEntry &objectEntry
		= NanoLibHelper::checkedResult("ObjectDictionary::getObjectEntry",
									   objectDictionary.getObjectEntry(odControlWord.getIndex()))
			  .getResult();


	std::cout << "Object(0x6040).ObjectCode = "; //<< static_cast<int>(objectEntry.getObjectCode())
												//<< std::endl;
	const auto objectCode = objectEntry.getObjectCode();
	switch (objectCode) {
	case nlc::ObjectCode::Null:
		std::cout << "Null";
		break;
	case nlc::ObjectCode::Deftype:
		std::cout << "Deftype";
		break;
	case nlc::ObjectCode::Defstruct:
		std::cout << "Defstruct";
		break;
	case nlc::ObjectCode::Var:
		std::cout << "Var";
		break;
	case nlc::ObjectCode::Array:
		std::cout << "Array";
		break;
	case nlc::ObjectCode::Record:
		std::cout << "Record";
		break;
	default:
		std::cout << static_cast<int>(objectCode);
		break;
	}
	std::cout << std::endl;

	std::cout << "Object(0x6040).DataType = "
			  << nlc::OdTypesHelper::objectEntryDataTypeToString(objectEntry.getDataType())
			  << std::endl;

	std::cout << "Some ObjectSubEntry properties:" << std::endl;

	nlc::ResultObjectSubEntry resultObjectSubEntry = NanoLibHelper::checkedResult(
		"ObjectDictionary::getObject", objectDictionary.getObject(odControlWord));
	const nlc::ObjectSubEntry &objectSubEntry = resultObjectSubEntry.getResult();

	std::cout << "OdIndex(" << odControlWord.toString()
			  << ").DataType = " << nlc::OdTypesHelper::objectEntryDataTypeToString(objectSubEntry.getDataType())
			  << std::endl;
	std::cout << "OdIndex(" << odControlWord.toString()
			  << ").BitLength = " << objectSubEntry.getBitLength()
			  << std::endl;
}

void NanoLibExampleProgram::processProfinetDCPExample() {

	nlc::BusHardwareId busHardwareId(openedBusHardware.value());
	nlc::ProfinetDCP &profinetDCP = nanolibHelper->getProfinetDCP();


	std::cout << "Scanning " << busHardwareId.getName() << " for Profinet devices..."
			  << std::endl;
	nlc::ResultProfinetDevices resultProfinetDevices
		= profinetDCP.scanProfinetDevices(busHardwareId);

	if (resultProfinetDevices.hasError()
		&& (resultProfinetDevices.getErrorCode() != nlc::NlcErrorCode::TimeoutError)) {
		std::cerr << "ProfinetDCP::scanProfinetDevices failed with error: "
				  << resultProfinetDevices.getError() << std::endl;
		return;
	}

	const std::vector<nlc::ProfinetDevice> &profinetDevices = resultProfinetDevices.getResult();
	const size_t numberOfProfinetDevices = profinetDevices.size();

	if (numberOfProfinetDevices < 1) {
		std::cout << "No Profinet devices found." << std::endl;
		return;
	}

	std::cout << numberOfProfinetDevices << " Profinet device(s) found:" << std::endl;
	for (const auto &profinetDevice : profinetDevices) {

		std::cout << "IP: " << ((profinetDevice.ipAddress >> 24) & 0x000000FF) << "."
				  << ((profinetDevice.ipAddress >> 16) & 0x000000FF) << "."
				  << ((profinetDevice.ipAddress >> 8) & 0x000000FF) << "."
				  << (profinetDevice.ipAddress & 0x000000FF)
				  << "\tName: " << profinetDevice.deviceName << std::endl;

		// Checking the IP address against the context of the current network configuration
		const auto resultValid
			= profinetDCP.validateProfinetDeviceIp(busHardwareId, profinetDevice);
		std::cout << "\tDevice IP is " << (resultValid.hasError() ? " not " : "")
				  << "valid into current network." << std::endl;

		// Trying to blink the device
		const auto resultBlink = profinetDCP.blinkProfinetDevice(busHardwareId, profinetDevice);
		std::cout << "\tBlink the device ";
		if (resultBlink.hasError())
			std::cout << "failed with error: " << resultBlink.getError();
		else
			std::cout << "succeeded.";
		std::cout << std::endl;
	}
}