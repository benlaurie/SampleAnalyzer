#ifndef WS2812_ANALYZER_SETTINGS
#define WS2812_ANALYZER_SETTINGS

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>

class WS2812AnalyzerSettings : public AnalyzerSettings {
public:
  WS2812AnalyzerSettings();
  virtual ~WS2812AnalyzerSettings();

  virtual bool SetSettingsFromInterfaces();
  void UpdateInterfacesFromSettings();
  virtual void LoadSettings( const char* settings );
  virtual const char* SaveSettings();

	
  Channel mInputChannel;
  U32 mBitRate;

protected:
  std::auto_ptr< AnalyzerSettingInterfaceChannel >	mInputChannelInterface;
  std::auto_ptr< AnalyzerSettingInterfaceInteger >	mBitRateInterface;
};

#endif //WS2812_ANALYZER_SETTINGS
