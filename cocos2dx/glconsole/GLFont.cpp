// Ŭnicode please
#include "platform/CCPlatformConfig.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

#include "GLFont.h"
#include "CCGL.h"

#include "sprite_nodes/CCSprite.h"
#include "kazmath/GL/matrix.h"
#include "shaders/CCShaderCache.h"

using namespace std;
using namespace cocos2d;

unsigned int GLFont::font_tex = 0;
cocos2d::CCSprite *font_sprite = NULL;

//float font_scale_x = 1.0f * 0.5f * 0.5;
//float font_scale_y = 1.0f * 0.5f;
const float font_scale_x = 1.0f * 1.0f * 0.5f;
const float font_scale_y = 1.0f * 1.0f * 0.5f;

//sys font 옮겨온거
void GLFont::GetCharacterCoord(unsigned char ch, int *x, int *y) 
{
	*y = ch / kColCount;
	*x = ch % kColCount;
}
int GLFont::GetCharacterPixelIndex(int base_x, int base_y, int x, int y) 
{
	int pixel_x = base_x * kFontSize + x;
	int pixel_y = base_y * kFontSize + y;
	int index = pixel_y * kTextureWidth + pixel_x;
	return index;
}

void GLFont::GetCharacterTextureQuad(unsigned char ch, float *left, float *right, float *top, float *bottom) 
{
	int x, y;
	GetCharacterCoord(ch, &x, &y);
	y = kRowCount - y - 1;
	*left = x * (1.0f / kColCount);
	*right = (x + 1) * (1.0f / kColCount);
	*bottom = 1 - y * (1.0f / kRowCount);
	*top = 1 - (y + 1) * (1.0f / kRowCount);
}

void GLFont::GetTextData(const char *text, int length, std::vector<FontVertex> &vert_list, std::vector<unsigned short> &index_list)
{
	for (int i = 0 ; i < length ; i++) {
		unsigned char ch = text[i];
		float tex_left, tex_right, tex_top, tex_bottom;
		GetCharacterTextureQuad(ch, &tex_left, &tex_right, &tex_top, &tex_bottom);

		short left, right, top, bottom;
		left = i * kFontSize;
		right = left + kFontSize;
		top = 0;
		bottom = top - kFontSize;

		// 3 2
		// 0 1

		FontVertex v0 = FontVertex::Create(left, bottom, tex_left, tex_bottom);
		FontVertex v1 = FontVertex::Create(right, bottom, tex_right, tex_bottom);
		FontVertex v2 = FontVertex::Create(right, top, tex_right, tex_top);
		FontVertex v3 = FontVertex::Create(left, top, tex_left, tex_top);

		int base_index = vert_list.size();

		vert_list.push_back(v0);
		vert_list.push_back(v1);
		vert_list.push_back(v2);
		vert_list.push_back(v3);

		index_list.push_back(base_index + 0);
		index_list.push_back(base_index + 1);
		index_list.push_back(base_index + 2);

		index_list.push_back(base_index + 0);
		index_list.push_back(base_index + 2);
		index_list.push_back(base_index + 3);
	}
}

////////////////////////////////////////////////////////////
GLFont *GLFont::last_font_ = NULL;
GLFont *GLFont::Last()
{
	return last_font_;
}

GLFont::GLFont()
{
	GLFontInit();
	last_font_ = this;

	//m_nCharWidth = 8;
	//m_nCharHeight = 13;
	//m_nCharHeight = 40;
	m_nCharWidth = 32 * font_scale_x;
	m_nCharHeight = 32 * font_scale_y;
}

bool GLFont::GLFontDeinit()
{
	if(font_tex == 0) {
		return false;
	}
	glDeleteTextures(1, &font_tex);
	font_tex = 0;
	return true;
}
bool GLFont::GLFontInit()
{
	if(font_tex > 0) {
		//이미 생성되어있는 경우
		return true;
	}

	/*
	// 텍스쳐는 1/0만 처리하면됨. 작게 할당받자
	typedef unsigned char byte;
	int img_size = sizeof(byte) * kTextureWidth * kTextureHeight;
	byte *data = static_cast<byte*>(malloc(img_size));
	memset(data, 0, sizeof(byte) * img_size);

	// 이미지 생성.
	for (int i = 0 ; i < 128 ; i++) {
		unsigned char *character_font = vincent_data[i];
		int base_x;
		int base_y;
		GetCharacterCoord(i, &base_x, &base_y);

		for (int y = 0 ; y < kFontSize ; y++) {
			char character_scanline = character_font[y];
			for (int x = 0 ; x < kFontSize ; x++) {
				int raw_pixel = (1 << (kFontSize - 1 - x)) & character_scanline;
				bool pixel = raw_pixel > 0 ? true : false;
				int pixel_index = GetCharacterPixelIndex(base_x, base_y, x, y);

				if(pixel == true) {
					data[pixel_index] = UCHAR_MAX;
				}
			}
		}
	}

	//create texture
	GLenum format = GL_ALPHA;
	GLenum elem_type = GL_UNSIGNED_BYTE;

	glGenTextures(1, &font_tex);
	glBindTexture(GL_TEXTURE_2D, font_tex);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, format, kTextureWidth, kTextureHeight, 0, format, elem_type, data);
	CHECK_GL_ERROR("font tex");

	//메모리 해제
	free(data);
	*/

	CCSprite *sprite = new CCSprite();
	//sprite->initWithFile("sys_font.png");
	//sprite->initWithFile("font512_alpha.png");
	//sprite->initWithFile("font512.png");
	sprite->initWithFile("font512.png");
	ccTexParams tex_param;
	tex_param.magFilter = GL_LINEAR;
	tex_param.minFilter = GL_LINEAR;
    // gl get error에서 뻑난다. 아마도 상태 관련 문제겟지..
	//sprite->getTexture()->setTexParameters(&tex_param);
	font_sprite = sprite;
	
	return true;
}

void GLFont::BeginFontGLState()
{
	//알파값만 존재하는 텍스쳐이기 때문에 
	//블렌딩이 미묘하게 다르다
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_ONE, GL_ONE);

	//glBindTexture(GL_TEXTURE_2D, font_tex);
    ccGLBindTexture2D(font_sprite->getTexture()->getName());
}
void GLFont::EndFontGLState()
{
	//cocos2d 기본 상태
    ccGLBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

////////////////////////////////////////////////////////////////////////////////
GLFont::~GLFont()
{
	if(last_font_ == this) {
		last_font_ = NULL;
	}
}


////////////////////////////////////////////////////////////////////////////////
// printf style print function
// NB: coordinates start from bottom left
void GLFont::glPrintf(int x, int y, const char *fmt, ...)   
{	
	char        text[MAX_TEXT_LENGTH];	// Holds Our String
	va_list     ap;	// Pointer To List Of Arguments
	if( fmt == NULL ) {// If There's No Text
		return;	// Do Nothing
	}

	va_start( ap, fmt );	// Parses The String For Variables
	int length =vsnprintf( text, MAX_TEXT_LENGTH, fmt, ap );// And Converts Symbols To Actual Numbers
	va_end( ap ); // Results Are Stored In Text

	vector<FontVertex> vert_list;
	vector<unsigned short> index_list;
	GetTextData(text, length, vert_list, index_list);

    kmGLPushMatrix();
    kmGLTranslatef(x, y + (font_scale_x * kFontSize), 0);
    kmGLScalef(font_scale_x, font_scale_y, 1);

    CCGLProgram *prog = CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTexture_uColor);
    prog->use();
    prog->setUniformForModelViewProjectionMatrix();
    
    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_SHORT, GL_FALSE, sizeof(FontVertex), &vert_list[0].x);
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(FontVertex), &vert_list[0].s);
	glDrawElements(GL_TRIANGLES, index_list.size(), GL_UNSIGNED_SHORT, &index_list[0]);

    kmGLPopMatrix();
}

////////////////////////////////////////////////////////////////////////////////
//printf style print function
//NOTE: coordinates start from bottom left
//ASSUMES ORTHOGRAPHIC PROJECTION ALREADY SET UP...
void GLFont::glPrintfFast(int x, int y, const char *fmt, ...)   
{
	char        text[MAX_TEXT_LENGTH];	// Holds Our String
	va_list     ap;	// Pointer To List Of Arguments
	if( fmt == NULL ) {// If There's No Text
		return;	// Do Nothing
	}

	va_start( ap, fmt );	// Parses The String For Variables
	int length =vsnprintf( text, MAX_TEXT_LENGTH, fmt, ap );// And Converts Symbols To Actual Numbers
	va_end( ap ); // Results Are Stored In Text

	vector<FontVertex> vert_list;
	vector<unsigned short> index_list;
	GetTextData(text, length, vert_list, index_list);

	kmGLPushMatrix();	
	kmGLTranslatef(x, y + (font_scale_x * kFontSize), 0);
	kmGLScalef(font_scale_x, font_scale_y, 1);

    CCGLProgram *prog = CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTexture_uColor);
    prog->use();
    prog->setUniformForModelViewProjectionMatrix();

	glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_SHORT, GL_FALSE, sizeof(FontVertex), &vert_list[0].x);
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(FontVertex), &vert_list[0].s);
	glDrawElements(GL_TRIANGLES, index_list.size(), GL_UNSIGNED_SHORT, &index_list[0]);

	kmGLPopMatrix();
}

void GLFont::GetBatchData(std::vector<FontVertex> &vert_list, std::vector<unsigned short> &index_list, const char *fmt, ...)
{
    char        text[MAX_TEXT_LENGTH];	// Holds Our String
	va_list     ap;	// Pointer To List Of Arguments
	if( fmt == NULL ) {// If There's No Text
		return;	// Do Nothing
	}

	va_start( ap, fmt );	// Parses The String For Variables
	int length =vsnprintf( text, MAX_TEXT_LENGTH, fmt, ap );// And Converts Symbols To Actual Numbers
	va_end( ap ); // Results Are Stored In Text

	GetTextData(text, length, vert_list, index_list);
}

#endif