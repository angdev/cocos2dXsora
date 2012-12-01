// Ŭnicode please
#pragma once

typedef enum {
  kCompPlayer,
  kCompItem,
  kCompBullet,
  kCompEnemy,
  kCompPhy,
  kCompDrawable, //일단 넣기
} CompType;

typedef enum {

} ObjectType;


//너무 뻔한 typedef와 클래스 선언은 전역범위에서 쓰기 쉽도록 여기에 선언
class GameObject;
class GameWorld;
class GameComponent;
typedef std::shared_ptr<GameObject> GameObjectPtr;