#include "../Include/HelpComponent.h"

HelpComponent::HelpComponent() : Component()
{
   Font font;
   font.setHeight(25);
   //
   setOpaque(false);
   //
   text = new TextEditor();
   text->setReadOnly(true);
   text->setCaretVisible(false);
   text->setMultiLine(true);
   text->setScrollbarsShown(true);
   text->setFont(font);
   text->setText("This is help text. This is help text. This is help text. This is help text. This is help text.\nThis is help text.\nThis is help text.\nThis is help text.\nThis is help text.\nThis is help text.\nThis is help text.\nThis is help text.\n");
   addAndMakeVisible(text);
   //
   emoji = new ImageComponent();
   emoji->setImage(ImageCache::getFromFile(File::getCurrentWorkingDirectory().getChildFile("emoji1.png")));
   addAndMakeVisible(emoji);
   //
   compsList = new Component * [4];
   //
   compsList[0] = text;
   compsList[1] = emoji;
   //
   FSizer = new StretchableLayoutManager();
   FSizer->setItemLayout(0, 1, 100000, -1);
   FSizer->setItemLayout(1, 1, 100000, -1);
   //
   setSize(600, 700);
   //resized();
}

HelpComponent::~HelpComponent()
{
   deleteAndZero(text);
   deleteAndZero(emoji);
   deleteAndZero(FSizer);
   delete[] compsList;
}

void HelpComponent::paint(juce::Graphics&)
{
   //g.fillAll(Colour::fromRGBA(200, 0, 0, 128));
}

void HelpComponent::resized()
{
   FSizer->layOutComponents(compsList, 2, 0, 0, getWidth(), getHeight(), true, true);
}

HelpWindow::~HelpWindow()
{
}

void HelpWindow::closeButtonPressed()
{
   this->~HelpWindow();
}

//void HelpWindow::paint(juce::Graphics&)
//{
//}
//
//void HelpWindow::resized()
//{
//}
