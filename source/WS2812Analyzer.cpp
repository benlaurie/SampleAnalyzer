#include "WS2812Analyzer.h"
#include "WS2812AnalyzerSettings.h"
#include <AnalyzerChannelData.h>
#include <AnalyzerHelpers.h>

WS2812Analyzer::WS2812Analyzer()
  : Analyzer(),  
    mSettings( new WS2812AnalyzerSettings() ),
    mSimulationInitilized( false ) {
  SetAnalyzerSettings( mSettings.get() );
}

WS2812Analyzer::~WS2812Analyzer() {
  KillThread();
}

void WS2812Analyzer::WorkerThread() {
  mResults.reset( new WS2812AnalyzerResults( this, mSettings.get() ) );
  SetAnalyzerResults( mResults.get() );
  mResults->AddChannelBubblesWillAppearOn( mSettings->mInputChannel );

  mSampleRateHz = GetSampleRate();
  const U64 reset = RESET_US * mSampleRateHz / 1e6;
  const U64 sample = 5 * mSampleRateHz / 4e6;  // 1.25 us

  auto data = GetAnalyzerChannelData(mSettings->mInputChannel);

  // Find low
  if (data->GetBitState() == BIT_HIGH) {
    data->AdvanceToNextEdge();
  }
  // Find a reset
  U64 start = data->GetSampleNumber();
  for (;;) {
    data->AdvanceToNextEdge(); // Rising
    U64 end = data->GetSampleNumber();
    if (data->GetBitState() != BIT_HIGH)
      AnalyzerHelpers::Assert("Should be high");
    if (end - start > reset) {
      Frame frame;
      frame.mData1 = WS2812AnalyzerResults::RESET_VALUE;
      frame.mStartingSampleInclusive = start;
      frame.mEndingSampleInclusive = end;
      mResults->AddFrame(frame);
      mResults->CommitResults();
      ReportProgress(frame.mEndingSampleInclusive);
      break;
    }
    data->AdvanceToNextEdge(); // Falling
    start = data->GetSampleNumber();
  }

  WS2812AnalyzerSettings::Type type = mSettings->mType;
    
  double prevval(0);
  int n(0);
  U32 rgb = 0;
  U64 rgbstart;
  for (;;) {
    U64 start = data->GetSampleNumber(); // Rising
    if (n == 0)
      rgbstart = start;
    data->AdvanceToNextEdge();
    U64 mid = data->GetSampleNumber(); // Falling
    data->AdvanceToNextEdge();
    U64 end = data->GetSampleNumber(); // Rising
    U64 sample_end = std::min(end, start + sample);

    if (type == WS2812AnalyzerSettings::FRAME) {
      // do RGB
      int val = mid - start > sample_end - mid;
      rgb <<= 1;
      rgb |= val;
      if (++n == 24) {
	n = 0;
	Frame frame;
	frame.mData1 = rgb;
	rgb = 0;
	frame.mStartingSampleInclusive = rgbstart;
	frame.mEndingSampleInclusive = sample_end;
	frame.mFlags = DISPLAY_AS_ERROR_FLAG;
	mResults->AddFrame(frame);
	mResults->CommitResults();
	ReportProgress(frame.mEndingSampleInclusive);
      }
    } else {
      Frame frame;
      frame.mData1 = mid;
      frame.mStartingSampleInclusive = start;
      frame.mEndingSampleInclusive = sample_end;
      mResults->AddFrame(frame);
      mResults->CommitResults();
      ReportProgress(frame.mEndingSampleInclusive);
    }

    if (end - mid > reset) {
      Frame frame;
      frame.mData1 = WS2812AnalyzerResults::RESET_VALUE;
      frame.mStartingSampleInclusive = sample_end; // strictly, mid
      frame.mEndingSampleInclusive = end;
      mResults->AddFrame(frame);
      mResults->CommitResults();
      ReportProgress(frame.mEndingSampleInclusive);
      n = 0; 
      continue;
    }
  }
}

bool WS2812Analyzer::NeedsRerun() {
  return false;
}

U32 WS2812Analyzer::GenerateSimulationData(U64 minimum_sample_index,
					   U32 device_sample_rate,
					   SimulationChannelDescriptor** simulation_channels ) {
  if(mSimulationInitilized == false) {
    mSimulationDataGenerator.Initialize(GetSimulationSampleRate(),
					mSettings.get());
    mSimulationInitilized = true;
  }

  return mSimulationDataGenerator.GenerateSimulationData(minimum_sample_index,
							 device_sample_rate,
							 simulation_channels);
}

U32 WS2812Analyzer::GetMinimumSampleRateHz() {
  return 8e5 * 16;  // 16 samples per bit at 800 kHz
}

const char* WS2812Analyzer::GetAnalyzerName() const {
  return "WS2812";
}

const char* GetAnalyzerName() {
  return "WS2812";
}

Analyzer* CreateAnalyzer() {
  return new WS2812Analyzer();
}

void DestroyAnalyzer( Analyzer* analyzer ) {
  delete analyzer;
}
