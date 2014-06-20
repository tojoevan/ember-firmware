/* 
 * File:   CommandInterpreterUT.cpp
 * Author: greener
 *
 * Created on Jun 12, 2014, 11:51:33 AM
 */

#include <stdlib.h>
#include <iostream>

#include <CommandInterpreter.h>
#include <Hardware.h>


/*
 * Simple C++ Test Suite
 */
Command expected;
bool handled = false;

const char* expectedErrorMsg;
bool gotExpectedError = false;

class TestTarget: public ICommandTarget
{
    void Handle(Command command)
    {
        if(command != expected)
        {
            std::cout << "%TEST_FAILED% time=0 testname=test1 (CommandInterpreterUT) message=unexpected command.  " << 
                          "expected: " << expected << " but got: " << command << std::endl;
        }
        else
        {
            std::cout << "got expected command: " << command << std::endl;
            handled = true;
        }
    }
    
    void HandleError(const char* baseMsg, bool fatal = false, 
                             const char* str = NULL, int value = INT_MAX)
    {
        // check for expected error
        if(strcmp(expectedErrorMsg, baseMsg) != 0)
        {
            std::cout << "%TEST_FAILED% time=0 testname=test1 (CommandInterpreterUT) message=unexpected error.  " << 
                          "expected: " << expectedErrorMsg << " but got: " << baseMsg << std::endl;
        }
        else
        {
            std::cout << "got expected error: " << expectedErrorMsg << std::endl;
            gotExpectedError = true;
        }
    }
};

void CheckHandled(Command expected)
{
    if(!handled)
    {
        std::cout << "%TEST_FAILED% time=0 testname=test1 (CommandInterpreterUT) message=command not handled: " << 
                      expected <<  std::endl;
    }
    handled = false;
}

void CheckNotHandled()
{
    if(handled)
    {
        std::cout << "%TEST_FAILED% time=0 testname=test1 (CommandInterpreterUT) message=illegal command handled" << 
                      std::endl;
    }
    handled = false;
    if(!gotExpectedError)
    {
        std::cout << "%TEST_FAILED% time=0 testname=test1 (CommandInterpreterUT) message=expected error not found" << 
                      std::endl;
    }
    gotExpectedError = false;
}

void test1() {
    std::cout << "CommandInterpreterUT test 1" << std::endl;
    TestTarget target;
    CommandInterpreter cmdInterp(&target);
    
    // check that button events correctly handled
    unsigned char btnData;
    btnData = BTN1_PRESS;
    expected = StartPauseOrResume;
    cmdInterp.Callback(ButtonInterrupt, &btnData);
    CheckHandled(expected);
    
    btnData = BTN2_PRESS;
    expected = Cancel;
    cmdInterp.Callback(ButtonInterrupt, &btnData);
    CheckHandled(expected);
    
    // check that text commands correctly handled
    char textCmd[32];
    strcpy(textCmd, "Start");
    expected = Start;
    cmdInterp.Callback(UICommand, textCmd);
    CheckHandled(expected);
    
    strcpy(textCmd, "getstatus");
    expected = GetStatus;
    cmdInterp.Callback(UICommand, textCmd);
    CheckHandled(expected);
    
    strcpy(textCmd, "reSUme");
    expected = Resume;
    cmdInterp.Callback(Keyboard, textCmd);
    CheckHandled(expected);
    
    strcpy(textCmd, "PAUSE");
    expected = Pause;
    cmdInterp.Callback(Keyboard, textCmd);
    CheckHandled(expected);
    
    // check that illegal commands are not handled
    expected = UndefinedCommand;
    expectedErrorMsg = FRONT_PANEL_ERROR;
    btnData = 0xFF;
    cmdInterp.Callback(ButtonInterrupt, &btnData);
    CheckNotHandled();    
    
    strcpy(textCmd, "garbageIn");
    expectedErrorMsg = UNKNOWN_TEXT_COMMAND_ERROR;
    cmdInterp.Callback(UICommand, textCmd);
    CheckNotHandled();
    
    strcpy(textCmd, "Paws");
    expectedErrorMsg = UNKNOWN_TEXT_COMMAND_ERROR;
    cmdInterp.Callback(Keyboard, textCmd);
    CheckNotHandled();
}



int main(int argc, char** argv) {
    std::cout << "%SUITE_STARTING% CommandInterpreterUT" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    std::cout << "%TEST_STARTED% test1 (CommandInterpreterUT)" << std::endl;
    test1();
    std::cout << "%TEST_FINISHED% time=0 test1 (CommandInterpreterUT)" << std::endl;

    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return (EXIT_SUCCESS);
}
