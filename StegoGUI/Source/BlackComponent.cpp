#include "../Include/BlackComponent.h"
//
BlackComponent::BlackComponent() : Component()
{
}
//
BlackComponent::~BlackComponent()
{
}
//
void BlackComponent::paint(juce::Graphics& g)
{
   g.fillAll(juce::Colour::fromRGBA(0, 0, 0, 180));
}