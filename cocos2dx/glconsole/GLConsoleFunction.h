/*
    
    \file GLConsoleFunction.h

    Collection of ConsoleFunc's for GLConsole.

    $Id: GLConsoleFunction.h 162 2010-02-15 19:11:05Z gsibley $
 */

#ifndef __GL_CONSOLE_FUNCTION__
#define __GL_CONSOLE_FUNCTION__

#include "platform/CCPlatformMacros.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

#include <vector>
#include <cstdlib>

class GLConsole;

////////////////////////////////////////////////////////////////////////////////
/**
 * The current console version.
 */
bool ConsoleVersion( std::vector<std::string>* );

////////////////////////////////////////////////////////////////////////////////
/**
 * Help for functions and variables or just general help.
 */
bool ConsoleHelp( std::vector<std::string> *vArgs );

////////////////////////////////////////////////////////////////////////////////
/**
 * Looks for the lists of substrings provided in vArgs in the CVarTrie.
 */
bool ConsoleFind( std::vector<std::string> *vArgs );

////////////////////////////////////////////////////////////////////////////////
/**
 * First argument indicates the file name, the following arguments are
 * used to choose a subset of variables to be saved using the provided
 * substrings.
 * Last argument can be used to be verbose when saving.
 */
bool ConsoleSave( std::vector<std::string> *vArgs );

////////////////////////////////////////////////////////////////////////////////
/**
 * Load CVars from a file
 */
bool ConsoleLoad( std::vector<std::string> *vArgs );

////////////////////////////////////////////////////////////////////////////////
/**
 * Exits program from command line
 */
bool ConsoleExit( std::vector<std::string> * );

////////////////////////////////////////////////////////////////////////////////
/**
 * Save the console history to a file
 */
bool ConsoleHistorySave( std::vector<std::string> *vArgs );

////////////////////////////////////////////////////////////////////////////////
/**
 * Load a previously saved console history from a file
 */
bool ConsoleHistoryLoad( std::vector<std::string> *vArgs );

////////////////////////////////////////////////////////////////////////////////
/**
 * clear the console history
 */
bool ConsoleHistoryClear( std::vector<std::string>* );

////////////////////////////////////////////////////////////////////////////////
/**
 * Save the current script
 */
bool ConsoleScriptSave( std::vector<std::string> *vArgs );

////////////////////////////////////////////////////////////////////////////////
/**
 * Load a script from a file
 */
bool ConsoleScriptLoad( std::vector<std::string> *vArgs );

////////////////////////////////////////////////////////////////////////////////
/**
 * Start script recording
 */
bool ConsoleScriptRecordStart( std::vector<std::string> * );

////////////////////////////////////////////////////////////////////////////////
/**
 * Stop script recording
 */
bool ConsoleScriptRecordStop( std::vector<std::string> * );

////////////////////////////////////////////////////////////////////////////////
/**
 * Pause script recording
 */
bool ConsoleScriptRecordPause( std::vector<std::string> * );

////////////////////////////////////////////////////////////////////////////////
/**
 * Show the current script
 */
bool ConsoleScriptShow( std::vector<std::string> * );

////////////////////////////////////////////////////////////////////////////////
/**
 * Run the current scipt or one from the named file
 */
bool ConsoleScriptRun( std::vector<std::string> *vArgs );

////////////////////////////////////////////////////////////////////////////////
/**
 * Save the console settings
 */
bool ConsoleSettingsSave( std::vector<std::string> *vArgs );

////////////////////////////////////////////////////////////////////////////////
/**
 * Load console settings
 */
bool ConsoleSettingsLoad( std::vector<std::string> *vArgs );


#endif
#endif


