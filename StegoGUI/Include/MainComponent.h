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
class MainComponent : public Component, public Button::Listener/*, public MouseListener*//*, public ComboBox::Listener*/
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;
    void buttonClicked(Button* butt) override;
    //void mouseUp(const MouseEvent &event) override;
    //void comboBoxChanged(ComboBox* cb) override;
    void startDecode();
    //
private:
    //==============================================================================
    // Your private member variables go here...
    //StretchableLayoutManager* FSizer;
    //StretchableLayoutManager* imagesSizer;
    //Grid* grid1;
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
    ImageComponent* clock;
    ImageComponent* error;
    TextButton* closeErr;
    //
    //TextButton* imageCh;
    //ComboBox* secrCh;
    ImageButton* startBut;
    ImageButton* hideBut;
    //ToggleButton* attack;
    //
    //Component** compsList;
    //
    Label* origLabel;
    Label* decodeLabel;
    Label* textLabel;
    // 
    //Label* origInfo;
    //Label* decodeInfo;
    //Label* decodeText;
    TextEditor* origInfo;
    TextEditor* decodeInfo;
    TextEditor* decodeText;
    //
    //SidePanel* sdPanel;
    //
    //int selectedTr = 1;
    //
    bool hided = false;
    //
    bool loadingFlag = false;
    bool startScreen = true;
    //
    MenuComponent* menuC;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};