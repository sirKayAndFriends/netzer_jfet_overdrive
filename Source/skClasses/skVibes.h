#pragma once
#include "JuceHeader.h"

class SKVibes : public juce::LookAndFeel_V4
{
public:

    enum skColours {
        skLavendar = 0xffaa46d8,
        skLightBlue = 0xff00c3ff,
        skPaleCyan = 0xff00eaff,
        skIBMcyan = 0xFF6CFAFF,
        skDaisyYellow = 0xfffff700,
        skPaleYellow = 0xffffff4d,
        skCream = 0xffffff90,
        skTomato = 0xffff0000,
        skSeville = 0xffffaf00,
        skPsychOrange = 0xffc54f00,
        skMauve = 0xffbb00c0,
        skPalePurple = 0xffbb56ff,
        skPsychPurple = 0xff9c39ff,
        skPlum = 0xff6a008b,
        skIBMlavendar = 0xFF7369FF,
        skPeach = 0xffffbaae,
        skBerryPink = 0xffd800a5,
        skPrincessPink = 0xffff38c3,
        skStrawbYog = 0xffffcfff,
        skRaspberry = 0xffff0064,
        skMintGreen = 0xff5cfff9,
        skBarleyGreen = 0xff3ee8bb,
        skForestGreen = 0xff00e500,
        skDuckEgg = 0xff3ee1ff,
        skCocoa = 0xff550030,
        skClear = 0x00ffffff,
        skWhite = 0xffffffff,
        skBlack = 0xff000000
    };

    struct diodeArray
    {
        juce::Button* button;
        juce::Point<float> position;
        juce::Rectangle<float> bounds;
        juce::Point<float> midPoint;

        std::vector<juce::Point<float>> triangle;
        std::vector<juce::Point<float>> wire;
        std::vector<juce::Point<float>> cathode;

        bool soft;//true = soft clipping diode -> is red when clipping mode is soft
        bool direction;
        bool state;
        int unit;
        double thickness;
    } diode1, diode2, diode3, diode4;

    std::vector<diodeArray*> diodes
    {
        &diode1,
        &diode2,
        &diode3,
        &diode4
    };

    struct visualParams
    {
        int lineStartX;
        int visualWidth;
        int lineEndX;
        float visualCurveWidth;
        int controlMin;
        int controlMax;
        float controlWidth;
        int lineY;
        int maxY;
        int humpHeight;

        juce::Path curve;
        juce::Point<float> curveStart, controlDot, curveEnd, lineStart, lineEnd;
    };

    struct doublePanelParams
    {
        int borderColour;
        int fillColour;
        juce::Point<float> position;
        double width, height;
        int margin;
    };

    struct panelParams
    {
        juce::Point<float> position;
        double width, height;
    };

    struct pixelParams
    {
        juce::Point<float> pos;
        juce::Rectangle<float> border;
        float size;

        juce::Point<float> startPoint;
        juce::Point<float> endPoint;

        juce::Point<float> midPoint;

        juce::Point<float> variStart;
        juce::Point<float> variEnd;

        juce::Colour col;
    } pixels;

    struct jfetParams
    {
        juce::Button* button;

        juce::Rectangle<float> buttonRect;
        juce::Rectangle<float> shadow;

        juce::Colour bgCol, shadowCol, borderCol;

        bool state;
    } fet1, fet2, fet3;

    std::vector<jfetParams*> fets
    {
        &fet1,
        &fet2,
        &fet3
    };

    struct windowParams
    {
        double width;
        double height;
        double unit;
        int unitsNumX;
        int unitsNumY;
    } wParams;

    static const juce::Font& getPluginFont()
    {
        static juce::Font IBM(juce::Font(juce::Typeface::createSystemTypefaceFor(BinaryData::IBM_VGA_8x16_ttf,
            BinaryData::IBM_VGA_8x16_ttfSize)));

        return IBM;
    };

    //returns second axis size from inputted first axis and aspect ratio! 
    windowParams* aspectRatio(double unitSize, int numX, int numY);

    void setupSKSliders(std::vector<juce::Slider*>& knobs, std::vector<juce::Label*>& labels, std::vector<juce::String>& text);

    void setupSKHiddenButtons(std::vector<juce::ToggleButton*>& buttons);
    
    void setupSKButtons(juce::Button* button);

    void setupSKTitle(juce::Label* title);

    float getKnobSize();

    float getButtonSize();

    std::vector<float> SKVibes::getTitleSize();

    //create a grid of squares
    void createDivisions();

    float lerp(float x, float y, float t);

    //return grid
    std::vector<juce::Rectangle<float>>& getGrid();

    //return a single corner point in row/column (0 index)
    juce::Point<float>& getGridPoint(int row, int col);
   
    //return single centre point in row/column (0 index)
    juce::Point<float>& getGridCentrePoint(int row, int col);

    //return a single rectangle from grid
    juce::Rectangle<float>& getGridRect(int row, int col);

    //return XY value of bottom left corner of selected rectangle
    juce::Point<float> getGridBottomLeft(int row, int col);

    //return XY value of bottom right corner of selected rectangle
    juce::Point<float> getGridBottomRight(int row, int col);

    //return XY value of top left of selected rectangle
    juce::Point<float> getGridTopLeft(int row, int col);

    //return XY value of top right of selected rectangle
    juce::Point<float> getGridTopRight(int row, int col);

    //returns size of grid squares
    /*std::vector<float> getUnitSize();*/
    float getUnitSize();

    //returns coordinate for centering on intersections of grid
    juce::Point<float> drawOnIntersection(int row, int col, juce::Component* object);

    juce::Point<float> drawOnIntersection(int row, int col);

    //returns coordinate for centering object on centre of grid square
    juce::Point<float> drawOnCentre(int row, int col, juce::Component* object);

    juce::Point<float> drawOnCentre(int row, int col);

    //coordinate for drawing top left corner from intersection
    juce::Point<float> drawFromIntersection(int row, int col);

    //coordinate for drawing top left corner from centre of square
    juce::Point<float> drawFromCentre(int row, int col);

    //x = square centre, y = bottom edge
    juce::Point<float> drawOnHorizontal(int row, int col, juce::Component* object);

    juce::Point<float> drawOnHorizontal(int row, int col);

    //x = left edge, y = square centre
    juce::Point<float> drawOnVertical(int row, int col, juce::Component* object);

    juce::Point<float> drawOnVertical(int row, int col);
    
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider) override;

    void setupJFET(juce::Button* button, juce::Point<float> bounds, float width, float height, int index, bool state);

    void drawJFET(juce::Graphics& g, int index, bool state);
   
    void drawPanel(juce::Graphics& g, juce::Point<float> pos, double width, double height);
    
    void drawDoublePanel(juce::Graphics& g, doublePanelParams* panel);

    void updateDoublePanels(doublePanelParams* doublePanel);

    void updatePanels(panelParams* panel);

    void drawLabels(juce::Slider* knob, juce::String& text);

    pixelParams* setupPixelShape(float x, float y, float width, float height);
    
    void drawPixelShape(juce::Graphics& g, pixelParams* pixels, float newY);

    visualParams setVisualParams(int x, int y, int width, int height);

    void drawMidVisual(juce::Graphics& g, visualParams* params, float peakX, float peakY);

    void diodeInit(juce::Button* button, float x, float y, int size, bool direction, bool soft, bool state, int index);

    void drawDiode(juce::Graphics& g, int index, bool state);

    void diodeSort(juce::Button* button, bool state);

private:

    std::vector<juce::Rectangle<float>> gridSquare;

    float knobSize, buttonSize;

    std::vector<float> titleSize;
};