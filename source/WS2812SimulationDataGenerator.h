#ifndef WS2812_SIMULATION_DATA_GENERATOR
#define WS2812_SIMULATION_DATA_GENERATOR

#include <SimulationChannelDescriptor.h>
#include <string>
class WS2812AnalyzerSettings;

class WS2812SimulationDataGenerator
{
public:
	WS2812SimulationDataGenerator();
	~WS2812SimulationDataGenerator();

	void Initialize( U32 simulation_sample_rate, WS2812AnalyzerSettings* settings );
	U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channel );

protected:
	WS2812AnalyzerSettings* mSettings;
	U32 mSimulationSampleRateHz;

protected:
	void CreateSerialByte();
	std::string mSerialText;
	U32 mStringIndex;

	SimulationChannelDescriptor mSerialSimulationData;

};
#endif //WS2812_SIMULATION_DATA_GENERATOR