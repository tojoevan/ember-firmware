/* 
 * File:   ScreenBuilder.cpp
 * Author: Richard Greene
 * 
 * Builds the screens shown on the front panel.
 * 
 * Created on July 23, 2014, 12:15 PM
 */

#include <ScreenBuilder.h>
#include <PrinterStatus.h>
#include <ScreenLayouts.h>

#define UNDEFINED_SCREEN_LINE1  Center, 64, 10, 1, 0xFFFF, "Screen?"

/// Create a key into the Screen map for the given print engine state 
/// and UI substate 
ScreenKey ScreenBuilder::GetKey(PrintEngineState state, UISubState subState)
{
    // This implementation assumes we never have more than 256 print
    // engine states or UI substates
    return state | (subState << 8);
}

/// Build the screens and add them to a map keyed by PrintEngine states and UI
/// sub states.
void ScreenBuilder::BuildScreens(std::map<int, Screen*>& screenMap) 
{
    ScreenText* unknown = new ScreenText();
    unknown->Add(new ScreenLine(UNDEFINED_SCREEN_LINE1));
    screenMap[UNKNOWN_SCREEN_KEY] =  new Screen(unknown, 0);
    
    // NULL screens for states that shouldn't change what's already displayed
    screenMap[GetKey(PrinterOnState, NoUISubState)] = NULL;  
    screenMap[GetKey(InitializingState, NoUISubState)] = NULL;
    screenMap[GetKey(HomeState, NoUISubState)] = NULL;
    screenMap[GetKey(DoorClosedState, NoUISubState)] = NULL;
    screenMap[GetKey(PrintSetupState, NoUISubState)] = NULL; 
    screenMap[GetKey(PrintingState, NoUISubState)] = NULL; 
    screenMap[GetKey(SeparatingState, NoUISubState)] = NULL; 
    
    ScreenText* readyLoaded = new ScreenText;
    readyLoaded->Add(new ScreenLine(READY_LOADED_LINE1));
    readyLoaded->Add(new ScreenLine(READY_LOADED_LINE2));
    readyLoaded->Add(new ScreenLine(READY_LOADED_LINE3));
    readyLoaded->Add(new ScreenLine(READY_LOADED_LINE4));
    readyLoaded->Add(new ScreenLine(READY_LOADED_BTN1_LINE1));
    readyLoaded->Add(new ScreenLine(READY_LOADED_BTN1_LINE2));
    readyLoaded->Add(new ScreenLine(READY_LOADED_BTN2_LINE1));
    readyLoaded->Add(new ScreenLine(READY_LOADED_BTN2_LINE2));
    screenMap[GetKey(HomeState, HavePrintData)] = 
                                new Screen(readyLoaded, READY_LOADED_LED_SEQ);
    
    ScreenText* startLoaded = new ScreenText;
    startLoaded->Add(new ReplaceableLine(START_LOADED_LINE1)); 
    startLoaded->Add(new ScreenLine(START_LOADED_LINE2));
    startLoaded->Add(new ScreenLine(START_LOADED_LINE3));
    startLoaded->Add(new ScreenLine(START_LOADED_LINE4));
    startLoaded->Add(new ScreenLine(START_LOADED_LINE5));
    startLoaded->Add(new ScreenLine(START_LOADED_BTN1_LINE2));
    startLoaded->Add(new ScreenLine(START_LOADED_BTN2_LINE2));
    screenMap[GetKey(HomeState, Downloaded)] = 
                        new JobNameScreen(startLoaded, START_LOADED_LED_SEQ);
    
    ScreenText* loadFail = new ScreenText;
    loadFail->Add(new ScreenLine(LOAD_FAIL_LINE1));
    loadFail->Add(new ScreenLine(LOAD_FAIL_LINE2));
    loadFail->Add(new ScreenLine(LOAD_FAIL_BTN1_LINE2));
    screenMap[GetKey(HomeState, DownloadFailed)] = 
                                new Screen(loadFail, LOAD_FAIL_LED_SEQ);
    
    ScreenText* printing = new ScreenText;
    printing->Add(new ScreenLine(PRINTING_LINE1));
    printing->Add(new ScreenLine(PRINTING_LINE3));
    printing->Add(new ScreenLine(PRINTING_BTN1_LINE2));
    printing->Add(new ScreenLine(PRINTING_BTN2_LINE2));
    screenMap[GetKey(PrintingLayerState, NoUISubState)] = 
                             new Screen(printing, PRINTING_LED_SEQ);
    
    ScreenText* countdown = new ScreenText;
    countdown->Add(new ReplaceableLine(PRINTING_CLEAR_LINE2));
    countdown->Add(new ReplaceableLine(PRINTING_LINE2));
    screenMap[GetKey(ExposingState, NoUISubState)] = 
                             new PrintStatusScreen(countdown, PRINTING_LED_SEQ);  
    
    ScreenText* paused = new ScreenText;
    paused->Add(new ScreenLine(PAUSED_LINE1));
    paused->Add(new ScreenLine(PAUSED_LINE2));
    paused->Add(new ScreenLine(PAUSED_BTN1_LINE2));
    paused->Add(new ScreenLine(PAUSED_BTN2_LINE1));
    paused->Add(new ScreenLine(PAUSED_BTN2_LINE2));
    // don't clear LEDs before showing paused screen's animation,
    // so that it will only animate those LEDs
    screenMap[GetKey(PausedState, NoUISubState)] = 
                                new Screen(paused, PAUSED_LED_SEQ, true, false);
   
    ScreenText* cancelPrompt = new ScreenText;
    cancelPrompt->Add(new ScreenLine(CONFIRM_CANCEL_LINE1));
    cancelPrompt->Add(new ScreenLine(CONFIRM_CANCEL_LINE2));
    cancelPrompt->Add(new ScreenLine(CONFIRM_CANCEL_BTN1_LINE1));
    cancelPrompt->Add(new ScreenLine(CONFIRM_CANCEL_BTN2_LINE1));
    cancelPrompt->Add(new ScreenLine(CONFIRM_CANCEL_BTN1_LINE2));
    cancelPrompt->Add(new ScreenLine(CONFIRM_CANCEL_BTN2_LINE2));
    screenMap[GetKey(ConfirmCancelState, NoUISubState)] = 
                            new Screen(cancelPrompt, CONFIRM_CANCEL_LED_SEQ);

    ScreenText* printComplete = new ScreenText;
    printComplete->Add(new ScreenLine(PRINT_COMPLETE_LINE1));
    printComplete->Add(new ScreenLine(PRINT_COMPLETE_LINE2));
    printComplete->Add(new ScreenLine(PRINT_COMPLETE_LINE3));
    screenMap[GetKey(HomingState, PrintCompleted)] = 
                            new Screen(printComplete, PRINT_COMPLETE_LED_SEQ);    
    
    ScreenText* startingPrint = new ScreenText;
    startingPrint->Add(new ScreenLine(STARTING_PRINT_LINE1));
    startingPrint->Add(new ReplaceableLine(STARTING_PRINT_LINE2));
    startingPrint->Add(new ScreenLine(STARTING_PRINT_BTN2_LINE2));
    screenMap[GetKey(MovingToStartPositionState, NoUISubState)] = 
                       new JobNameScreen(startingPrint, STARTING_PRINT_LED_SEQ);
    
    ScreenText* loadFirst = new ScreenText;
    loadFirst->Add(new ScreenLine(LOAD_FIRST_LINE1));
    loadFirst->Add(new ScreenLine(LOAD_FIRST_LINE2));
    loadFirst->Add(new ScreenLine(LOAD_FIRST_LINE3));
    loadFirst->Add(new ScreenLine(LOAD_FIRST_LINE4));
    screenMap[GetKey(HomeState, NoPrintData)] = 
                            new Screen(loadFirst, LOAD_FIRST_LED_SEQ);        
    
    ScreenText* loading = new ScreenText;
    loading->Add(new ScreenLine(LOADING_FILE_LINE1));
    screenMap[GetKey(HomeState, Downloading)] = 
                            new Screen(loading, LOADING_FILE_LED_SEQ);        
    
    ScreenText* printCanceled = new ScreenText;
    printCanceled->Add(new ScreenLine(CANCELED_LINE1));
    printCanceled->Add(new ScreenLine(CANCELED_LINE2));
    printCanceled->Add(new ScreenLine(CANCELED_LINE3));
    screenMap[GetKey(HomingState, PrintCanceled)] = 
                            new Screen(printCanceled, CANCELED_LED_SEQ);    
    
    ScreenText* doorOpen = new ScreenText;
    doorOpen->Add(new ScreenLine(DOOR_OPEN_LINE1));
    doorOpen->Add(new ScreenLine(DOOR_OPEN_LINE2));
    doorOpen->Add(new ScreenLine(DOOR_OPEN_LINE3));
    doorOpen->Add(new ScreenLine(DOOR_OPEN_LINE4));
    doorOpen->Add(new ScreenLine(DOOR_OPEN_LINE5));
    screenMap[GetKey(DoorOpenState, NoUISubState)] = 
                            new Screen(doorOpen, DOOR_OPEN_LED_SEQ); 
    
    // when leaving door opened, just clear the screen,
    // in case next state has no screen defined
    ScreenText* doorClosed = new ScreenText;
    screenMap[GetKey(DoorOpenState, ExitingDoorOpen)] = 
                            new Screen(doorClosed, 0);
    
    ScreenText* error = new ScreenText;
    error->Add(new ScreenLine(ERROR_CODE_LINE1));
    error->Add(new ReplaceableLine(ERROR_CODE_LINE2));
    error->Add(new ReplaceableLine(ERROR_CODE_LINE3));
    error->Add(new ScreenLine(ERROR_CODE_LINE4));
    error->Add(new ScreenLine(ERROR_CODE_LINE5));
    error->Add(new ScreenLine(ERROR_CODE_BTN1_LINE2));
    error->Add(new ScreenLine(ERROR_CODE_BTN2_LINE2));
    screenMap[GetKey(IdleState, NoUISubState)] = 
                            new ErrorScreen(error, ERROR_CODE_LED_SEQ);   
    
    ScreenText* homing = new ScreenText;
    homing->Add(new ScreenLine(HOMING_LINE1));
    homing->Add(new ScreenLine(HOMING_LINE2));
    screenMap[GetKey(HomingState, NoUISubState)] = 
                            new Screen(homing, HOMING_LED_SEQ);
    
    ScreenText* version = new ScreenText;
    version->Add(new ScreenLine(VERSION_LINE1));
    version->Add(new ScreenLine(VERSION_LINE2));
    version->Add(new ScreenLine(VERSION_BTN1_LINE2));
    screenMap[GetKey(ShowingVersionState, NoUISubState)] = 
                            new Screen(version, VERSION_LED_SEQ);  
    
    ScreenText* calibrate = new ScreenText;
    calibrate->Add(new ScreenLine(CALIBRATE_LINE1));
    calibrate->Add(new ScreenLine(CALIBRATE_LINE2));
    calibrate->Add(new ScreenLine(CALIBRATE_LINE3));
    calibrate->Add(new ScreenLine(CALIBRATE_BTN1_LINE2));
    calibrate->Add(new ScreenLine(CALIBRATE_BTN2_LINE2));
    screenMap[GetKey(CalibrateState, NoUISubState)] = 
                            new Screen(calibrate, CALIBRATE_LED_SEQ);       
    
    ScreenText* moveToCal = new ScreenText;
    moveToCal->Add(new ScreenLine(MOVING_TO_CAL_LINE1));
    moveToCal->Add(new ScreenLine(MOVING_TO_CAL_LINE2));
    moveToCal->Add(new ScreenLine(MOVING_TO_CAL_LINE3));
    moveToCal->Add(new ScreenLine(MOVING_TO_CAL_BTN2_LINE2));
    screenMap[GetKey(MovingToCalibrationState, NoUISubState)] = 
                            new Screen(moveToCal, MOVING_TO_CAL_LED_SEQ);       
    
    ScreenText* calibrating = new ScreenText;
    calibrating->Add(new ScreenLine(CALIBRATING_LINE1));
    calibrating->Add(new ScreenLine(CALIBRATING_LINE2));
    calibrating->Add(new ScreenLine(CALIBRATING_LINE3));
    calibrating->Add(new ScreenLine(CALIBRATING_LINE4));
    calibrating->Add(new ScreenLine(CALIBRATING_LINE5));
    calibrating->Add(new ScreenLine(CALIBRATING_BTN1_LINE2));
    screenMap[GetKey(CalibratingState, NoUISubState)] = 
                            new Screen(calibrating, CALIBRATING_LED_SEQ);  
    }

