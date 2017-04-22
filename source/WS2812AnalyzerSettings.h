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
  enum Type {
    BIT,
    FRAME
  } mType;

protected:
  std::auto_ptr<AnalyzerSettingInterfaceChannel> mInputChannelInterface;
  std::auto_ptr<AnalyzerSettingInterfaceNumberList> mTypeInterface;
};

#endif //WS2812_ANALYZER_SETTINGS
