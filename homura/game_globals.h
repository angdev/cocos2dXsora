// Ŭnicode please
#pragma once

#include <memory>

typedef enum {
    kCompNull,

    //logic comp. object type으로써도 사용가능
    kCompPlayer,
    kCompItem,
    kCompBullet,
    kCompEnemy,
    kCompCombatPlane,   //전투기
    kCompAI,    //그냥 AI를 가지는 객체 (Player, Enemy에 들어갈 예정)

    //가상의 객체를 위한 컴포넌트
    kCompBoundCheck,

    //phy comp
    kCompPhy,
    kCompSinglePhy,     //b2Body 1개로 구성되는 물리 객체. 물리객체의 구조가 복잡해질 경우를 대비해서 뺴놓긴햇다

    //drawable comp
    kCompDrawable, //일단 넣기
} CompType;

typedef CompType ObjectType;

//씬마다 고유번호 주기
typedef enum {
    kSceneGame,
    kSceneMainMenu,
    kSceneLab,
    kSceneOption,
    kSceneStory,
    kSceneHelp,
    kSceneCredit,
    kSceneHelloWorld,
} SceneType;

typedef enum {
    kTriggerNull,
    kTriggerSpecificDestroy,
} TriggerType;

//너무 뻔한 typedef와 클래스 선언은 전역범위에서 쓰기 쉽도록 여기에 선언
class GameObject;
class GameWorld;
class GameComponent;
typedef std::shared_ptr<GameObject> GameObjectPtr;

