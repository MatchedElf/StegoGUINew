#include "../Include/MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
    : Component("MainComponent")
{
    setLookAndFeel(this);
    Font font;
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
    origTitle->setText(String((std::wstring(L"Пустой контейнер")).c_str()), dontSendNotification);
    addAndMakeVisible(origTitle);
    //
    diffTitle = new Label();
    diffTitle->setFont(font);
    diffTitle->setText(String((std::wstring(L"Разностное изображение")).c_str()), dontSendNotification);
    addAndMakeVisible(diffTitle);
    //
    newTitle = new Label();
    newTitle->setFont(font);
    newTitle->setText(String((std::wstring(L"Заполненный контейнер")).c_str()), dontSendNotification);
    addAndMakeVisible(newTitle);
    //
    Image startLogo = ImageCache::getFromFile(File::getCurrentWorkingDirectory().getChildFile("play.png"));
    startBut = new ImageButton();
    startBut->addListener(this);
    startBut->setImages(false, true, true, startLogo, 1.0f, Colours::transparentWhite, startLogo, 0.3f, Colours::transparentWhite, startLogo, 1.0f, Colours::transparentWhite);
    //
    Image menuLogo = ImageCache::getFromFile(File::getCurrentWorkingDirectory().getChildFile("menu.png"));
    hideBut = new ImageButton();
    hideBut->addListener(this);
    hideBut->setImages(false, true, true, menuLogo, 1.0f, Colours::transparentWhite, menuLogo, 0.3f, Colours::transparentWhite, menuLogo, 1.0f, Colours::transparentWhite);
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
    ///
    font.setHeight(25);
    //
    origLabel = new Label();
    origLabel->setFont(font);
    origLabel->setText(String((std::wstring(L"Информация об изображении")).c_str()), dontSendNotification);
    addAndMakeVisible(origLabel);
    // 
    decodeLabel = new Label();
    decodeLabel->setFont(font);
    decodeLabel->setText(String((std::wstring(L"Информация об извлечении")).c_str()), dontSendNotification);
    addAndMakeVisible(decodeLabel);
    //
    textLabel = new Label();
    textLabel->setFont(font);
    textLabel->setText(String((std::wstring(L"Извлеченный текст")).c_str()), dontSendNotification);
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
    error = new ImageComponent();
    error->setImage(ImageCache::getFromFile(File::getCurrentWorkingDirectory().getChildFile("error.png")));
    addAndMakeVisible(error);
    error->setVisible(false);
    //
    closeErr = new TextButton(String((std::wstring(L"Файлы не выбраны!!!\nНажать сюда для закрытия")).c_str()));
    closeErr->addListener(this);
    addAndMakeVisible(closeErr);
    closeErr->setVisible(false);
    //
    chooseChecker->addComponentListener(this);
    setSize(600, 400);
    //
    doDecode("orig.bmp", "orig.png");
}

MainComponent::~MainComponent()
{
    setLookAndFeel(nullptr);
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
    deleteAndZero(error);
    deleteAndZero(closeErr);
    deleteAndZero(chooseChecker);
}

//==============================================================================
void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    g.setFont(juce::Font(16.0f));
    g.setColour(juce::Colours::white);

}

void MainComponent::resized()
{
   if (startScreen)
   {
      openTitle->setBounds((int)(getWidth() * 0.3), (int)(getHeight() * 0.22), (int)(getWidth() * 0.4), (int)(getHeight() * 0.08));
      openLogo->setBounds((int)(getWidth() * 0.3), (int)(getHeight() * 0.3), (int)(getWidth() * 0.4), (int)(getHeight() * 0.4));
      startBut->setBounds((int)(getWidth() * 0.45), (int)(getHeight() * 0.75), (int)(getWidth() * 0.1), (int)(getHeight() * 0.1));
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
      menuC->setBounds(0, 0, (int)(getWidth() * 0.1) - 10, getHeight());
      //
      hideBut->setBounds(0, 0, (int)(getWidth() * 0.05) - 10, (int)(getHeight() * 0.05) - 5);
      startBut->setBounds((int)(getWidth() * 0.05), 0, (int)(getWidth() * 0.05) - 10, (int)(getHeight() * 0.05) - 5);
      //
      if (hided)
      {
         grid.performLayout(juce::Rectangle<int>(0, (int)(getHeight() * 0.05), getWidth(), (int)(getHeight() * 0.94)));
         error->setBounds((int)(getWidth() * 0.35), (int)(getHeight() * 0.25), (int)(getWidth() * 0.3), (int)(getHeight() * 0.3));
         closeErr->setBounds((int)(getWidth() * 0.4), (int)(getHeight() * 0.55), (int)(getWidth() * 0.2), (int)(getHeight() * 0.2));
      }
      else
      {
         grid.performLayout(juce::Rectangle<int>((int)(getWidth() * 0.1), 0, (int)(getWidth() * 0.9), (int)(getHeight() * 0.99)));
         error->setBounds((int)(getWidth() * 0.4), (int)(getHeight() * 0.25), (int)(getWidth() * 0.3), (int)(getHeight() * 0.3));
         closeErr->setBounds((int)(getWidth() * 0.45), (int)(getHeight() * 0.55), (int)(getWidth() * 0.2), (int)(getHeight() * 0.2));
      }
   }
}

void MainComponent::buttonClicked(Button* butt)
{
    if (butt == startBut)
    {
       if (!startScreen)
       {
          if ((menuC->imageName != "-1") && (menuC->secrName != "-1"))
          {
             LoadWindow* processWnd = new LoadWindow(TRANS(std::wstring(L"Загрузка").c_str()),
                                                       TRANS(std::wstring(L"Процесс идет...").c_str()),
                                                       MessageBoxIconType::NoIcon);
             processWnd->setColour(processWnd->backgroundColourId, juce::Colour::fromRGBA(255, 255, 255, 255));
             processWnd->setBounds((int)(getWidth() * 0.45), (int)(getHeight() * 0.3), (int)(getWidth() * 0.1), (int)(getHeight() * 0.4));
             processWnd->enterModalState(true, nullptr, true);
             MessageManager::callAsync([this, processWnd]()
                {
                   startDecode();
                   if (nullptr != processWnd)
                   {
                      processWnd->exitModalState();
                   }
                });

          }
          else
          {
             error->setVisible(true);
             closeErr->setVisible(true);
             closeErr->enterModalState(true, nullptr, false);
             
             resized();
          }
       }

       else
       {
          startScreen = false;
          openLogo->setVisible(false);
          openTitle->setVisible(false);
          startBut->setEnabled(false);
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
       closeErr->exitModalState();
       resized();
    }
}
//
void MainComponent::componentNameChanged(Component& component)
{
   if (component.getName() == "Load")
   {
      resized();
      repaint();
   }
   else
   {
      paintOrig((menuC->imageName == "-1"));
   }
}
//
void MainComponent::paintOrig(bool _error)
{
   if (!_error)
   {
      //
      int height;
      int width;
      int size;
      String info;
      //
      ReadFile(menuC->imageName.toRawUTF8(), height, width, size, info);
      origInfo->setText(info, sendNotification);
      if ((size < 100) || (height < 32) || (height > 5000) || (width < 32) || (width > 5000))
      {
         orig->setImage(ImageFileFormat::loadFrom(File::getCurrentWorkingDirectory().getChildFile("error.png")), sendNotification);
         startBut->setEnabled(false);
      }
      else
      {
         doDecode(menuC->imageName.toRawUTF8(), "orig.png");
         orig->setImage(ImageFileFormat::loadFrom(File::getCurrentWorkingDirectory().getChildFile("orig.png")), sendNotification);
         startBut->setEnabled(true);
      }
   }
   else
   {
      orig->setImage(ImageFileFormat::loadFrom(File::getCurrentWorkingDirectory().getChildFile("empty.png")), sendNotification);
      origInfo->setText("", sendNotification);
   }
   diff->setImage(ImageCache::getFromFile(File::getCurrentWorkingDirectory().getChildFile("empty.png")));
   newIm->setImage(ImageCache::getFromFile(File::getCurrentWorkingDirectory().getChildFile("empty.png")));
   decodeInfo->setText("", sendNotification);
   decodeText->setText("", sendNotification);
}
//
void MainComponent::startDecode()
{
    srand((unsigned int)time(NULL));
    origInfo->setText("In progress", sendNotification);
    decodeInfo->setText("In progress", sendNotification);
    decodeText->setText("-", sendNotification);
    //
    int difference = 5;
    complex<double> differenceComplex(2.0, 0.0);
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
        pixelsNew = ReadFile("../../Images/Results/new.bmp", height, width, size, info);
        vector<int> key = ReadKey("key.txt", vect);
        //
        if (menuC->selectedTr == 1)
        {
            string result = decodeDCT(height, width, pixels, pixelsNew, vect, true, vectSzhat, key);
            decodeText->setText(result, sendNotification);
        }
        else if (menuC->selectedTr == 2)
        {
            string result = decodeDFT(height, width, pixels, pixelsNew, vect, vectSzhat, key);
            decodeText->setText(result, sendNotification);
        }
        else if (menuC->selectedTr == 3)
        {
            string result = decodeLSB(width, pixelsNew, vect, true, vectSzhat);
            decodeText->setText(result, sendNotification);
        }
    }
    else
    {
        FILE* newFile = Create_File("../../Images/Results/new.bmp", menuC->imageName.toRawUTF8());
        //
        pixelsNew = ReadFile(menuC->imageName.toRawUTF8(), height, width, size, info);
        //
        if (menuC->selectedTr != 3)
        {
           if ( (vect.size() * 8) >= (size / 64))
           {
              decodeInfo->setText(String((std::wstring(L"Ошибка! Слишком большое сообщение.")).c_str()), sendNotification);
              return;
           }
        }
        if (menuC->selectedTr == 1)
        {
            vector<int> key1 = CreateKey("key.txt", size, (int)vect.size(), true);
            //
            encodeDCT(width, pixelsNew, vect, secr_size, difference, key1);
            WriteToFile(newFile, pixelsNew, height, width);
            //
            string result = decodeDCT(height, width, pixels, pixelsNew, vect, true, vectSzhat, key1);
            decodeText->setText(result, sendNotification);
            //

        }
        //
        else if (menuC->selectedTr == 2)
        {
            vector<int> key1 = CreateKey("key.txt", size, (int)vect.size(), true);
            //
            encodeDFT(width, pixelsNew, vect, secr_size, differenceComplex, key1);
            WriteToFile(newFile, pixelsNew, height, width);
            //
            string result = decodeDFT(height, width, pixels, pixelsNew, vect, vectSzhat, key1);
            decodeText->setText(result, sendNotification);
        }
        //
        else if (menuC->selectedTr == 3)
        {
            vector<int> key = CreateKey("key.txt", size, (int)vect.size(), false);
            //
            encodeLSB(width, pixelsNew, vect, secr_size);
            WriteToFile(newFile, pixelsNew, height, width);
            //
            string result = decodeLSB(width, pixelsNew, vect, true, vectSzhat);
            decodeText->setText(result, sendNotification);
        }
        fclose(newFile);
    }
    //
    long double redP, greenP, blueP;
    PSNR(pixels, pixelsNew, redP, greenP, blueP, height, width);
    String inf = "";
    inf += String((std::wstring(L"Алгоритм: ")).c_str());
    if (menuC->selectedTr == 1) inf += "DCT\n";
    if (menuC->selectedTr == 2) inf += "DFT\n";
    if (menuC->selectedTr == 3) inf += "LSB\n";
    inf += "PSNR = ";
    inf += String(to_string(blueP));
    inf += "\n";
    //
    inf += String((std::wstring(L"Коэффициент использования = ")).c_str());
    double coef = word_size * 8.0 / (height * width);
    inf += String(to_string(coef));
    inf += "\n";
    //
    inf += String((std::wstring(L"Коэффициент сокрытия = ")).c_str());
    if(menuC->selectedTr == 3) 
       coef = 1;
    else
      coef = (height * width) / 64.0 / (height * width);
    inf += String(to_string(coef));
    inf += "\n";
    //
    inf += String((std::wstring(L"Доля испорченных бит = ")).c_str());
    double dolya = DolyaBitov(vect, vectSzhat);
    inf += String(to_string(dolya));
    inf += "\n";
    //
    inf += String((std::wstring(L"Коэффициент корреляции = ")).c_str());
    double corr = CorrCoef(pixels, pixelsNew, height, width);
    inf += String(to_string(corr));
    inf += "\n";
    //
    CreateDiffFile(menuC->imageName.toRawUTF8(), "../../Images/Results/new.bmp", "diff.bmp");
    doDecode("diff.bmp", "diff.png");
    //
    decodeInfo->setText(inf, sendNotification);
    diff->setImage(ImageFileFormat::loadFrom(File::getCurrentWorkingDirectory().getChildFile("diff.png")));
    diff->repaint();
    doDecode("../../Images/Results/new.bmp", "new.png");
    newIm->setImage(ImageFileFormat::loadFrom(File::getCurrentWorkingDirectory().getChildFile("new.png")));
    newIm->repaint();
    repaint();
    resized();
    String origTmp = "..\\..\\Images\\orig.bmp";
    String tmp = File(menuC->imageName).getRelativePathFrom(File::getCurrentWorkingDirectory());
    if (tmp != origTmp)
    {
       FILE* origCopyFile = Create_File("../../Images/orig.bmp", menuC->imageName.toRawUTF8());
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
    resized();
    repaint();
}
//
Font MainComponent::getTextButtonFont(TextButton&, int buttonHeight)
{
   return { jmin(25.0f, (float)buttonHeight * 0.8f) };
}