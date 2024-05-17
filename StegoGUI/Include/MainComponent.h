#pragma once

#include <JuceHeader.h>
#include "Stego.h"
#include "DecodeBMP.h"
#include "MenuComponent.h"
using namespace juce;
//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public Component, public Button::Listener/*, public ComboBox::Listener*/
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;
    void buttonClicked(Button* butt) override;
    //void comboBoxChanged(ComboBox* cb) override;
    void startDecode();
    //
private:
    //==============================================================================
    // Your private member variables go here...
    //StretchableLayoutManager* FSizer;
    //StretchableLayoutManager* imagesSizer;
    //Grid* grid1;
    Label* origTitle;
    Label* diffTitle;
    Label* newTitle;
    //
    ImageComponent* orig;
    ImageComponent* diff;
    ImageComponent* newIm;
    ImageComponent* clock;
    //
    //TextButton* imageCh;
    //ComboBox* secrCh;
    ImageButton* startBut;
    ImageButton* hideBut;
    //ToggleButton* attack;
    //
    //Component** compsList;
    //
    Label* origInfo;
    Label* decodeInfo;
    Label* decodeText;
    //
    //SidePanel* sdPanel;
    //
    //int selectedTr = 1;
    //
    bool hided = false;
    //
    MenuComponent* menuC;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};