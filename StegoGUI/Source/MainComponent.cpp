#include "../Include/MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
    : Component("MainComponent")
{
    //compsList = new Component * [10];
    Font font;
    int ttt = 255 * (5 < 1);
    font.setHeight(25);
    //
    setOpaque(true);
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
    /*imageCh = new TextButton("choose Image");
    addAndMakeVisible(imageCh);
    imageCh->addListener(this);*/
    //
    /*secrCh = new TextButton("choose Secret");
    addAndMakeVisible(secrCh);
    secrCh->addListener(this);*/
    /*secrCh = new ComboBox();
    secrCh->addItem("DCP", 1);
    secrCh->addItem("Furie", 2);
    secrCh->addItem("LSB", 3);
    secrCh->setSelectedId(1);
    addAndMakeVisible(secrCh);
    secrCh->addListener(this);*/
    //
    Image startLogo = ImageCache::getFromFile(File::getCurrentWorkingDirectory().getChildFile("play.png"));
    startBut = new ImageButton();
    //addAndMakeVisible(startBut);
    startBut->addListener(this);
    startBut->setImages(false, true, true, startLogo, 1, Colours::transparentWhite, startLogo, 0.3, Colours::transparentWhite, startLogo, 1, Colours::transparentWhite);
    //
    Image menuLogo = ImageCache::getFromFile(File::getCurrentWorkingDirectory().getChildFile("menu.png"));
    hideBut = new ImageButton();
    //addAndMakeVisible(hideBut);
    hideBut->addListener(this);
    hideBut->setImages(false, true, true, menuLogo, 1, Colours::transparentWhite, menuLogo, 0.3, Colours::transparentWhite, menuLogo, 1, Colours::transparentWhite);
    //
    orig = new ImageComponent();
    //orig->setHelpText("ORIIIG");
    //orig->setImage(ImageFileFormat::loadFrom(File::getCurrentWorkingDirectory().getChildFile("orig.png")));
    addAndMakeVisible(orig);
    //
    diff = new ImageComponent();
    //diff->setImage(ImageCache::getFromFile(File::getCurrentWorkingDirectory().getChildFile("newF.png")));
    addAndMakeVisible(diff);
    //
    newIm = new ImageComponent();
    //newIm->setImage(ImageCache::getFromFile(File::getCurrentWorkingDirectory().getChildFile("newF.png")));
    addAndMakeVisible(newIm);
    //
    //Image popLogo = ImageCache::getFromFile(File::getCurrentWorkingDirectory().getChildFile("orig.png"));
    //bool tmp = popLogo.isValid();
    //bool tmp1 = popLogo.isNull();
    //Image popLogo = ImageCache::getFromFile("C:\\Users\\andre\\source\\repos\\StegoGui\\Source\\orig.bmp");
    //
    font.setHeight(25);
    origInfo = new Label();
    origInfo->setFont(font);
    origInfo->setText(String((std::wstring(L"Информация об изображении")).c_str()), dontSendNotification);
    addAndMakeVisible(origInfo);
    //
    decodeInfo = new Label();
    decodeInfo->setFont(font);
    decodeInfo->setText(String((std::wstring(L"Информация о проведенном извлечении")).c_str()), dontSendNotification);
    addAndMakeVisible(decodeInfo);
    //
    //font.setHeight(30);
    decodeText = new Label();
    decodeText->setFont(font);
    decodeText->setText(String((std::wstring(L"Извлеченный текст")).c_str()), dontSendNotification);
    addAndMakeVisible(decodeText);
    //
    /*attack = new ToggleButton("ddd");
    attack->setButtonText("Attack");
    attack->addListener(this);
    addAndMakeVisible(attack);*/
    //
    /*compsList[0] = orig;
    compsList[1] = diff;
    compsList[2] = newIm;
    compsList[3] = imageCh;*/
    //
    /*FSizer = new StretchableLayoutManager();
    FSizer->setItemLayout(0, 50, 1000, -1);
    FSizer->setItemLayout(1, 50, 1000, -1);
    FSizer->setItemLayout(2, 50, 1000, -1);
    FSizer->setItemLayout(3, 50, 100, -0.06);*/
    //
    menuC = new MenuComponent();
    addAndMakeVisible(menuC);
    addAndMakeVisible(startBut);
    addAndMakeVisible(hideBut);
    //
    clock = new ImageComponent();
    //orig->setHelpText("ORIIIG");
    clock->setImage(ImageCache::getFromFile(File::getCurrentWorkingDirectory().getChildFile("clock.png")));
    addAndMakeVisible(clock);
    clock->setVisible(false);

    //sdPanel = new SidePanel("Menu", 100, true);
    //sdPanel->setContent(menuC);
    //addAndMakeVisible(sdPanel);
    //grid1 = new Grid();
    //
    setSize(600, 400);
}

MainComponent::~MainComponent()
{
    deleteAndZero(origTitle);
    deleteAndZero(diffTitle);
    deleteAndZero(newTitle);
    deleteAndZero(orig);
    deleteAndZero(diff);
    deleteAndZero(newIm);
    //deleteAndZero(imageCh);
    //deleteAndZero(secrCh);
    deleteAndZero(startBut);
    deleteAndZero(hideBut);
    deleteAndZero(origInfo);
    deleteAndZero(decodeInfo);
    deleteAndZero(decodeText);
    //deleteAndZero(attack);
    //deleteAndZero(sdPanel);
    deleteAndZero(menuC);
    deleteAndZero(clock);
    //deleteAndZero(grid);
    //deleteAndZero(imagesSizer);
    //deleteAndZero(FSizer);
    //deleteAndZero(FSizer);
}

//==============================================================================
void MainComponent::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    //g.fillRect(0, 0, )
    g.setFont(juce::Font(16.0f));
    g.setColour(juce::Colours::white);
    //g.drawText ("Hello World!", getLocalBounds(), juce::Justification::centred, true);
}

void MainComponent::resized()
{
    Grid grid;
    //
    using Track = Grid::TrackInfo;
    using Fr = Grid::Fr;
    //
    grid.templateRows = { Track(Fr(1)), Track(Fr(12)), Track(Fr(4)) };
    grid.templateColumns = { Track(Fr(1)), Track(Fr(1)), Track(Fr(1)) };
    //
    grid.items = { GridItem(origTitle), GridItem(diffTitle), GridItem(newTitle), GridItem(orig), GridItem(diff), GridItem(newIm), GridItem(origInfo), GridItem(decodeInfo), GridItem(decodeText) };
    //
    //menuC->setBounds(0, getHeight() * 0.05, getWidth() * 0.1 - 10, getHeight() * 0.95);
    menuC->setBounds(0, 0, getWidth() * 0.1 - 10, getHeight() * 1);
    
    hideBut->setBounds(0, 0, getWidth() * 0.05 - 10, getHeight() * 0.05 - 5);
    startBut->setBounds(getWidth() * 0.05, 0, getWidth() * 0.05 - 10, getHeight() * 0.05 - 5);
    //
    if (hided)
    {
       hideBut->setBounds(0, 0, getWidth() * 0.025 - 10, getHeight() * 0.025 - 5);
       startBut->setBounds(getWidth() * 0.025, 0, getWidth() * 0.025 - 10, getHeight() * 0.025 - 5);
       grid.performLayout(juce::Rectangle<int>(0, getHeight() * 0.05, getWidth(), getHeight() * 0.95));
    }
    else 
    {
       hideBut->setBounds(0, 0, getWidth() * 0.05 - 10, getHeight() * 0.05 - 5);
       startBut->setBounds(getWidth() * 0.05, 0, getWidth() * 0.05 - 10, getHeight() * 0.05 - 5);
       grid.performLayout(juce::Rectangle<int>(getWidth() * 0.1, 0, getWidth() * 0.9, getHeight()));

    }
    clock->setBounds(getWidth() * 0.4, getHeight() * 0.3, getWidth() * 0.2, getHeight() * 0.4);
    juce::Rectangle test(0, 0, 1, 1);
    
    //grid.performLayout(getLocalBounds());
    //
    //
}

void MainComponent::buttonClicked(Button* butt)
{
    //if (butt == imageCh)
    //{
    //    //orig->setImage(ImageCache::getFromFile(File::getCurrentWorkingDirectory().getChildFile("test.png")));
    //}
    if (butt == startBut)
    {
       /*clock->setVisible(true);
       resized();
       repaint(juce::Rectangle(0, 0, getWidth(), getHeight()));
       clock->setVisible(false);*/
       startDecode();
    }
    else if (butt == hideBut)
    {
       /*hided = !hided;
       menuC->setVisible(!(menuC->isVisible()));
       resized();*/
       clock->setVisible(true);
    }
}
//
//void MainComponent::comboBoxChanged(ComboBox* cb)
//{
//    if (cb == secrCh)
//    {
//        selectedTr = cb->getSelectedId();
//    }
//}

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
    string info;
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
            /*vector<float**> ret = */encodeDCP(height, width, pixelsNew, vect, secr_size, difference, key1);
            /*for (int i = 0; i < ret.size(); i++)
                delete ret[i];
            ret.clear();*/
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
            /*vector<complex<double>**> ret = */encodeFurie(height, width, pixelsNew, vect, secr_size, differenceComplex, key1);
            /*for (int i = 0; i < ret.size(); i++)
                delete ret[i];
            ret.clear();*/
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
    string inf = "";
    inf += "PSNR = ";
    inf += to_string(blueP);
    inf += "\n";
    //
    inf += "Coef sokr = ";
    double coef = word_size * 8.0 / (height * width);
    inf += to_string(coef);
    inf += "\n";
    //
    inf += "Coef isp = ";
    coef = (height * width) / 64.0 / (height * width);
    inf += to_string(coef);
    inf += "\n";
    //
    inf += "Dolya isporchennyh bit = ";
    double dolya = DolyaBitov(vect, vectSzhat);
    inf += to_string(dolya);
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
}