// Ŭnicode please
#pragma once


class GLESDebugDraw;
class GameWorld;
class CollisionManager;

class ContactListener : public b2ContactListener {
public:
    void BeginContact(b2Contact* contact) { }
    void EndContact(b2Contact* contact) { }
    void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {
        contact->SetEnabled(false);
    }
    void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) { }
};

class PhyWorld {
public:
    PhyWorld(GameWorld *game_world);
    ~PhyWorld();

public:
    void Update(float dt);
    void Init();

    // getter / setter
public:
    const b2World *b2_world() const { return b2_world_.get(); }
    b2World *b2_world() { return b2_world_.get(); }

private:
    void InitDebug(b2World *world);
    void InitGround(b2World *world);

private:
    std::unique_ptr<b2World> b2_world_;
    std::unique_ptr<GLESDebugDraw> debug_draw_;
    std::unique_ptr<CollisionManager> collision_mgr_;
    std::unique_ptr<ContactListener> contact_listener_;

    GameWorld *game_world_;
};