#include "../Include/EditComponent.h"

EditComponent::EditComponent(const wchar_t* _filename)
{
   newFile = Create_File("../../Images/Results/new1.bmp", _filename);
   int size;
   String tmp;
   pixels = ReadFile(_filename, height, width, size, tmp);
   image = new ImageComponent();
   image->setImage(ImageFileFormat::loadFrom(File::getCurrentWorkingDirectory().getChildFile("new.png")));
   //addAndMakeVisible(image);
   setSize(width, height);
   image->setBounds(0, 0, width, height);
   image->toBack();
   addMouseListener(this, true);
}

EditComponent::~EditComponent()
{
   WriteToFile(newFile, pixels, height, width);
   fclose(newFile);
   deleteAndZero(image);
}

void EditComponent::paint(juce::Graphics& g)
{
   juce::Rectangle<float> rec((float)getWidth(), (float)getHeight());
   g.drawImage(ImageFileFormat::loadFrom(File::getCurrentWorkingDirectory().getChildFile("new.png")), rec);
   g.setColour(Colours::black);
   g.fillRect(coord.getX(), coord.getY(), 10, 10);
   //g.fillAll();
}

void EditComponent::resized()
{
}

void EditComponent::mouseDown(const MouseEvent& ev)
{
   coord = ev.getMouseDownPosition();
   repaint();
   for (int i = 0; i < 10; i++)
   {
      for (int j = 0; j < 10; j++)
      {
         pixels[height - coord.getY() + i][coord.getX() + j] = MakeColor(0, 0, 0);
      }
   }
}

EditWindow::~EditWindow()
{

}

void EditWindow::closeButtonPressed()
{
   this->~EditWindow();
}
