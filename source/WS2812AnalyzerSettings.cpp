#include "WS2812AnalyzerSettings.h"
#include <AnalyzerHelpers.h>


WS2812AnalyzerSettings::WS2812AnalyzerSettings()
  : mInputChannel(UNDEFINED_CHANNEL),
    mType(FRAME) {
  mInputChannelInterface.reset(new AnalyzerSettingInterfaceChannel());
  mInputChannelInterface->SetTitleAndTooltip("WS2812",
					     "WS2812 LEDs (800 kHz only)");
  mInputChannelInterface->SetChannel(mInputChannel);

  mTypeInterface.reset(new AnalyzerSettingInterfaceNumberList());
  mTypeInterface->SetTitleAndTooltip("Analysis Type",
					     "Bits or frames?");
  mTypeInterface->AddNumber(BIT, "Bits", "Individual bits");
  mTypeInterface->AddNumber(FRAME, "Frames", "24 bit GBR frames");
  mTypeInterface->SetNumber(mType);

  AddInterface(mInputChannelInterface.get());
  AddInterface(mTypeInterface.get());

  AddExportOption( 0, "Export as text/csv file" );
  AddExportExtension( 0, "text", "txt" );
  AddExportExtension( 0, "csv", "csv" );

  ClearChannels();
  AddChannel( mInputChannel, "Serial", false );
}

WS2812AnalyzerSettings::~WS2812AnalyzerSettings() {
}

bool WS2812AnalyzerSettings::SetSettingsFromInterfaces() {
  mInputChannel = mInputChannelInterface->GetChannel();
  mType = static_cast<Type>(mTypeInterface->GetNumber());

  ClearChannels();
  AddChannel(mInputChannel, "WS2812", true);

  return true;
}

void WS2812AnalyzerSettings::UpdateInterfacesFromSettings() {
  mInputChannelInterface->SetChannel(mInputChannel);
  mTypeInterface->SetNumber(mType);
}

void WS2812AnalyzerSettings::LoadSettings(const char* settings) {
  SimpleArchive text_archive;
  text_archive.SetString( settings );

  text_archive >> mInputChannel;
  int t;
  text_archive >> t;
  mType = static_cast<Type>(t);
  if (mType != FRAME && mType != BIT)
    mType = FRAME;

  ClearChannels();
  AddChannel( mInputChannel, "WS2812", true );

  UpdateInterfacesFromSettings();
}

const char* WS2812AnalyzerSettings::SaveSettings() {
  SimpleArchive text_archive;

  text_archive << mInputChannel;
  text_archive << mType;

  return SetReturnString( text_archive.GetString() );
}
