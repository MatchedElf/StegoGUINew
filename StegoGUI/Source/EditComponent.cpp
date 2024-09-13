#include "../Include/EditComponent.h"

EditComponent::EditComponent(const wchar_t* _filename) : Component()
{
   newFile = Create_File("../../Images/Results/new1.bmp", _filename);
   int size;
   String tmp;
   //
   pixels = ReadFile(_filename, height, width, size, tmp);
   //
   pixelsNew = new RGB * [height + 1];
   for (int i = 0; i < height + 1; i++) pixelsNew[i] = new RGB[width + 1];
   for (int i = 0; i < height; i++)
      for (int j = 0; j < width; j++)
         pixelsNew[i][j] = pixels[i][j];
   //
   addMouseListener(this, true);
   //
   menu = new EditMenuComponent();
   addAndMakeVisible(menu);
   menu->setVisible(false);
   //
   Image menuLogo = ImageCache::getFromFile(File::getCurrentWorkingDirectory().getChildFile("menu.png"));
   hideBut = new ImageButton();
   hideBut->addListener(this);
   hideBut->setImages(false, true, true, menuLogo, 1.0f, Colours::transparentWhite, menuLogo, 0.3f, Colours::transparentWhite, menuLogo, 0.1f, Colours::transparentWhite);
   addAndMakeVisible(hideBut);
   //
   menu->undoBut->addListener(this);
   menu->eraseBut->addListener(this);
   //
   setSize(width, height);
}

EditComponent::~EditComponent()
{
   for (map<newPoint, Colour>::iterator it = paintMap.begin(); it != paintMap.end(); it++)
   {
      newPoint pos = it->first;
      Colour curColour = it->second;
      pixelsNew[height - pos.getY()][pos.getX()] = MakeColor(curColour.getRed(),
                                                             curColour.getGreen(),
                                                             curColour.getBlue());
   }
   WriteToFile(newFile, pixelsNew, height, width);
   fclose(newFile);
   deleteAndZero(menu);
   deleteAndZero(hideBut);
}

void EditComponent::paint(juce::Graphics& g)
{
   juce::Rectangle<float> rec((float)getWidth(), (float)getHeight());
   g.drawImage(ImageFileFormat::loadFrom(File::getCurrentWorkingDirectory().getChildFile("new.png")), rec);
   //
   for (map<newPoint, Colour>::iterator it = paintMap.begin(); it != paintMap.end(); it++)
   {
      g.setColour(it->second);
      //g.fillRect(it->first.getX(), it->first.getY(), 1, 1);
      g.drawRect(it->first.getX(), it->first.getY(), 1, 1);
   }
}

void EditComponent::resized()
{
   hideBut->setBounds(0, 0, (int)(getWidth() * 0.1), (int)(getHeight() * 0.1));
   if (!hided)
      menu->setBounds((int)(getWidth() * 0.15), 0, (int)(getWidth() * 0.75), (int)(getHeight() * 0.1));
}

void EditComponent::buttonClicked(Button* butt)
{
   if (butt == hideBut)
   {
      hided = !hided;
      menu->setVisible(!menu->isVisible());
      resized();
   }
   /*else if (butt == menu->undoBut)
   {
      paintPoints.erase(paintPoints.end() - 1);
      repaint();
   }*/
}

void EditComponent::mouseDown(const MouseEvent& ev)
{
   if (hided)
   {
      if (!menu->eraseBut->getToggleState())
         drawPoint(ev.getPosition());
      else
         erasePoint(ev.getPosition());
   }
   else if(!menu->isMouseOver(true))
   {
      hided = true;
      menu->setVisible(!menu->isVisible());
      resized();
   }
}

void EditComponent::mouseDrag(const MouseEvent& ev)
{
   if (hided)
   {
      if (!menu->eraseBut->getToggleState())
      {
         drawPoint(ev.getPosition());
#if 0
         int xPos = lastPoint.getX();
         int yPos = lastPoint.getY();
         //
         int xEnd = ev.getPosition().getX();
         int yEnd = ev.getPosition().getY();
         //
         int yOff = abs(ev.getPosition().getY() - lastPoint.getY());
         int xOff = abs(ev.getPosition().getX() - lastPoint.getX());
         //
         int sx = (xPos < lastPoint.getX()) ? -1 : 1;
         int sy = (yPos < lastPoint.getY()) ? -1 : 1;
         //
         int err = xOff - yOff;
         //
         while (true)
         {
            Point<int> _point(xPos, yPos);
            drawPoint(_point);
            //
            if ( (xPos == xEnd) && (yPos == yEnd))
               break;
            int e2 = 2 * err;
            if (e2 > -yOff)
            {
               err -= yOff;
               xPos += sx;
            }
            if (e2 < xOff)
            {
               err += xOff;
               yPos += sy;
            }


         }
#elseif 0
         Point<int> lastPoint = lastPointCheck;
         //
         int xPos = lastPoint.getX();
         int yPos = lastPoint.getY();
         //
         int xEnd = ev.getPosition().getX();
         int yEnd = ev.getPosition().getY();
         //
         int yOff = abs(yEnd - yPos);
         int xOff = abs(xEnd - xPos);
         //
         int y = 0;
         int x = 0;
         int sx = (xEnd < xPos) ? -1 : 1;
         int sy = (yEnd < yPos) ? -1 : 1;
         int count = 0;
         //int step = (min(xOff, yOff) != 0) ? max(xOff, yOff) / min(xOff, yOff) : max(xOff, yOff);
         int step = (xOff != yOff) ? max(xOff, yOff) - min(xOff, yOff) : 200;
         /*while (true)
         {
            if (((xPos == xEnd) && (yPos == yEnd)) || (count > 200))
               break;
            if ((count % step) == 0)
            {
               yPos+=sy;
            }
            else
            {
               xPos+=sx;
            }
            Point<int> _point(xPos, yPos);
            drawPoint(_point);
            count++;
         }*/
         for (int i = 0; i < xOff; i ++)
         {
            for (int j = 0; j < yOff; j ++)
            {
               if (((xPos == xEnd) && (yPos == yEnd)) || (count > 200))
                  break;
               /*xPos += sx;
               yPos += sy;*/
               Point<int> _point(xPos + i, yPos + j);
               drawPoint(_point);
            }
         }
#endif
         //lastPoint = ev.getPosition();

      }
      else
         erasePoint(ev.getPosition());
   }
   else if (!menu->isMouseOver(true))
   {
      hided = true;
      menu->setVisible(!menu->isVisible());
      resized();
   }
   /*newPoint pos(ev.getPosition().getX(), ev.getPosition().getY());
   Colour curColour = menu->colourSelect->getCurrentColour();
   paintMap.insert(pair<newPoint, Colour>(pos, curColour));
   repaint();*/
}

void EditComponent::mouseMove(const MouseEvent& event)
{
   lastPointCheck = event.getPosition();
}

void EditComponent::drawPoint(Point<int> _point)
{
   for (int i = 0; i < menu->thickSlider->getValue(); i++)
   {
      for (int j = 0; j < menu->thickSlider->getValue(); j++)
      {
         newPoint pos(_point.getX() + j, _point.getY() + i);
         Colour curColour = menu->colourSelect->getCurrentColour();
         //
         //
         if (((height - pos.getY() + i) < height) &&
            ((height - pos.getY() + i) > 0) &&
            ((pos.getX() + j) < width) &&
            ((pos.getX() + j) > 0))
         {
            paintMap.insert(pair<newPoint, Colour>(pos, curColour));
         }
      }

   }
   repaint();
}

void EditComponent::erasePoint(Point<int> _point)
{
   for (int i = 0; i < menu->thickSlider->getValue(); i++)
   {
      for (int j = 0; j < menu->thickSlider->getValue(); j++)
      {
         newPoint pos(_point.getX() + j, _point.getY() + i);
         paintMap.erase(pos);       
      }
   }
   repaint();
}

EditWindow::~EditWindow()
{

}

void EditWindow::closeButtonPressed()
{
   this->~EditWindow();
}

EditComponent::EditMenuComponent::EditMenuComponent() : Component()
{
   setOpaque(true);
   setLookAndFeel(this);
   compsList = new Component * [4];
   //
   eraseBut = new ToggleButton();
   addAndMakeVisible(eraseBut);
   eraseBut->setButtonText(String((std::wstring(L"")).c_str()));
   //
   Image undoLogo = ImageCache::getFromFile(File::getCurrentWorkingDirectory().getChildFile("Undo.png"));
   undoBut = new ImageButton();
   addAndMakeVisible(undoBut);
   undoBut->setImages(false, true, true, undoLogo, 1.0f, Colour::fromRGB(255, 255, 255), undoLogo, 0.01f, Colour::fromRGB(0, 255, 255), undoLogo, 0.3f, Colour::fromRGB(128, 128, 128));
   //
   colourSelect = new ColourSelector(ColourSelector::showColourspace);
   addAndMakeVisible(colourSelect);
   //
   thickSlider = new Slider(Slider::LinearHorizontal, Slider::TextBoxRight);
   addAndMakeVisible(thickSlider);
   thickSlider->setRange(1, 20, 1);
   //
   compsList[0] = eraseBut;
   compsList[1] = undoBut;
   compsList[2] = colourSelect;
   compsList[3] = thickSlider;
   //
   FSizer = new StretchableLayoutManager();
   FSizer->setItemLayout(0, 1, 100000, -1);
   FSizer->setItemLayout(1, 1, 100000, -1);
   FSizer->setItemLayout(2, 1, 100000, -1);
   FSizer->setItemLayout(3, 1, 100000, -1);
}

EditComponent::EditMenuComponent::~EditMenuComponent()
{
   setLookAndFeel(nullptr);
   deleteAndZero(eraseBut);
   deleteAndZero(undoBut);
   deleteAndZero(colourSelect);
   deleteAndZero(thickSlider);
   deleteAndZero(FSizer);
   delete[] compsList;
}

void EditComponent::EditMenuComponent::paint(juce::Graphics& g)
{
   g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
   g.setFont(juce::Font(16.0f));
   g.setColour(juce::Colours::white);
}

void EditComponent::EditMenuComponent::resized()
{
   FSizer->layOutComponents(compsList, 4, 0, 0, getWidth(), getHeight(), false, true);
}

void EditComponent::EditMenuComponent::drawTickBox(Graphics& g, Component& but, float , float , float , float , bool ticked, bool , bool , bool )
{
   juce::Rectangle<float> rec(but.getWidth() * 0.2f, but.getHeight() * 0.2f, but.getWidth() * 0.6f, but.getHeight() * 0.6f);
   Image eraseIm = ImageCache::getFromFile(File::getCurrentWorkingDirectory().getChildFile("Eraser.png"));
   g.setColour(Colours::red);
   if(ticked)
      g.drawImage(eraseIm, rec, RectanglePlacement::stretchToFit, true);
   else
      g.drawImage(eraseIm, rec);
}

bool EditComponent::newPoint::operator<(newPoint t1) const
{
   int _x = getX();
   int _x1 = t1.getX();
   int _y = getY();
   int _y1 = t1.getY();
   if (_x < _x1) return true;
   else if ((_x == _x1) && (_y < _y1)) return true;
   else return false;
}

EditComponent::newPoint::newPoint(int x, int y)
{
   setX(x);
   setY(y);
}
