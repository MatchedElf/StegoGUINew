#pragma once

#include <JuceHeader.h>
using namespace juce;

class LoadComponent : public AnimatedAppComponent
{
public:
   LoadComponent(bool&);
   ~LoadComponent();
   void paint(juce::Graphics&) override;
   void update() override;
private:
   bool* load;
};