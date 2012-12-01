// Ŭnicode please
#pragma once

typedef enum {
    kCompNull,

    //logic comp. object type으로써도 사용가능
    kCompPlayer,
    kCompItem,
    kCompBullet,
    kCompEnemy,

    //phy comp
    kCompPhy,

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


//너무 뻔한 typedef와 클래스 선언은 전역범위에서 쓰기 쉽도록 여기에 선언
class GameObject;
class GameWorld;
class GameComponent;
typedef std::shared_ptr<GameObject> GameObjectPtr;