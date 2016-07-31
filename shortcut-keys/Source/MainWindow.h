/*
  ==============================================================================

    MainWindow.h
    Created: 31 Jul 2016 1:22:57pm
    Author:  blnv

  ==============================================================================
*/

#ifndef MAINWINDOW_H_INCLUDED
#define MAINWINDOW_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "SlidersPlot.h"

class KeyMappings;

class MainWindow    :   public DocumentWindow,
                        private AsyncUpdater
{
public:
    MainWindow (String name);
    ~MainWindow();
        
    void closeButtonPressed() override
    {
        // This is called when the user tries to close this window. Here, we'll just
        // ask the app to quit when this happens, but you can change this to do
        // whatever you need.
        JUCEApplication::getInstance()->systemRequestedQuit();
    }
    
    // (return the command manager object used to dispatch command events)
    static ApplicationCommandManager& getApplicationCommandManager();    

private:    
    ScopedPointer<KeyMappings> keyMappings;
    
    void handleAsyncUpdate() override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
};

#endif  // MAINWINDOW_H_INCLUDED
