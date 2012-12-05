// Ŭnicode please
#ifndef __GAME_STAGE_FACTORY_H__
#define __GAME_STAGE_FACTORY_H__

class GameStage;
class GameWorld;

class GameStageFactory {
public:
    GameStageFactory(GameWorld *world);
    virtual ~GameStageFactory();

public:
    GameStage *CreateTestStage();

private:
    GameWorld *world_;
};

#endif