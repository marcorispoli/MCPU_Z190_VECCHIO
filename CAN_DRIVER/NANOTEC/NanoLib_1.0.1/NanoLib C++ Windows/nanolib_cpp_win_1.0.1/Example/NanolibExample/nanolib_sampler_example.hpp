#pragma once

#include "nanolib_helper.hpp"

class NanoLibSamplerExample {
public:
	NanoLibSamplerExample(NanoLibHelper &helper, const nlc::DeviceHandle connectedDeviceHandle);
	virtual ~NanoLibSamplerExample();

	void process();

protected:

	struct TrackedAddress {

		const char *name;

		nlc::OdIndex odIndex;
	};

	class SamplerNotifyExample : public nlc::SamplerNotify {
	public:
		SamplerNotifyExample(NanoLibSamplerExample &samplerExample);
		~SamplerNotifyExample();

	public:

		bool isRunning() const noexcept {
			return samplerRunning;
		}

		void setInactive() noexcept {
			samplerRunning = false;
		}

	public:

		void notify(const nlc::ResultVoid &lastError, const nlc::SamplerState samplerState,
					const std::vector<nlc::SampleData> &sampleDatas,
					int64_t applicationData) override;

	private:
		NanoLibSamplerExample &samplerExample;
		volatile bool samplerRunning;		
	};

protected:

	void configure(const nlc::SamplerMode mode);

	void start(SamplerNotifyExample *samplerNotify = nullptr,
			   int64_t applicationData = 0);

	nlc::SamplerState getSamplerState();

	std::vector<nlc::SampleData> getSamplerData();

	void handleSamplerFailed(const nlc::ResultVoid *lastErrorPtr = nullptr);

	void processSampledData(const std::vector<nlc::SampleData> &sampleDatas);

	void processExamplesWithoutNotification();
	void processSamplerWithoutNotificationNormal();
	void processSamplerWithoutNotificationRepetitive();
	void processSamplerWithoutNotificationContinuous();

	void processExamplesWithNotification();
	void processSamplerWithNotificationNormal();
	void processSamplerWithNotificationRepetitive();
	void processSamplerWithNotificationContinuous();
	
protected:

	static const TrackedAddress trackedAddresses[];
	static const nlc::OdIndex triggerAddress;
	static const nlc::SamplerTriggerCondition triggerCondition;
	static const uint32_t triggerValue;
	static const uint32_t triggerValueInactive;
	static const uint32_t triggerValueActive;
	static const uint16_t periodMilliseconds;
	static const uint16_t numberOfSamples;
	

protected:
	NanoLibHelper &nanolibHelper;
	nlc::DeviceHandle deviceHandle;
	volatile uint64_t lastIteration;
	volatile uint64_t sampleNumber;
	bool headerPrinted;
};
