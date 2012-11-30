// Ŭnicode please
#include "CCConsoleLayer.h"
#include "platform/CCPlatformMacros.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

#include "CCDirector.h"

#include "GLFont.h"
#include "GLConsole.h"
#include <iostream>
#include <vector>
#include "CCConsoleKeyboard.h"
#include "shaders/CCShaderCache.h"

using namespace std;

using CVarUtils::operator<<;
using CVarUtils::operator>>;

namespace cocos2d {;

// Single global instance so glut can get access
GLConsole *theConsole = NULL;

CCConsoleLayer::CCConsoleLayer()
	: font_(NULL), debug_kbd_(false)
{
}
CCConsoleLayer::~CCConsoleLayer()
{
	if(font_ != NULL) {
		delete(font_);
		font_ = NULL;
	}
    CC_SAFE_RELEASE_NULL(theConsole);
}
bool CCConsoleLayer::init()
{
	if(!CCLayer::init()) {
		return false;
	}
    theConsole = new GLConsole();
    theConsole->init();

	//create font
	font_ = new GLFont();

    //setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionColor));
    //setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor));
    setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTexture_uColor));

	return true;

}
void CCConsoleLayer::update(float dt)
{
	CCConsoleKeyboard &keyboard = CCConsoleKeyboard::GetInstance();

	//toggle
	if(keyboard.IsEmpty() == false) {
		if(keyboard.Top().keycode == '`') {
			theConsole->ToggleConsole();
			keyboard.Pop();
		}
	}

	//콘솔 잇을때만 키 입력 넘기기
	while(keyboard.IsEmpty() == false && theConsole->IsOpen()) {
		CCKeyCmd cmd = keyboard.Pop();
		int state = cmd.state;
		theConsole->KeyboardFunc(cmd.keycode, state);
	}

    if(debug_kbd_) {
	    //디버깅용으로 키입력 출력하기
	    while(keyboard.IsEmpty() == false) {
		    CCKeyCmd cmd = keyboard.Pop();
		    string cmd_str = cmd.ToString();
		    printf("%s\n", cmd_str.c_str());
	    }
    }

	//모든거 완료한뒤에 적절히 처리하기
	keyboard.Flush();
}

void CCConsoleLayer::draw()
{
	CCDirector *director = CCDirector::sharedDirector();
	//퀘이크 스타일의 콘솔 실제 구현부분
	//2차원에 대충 삼각형 그리기. 여기에 썡으로 GL함수 써서 그려서
	//화면이 박살나지 않으면 될듯?
	director->setDepthTest(false);

	//projection은 엔진 내부에서 설정된값을 일단 따라간다
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//glOrthof(0, 100, 0, 100, -1, 1);
    
    /*
    CC_NODE_DRAW_SETUP();
    //ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position | kCCVertexAttribFlag_Color );
    ccGLEnableVertexAttribs(kCCVertexAttribFlag_Position | kCCVertexAttribFlag_TexCoords);
    
    //
    // Attributes
    //
    float vert[] = {
        0, 0,
        1000, 0,
        0, 1000
    };
    float color[] = {
        1, 1, 0, 1,
        0,1,1,1,
        1,0,1,1
    };
    float texcoord[] = {
        0, 0,
        1, 0,
        0, 1
    };
    GLfloat colors[4] = { 1, 0, 1, 1 };
    unsigned int m_nUniformColor = glGetUniformLocation( getShaderProgram()->getProgram(), "u_color");
    getShaderProgram()->setUniformLocationWith4fv(m_nUniformColor, colors, 1);


    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, vert);
    //glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_FLOAT, GL_FALSE, 0, color);
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0, texcoord);
    //ccGLBlendFunc( m_tBlendFunc.src, m_tBlendFunc.dst );
    glDrawArrays(GL_TRIANGLES, 0, 3);
    CC_INCREMENT_GL_DRAWS(1);
    */
    
	{
		GLFont::BeginFontGLState();
		//draw font
		//font_->glPrintf(100, 100, "asdf");
		//font_->glPrintf(0, 0, "asdf");

		try {
			theConsole->RenderConsole();
		}
		catch( CVarUtils::CVarException e  ){
			switch( e ) {
			case CVarUtils::CVarNonExistant:
				cout << "CVar does not exist" << endl;
				break;
			case CVarUtils::CVarAlreadyCreated:
				cout << "CVarAlreadyCreated" << endl;
				break;
			default:
				cout << "Unknown exception." << endl;
				break;
			}
		}

		GLFont::EndFontGLState();
	}
	director->setDepthTest(true);

	glColor4f(1, 1, 1, 1);
}

}	//namespace cocos2d

#endif