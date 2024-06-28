#pragma once
//
#include <JuceHeader.h>
using namespace juce;
//
class BlackComponent : public Component
{
public:
   BlackComponent();
   ~BlackComponent();
   void paint(juce::Graphics&) override;
private:
//
};