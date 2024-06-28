#pragma once

#include <JuceHeader.h>
#include "Stego.h"
#include "DecodeBMP.h"
#include "BlackComponent.h"
#include "MenuComponent.h"
#include "LoadWindow.h"
#include <cstdlib>
//
using namespace juce;
//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public Component, public Button::Listener, public ComponentListener, public LookAndFeel_V4, public Timer
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;
    void buttonClicked(Button* butt) override;
    void componentNameChanged(Component& component) override;
    void paintOrig(bool error);
    void startDecode();
    Font getTextButtonFont(TextButton&, int buttonHeight) override;
    void timerCallback() override;
    //
private:
    ImageComponent* openLogo;
    ImageComponent* openTitle;
    //
    Label* origTitle;
    Label* diffTitle;
    Label* newTitle;
    //
    ImageComponent* orig;
    ImageComponent* diff;
    ImageComponent* newIm;
    //
    ImageComponent* error;
    TextButton* closeErr;
    //
    ImageButton* startBut;
    ImageButton* hideBut;
    //
    Label* origLabel;
    Label* decodeLabel;
    Label* textLabel;
    // 
    TextEditor* origInfo;
    TextEditor* decodeInfo;
    TextEditor* decodeText;
    //
    Component* chooseChecker;
    //
    bool hidden = false;
    //
    bool startScreen = true;
    //
    BlackComponent* black;
    //
    MenuComponent* menuC;
    //
    LoadWindow* loadingGif;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};