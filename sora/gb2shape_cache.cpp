// Ŭnicode please
//
//  GB2ShapeCache.h
//  
//  Loads physics sprites created with http://www.PhysicsEditor.de
//
//  Generic Shape Cache for box2d
//
//  Copyright by Andreas Loew 
//      http://www.PhysicsEditor.de
//      http://texturepacker.com
//      http://www.code-and-web.de
//  
//  All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//  
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//  
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//
#include "sora_stdafx.h"
#include "cocos2d.h"
#include "gb2shape_cache.h"
#include "template_lib.h"

using namespace sora;
using namespace std;
using namespace cocos2d;

//#if defined(__IPHONE_OS_VERSION_MIN_REQUIRED)
//#   define CGPointFromString_ CGPointFromString
//#else
// well - not nice but works for now
static CCPoint CGPointFromString_(CCString* str)
{
	IUASSERT(str != NULL);
	//적절히 내부 구현고치기
	//{ 96.500,0.500 } 같은식으로 들어올것이다
	string theString = str->m_sString;

	string x_str;
	string y_str;
	int curr_target = 0;
	for(size_t i = 2 ; i < theString.size() - 2 ; i++) {
		char ch = theString[i];
		string *str = NULL;
		if(curr_target == 0) {
			str = &x_str;
		} else {
			str = &y_str;
		}

		if(ch == ',') {
			curr_target++;
			continue;
		} else {
			str->push_back(ch);
		}
	}
	float x = (float)atof(x_str.c_str());
	float y = (float)atof(y_str.c_str());
	return ccp(x, y);
	//theString = [theString stringByReplacingOccurrencesOfString:@"{ " withString:@""];
	//theString = [theString stringByReplacingOccurrencesOfString:@" }" withString:@""];
	//CCArray *arr = [theString componentsSeparatedByString:@","];
	//return CGPointMake([[arr objectAtIndex:0] floatValue], [[array objectAtIndex:1] floatValue]);
}
//#endif

/**
* Internal class to hold the fixtures
*/
class FixtureDef  {
public:
	FixtureDef()
		: next(0)
	{}

	~FixtureDef() {
		delete next;
		delete fixture.shape;
	}

	FixtureDef *next;
	b2FixtureDef fixture;
	int callbackData;
};

/**
* Body definition
* Holds the body and the anchor point
*/
class BodyDef {
public:
	BodyDef();
	~BodyDef();
	FixtureDef *fixtures;
	CCPoint anchorPoint;
};

BodyDef::BodyDef() 
{
	fixtures = 0;
}

BodyDef::~BodyDef() 
{
	delete fixtures;
}



GB2ShapeCache *GB2ShapeCache::sharedShapeCache()
{
	static GB2ShapeCache *shapeCache = 0;
	if(!shapeCache) {
		shapeCache = new GB2ShapeCache();
	}
	return shapeCache;
}

GB2ShapeCache::GB2ShapeCache()
{
}

GB2ShapeCache::~GB2ShapeCache()
{
	//삭제
	Clear();	
}

void GB2ShapeCache::Clear()
{
	sora::ClearAssocContainer(shapeObjects_);
}

void GB2ShapeCache::addFixturesToBody(b2Body *body, const std::string &shape)
{
	BodyDef *so = shapeObjects_[shape];
	assert(so);

	FixtureDef *fix = so->fixtures;
	while(fix)
	{
		body->CreateFixture(&fix->fixture);
		fix = fix->next;
	}
}

CCPoint GB2ShapeCache::anchorPointForShape(const std::string &shape)
{
	BodyDef *bd = shapeObjects_[shape];
	assert(bd);
	return bd->anchorPoint;
}

void GB2ShapeCache::addShapesWithFile(const std::string &plist){
    std::string path = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(plist.c_str());
    CCDictionary *dictionary = CCDictionary::dictionaryWithContentsOfFileThreadSafe(path.c_str());

	CCDictionary *metadataDict = NULL;
	metadataDict = (CCDictionary*)(dictionary->objectForKey(string("metadata")));
	
	CCObject *formatObj = metadataDict->objectForKey("format");
	CCString *formatStr = static_cast<CCString*>(formatObj);
    int format = formatStr->intValue();

    ptmRatio_ =  static_cast<CCString*>(metadataDict->objectForKey("ptm_ratio"))->floatValue();
	/*
	float content_scale = CC_CONTENT_SCALE_FACTOR();
	if(content_scale == 2) {
		ptmRatio_ *= CC_CONTENT_SCALE_FACTOR();
	} else {
		ptmRatio_ *= 2.0f;
	}
	*/

	assert(format == 1 && "Format not supported");

	CCDictionary *bodyDict = (CCDictionary*)dictionary->objectForKey("bodies");

	b2Vec2 vertices[b2_maxPolygonVertices];

	//key list
    CCArray *key_list = bodyDict->allKeys();
    for(size_t i = 0 ; i < key_list->count() ; ++i) {
        CCString *key_str = (CCString*)key_list->objectAtIndex(i);
        const string &bodyName = key_str->m_sString;
	
		// get the body data
		CCDictionary *bodyData = (CCDictionary*)bodyDict->objectForKey(bodyName);

		// create body object
		BodyDef *bodyDef = new BodyDef();

		bodyDef->anchorPoint = CGPointFromString_((CCString*)bodyData->objectForKey("anchorpoint"));

		// iterate through the fixtures
		CCArray *fixtureList = (CCArray*)bodyData->objectForKey("fixtures");
		FixtureDef **nextFixtureDef = &(bodyDef->fixtures);

		for(size_t i = 0 ; i < fixtureList->count() ; i++) {
			CCObject *obj = fixtureList->objectAtIndex(i);
			CCDictionary *fixtureData = (CCDictionary *)obj;

			b2FixtureDef basicData;

            basicData.filter.categoryBits = ((CCString*)fixtureData->objectForKey("filter_categoryBits"))->intValue();
            basicData.filter.maskBits = ((CCString*)fixtureData->objectForKey("filter_maskBits"))->intValue();
            basicData.filter.groupIndex = ((CCString*)fixtureData->objectForKey("filter_groupIndex"))->intValue();
            basicData.friction = ((CCString*)fixtureData->objectForKey("friction"))->floatValue();
            basicData.density = ((CCString*)fixtureData->objectForKey("density"))->floatValue();
            basicData.restitution = ((CCString*)fixtureData->objectForKey("restitution"))->floatValue();
            basicData.isSensor = ((CCString*)fixtureData->objectForKey("isSensor"))->boolValue();
			int callbackData = 0;
			if(fixtureData->objectForKey("userdataCbValue") != NULL) {
                callbackData = ((CCString*)fixtureData->objectForKey("userdataCbValue"))->intValue();
			}

			CCString *fixtureType = (CCString*)fixtureData->objectForKey("fixture_type");

			// read polygon fixtures. One convave fixture may consist of several convex polygons
			if(fixtureType->m_sString == "POLYGON") {
				CCArray *polygonsArray = (CCArray*)fixtureData->objectForKey("polygons");

				for(size_t i = 0 ; i < polygonsArray->count() ; i++) {
					CCArray *polygonArray = (CCArray*)polygonsArray->objectAtIndex(i);

					FixtureDef *fix = new FixtureDef();
					fix->fixture = basicData; // copy basic data
					fix->callbackData = callbackData;

					b2PolygonShape *polyshape = new b2PolygonShape();
					int vindex = 0;

					assert(polygonArray->count() <= b2_maxPolygonVertices);
					for(size_t j = 0 ; j < polygonArray->count() ; j++) {
                        CCString *pointString = (CCString*)polygonArray->objectAtIndex(j);
						CCPoint offset = CGPointFromString_(pointString);
						vertices[vindex].x = (offset.x / ptmRatio_) ; 
						vertices[vindex].y = (offset.y / ptmRatio_) ; 
						vindex++;
					}

					polyshape->Set(vertices, vindex);
					fix->fixture.shape = polyshape;

					// create a list
					*nextFixtureDef = fix;
					nextFixtureDef = &(fix->next);
				}
			} else if(fixtureType->m_sString == "CIRCLE") {
				FixtureDef *fix = new FixtureDef();
				fix->fixture = basicData; // copy basic data
				fix->callbackData = callbackData;

				CCDictionary *circleData = (CCDictionary*)fixtureData->objectForKey("circle");

				b2CircleShape *circleShape = new b2CircleShape();
                circleShape->m_radius = ((CCString*)circleData->objectForKey("radius"))->floatValue() / ptmRatio_;
				CCPoint p = CGPointFromString_((CCString*)circleData->objectForKey("position"));
				circleShape->m_p = b2Vec2(p.x / ptmRatio_, p.y / ptmRatio_);
				fix->fixture.shape = circleShape;

				// create a list
				*nextFixtureDef = fix;
				nextFixtureDef = &(fix->next);
			} else {
				// unknown type
				assert(0);
			}
		}

		// add the body element to the hash
		shapeObjects_[bodyName] = bodyDef;
	}
}

float GB2ShapeCache::ptmRatio()
{
	return ptmRatio_;
}
