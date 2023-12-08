#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "axmol.h"
USING_NS_AX;

#include "characters/Player.h"

class GameScene : public Scene
{
    enum class GameState
    {
        init = 0,
        update,
        pause,
        end,
        menu1,
        menu2,
    };

public:
    bool init() override;
    void update(float delta) override;

private:
    GameState gameState = GameState::init;

    Player *player;

    Layer *gameLayer;
    Camera *gameCamera;

    void setupGame();
    void setupPhysics();
    void setupPlayer();
    void setupCamera();
    void setupBackground();
    void setupControls();
    void setupMap();
    void setupSound();
    void setupLayers();
    void onKeyPressed(EventKeyboard::KeyCode keyCode, Event *event);
    void onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event);
    void onControllerButtonDown(Controller *controller, int keyCode, Event *event);
    void onControllerButtonUp(Controller *controller, int keyCode, Event *event);
    void menuCloseCallback(Ref *sender);
    void onControllerAxis(Controller *controller, int keyCode, Event *event);
    bool onPhysicsContact(PhysicsContact &contact);
};

#endif // __GAME_SCENE_H__
