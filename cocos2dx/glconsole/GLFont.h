// Ŭnicode please
#pragma once

#include "platform/CCPlatformConfig.h"

//콘솔은 윈도우에서만 지원되면됨
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

#include <assert.h>
#include <stdio.h>

#include <cstdarg>
#include <string>
#include <string.h>
#include <vector>


#define MAX_TEXT_LENGTH 512

struct FontVertex {
	short x, y;
	float s, t;
	static FontVertex Create(short x, short y, float s, float t)
	{
		FontVertex v;
		v.x = x;
		v.y = y;
		v.s = s;
		v.t = t;
		return v;
	}
};

class GLFont
{
public:
	static bool GLFontInit();
	static bool GLFontDeinit();

	static void BeginFontGLState();
	static void EndFontGLState();

public:
	static GLFont *Last();
	GLFont();
	~GLFont();        

	// printf style function take position to print to as well
	// NB: coordinates start from bottom left
    void GetBatchData(std::vector<FontVertex> &vert_list, std::vector<unsigned short> &index_list, const char *fmt, ...);

	void glPrintf(int x, int y, const char *fmt, ...);
	void glPrintf(int x, int y, const std::string fmt, ...){ glPrintf(x,y, fmt.c_str()); }
	void glPrintfFast(int x, int y, const char *fmt, ...);
	void glPrintfFast(int x, int y, const std::string fmt, ...){ glPrintfFast(x,y, fmt.c_str()); }

	unsigned int   CharWidth() { return m_nCharWidth; }
	unsigned int   CharHeight() { return m_nCharHeight; }

private:
	unsigned int   m_nCharWidth; // fixed width
	unsigned int   m_nCharHeight; // fixed width

	//텍스쳐는 1개만 잇어도 충분하니까 적절히 설정
	static unsigned int font_tex;

private:
	// vincent 폰트를 기반으로 텍스쳐 만들기
	// 128글자, 8*8폰트니까
	// 256글자를 저장할수있는 영역을 할당하고
	// 16 * 16개의 글자를 배열할수있도록 텍스쳐의 크기를
	// 128 * 128의 텍스쳐 영역에 글자를 적절히 배치한다
	/*
	enum {
		kTextureWidth = 128,
		kTextureHeight = 64,
		kFontSize = 8,
		kRowCount = kTextureHeight / kFontSize,
		kColCount = kTextureWidth / kFontSize,
	};
	*/
	/*
	enum {
		kTextureWidth = 256,
		kTextureHeight = 256,
		kFontSize = 16,
		kRowCount = kTextureHeight / kFontSize,
		kColCount = kTextureWidth / kFontSize
	};
	*/
	
	enum {
		kTextureWidth = 512,
		kTextureHeight = 512,
		kFontSize = 32,
		kRowCount = kTextureHeight / kFontSize,
		kColCount = kTextureWidth / kFontSize
	};
	
	
	static void GetCharacterCoord(unsigned char ch, int *x, int *y);
	static int GetCharacterPixelIndex(int base_x, int base_y, int x, int y);

	static void GetCharacterTextureQuad(unsigned char ch,
		float *left, float *right, float *top, float *bottom);
	//문자열 뽑는 버텍스 데이터 생성하는 함수
	static void GetTextData(const char *text, int length, std::vector<FontVertex> &vert_list, std::vector<unsigned short> &index_list);
	static void GetTextData(const std::string &text, std::vector<FontVertex> &vert_list, std::vector<unsigned short> &index_list) {
		GetTextData(text.c_str(), text.length(), vert_list, index_list);
	}

	static GLFont *last_font_;
};



#endif