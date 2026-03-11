#pragma once

#include <JuceHeader.h>
#include "Stego.h"
#include "DecodeBMP.h"
#include "BlackComponent.h"
#include "MenuComponent.h"
#include "HelpComponent.h"
#include "EditComponent.h"
#include "LoadWindow.h"
#include <cstdlib>

namespace Stego
{
    enum AlgNum
    {
        DCT = 1,
        DFT = 2,
        LSB = 3,
        DCT_KOCH = 4,
        HAAR = 5
    };
}
//
using namespace juce;
//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public Component, public Button::Listener, public ComponentListener, public LookAndFeel_V4/*, public ThreadWithProgressWindow*/
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    
    void initGui();
    
    void paintOrig(bool error);
    void paintDiffText(string _orig, string _new);
    void startDecode();

    void paint(juce::Graphics&) override;
    void resized() override;
    void buttonClicked(Button* butt) override;
    void componentNameChanged(Component& component) override;
    Font getTextButtonFont(TextButton&, int buttonHeight) override;
    //void timerCallback() override;
    //void threadComplete(bool userPressedCancel) override;
    //void run() override;
    //
private:
    ImageComponent* _openLogo;
    ImageComponent* _openTitle;
    //
    Label* _origTitle;
    Label* _diffTitle;
    Label* _newTitle;
    //
    ImageComponent* _orig;
    ImageComponent* _diff;
    ImageComponent* _newIm;
    //
    ImageComponent* _error;
    TextButton* _closeErr;
    //
    ImageButton* _startBut;
    ImageButton* _infoBut;
    ImageButton* _mainBut;
    ImageButton* _hideBut;
    ImageButton* _homeBut;
    ImageButton* _editBut;
    //
    Label* _origLabel;
    Label* _decodeLabel;
    Label* _textLabel;
    // 
    TextEditor* _origInfo;
    TextEditor* _decodeInfo;
    TextEditor* _decodeText;
    //
    Component* _chooseChecker;
    //
    bool _hidden = false;
    //
    bool _startScreen = true;
    bool _edited = false;
    //
    BlackComponent* _black;
    //
    MenuComponent* _menuC;
    HelpComponent* _helpC;
    //
    //LoadWindow* loadingGif;
    //ProgressBar* progress;
    //
    Component** _compsList;
    StretchableLayoutManager* _FSizer;
    //
    double _progressStatus = 0.0;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};