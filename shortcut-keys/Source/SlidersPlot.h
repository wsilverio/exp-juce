/*
  ==============================================================================

    SlidersPlot.h
    Created: 31 Jul 2016 8:17:06pm
    Author:  blnv

  ==============================================================================
*/

#ifndef SLIDERSPLOT_H_INCLUDED
#define SLIDERSPLOT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class SlidersPlot    :  public Component,
                        private Timer
{
public:
    SlidersPlot() : run(true), elapsed(0.0f), twoPi(2*std::acos(-1))
    {
        for (int i = 0; i < 50; ++i){
            Slider* slider = new Slider();
            slider->setRange (0, 100, 0);
            slider->setSliderStyle (Slider::LinearVertical);
            slider->setTextBoxStyle (Slider::NoTextBox, true, 80, 20);
            slider->setColour (Slider::thumbColourId, Colour(0xffffffff));
            slider->setColour (Slider::textBoxOutlineColourId, Colour (0x00000000));
            sliders.add(slider);
            addAndMakeVisible(slider);
        }
        
        runAnimation();
    }

    ~SlidersPlot()
    {
    }

    void paint (Graphics& g) override
    {
    }

    void resized() override
    {
        Rectangle<int> rect ( getLocalBounds().reduced(10) );
        
        float sliderWidth = (float)rect.getWidth() / sliders.size();
        
        for (int i = 0; i < sliders.size(); ++i){
            sliders[i]->setBounds( i*sliderWidth + rect.getX(), rect.getY(), sliderWidth, rect.getHeight() );
        }        
    }
    
    void runAnimation(){
        startTimerHz(30); // 30 fps
    }
    
    void stopAnimation(){
        stopTimer();
    }    

private:
    bool run;
    float elapsed;
    const double twoPi;
    OwnedArray<Slider> sliders;
    
    void timerCallback() override{
        for (int i = 0; i < sliders.size(); ++i){
            const float amp = sliders[i]->getMaximum()/2.0f * ( 1 + std::sin(i/twoPi + elapsed*twoPi*0.4f) );
            
            sliders[i]->setValue(amp, dontSendNotification);
        }
            
        elapsed += 0.02f;
    }    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SlidersPlot)
};


#endif  // SLIDERSPLOT_H_INCLUDED
