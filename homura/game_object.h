// Ŭnicode please
#pragma once

#include "game_globals.h"
#include <memory>

#if SR_USE_PCH == 0
#include <string>
#endif

//unique_ptr 삭제자 필요; 인클루드 해야하나..?
/*
#include "logic_component.h"
#include "phy_component.h"
#include "drawable_component.h"
*/

class GameWorld;
class GameObject;
class GameComponent;
class DrawableComponent;
class LogicComponent;
class AIComponent;
class PhyComponent;
struct GameMessage;

typedef std::shared_ptr<GameObject> GameObjectPtr;

class GameObject {
public:
    explicit GameObject(GameWorld *world);
    ~GameObject();
    
public:
    void Update(float dt);
    void OnMessage(GameMessage *msg);  

    //getter/setter
public:
    const std::string name() const { return name_; }
    const GameWorld *world() const { return world_; }
    GameWorld *world() { return world_; }

    bool IsEnabled() const { return enable_; }
    void ToggleEnable() { enable_ = !enable_; }
    void Enable() { enable_ = true; }
    void Disable() { enable_ = false; }
    void set_enable(bool b) { enable_ = b; }

    void set_drawable_comp(DrawableComponent *comp);
    DrawableComponent* drawable_comp() { return drawable_comp_.get(); }

    void set_logic_comp(LogicComponent *comp);
    LogicComponent* logic_comp() { return logic_comp_.get(); }

    void set_phy_comp(PhyComponent *comp);
    PhyComponent* phy_comp() { return phy_comp_.get(); }

    void set_ai_comp(AIComponent *comp);
    AIComponent* ai_comp() { return ai_comp_.get(); }

    int id() const { return id_; }

    ObjectType Type() const;

private:
    std::string name_;
    GameWorld *world_;
    bool enable_;

    const int id_;

private:
    //Components
    std::unique_ptr<DrawableComponent> drawable_comp_;
    std::unique_ptr<LogicComponent> logic_comp_;
    std::unique_ptr<PhyComponent> phy_comp_;
    std::unique_ptr<AIComponent> ai_comp_;

private:
    static int next_id_;

};
