// ?nicode please
#include "stdafx.h"
#include "game_console.h"
#include "CVars/CVar.h"
#include "glconsole/GLConsole.h"
#include <sstream>

#if CC_CONSOLE

using namespace std;
using namespace cocos2d;

// 변수 등록 예제 함수
void InitSampleVariable()
{
    ////// How to make some CVars //////
    //CVars have a name and a default value
    CVarUtils::CreateCVar( "myFirstCVar", 1);
    //You can make groups using a dot in the name. Helps keep things tidy
    CVarUtils::CreateCVar( "LevelOne.myVar1 ", 2 );
    CVarUtils::CreateCVar( "LevelOne.myVar2",  3 );
    //You can have lots of levels
    CVarUtils::CreateCVar( "LevelOne.LevelTwo.another",  3.14 );

    //CVars can also have a help string associated
    //Just type help var_name on the console
    CVarUtils::CreateCVar( "whatAmIFor", 47, "A test variable to teach you about help");

    //To actually make these CVars useful they need effect something in your program,
    //so you normally create them as follows
    int& nTest = CVarUtils::CreateCVar( "testVar", 100, "Another test CVar" );

    //nTest is now always equal the value of testVar and vice-vera as it is a reference
    cout << "nTest: " << nTest << endl;
    //This is how you get a CVar value. Note the type specification
    cout << "testVar: " << CVarUtils::GetCVar<int>( "testVar" ) << endl;

    nTest = 200;
    cout << "testVar: " << CVarUtils::GetCVar<int>( "testVar" ) << endl;

    //this is how to set a CVar
    CVarUtils::SetCVar<int>( "testVar", 300 );
    cout << "nTest: " << nTest << endl;


    //CVars have exception handling
    try {
        cout << "Nonexistant: " << CVarUtils::GetCVar<int>( "nonExistant" );
    }
    catch( CVarUtils::CVarException e  ){
        switch( e ) {
        case CVarUtils::CVarNonExistant:
            cout << "CVar does not exist" << endl;
            break;
        default:
            cout << "Unknown exception." << endl;
            break;
        }
    }
}


bool echo_cmd(std::vector<std::string> *args)
{
    GLConsole *console = GetConsole();

    ostringstream oss;
    int argc = args->size();
    oss << "[" << argc << "] ";
    for(auto it = args->begin() ; it != args->end() ; ++it) {
        oss << *it;
        oss << "/";
    }
    console->EnterLogLine(oss.str().c_str());
    return true;
}

//함수 등록 예제
void InitSampleFunction()
{
    CVarUtils::CreateCVar("echo_cmd", echo_cmd, "Echo Command");
}

bool GameConsole::Init()
{
    InitSampleVariable();
    InitSampleFunction();
    return true;
}

bool GameConsole::Deinit()
{
    return true;
}
#endif