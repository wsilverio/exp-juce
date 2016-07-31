/*
  ==============================================================================

    MainWindow.cpp
    Created: 31 Jul 2016 1:22:57pm
    Author:  blnv

  ==============================================================================
*/

#include "MainWindow.h" 

static ScopedPointer<ApplicationCommandManager> applicationCommandManager;

class KeyMappingsShow   : public Component
{
public:
    KeyMappingsShow()
        : keyMappingEditor (*MainWindow::getApplicationCommandManager().getKeyMappings(), true)
    {
        setOpaque (true);
        addAndMakeVisible (keyMappingEditor);
    }

    void paint (Graphics& g) override
    {
        g.fillAll (Colour::greyLevel (0.93f));
    }

    void resized() override
    {
        keyMappingEditor.setBounds (getLocalBounds().reduced (4));
    }

private:
    KeyMappingEditorComponent keyMappingEditor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KeyMappingsShow)
};

class KeyMappings    :  public Component,
                        public ApplicationCommandTarget
{
public:
    KeyMappings()
    {
        slidersPlot = new SlidersPlot();
        keyMappingsShow = new KeyMappingsShow();
        
        addAndMakeVisible (tabbedComponent = new TabbedComponent (TabbedButtonBar::TabsAtTop));
        tabbedComponent->setTabBarDepth (30);
        tabbedComponent->addTab (TRANS("plot"), Colour(0xff014040), slidersPlot, true);
        tabbedComponent->addTab (TRANS("keys"), Colour::greyLevel (0.93f), keyMappingsShow, false);
        tabbedComponent->setCurrentTabIndex (0);
    }

    ~KeyMappings()
    {
        keyMappingsShow = nullptr;
        slidersPlot = nullptr;
        tabbedComponent = nullptr;
    }
    
    void paint (Graphics& g) override
    {
        g.fillAll (Colour(0xff001f1f));   // background
    }
    
    void resized() override
    {
        tabbedComponent->setBounds (0, 0, getWidth() - 0, getHeight() - 0);
    }

private:
    ScopedPointer<TabbedComponent> tabbedComponent;
    ScopedPointer<SlidersPlot> slidersPlot;
    ScopedPointer<KeyMappingsShow> keyMappingsShow;

    enum CommandIDs{
        playPauseCmd = 0x2100,
        stopCmd
    };

    ApplicationCommandTarget* getNextCommandTarget() override {
        // this will return the next parent component that is an ApplicationCommandTarget (in this
        // case, there probably isn't one, but it's best to use this method in your own apps).
        return findFirstTargetParentComponent();
    }

    void getAllCommands (Array<CommandID>& commands) override {
        const CommandID ids[] = {   playPauseCmd,
                                    stopCmd
        };

        commands.addArray (ids, numElementsInArray (ids));
    }

    void getCommandInfo (CommandID commandID, ApplicationCommandInfo& result) override {              
        const String generalCategory ("General");

        switch (commandID){
            case playPauseCmd:
                result.setInfo ("start animation", "test 1", generalCategory, 0);
                result.addDefaultKeypress ('p', ModifierKeys::noModifiers);
                break;

            case stopCmd:
                result.setInfo ("stop animation", "test 2", generalCategory, 0);
                result.addDefaultKeypress ('s', ModifierKeys::noModifiers);
                break;

            default:
                break;
        }
    }

    bool perform (const InvocationInfo& info) override {

        if (nullptr == slidersPlot){
            return true;
        }
        
        switch (info.commandID){
            case playPauseCmd:
                slidersPlot->runAnimation();
                break;

            case stopCmd:
                slidersPlot->stopAnimation();
                break;

            default:
                return false;
        }

        return true;
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KeyMappings)
};

MainWindow::MainWindow(String name) :    DocumentWindow (   name,
                                                            Colours::lightgrey,
                                                            DocumentWindow::allButtons)
{      
    setUsingNativeTitleBar (true);
    setResizable (true, false);
    setSize (800, 450);
    centreWithSize (getWidth(), getHeight());  
    
    keyMappings = new KeyMappings();
    setContentOwned (keyMappings, false);
    setVisible (true);
    
    addKeyListener (getApplicationCommandManager().getKeyMappings());
    triggerAsyncUpdate();
}

MainWindow::~MainWindow()
{
    keyMappings = nullptr;
    applicationCommandManager = nullptr;
}

void MainWindow::handleAsyncUpdate()
{   
    // This registers all of our commands with the command manager but has to be done after the window has
    // been created so we can find the number of rendering engines available
    ApplicationCommandManager& commandManager = getApplicationCommandManager();
    commandManager.registerAllCommandsForTarget (keyMappings);
    commandManager.registerAllCommandsForTarget (JUCEApplication::getInstance());
}

ApplicationCommandManager& MainWindow::getApplicationCommandManager()
{   
    if (applicationCommandManager == nullptr)
        applicationCommandManager = new ApplicationCommandManager();
    
    return *applicationCommandManager;
}
