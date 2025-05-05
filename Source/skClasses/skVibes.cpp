#pragma once
#include "JuceHeader.h"
#include "skVibes.h"

float SKVibes::lerp(float x, float y, float t)
{
    return x + (y - x) * t;
}

SKVibes::windowParams* SKVibes::aspectRatio(double unitSize, int numX, int numY)
{    
    wParams.width = unitSize * numX;
    wParams.height = unitSize * numY;

    wParams.unitsNumX = numX;
    wParams.unit = unitSize;
    wParams.unitsNumY = numY;

    return &wParams;
}

void SKVibes::setupSKSliders(std::vector<juce::Slider*>& knobs, std::vector<juce::Label*>& labels, std::vector<juce::String>& text)
{
    for (int i = 0; i < knobs.size(); i++)
    {
        knobs[i]->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        knobs[i]->setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
        knobs[i]->setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colour(juce::Colours::white));
        knobs[i]->setSize(wParams.unit * 1.5, wParams.unit * 1.5);

        labels[i]->setText(text[i], juce::NotificationType::dontSendNotification);
        labels[i]->setFont(SKVibes::getPluginFont().boldened().withHeight(24.0f));
        labels[i]->setJustificationType(juce::Justification::centred);
        labels[i]->setColour(juce::Label::ColourIds::textColourId, juce::Colour(juce::Colours::black));
    }

    knobSize = wParams.unit * 1.5;
}

void SKVibes::setupSKHiddenButtons(std::vector<juce::ToggleButton*>& buttons)
{
    for (int i = 0; i < buttons.size(); i++)
    {
        buttons[i]->setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour(skClear));
        buttons[i]->setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colour(skClear));
        buttons[i]->setColour(juce::ComboBox::outlineColourId, juce::Colour(skClear));
        buttons[i]->setClickingTogglesState(true);
        buttons[i]->setToggleable(true);
        buttons[i]->setSize(knobSize * 0.75, knobSize * 0.75);
    }
}

void SKVibes::setupSKTitle(juce::Label* title)
{
    title->setSize(wParams.unit * 6, wParams.unit * 2);
    title->setFont(SKVibes::getPluginFont().boldened().withHeight(38.0f));
    title->setJustificationType(juce::Justification::centred);
    title->setColour(juce::Label::ColourIds::textColourId, juce::Colour(juce::Colours::white));

    titleSize.push_back(title->getWidth());
    titleSize.push_back(title->getHeight());
}

float SKVibes::getKnobSize()
{
    return knobSize;
}

float SKVibes::getButtonSize()
{
    return buttonSize;
}

std::vector<float> SKVibes::getTitleSize()
{
    return titleSize;
}

void SKVibes::createDivisions()
{  
    float startX = 0.0f;
    float startY = 0.0f;

    for (int j = 0; j < wParams.unitsNumY; j++)
    {
        for (int i = 0; i < wParams.unitsNumX; i++)
        {
            gridSquare.push_back(juce::Rectangle<float>(startX, startY, wParams.unit, wParams.unit));
            startX += wParams.unit;
        }
        startX = 0.0;
        startY += wParams.unit;
    }
}

std::vector<juce::Rectangle<float>>& SKVibes::getGrid()
{
    return gridSquare;
}

juce::Point<float>& SKVibes::getGridPoint(int row, int col)
{
    int index = (col * wParams.unitsNumX) + row;
    return juce::Point<float>(gridSquare[index].getX(), gridSquare[index].getY());
}

juce::Point<float>& SKVibes::getGridCentrePoint(int row, int col)
{
    int index = (col * wParams.unitsNumX) + row;
    return gridSquare[index].getCentre();
}

juce::Rectangle<float>& SKVibes::getGridRect(int row, int col)
{
    int index = (col * wParams.unitsNumX) + row;
    return gridSquare[index];
}

juce::Point<float> SKVibes::getGridBottomLeft(int row, int col)
{
    int index = (col * wParams.unitsNumX) + row;
    return gridSquare[index].getBottomLeft();
}

juce::Point<float> SKVibes::getGridBottomRight(int row, int col)
{
    int index = (col * wParams.unitsNumX) + row;
    return gridSquare[index].getBottomRight();
}

juce::Point<float> SKVibes::getGridTopLeft(int row, int col)
{
    int index = (col * wParams.unitsNumX) + row;
    return gridSquare[index].getTopLeft();
}

juce::Point<float> SKVibes::getGridTopRight(int row, int col)
{
    int index = (col * wParams.unitsNumX) + row;
    return gridSquare[index].getTopRight();
}

float SKVibes::getUnitSize()
{
    return wParams.unit;
}

juce::Point<float> SKVibes::drawOnIntersection(int row, int col, juce::Component* object)
{
    int index = (col * wParams.unitsNumX) + row;
    return juce::Point<float>(gridSquare[index].getX() - object->getWidth() / 2.0, gridSquare[index].getY()  - object->getHeight() / 2.0);
}

juce::Point<float> SKVibes::drawOnIntersection(int row, int col)
{
    int index = (col * wParams.unitsNumX) + row;
    return juce::Point<float>(gridSquare[index].getX(), gridSquare[index].getY());
}

juce::Point<float> SKVibes::drawOnCentre(int row, int col, juce::Component* object)
{
    int index = (col * wParams.unitsNumX) + row;
    return juce::Point<float>(gridSquare[index].getCentreX() - object->getWidth() / 2.0, gridSquare[index].getCentreY() - object->getHeight() / 2.0);
}

juce::Point<float> SKVibes::drawOnCentre(int row, int col)
{
    int index = (col * wParams.unitsNumX) + row;
    return juce::Point<float>(gridSquare[index].getCentreX(), gridSquare[index].getCentreY());
}

juce::Point<float> SKVibes::drawFromIntersection(int row, int col)
{
    int index = (col * wParams.unitsNumX) + row;
    return juce::Point<float>(gridSquare[index].getX(), gridSquare[index].getY());
}

juce::Point<float> SKVibes::drawFromCentre(int row, int col)
{
    int index = (col * wParams.unitsNumX) + row;
    return juce::Point<float>(gridSquare[index].getCentreX(), gridSquare[index].getCentreY());
}

juce::Point<float> SKVibes::drawOnHorizontal(int row, int col, juce::Component* object)
{
    int index = (col * wParams.unitsNumX) + row;
    return juce::Point<float>(gridSquare[index].getCentreX() - object->getWidth() / 2.0, gridSquare[index].getY() - object->getHeight() / 2.0);
}

juce::Point<float> SKVibes::drawOnHorizontal(int row, int col)
{
    int index = (col * wParams.unitsNumX) + row;
    return juce::Point<float>(gridSquare[index].getCentreX(), gridSquare[index].getY());
}

juce::Point<float> SKVibes::drawOnVertical(int row, int col, juce::Component* object)
{
    int index = (col * wParams.unitsNumX) + row;
    return juce::Point<float>(gridSquare[index].getX() - object->getWidth() / 2.0, gridSquare[index].getCentreY() - object->getHeight() / 2.0);
}

juce::Point<float> SKVibes::drawOnVertical(int row, int col)
{
    int index = (col * wParams.unitsNumX) + row;
    return juce::Point<float>(gridSquare[index].getCentreX(), gridSquare[index].getY());
}

void SKVibes::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider)
{

    auto radius = (float)juce::jmin(width / 2, height / 2);
    auto knobRect = juce::Rectangle<float>(x, y, width, height);
    auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    double pointerThickness = 10.0;

    juce::Point<float> centre(knobRect.getCentreX(), knobRect.getCentreY());

    auto knobBorder = juce::Rectangle<float>(width - 3.0f, height - 3.0f).withCentre(centre);

    auto pointerRect = juce::Rectangle<float>(sliderPos, centre.y - radius, 10.0, 10.0);

    g.setColour(juce::Colour(0xffffffff));
    g.fillEllipse(knobRect);
    g.setColour(juce::Colour(0xff000000));
    g.drawEllipse(knobBorder, 3.0f);

    juce::Path p;
    p.addEllipse(-pointerThickness * 0.5, -radius * 0.85, pointerThickness, pointerThickness);
    p.applyTransform(juce::AffineTransform::rotation(angle).translated(centre));
    g.fillPath(p);
}

void SKVibes::setupJFET(juce::Button* button, juce::Point<float> bounds, float width, float height, int index, bool state)
{
    fets[index]->button = button;
    fets[index]->buttonRect = juce::Rectangle<float>(bounds.x, bounds.y, width - 3.0, height - 3.0);
    fets[index]->shadow = juce::Rectangle<float>(bounds.x + 3.0, bounds.y + 3.0, width, height);
    fets[index]->bgCol = juce::Colour(0xffffffff);
    fets[index]->shadowCol = juce::Colour(0xff000000);
    fets[index]->state = state;

    if (state)
    {
        button->setToggleState(state, true);
        button->setEnabled(false);
    }
}

void SKVibes::drawJFET(juce::Graphics& g, int index, bool state)
{
    SKVibes::jfetParams* fet = fets[index];

    g.setColour(fet->shadowCol);
    g.fillRect(fet->shadow);

    if(state)
    {
        g.setColour(fet->bgCol);
        g.fillRect(fet->shadow);

        g.setColour(fet->shadowCol);
        g.drawRect(fet->shadow, 2.0f);

        fet->state = true;
    }

    else
    {
        g.setColour(fet->bgCol);
        g.fillRect(fet->buttonRect);

        g.setColour(fet->shadowCol);
        g.drawRect(fet->buttonRect, 2.0f);

        fet->state = false;
    }

   
    juce::Point<float> centre(fet->buttonRect.getCentreX(), fet->buttonRect.getCentreY());

    //gate
    juce::Point<float> p1(centre.x - 20.0, centre.y);
    juce::Point<float> p2(centre.x - 7.0, centre.y - 7.0);
    juce::Point<float> p3(centre.x - 7.0, centre.y + 7.0);
    juce::Point<float> p4(centre.x - 2.0, centre.y);

    //fet
    auto gate = juce::Rectangle<float>(3.0f, 25.0f).withCentre(centre);

    //drain
    juce::Point<float> p5(centre.x, centre.y - 8.0);
    juce::Point<float> p6(centre.x + 12.0, centre.y - 8.0);
    juce::Point<float> p7(centre.x + 12.0, centre.y - 20.0);

    //source
    juce::Point<float> p8(centre.x, centre.y + 8);
    juce::Point<float> p9(centre.x + 12.0, centre.y + 8);
    juce::Point<float> p10(centre.x + 12.0, centre.y + 20.0);

    juce::Path p;

    p.startNewSubPath(p1);
    p.lineTo(p4);
    p.startNewSubPath(p4);
    p.lineTo(p2);
    p.startNewSubPath(p4);
    p.lineTo(p3);

    p.startNewSubPath(p5);
    p.lineTo(p6);
    p.lineTo(p7);

    p.startNewSubPath(p8);
    p.lineTo(p9);
    p.lineTo(p10);

    juce::Path r;
    r.addRectangle(gate);

    if (fet->state)
    {
        g.setColour(juce::Colour(skIBMcyan));
        r.applyTransform(juce::AffineTransform::translation(3.0, 3.0));
        p.applyTransform(juce::AffineTransform::translation(3.0, 3.0));
    }

    else
    {
        g.setColour(juce::Colour(0xff000000));
    }

    g.fillPath(r);
    g.strokePath(p, juce::PathStrokeType(2.0f));
}

void SKVibes::drawPanel(juce::Graphics& g, juce::Point<float> pos, double width, double height)
{
    juce::Rectangle<float> bgPanel(pos.x, pos.y, width, height);
    g.setColour(juce::Colour(juce::Colour(juce::Colours::white)));
    g.fillRect(bgPanel);

    float margin = 15.0f;

    g.setColour(juce::Colour(juce::Colour(juce::Colours::black)));
    g.drawRect(bgPanel, 1.0f);
    juce::Line<float> line1(juce::Point<float>(pos.x + margin, pos.y), juce::Point<float>(pos.x + margin, pos.y + height));
    juce::Line<float> line2(juce::Point<float>(pos.x + width - margin, pos.y), juce::Point<float>(pos.x + width - margin, pos.y + height));
    juce::Line<float> line3(juce::Point<float>(pos.x, pos.y + margin), juce::Point<float>(pos.x + width, pos.y + margin));
    juce::Line<float> line4(juce::Point<float>(pos.x, pos.y + height - margin), juce::Point<float>(pos.x + width, pos.y + height - margin));
    
    g.drawLine(line1, 1.0f);
    g.drawLine(line2, 1.0f);
    g.drawLine(line3, 1.0f);
    g.drawLine(line4, 1.0f);
}

void SKVibes::drawDoublePanel(juce::Graphics& g, doublePanelParams* panel)
{
    juce::Rectangle<float> outerPanel(panel->position.x, panel->position.y, panel->width, panel->height);
    juce::Rectangle<float> innerPanel(panel->position.x + panel->margin, panel->position.y + panel->margin, panel->width - (panel->margin * 2.0f), panel->height - (panel->margin * 2.0f));
    g.setColour(juce::Colour(panel->fillColour));
    g.fillRect(outerPanel);
    g.fillRect(innerPanel);
    g.setColour(juce::Colour(panel->borderColour));
    g.drawRect(outerPanel, 1.0f);
    g.drawRect(innerPanel, 1.0f);
}

void SKVibes::updateDoublePanels(doublePanelParams* doublePanel)
{
    doublePanelParams params;

    params.borderColour = doublePanel->borderColour;
    params.fillColour = doublePanel->fillColour;

    params.position = doublePanel->position;
    params.width = doublePanel->width;
    params.height = doublePanel->height;
}

void SKVibes::updatePanels(panelParams* panel)
{
    panelParams params;

    params.width = panel->width;
    params.height = panel->height;
    params.position = panel->position;
}

void SKVibes::drawLabels(juce::Slider* knob, juce::String& text)
{
    juce::Label label;
    label.setText(text, juce::NotificationType::dontSendNotification);
    label.setFont(SKVibes::getPluginFont());
    label.setJustificationType(juce::Justification::centred);
    label.setColour(juce::Label::ColourIds::textColourId, juce::Colour(juce::Colours::black));
    juce::Rectangle<int> knobBounds = knob->getBounds();
    label.setBounds(knobBounds.getX(), knobBounds.getY(), knobBounds.getWidth(), knobBounds.getHeight());
}

SKVibes::pixelParams* SKVibes::setupPixelShape(float x, float y, float width, float height)
{
    pixels.border = juce::Rectangle<float>(x, y, width, height);
    pixels.pos = juce::Point<float>(x, y);
    pixels.col = juce::Colour(skIBMcyan);
    pixels.startPoint = juce::Point<float>(x, y + height / 2.0);
    pixels.endPoint = juce::Point<float>(x + width, y + height / 2.0);
    pixels.midPoint = juce::Point<float>(x + width / 2.0, y);

    pixels.variStart = juce::Point<float>(pixels.startPoint.x + width * 0.25, pixels.startPoint.y);
    pixels.variEnd = juce::Point<float>(pixels.startPoint.x + width * 0.75, pixels.startPoint.y);

    return &pixels;
}

void SKVibes::drawPixelShape(juce::Graphics& g, pixelParams* pixels, float newY)
{
    pixels->midPoint.y = newY;

    juce::Point<float> borderBottomLeft(pixels->border.getBottomLeft());
    juce::Point<float> borderBottomRight(pixels->border.getBottomRight());

    juce::Path p;

    g.setColour(pixels->col);

    p.startNewSubPath(borderBottomLeft);
    p.lineTo(pixels->startPoint);
    p.lineTo(pixels->variStart);
    p.quadraticTo(pixels->midPoint, pixels->variEnd);
    p.lineTo(juce::Point<float>(pixels->endPoint));
    p.lineTo(borderBottomRight);
    p.closeSubPath();
    g.fillPath(p);    
}