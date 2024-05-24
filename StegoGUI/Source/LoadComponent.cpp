/*
  ==============================================================================

    LoadComponent.cpp
    Created: 24 May 2024 12:59:35pm
    Author:  fomin

  ==============================================================================
*/

#include "../Include/LoadComponent.h"

LoadComponent::LoadComponent(bool& _load)
{
   setSize(800, 600);
   setFramesPerSecond(10);
   load = &_load;
}

LoadComponent::~LoadComponent()
{
}

void LoadComponent::paint(juce::Graphics& g)
{
   if( (getFrameCounter() % 2) == 0)
      g.fillAll(Colours::red);
   else
      g.fillAll(Colours::blue);
}

void LoadComponent::update()
{
   if (*load) {
      setVisible(true);
   }
   else {
      setVisible(false);
   }
   //
   repaint();
}
