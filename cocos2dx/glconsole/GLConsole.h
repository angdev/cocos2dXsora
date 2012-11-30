// Ŭnicode please
#pragma once

#include "platform/CCPlatformConfig.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

#include "layers_scenes_transitions_nodes/CCLayer.h"
#include "CVars/CVar.h"
#include "Cvars/Timestamp.h"

#include "GLFont.h"
#include "GLColor.h"

#include <deque>
#include <string>

#define CVAR_DEL_KEY 127
#define GLCONSOLE_KEY 96

#define GLCONSOLE_HELP_FILE "helpfile.txt"
#define GLCONSOLE_HISTORY_FILE ".cvar_history"
#define GLCONSOLE_SETTINGS_FILE ".glconsole_settings"
#define GLCONSOLE_SCRIPT_FILE "default.script"
#define GLCONSOLE_INITIAL_SCRIPT_FILE "initial.script"

////////////////////////////////////////////////////////////////////////////////
/// The type of line entered. Used to determine how each line is treated.
enum LineProperty 
{
    LINEPROP_LOG,         // text coming from a text being logged to the console
    LINEPROP_COMMAND,     // command entered at the console
    LINEPROP_FUNCTION,    // a function
    LINEPROP_ERROR,       // an error
    LINEPROP_HELP         //help text
};

class GLConsole;


////////////////////////////////////////////////////////////////////////////////
///  A line of text contained in the console can be either inputted commands or
//  log text from the application
class ConsoleLine
{
    public:
        ConsoleLine(std::string t, LineProperty p = LINEPROP_LOG, bool display = true ){
            m_sText = t;
            m_nOptions = p;
            m_bDisplay = display;
        }
        std::string m_sText;        //actual text
        LineProperty m_nOptions;    //see LineProperty
        bool m_bDisplay;            // display on the console screen?
};


////////////////////////////////////////////////////////////////////////////////
/// This function returns a pointer to the very first GLConsole ever created.
//  As there should only be one, this is ok.  
//  This is a workaround for header only programming.
extern CC_DLL GLConsole*  GetConsole( GLConsole* pFirstConsole = NULL );

////////////////////////////////////////////////////////////////////////////////
/// This function calls "GetConsole" to set the static variable pSavedConsole so
//  we can get access to the console globally.
//  This is a workaround for header only programming.
inline void SetConsole( GLConsole* pFirstConsole )
{
    GetConsole( pFirstConsole );
}

////////////////////////////////////////////////////////////////////////////////
/// Return whether first element is greater than the second.
inline bool StringIndexPairGreater( std::pair<std::string,int> e1, std::pair<std::string,int> e2 )
{
    return e1.first < e2.first;
}

////////////////////////////////////////////////////////////////////////////////
/// Utility function.
inline std::string FindLevel( std::string sString, int iMinRecurLevel )
{   
    int level = 0;
    int index = sString.length();
    for( unsigned int ii = 0; ii < sString.length(); ii++ ) {
        if( sString.c_str()[ii]=='.' ) {
            level ++;
        }
        if( level == iMinRecurLevel ) {
            index = ii+1;
        }
    }
    return sString.substr( 0, index );
}

////////////////////////////////////////////////////////////////////////////////
// remove all spaces from the front and back...
inline std::string& RemoveSpaces( std::string &str )
{
    // remove them off the front
    int idx = str.find_first_not_of( ' ' );
    if( idx > 0 && idx != 0 ) {
        str = str.substr( idx, str.length() );
    }

    // remove them off the back
    idx = str.find_last_not_of(' ');
    if( idx != -1 ) {
        str = str.substr( 0, idx+1 );
    }

    return str;
}

////////////////////////////////////////////////////////////////////////////////
///  The GLConsole class.
class CC_DLL GLConsole : public cocos2d::CCLayer
{
    friend void GLConsoleCheckInit( GLConsole* pConsole ); 

    public:
         GLConsole();
        virtual ~GLConsole();

        // call this after OpenGL is up 
        void Init();
 
        //Prints to console using familiar printf style
        void Printf(const char *msg, ...);
        void Printf_All(const char *msg, ...);

        //error and help versions of printf.
        void PrintError(const char *msg, ...);
        void PrintHelp(const char *msg, ...);
        
        //commands to the console...
        void ToggleConsole();
        bool IsOpen();
		bool IsChanging();
        void OpenConsole();
        void CloseConsole();
 
        //scrolling text up and down in the console
        void ScrollUp(int pixels);
        void ScrollDown(int pixels);
        void ScrollUpLine();
        void ScrollDownLine();
        void CursorLeft();
        void CursorRight();
        void CursorToBeginningOfLine();
        void CursorToEndOfLine();
        void ScrollUpPage();
        void ScrollDownPage();
        void SpecialFunc( int key );

        void PrintAllCVars();

        /// Add a character to the command line.
        void KeyboardFunc( unsigned char key , int state);

        /// Clear the current command.
        void ClearCurrentCommand();

        /// enter a full line of text to the log text.
        void EnterLogLine(const char *line, LineProperty prop = LINEPROP_LOG, bool display = true);

        /// display previous command in history on command line.
        void HistoryBack();
   
        /// go forward in the history.
        void HistoryForward();

        /// clears all of the console's history.
        void HistoryClear();

        bool HistorySave( std::string sFileName = "" );
        bool HistoryLoad( std::string sFileName = "" );

        bool SettingsSave(std::string sFileName = "");
        bool SettingsLoad(std::string sFileName = "");

        //script interface
        void ScriptRecordStart();
        void ScriptRecordStop();
        void ScriptRecordPause();
        void ScriptShow();
        bool ScriptRun( std::string sFileName = "" );
        bool ScriptSave( std::string sFileName = "" );
        bool ScriptLoad( std::string sFileName = "" );

        //help
        bool Help(std::vector<std::string> *vArgs);
        
        GLFont * GetFont() const { return m_pGLFont; }

        void SetLogColor(float r, float g, float b) {
            m_logColor.r = r; m_logColor.g = g; m_logColor.b = b;
        }

        void SetCommandColor( float r, float g, float b ) { 
            m_commandColor.r = r; m_commandColor.g = g; m_commandColor.b = b; 
        }

        void SetFunctionColor(float r, float g, float b) {
            m_functionColor.r = r; m_functionColor.g = g; m_functionColor.b = b;
        }
      
        void SetErrorColor(float r, float g, float b) { 
            m_errorColor.r = r; m_errorColor.g = g; m_errorColor.b = b;
        }

        void SetHelpColor(float r, float g, float b) {
            m_helpColor.r = r; m_helpColor.g = g; m_helpColor.b = b;
        }
        void RenderConsole();

        // member cvars accessible from console
        float& m_fConsoleBlinkRate;
        float& m_fConsoleAnimTime;
        int&   m_nConsoleMaxHistory;
        int&   m_nConsoleLineSpacing;
        int&   m_nConsoleLeftMargin;
        int&   m_nConsoleVerticalMargin;
        int&   m_nConsoleMaxLines;
        float& m_fOverlayPercent;
        std::string& m_sHistoryFileName;
        std::string& m_sScriptFileName;
        std::string& m_sSettingsFileName;
        std::string& m_sInitialScriptFileName;

    private:
        void _CheckInit();

        /// Height of the console in pixels (even if it is currently animating)
        int  _GetConsoleHeight();
        void _RenderText();
        void _TabComplete();
        bool _ProcessCurrentCommand( bool bExecute = true );
        bool _ExecuteFunction( CVarUtils::CVar<ConsoleFunc> * cvar, bool bExecute );
        bool _IsCursorOn();
        bool _IsConsoleFunc( TrieNode *node );
        int  _FindRecursionLevel( std::string sCommand );
        bool _LoadExecuteHistory( std::string sFileName = "", bool bExecute=false );

        std::string _GetHistory();

    private:
        bool          m_bExecutingHistory; //Are we executing a script or not.
        bool          m_bSavingScript; // Are we saving a script
        bool          m_bConsoleOpen; // whether the console is drawing or not
        bool          m_bIsChanging; // whether the console is currently transitioning 
        TimeStamp     m_Timer;
        TimeStamp     m_BlinkTimer;
        int           m_nWidth;
        int           m_nHeight;
        int           m_nViewportX;
        int           m_nViewportY;
        int           m_nViewportWidth;
        int           m_nViewportHeight;
        int           m_nTextHeight;
		int	m_nTextWidth;
        int           m_nScrollPixels;  //the number of pixels the text has been scrolled "up"
        int           m_nCommandNum;
        GLFont*       m_pGLFont;
 
        // Text colors
        GLColor&      m_logColor;
        GLColor&      m_commandColor;
        GLColor&      m_functionColor;
        GLColor&      m_errorColor;
        GLColor&      m_helpColor;

        //CVar<GLColor> m_consoleColor;
        GLColor&      m_consoleColor;

        //history variables
        std::string   m_sOldCommand;
        char          m_cLastChar;
        unsigned int  m_nSuggestionNum;

        // simplify getting gl viewport
        struct {
            int x;
            int y;
            int width;
            int height;
        } m_Viewport;

        //std::string m_sMaxPrefix; // max prefix
        std::string m_sCurrentCommandBeg;      //current command being typed
        std::string m_sCurrentCommandEnd;      //current command being typed
        std::deque<ConsoleLine> m_consoleText; // all the console text
        std::deque<ConsoleLine> m_ScriptText; // all the console text
};


#endif