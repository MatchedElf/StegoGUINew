#pragma once

#include <JuceHeader.h>
//
using namespace juce;

class LoadWindow : public AlertWindow, public Timer
{
public:
   LoadWindow(const String& title, const String& message, MessageBoxIconType iconType, Component* associatedComponent = nullptr);
   ~LoadWindow() override;
   void timerCallback() override;
   void paint(juce::Graphics&) override;
private:
   String status = "0";
};
