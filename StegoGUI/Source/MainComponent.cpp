#include "../Include/MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
    : Component("MainComponent")
{
    Font font;
    int ttt = 255 * (5 < 1);
    font.setHeight(25);
    //
    setOpaque(true);
    addMouseListener(this, true);
    //
    openLogo = new ImageComponent();
    openLogo->setImage(ImageCache::getFromFile(File::getCurrentWorkingDirectory().getChildFile("logo4.png")));
    addAndMakeVisible(openLogo);
    //
    openTitle = new ImageComponent();
    openTitle->setImage(ImageCache::getFromFile(File::getCurrentWorkingDirectory().getChildFile("StegoGUI.png")));
    addAndMakeVisible(openTitle);
    //
    origTitle = new Label();
    origTitle->setFont(font);
    origTitle->setText(String((std::wstring(L"������ ���������")).c_str()), dontSendNotification);
    addAndMakeVisible(origTitle);
    //
    diffTitle = new Label();
    diffTitle->setFont(font);
    diffTitle->setText(String((std::wstring(L"���������� �����������")).c_str()), dontSendNotification);
    addAndMakeVisible(diffTitle);
    //
    newTitle = new Label();
    newTitle->setFont(font);
    newTitle->setText(String((std::wstring(L"����������� ���������")).c_str()), dontSendNotification);
    addAndMakeVisible(newTitle);
    //
    Image startLogo = ImageCache::getFromFile(File::getCurrentWorkingDirectory().getChildFile("play.png"));
    startBut = new ImageButton();
    startBut->addListener(this);
    startBut->setImages(false, true, true, startLogo, 1, Colours::transparentWhite, startLogo, 0.3, Colours::transparentWhite, startLogo, 1, Colours::transparentWhite);
    //
    Image menuLogo = ImageCache::getFromFile(File::getCurrentWorkingDirectory().getChildFile("menu.png"));
    hideBut = new ImageButton();
    hideBut->addListener(this);
    hideBut->setImages(false, true, true, menuLogo, 1, Colours::transparentWhite, menuLogo, 0.3, Colours::transparentWhite, menuLogo, 1, Colours::transparentWhite);
    //
    orig = new ImageComponent();
    orig->setImage(ImageCache::getFromFile(File::getCurrentWorkingDirectory().getChildFile("empty.png")));
    addAndMakeVisible(orig);
    //
    diff = new ImageComponent();
    diff->setImage(ImageCache::getFromFile(File::getCurrentWorkingDirectory().getChildFile("empty.png")));
    addAndMakeVisible(diff);
    //
    newIm = new ImageComponent();
    newIm->setImage(ImageCache::getFromFile(File::getCurrentWorkingDirectory().getChildFile("empty.png")));
    addAndMakeVisible(newIm);
    //
    font.setHeight(25);
    //
    origLabel = new Label();
    origLabel->setFont(font);
    origLabel->setText(String((std::wstring(L"���������� �� �����������")).c_str()), dontSendNotification);
    addAndMakeVisible(origLabel);
    // 
    decodeLabel = new Label();
    decodeLabel->setFont(font);
    decodeLabel->setText(String((std::wstring(L"���������� �� ����������")).c_str()), dontSendNotification);
    addAndMakeVisible(decodeLabel);
    //
    textLabel = new Label();
    textLabel->setFont(font);
    textLabel->setText(String((std::wstring(L"����������� �����")).c_str()), dontSendNotification);
    addAndMakeVisible(textLabel);
    //
    origInfo = new TextEditor();
    origInfo->setCaretVisible(false);
    origInfo->setMultiLine(true);
    origInfo->setReadOnly(true);
    origInfo->setScrollbarsShown(true);
    origInfo->setFont(font);
    addAndMakeVisible(origInfo);
    //
    decodeInfo = new TextEditor();
    decodeInfo->setCaretVisible(false);
    decodeInfo->setMultiLine(true);
    decodeInfo->setReadOnly(true);
    decodeInfo->setScrollbarsShown(true);
    decodeInfo->setFont(font);
    addAndMakeVisible(decodeInfo);
    //
    decodeText = new TextEditor();
    decodeText->setCaretVisible(false);
    decodeText->setMultiLine(true);
    decodeText->setReadOnly(true);
    decodeText->setScrollbarsShown(true);
    decodeText->setFont(font);
    addAndMakeVisible(decodeText);
    //
    chooseChecker = new Component("1");
    chooseChecker->addComponentListener(this);
    menuC = new MenuComponent(chooseChecker);
    //
    addAndMakeVisible(menuC);
    addAndMakeVisible(startBut);
    addAndMakeVisible(hideBut);
    //
    clock = new ImageComponent();
    clock->setImage(ImageCache::getFromFile(File::getCurrentWorkingDirectory().getChildFile("clock.png")));
    addAndMakeVisible(clock);
    clock->setVisible(false);
    //
    error = new ImageComponent();
    error->setImage(ImageCache::getFromFile(File::getCurrentWorkingDirectory().getChildFile("error.png")));
    addAndMakeVisible(error);
    error->setVisible(false);
    //
    closeErr = new TextButton(String((std::wstring(L"����� �� �������!!!\n������ ���� ��� ��������")).c_str()));
    closeErr->addListener(this);
    addAndMakeVisible(closeErr);
    closeErr->setVisible(false);
    //
    chooseChecker->addComponentListener(this);
    setSize(600, 400);
}

MainComponent::~MainComponent()
{
    deleteAndZero(openLogo);
    deleteAndZero(openTitle);
    deleteAndZero(origTitle);
    deleteAndZero(diffTitle);
    deleteAndZero(newTitle);
    deleteAndZero(orig);
    deleteAndZero(diff);
    deleteAndZero(newIm);
    deleteAndZero(startBut);
    deleteAndZero(hideBut);
    deleteAndZero(origLabel);
    deleteAndZero(decodeLabel);
    deleteAndZero(textLabel);
    deleteAndZero(origInfo);
    deleteAndZero(decodeInfo);
    deleteAndZero(decodeText);
    deleteAndZero(menuC);
    deleteAndZero(clock);
    deleteAndZero(error);
    deleteAndZero(closeErr);
    deleteAndZero(chooseChecker);
}

//==============================================================================
void MainComponent::paint(juce::Graphics& g)
{
   if (loadingFlag) { 
      g.fillAll(Colours::red);
   }
   else { 
      g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId)); 
   }
    g.setFont(juce::Font(16.0f));
    g.setColour(juce::Colours::white);

}

void MainComponent::resized()
{
   if (startScreen)
   {
      openTitle->setBounds(getWidth() * 0.3, getHeight() * 0.22, getWidth() * 0.4, getHeight() * 0.08);
      openLogo->setBounds(getWidth() * 0.3, getHeight() * 0.3, getWidth() * 0.4, getHeight() * 0.4);
      startBut->setBounds(getWidth() * 0.45, getHeight() * 0.75, getWidth() * 0.1, getHeight() * 0.1);
   }
   else
   {
      Grid grid;
      //
      using Track = Grid::TrackInfo;
      using Fr = Grid::Fr;
      //
      grid.templateRows = { Track(Fr(1)), Track(Fr(12)), Track(Fr(1)), Track(Fr(4)) };
      grid.templateColumns = { Track(Fr(1)), Track(Fr(1)), Track(Fr(1)) };
      //
      grid.items = { GridItem(origTitle), GridItem(diffTitle), GridItem(newTitle), GridItem(orig), GridItem(diff), GridItem(newIm), GridItem(origLabel), GridItem(decodeLabel), GridItem(textLabel), GridItem(origInfo), GridItem(decodeInfo), GridItem(decodeText) };
      //
      menuC->setBounds(0, 0, getWidth() * 0.1 - 10, getHeight() * 1);

      hideBut->setBounds(0, 0, getWidth() * 0.05 - 10, getHeight() * 0.05 - 5);
      startBut->setBounds(getWidth() * 0.05, 0, getWidth() * 0.05 - 10, getHeight() * 0.05 - 5);
      //
      if (hided)
      {
         grid.performLayout(juce::Rectangle<int>(0, getHeight() * 0.05, getWidth(), getHeight() * 0.95));
      }
      else
      {
         grid.performLayout(juce::Rectangle<int>(getWidth() * 0.1, 0, getWidth() * 0.9, getHeight()));
      }
      clock->setBounds(getWidth() * 0.4, getHeight() * 0.3, getWidth() * 0.2, getHeight() * 0.4);
      error->setBounds(getWidth() * 0.35, getHeight() * 0.25, getWidth() * 0.3, getHeight() * 0.3);
      closeErr->setBounds(getWidth() * 0.4, getHeight() * 0.55, getWidth() * 0.2, getHeight() * 0.2);
      juce::Rectangle test(0, 0, 1, 1);
   }
}

void MainComponent::buttonClicked(Button* butt)
{
   int t = 1;
    if (butt == startBut)
    {
       if (!startScreen)
       {
          if( (menuC->imageName != "-1") && (menuC->secrName != "-1"))
            startDecode();
          else
          {
             error->setVisible(true);
             closeErr->setVisible(true);
             resized();
          }
       }

       else
       {
          startScreen = false;
          openLogo->setVisible(false);
          openTitle->setVisible(false);
          resized();
       }
    }
    else if (butt == hideBut)
    {
       menuC->setName("HI");
       hided = !hided;
       menuC->setVisible(!(menuC->isVisible()));
       resized();
    }
    else if (butt == closeErr)
    {
       error->setVisible(false);
       closeErr->setVisible(false);
       resized();
    }
}
void MainComponent::componentNameChanged(Component& component)
{
   paintOrig();
}
void MainComponent::paintOrig()
{

   doDecode(menuC->imageName.toRawUTF8(), "orig.png");
   orig->setImage(ImageFileFormat::loadFrom(File::getCurrentWorkingDirectory().getChildFile("orig.png")), sendNotification);
   //
   int height;
   int width;
   int size;
   String info;
   //
   ReadFile(menuC->imageName.toRawUTF8(), height, width, size, info);
   origInfo->setText(info, sendNotification);
   diff->setImage(ImageCache::getFromFile(File::getCurrentWorkingDirectory().getChildFile("empty.png")));
   newIm->setImage(ImageCache::getFromFile(File::getCurrentWorkingDirectory().getChildFile("empty.png")));
   decodeInfo->setText("", sendNotification);
   decodeText->setText("", sendNotification);
}
//
void MainComponent::startDecode()
{
    srand(time(NULL));
    if (menuC->imageName == "")
    {
       menuC->imageName = "orig.bmp";
    }
    doDecode(menuC->imageName.toRawUTF8(), "orig.png");
    orig->setImage(ImageFileFormat::loadFrom(File::getCurrentWorkingDirectory().getChildFile("orig.png")), sendNotification);
    origInfo->setText("In progress", sendNotification);
    decodeInfo->setText("In progress", sendNotification);
    decodeText->setText("In progress", sendNotificationAsync);
    //
    int difference = 5;
    complex<double> differenceComplex(1.0, 0.0);
    int word_size = 0;
    //
    vector<bitset<8>> vect = ReadWord(menuC->secrName.toRawUTF8(), word_size);
    vector<bitset<8>> vectSzhat;
    bitset<16> secr_size(word_size);
    bitset<8> empty;
    vectSzhat.push_back(empty);
    vectSzhat.push_back(empty);
    //
    int height;
    int width;
    int size;
    //
    String info;
    //
    RGB** pixels = ReadFile(menuC->imageName.toRawUTF8(), height, width, size, info);
    RGB** pixelsNew;
    //
    origInfo->setText(info, sendNotification);
    //
    if (menuC->isAttack)
    {
        pixelsNew = ReadFile("new.bmp", height, width, size, info);
        vector<int> key = ReadKey("key.txt", vect);
        //
        if (menuC->selectedTr == 1)
        {
            string result = decodeDCP(height, width, pixels, pixelsNew, vect, true, vectSzhat, key);
            decodeText->setText(result, sendNotification);
        }
        else if (menuC->selectedTr == 2)
        {
            string result = decodeFurie(height, width, pixels, pixelsNew, vect, vectSzhat, key);
            decodeText->setText(result, sendNotification);
        }
        else if (menuC->selectedTr == 3)
        {
            string result = decodeLSB(height, width, pixelsNew, vect, true, vectSzhat);
            decodeText->setText(result, sendNotification);
        }
    }
    else
    {
        FILE* newFile = Create_File("new.bmp", menuC->imageName.toRawUTF8());
        //
        pixelsNew = ReadFile(menuC->imageName.toRawUTF8(), height, width, size, info);
        //
        if (menuC->selectedTr == 1)
        {
            vector<int> key1 = CreateKey("key.txt", size, vect.size(), true);
            //
            encodeDCP(height, width, pixelsNew, vect, secr_size, difference, key1);
            WriteToFile(newFile, pixelsNew, height, width);
            //
            string result = decodeDCP(height, width, pixels, pixelsNew, vect, true, vectSzhat, key1);
            decodeText->setText(result, sendNotification);
            //

        }
        //
        else if (menuC->selectedTr == 2)
        {
            vector<int> key1 = CreateKey("key.txt", size, vect.size(), true);
            //
            encodeFurie(height, width, pixelsNew, vect, secr_size, differenceComplex, key1);
            WriteToFile(newFile, pixelsNew, height, width);
            //
            string result = decodeFurie(height, width, pixels, pixelsNew, vect, vectSzhat, key1);
            decodeText->setText(result, sendNotification);
        }
        //
        else if (menuC->selectedTr == 3)
        {
            vector<int> key = CreateKey("key.txt", size, vect.size(), false);
            //
            encodeLSB(height, width, pixelsNew, vect, secr_size);
            WriteToFile(newFile, pixelsNew, height, width);
            //
            string result = decodeLSB(height, width, pixelsNew, vect, true, vectSzhat);
            decodeText->setText(result, sendNotification);
        }
        fclose(newFile);
    }

    //
    long double redP, greenP, blueP;
    PSNR(pixels, pixelsNew, redP, greenP, blueP, height, width);
    String inf = "";
    inf += "PSNR = ";
    inf += String(to_string(blueP));
    inf += "\n";
    //
    inf += String((std::wstring(L"����������� ������������� = ")).c_str());
    double coef = word_size * 8.0 / (height * width);
    inf += String(to_string(coef));
    inf += "\n";
    //
    inf += String((std::wstring(L"����������� �������� = ")).c_str());
    coef = (height * width) / 64.0 / (height * width);
    inf += String(to_string(coef));
    inf += "\n";
    //
    inf += String((std::wstring(L"���� ����������� ��� = ")).c_str());
    double dolya = DolyaBitov(vect, vectSzhat);
    inf += String(to_string(dolya));
    inf += "\n";
    //
    inf += String((std::wstring(L"����������� ���������� = ")).c_str());
    double corr = CorrCoef(pixels, pixelsNew, height, width);
    inf += String(to_string(corr));
    inf += "\n";
    //
    CreateDiffFile(menuC->imageName.toRawUTF8(), "new.bmp", "diff.bmp");
    doDecode("diff.bmp", "diff.png");
    //
    decodeInfo->setText(inf, sendNotification);
    diff->setImage(ImageFileFormat::loadFrom(File::getCurrentWorkingDirectory().getChildFile("diff.png")));
    diff->repaint();
    doDecode("new.bmp", "new.png");
    newIm->setImage(ImageFileFormat::loadFrom(File::getCurrentWorkingDirectory().getChildFile("new.png")));
    newIm->repaint();
    repaint();
    resized();
    const char* tmp = menuC->imageName.toRawUTF8();
    const char* origTmp = "orig.bmp";
    if (*origTmp != *tmp)
    {
       FILE* origCopyFile = Create_File("orig.bmp", menuC->imageName.toRawUTF8());
       if (origCopyFile != NULL)
       {
         WriteToFile(origCopyFile, pixels, height, width);
         fclose(origCopyFile);
       }
    }
    for (int i = 0; i < height + 2; i++)
    {
        delete[] pixels[i];
        delete[] pixelsNew[i];
    }
    //
    delete[] pixels;
    delete[] pixelsNew;
    //
    clock->setVisible(false);
    resized();
    repaint();
}