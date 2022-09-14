#include "nanolib_sampler_example.hpp"

#include <cassert>
#include <climits>
#include <iostream>
#include <sstream>
#include <thread>

const NanoLibSamplerExample::TrackedAddress NanoLibSamplerExample::trackedAddresses[]
	= {{"Up time", nlc::OdIndex(0x230F, 0x00)}, {"Temperature", nlc::OdIndex(0x4014, 0x03)}};
// In this sample we use first NanoJ input as trigger
const nlc::OdIndex NanoLibSamplerExample::triggerAddress = nlc::OdIndex(0x2400, 0x01);
const nlc::SamplerTriggerCondition NanoLibSamplerExample::triggerCondition
	= nlc::SamplerTriggerCondition::TC_GREATER;
const uint32_t NanoLibSamplerExample::triggerValue = 10;
// Depends from condition
const uint32_t NanoLibSamplerExample::triggerValueInactive = NanoLibSamplerExample::triggerValue;
// Depends from condition
const uint32_t NanoLibSamplerExample::triggerValueActive = NanoLibSamplerExample::triggerValue + 1;
const uint16_t NanoLibSamplerExample::periodMilliseconds = 1000;
const uint16_t NanoLibSamplerExample::numberOfSamples = 5;


NanoLibSamplerExample::NanoLibSamplerExample(NanoLibHelper &helper,
											   const nlc::DeviceHandle connectedDeviceHandle)
	: nanolibHelper(helper), deviceHandle(connectedDeviceHandle) {
}

NanoLibSamplerExample::~NanoLibSamplerExample() {
}

NanoLibSamplerExample::SamplerNotifyExample::SamplerNotifyExample(
	NanoLibSamplerExample &example)
	: samplerExample(example), samplerRunning(true) {
}

NanoLibSamplerExample::SamplerNotifyExample::~SamplerNotifyExample() {
	// Destroying this notification object is safe only when the sampler is not active
	assert(!samplerRunning);
}

void NanoLibSamplerExample::SamplerNotifyExample::notify(
	const nlc::ResultVoid &lastError, const nlc::SamplerState samplerState,
	const std::vector<nlc::SampleData> &sampleDatas, int64_t applicationData) {

	// Be aware that notifications are executed in the context of separate threads 
	// other than thread that started the sampler.
	// 
	// Be careful when calling Nanolib functionality here, as doing so may cause this method
	// to be called recursively, potentially causing your application to deadlock.
	// 
	// For the same reason, this method should not throw exceptions.

	assert(samplerRunning);

	(void)applicationData;

	if (!sampleDatas.empty())
		samplerExample.processSampledData(sampleDatas);

	if (samplerState == nlc::SamplerState::Failed) {
		try {
			samplerExample.handleSamplerFailed(&lastError);
		} catch (...) {
			// see comment above
		}
	}

	if ((samplerState != nlc::SamplerState::Ready)
		&& (samplerState != nlc::SamplerState::Running)) {
		// It's now safe to destroy this notification object
		samplerRunning = false;
	}
}

void NanoLibSamplerExample::process() {

	processExamplesWithoutNotification();
	processExamplesWithNotification();
}

void NanoLibSamplerExample::processExamplesWithoutNotification() {

	processSamplerWithoutNotificationNormal();
	processSamplerWithoutNotificationRepetitive();
	processSamplerWithoutNotificationContinuous();
}

void NanoLibSamplerExample::processSamplerWithoutNotificationNormal() {

	const std::chrono::milliseconds sleepTimeMsec(periodMilliseconds);

	std::cout << "\nSampler without notification in normal mode:" << std::endl;

	configure(nlc::SamplerMode::Normal);
	start();

	nlc::SamplerState samplerState;

	do {

		std::this_thread::sleep_for(sleepTimeMsec);

		processSampledData(getSamplerData());
		samplerState = getSamplerState();

	} while ((samplerState == nlc::SamplerState::Ready)
			 || (samplerState == nlc::SamplerState::Running));

	// Process any remaining data
	processSampledData(getSamplerData());

	if (samplerState == nlc::SamplerState::Failed)
		handleSamplerFailed();
}

void NanoLibSamplerExample::processSamplerWithoutNotificationRepetitive() {

	const std::chrono::milliseconds sleepTimeMsec(periodMilliseconds);

	std::cout << "\nSampler without notification in repetative mode:" << std::endl;

	configure(nlc::SamplerMode::Repetitive);
	start();

	nlc::SamplerState samplerState(nlc::SamplerState::Ready);
		
	do {

		std::this_thread::sleep_for(sleepTimeMsec);

		processSampledData(getSamplerData());

		if (lastIteration >= 4) {
			// In repetative mode the sampler will continue to run until it is stopped or an error occurs
			nanolibHelper.stopSampler(deviceHandle);
			break;
		}

		samplerState = getSamplerState();

	} while ((samplerState == nlc::SamplerState::Ready)
			 || (samplerState == nlc::SamplerState::Running));

	// Process any remaining data
	processSampledData(getSamplerData());

	if (samplerState == nlc::SamplerState::Failed)
		handleSamplerFailed();
}

void NanoLibSamplerExample::processSamplerWithoutNotificationContinuous() {

	const std::chrono::milliseconds sleepTimeMsec(periodMilliseconds);
	
	std::cout << "\nSampler without notification in continuous mode:" << std::endl;

	configure(nlc::SamplerMode::Continuous);
	start();

	nlc::SamplerState samplerState(nlc::SamplerState::Ready);
	constexpr unsigned maxCycles = 10;
	unsigned cycles = 0;

	do {

		std::this_thread::sleep_for(sleepTimeMsec);

		processSampledData(getSamplerData());

		if (++cycles == maxCycles) {
			// In continuous mode the sampler will continue to run until it is stopped or an error occurs
			nanolibHelper.stopSampler(deviceHandle);
			break;
		}

		samplerState = getSamplerState();

	} while ((samplerState == nlc::SamplerState::Ready)
			 || (samplerState == nlc::SamplerState::Running));

	// Process any remaining data
	processSampledData(getSamplerData());

	if (samplerState == nlc::SamplerState::Failed)
		handleSamplerFailed();
}

void NanoLibSamplerExample::processExamplesWithNotification() {

	processSamplerWithNotificationNormal();
	processSamplerWithNotificationRepetitive();
	processSamplerWithNotificationContinuous();
}

void NanoLibSamplerExample::processSamplerWithNotificationNormal() {

	const std::chrono::milliseconds sleepTimeMsec(periodMilliseconds);
		
	std::cout << "\nSampler with notification in normal mode:" << std::endl;

	configure(nlc::SamplerMode::Normal);

	SamplerNotifyExample samplerNotify(*this);

	start(&samplerNotify);
	while (samplerNotify.isRunning()) {
		std::this_thread::sleep_for(sleepTimeMsec);
	}
}

void NanoLibSamplerExample::processSamplerWithNotificationRepetitive() {

	const std::chrono::milliseconds sleepTimeMsec(periodMilliseconds);

	std::cout << "\nSampler with notification in repetative mode:" << std::endl;

	configure(nlc::SamplerMode::Repetitive);

	SamplerNotifyExample samplerNotify(*this);

	start(&samplerNotify);
	while (samplerNotify.isRunning()) {
		std::this_thread::sleep_for(sleepTimeMsec);

		if (lastIteration >= 4) {
			// In repetative mode the sampler will continue to run until it is stopped or an error occurs
			nanolibHelper.stopSampler(deviceHandle);
			break;
		}
	}
}

void NanoLibSamplerExample::processSamplerWithNotificationContinuous() {

	std::cout << "\nSampler with notification in continuous mode:" << std::endl;

	configure(nlc::SamplerMode::Continuous);

	SamplerNotifyExample samplerNotify(*this);

	start(&samplerNotify);
	std::this_thread::sleep_for(std::chrono::milliseconds(periodMilliseconds * 10));
	// In continuous the sampler will continue to run until it is stopped or an error occurs
	nanolibHelper.stopSampler(deviceHandle);
}

void NanoLibSamplerExample::configure(const nlc::SamplerMode mode) {

	constexpr size_t numberOfTrackedAddresses
		= sizeof(trackedAddresses) / sizeof(trackedAddresses[0]);

	nlc::SamplerConfiguration samplerConfiguration;

	for (size_t trackedAddressIndex = 0; trackedAddressIndex < numberOfTrackedAddresses;
		 ++trackedAddressIndex) {
		samplerConfiguration.trackedAddresses.push_back(
			trackedAddresses[trackedAddressIndex].odIndex);
	}

	samplerConfiguration.triggerAddress = triggerAddress;
	samplerConfiguration.triggerCondition = triggerCondition;
	samplerConfiguration.triggerValue = triggerValue;
	samplerConfiguration.periodMilliseconds = periodMilliseconds;
	samplerConfiguration.numberOfSamples = numberOfSamples;
	// Currrently this value is not used
	samplerConfiguration.preTriggerNumberOfSamples = 0;
	samplerConfiguration.mode = mode;
	samplerConfiguration.forceSoftwareImplementation = (mode == nlc::SamplerMode::Continuous);

	nanolibHelper.configureSampler(deviceHandle, samplerConfiguration);
}

void NanoLibSamplerExample::start(SamplerNotifyExample *samplerNotify /* = nullptr*/,
								  int64_t applicationData /* = 0*/) {

	lastIteration = std::numeric_limits<uint64_t>::max();
	sampleNumber = 0;
	headerPrinted = false;

	// Deactivate the start trigger
	nanolibHelper.writeInteger(deviceHandle, triggerValueInactive, triggerAddress, 32);

	// Start the sampler
	try {
		nanolibHelper.startSampler(deviceHandle, samplerNotify, applicationData);
	} catch (...) {
		if (samplerNotify != nullptr)
			samplerNotify->setInactive();
		throw;
	}

	// Activate start trigger
	try {
		nanolibHelper.writeInteger(deviceHandle, triggerValueActive, triggerAddress, 32);
	} catch (...) {
		nanolibHelper.stopSampler(deviceHandle);
		throw;
	}
}

nlc::SamplerState NanoLibSamplerExample::getSamplerState() {
	return nanolibHelper.getSamplerState(deviceHandle);
}

std::vector<nlc::SampleData> NanoLibSamplerExample::getSamplerData() {
	return nanolibHelper.getSamplerData(deviceHandle);
}

void NanoLibSamplerExample::handleSamplerFailed(
	const nlc::ResultVoid *lastErrorPtr /* = nullptr*/) {

	nlc::ResultVoid lastError;

	if (lastErrorPtr != nullptr) {
		lastError = *lastErrorPtr;
	} else {
		assert(getSamplerState() == nlc::SamplerState::Failed);
		lastError = nanolibHelper.getSamplerLastError(deviceHandle);
	}

	assert(lastError.hasError());
	std::cerr << std::endl
			  << "Sampler execution failed with error: " << lastError.getError() << std::endl;
}

void NanoLibSamplerExample::processSampledData(const std::vector<nlc::SampleData> &sampleDatas) {

	constexpr size_t numberOfTrackedAddresses
		= sizeof(trackedAddresses) / sizeof(trackedAddresses[0]);

	for (const auto sampleData : sampleDatas) {

		const auto &sampledValues = sampleData.sampledValues;
		const size_t numberOfSampledValues = sampledValues.size();

		assert((numberOfSampledValues % numberOfTrackedAddresses) == 0);

		if (lastIteration != sampleData.iterationNumber) {
			sampleNumber = 0;
			lastIteration = sampleData.iterationNumber;
		}
		
		std::stringstream stream;

		if (!headerPrinted) {

			static const char *cszHorzLine = "------------------------------------------------------------\n";

			stream << cszHorzLine;
			stream << std::left << std::setw(10) << "Iteration" << std::left << std::setw(10)
				   << "Sample";
			for (size_t trackedAddressIndex = 0; trackedAddressIndex < numberOfTrackedAddresses;
				 ++trackedAddressIndex) {

				std::string addressName("[");

				addressName += trackedAddresses[trackedAddressIndex].name;
				addressName += "]";
				stream << std::left << std::setw(14) << addressName << std::left << std::setw(8)
					   << "Time";
			}

			stream << "\n";
			stream << cszHorzLine;

			headerPrinted = true;
		}

		for (size_t index = 0; index < numberOfSampledValues; index += numberOfTrackedAddresses) {

			stream << std::left << std::setw(10) << lastIteration;
			stream << std::left << std::setw(10) << sampleNumber;

			for (size_t trackedAddressIndex = 0; trackedAddressIndex < numberOfTrackedAddresses;
				 ++trackedAddressIndex) {

				const auto &sampledValue = sampledValues[index + trackedAddressIndex];

				stream << std::left << std::setw(14) << sampledValue.value;
				stream << std::left << std::setw(8) << sampledValue.collectTimeMsec;
			}
			
			stream << "\n";
			++sampleNumber;
		}

		std::cout << stream.str();
	}
}