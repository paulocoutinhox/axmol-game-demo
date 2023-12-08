#include "scenes/GameScene.h"
#include "app/Global.h"
#include "audio/AudioEngine.h"
#include "characters/Player.h"
#include "maps/GameMap.h"

bool GameScene::init()
{
    // super init first
    if (!Scene::initWithPhysics())
    {
        return false;
    }

    // setup
    setupGame();

    // physics
    setupPhysics();

    // background
    setupBackground();

    // layers
    setupLayers();

    // player
    setupPlayer();

    // touch
    setupControls();

    // map
    setupMap();

    // camera
    setupCamera();

    // sound
    setupSound();

    // schedule update
    scheduleUpdate();

    return true;
}

void GameScene::setupGame()
{
    // init random
    srand(static_cast<unsigned int>(time(nullptr)));
}

void GameScene::setupPhysics()
{
    // world
    getPhysicsWorld()->setGravity(Vec2(0, 0));
    getPhysicsWorld()->setSlopBias(0, 0);
    getPhysicsWorld()->setSubsteps(4);

#if _AX_DEBUG
    getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
#endif

    // contact
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = AX_CALLBACK_1(GameScene::onPhysicsContact, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
}

bool GameScene::onPhysicsContact(PhysicsContact &contact)
{
    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();

    if (nodeA && nodeB)
    {
        if (nodeA == player || nodeB == player)
        {
            player->getPhysicsBody()->setVelocity(Vec2::ZERO);
        }
    }

    return true;
};

void GameScene::setupBackground()
{
    // background
    auto background = LayerColor::create(Color4B(64, 31, 7, 255));
    background->setPosition(Vec2(Global::getInstance().visibleSize.width / 2 + Global::getInstance().origin.x, Global::getInstance().visibleSize.height / 2 + Global::getInstance().origin.y));

    float scale = std::max(Global::getInstance().visibleSize.width, Global::getInstance().visibleSize.height);
    background->setScale(scale);

    addChild(background);
}

void GameScene::setupPlayer()
{
    player = Player::createPlayer("player");
    gameLayer->addChild(player, 100);
    player->setPosition(500, 500);
    player->setSpeed(400.0f);
}

void GameScene::setupCamera()
{
    gameCamera = Camera::create();
    gameCamera->setPosition3D(Vec3(0, 0, 900));
    gameCamera->lookAt(Vec3(0, 0, 0), Vec3(0, 1, 0));

#if defined(AX_TARGET_OS_TVOS)
    gameCamera->setZoom(1);
#else
    gameCamera->setZoom(2);
#endif

    gameLayer->addChild(gameCamera, 1000);
}

void GameScene::setupControls()
{
    Controller::startDiscoveryController();

    // add touch listener
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = AX_CALLBACK_2(GameScene::onKeyPressed, this);
    keyboardListener->onKeyReleased = AX_CALLBACK_2(GameScene::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

    // add controller listener
    auto controllerListener = EventListenerController::create();
    controllerListener->onKeyDown = AX_CALLBACK_3(GameScene::onControllerButtonDown, this);
    controllerListener->onKeyUp = AX_CALLBACK_3(GameScene::onControllerButtonUp, this);
    controllerListener->onAxisEvent = AX_CALLBACK_3(GameScene::onControllerAxis, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(controllerListener, this);
}

void GameScene::setupMap()
{
    // map
    auto map = FastTMXTiledMap::create("maps/map1/map.tmx");
    gameLayer->addChild(map, 0, 0);
    map->setScale(1.00f);

    // collision layer
    auto collisionLayer = map->getLayer("Collision Layer");

    if (collisionLayer)
    {
        collisionLayer->setVisible(false);

        for (int y = 0; y < collisionLayer->getLayerSize().height; y++)
        {
            for (int x = 0; x < collisionLayer->getLayerSize().width; x++)
            {
                auto tileSprite = collisionLayer->getTileAt(Vec2(x, y));

                if (tileSprite)
                {
                    auto physicsBody = PhysicsBody::createBox(tileSprite->getContentSize(), PhysicsMaterial(1.0f, 0.1f, 0.0f));
                    physicsBody->setDynamic(false);
                    physicsBody->setGravityEnable(false);
                    physicsBody->setRotationEnable(false);
                    tileSprite->setPhysicsBody(physicsBody);
                }
            }
        }
    }
}

void GameScene::setupSound()
{
    AudioEngine::play2d("sounds/music.mp3", true);
}

void GameScene::setupLayers()
{
    gameLayer = Layer::create();
    addChild(gameLayer);
}

void GameScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event *event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_W)
    {
        player->setMoveDirection(1);
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_S)
    {
        player->setMoveDirection(2);
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_A)
    {
        player->setRotateSide(1);
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_D)
    {
        player->setRotateSide(2);
    }
}

void GameScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event *event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_W)
    {
        player->stopMoving();
        player->stopRotatingSide();
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_S)
    {
        player->stopMoving();
        player->stopRotatingSide();
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_A)
    {
        player->stopMoving();
        player->stopRotatingSide();
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_D)
    {
        player->stopMoving();
        player->stopRotatingSide();
    }
}

void GameScene::onControllerButtonDown(Controller *controller, int keyCode, Event *event)
{
    if (keyCode == Controller::Key::BUTTON_DPAD_UP)
    {
        player->setMoveDirection(1);
    }
    else if (keyCode == Controller::Key::BUTTON_DPAD_DOWN)
    {
        player->setMoveDirection(2);
    }
    else if (keyCode == Controller::Key::BUTTON_DPAD_LEFT)
    {
        player->setRotateSide(1);
    }
    else if (keyCode == Controller::Key::BUTTON_DPAD_RIGHT)
    {
        player->setRotateSide(2);
    }
}

void GameScene::onControllerButtonUp(Controller *controller, int keyCode, Event *event)
{
    if (keyCode == Controller::Key::BUTTON_DPAD_UP)
    {
        player->stopMoving();
        player->stopRotatingSide();
    }
    else if (keyCode == Controller::Key::BUTTON_DPAD_DOWN)
    {
        player->stopMoving();
        player->stopRotatingSide();
    }
    else if (keyCode == Controller::Key::BUTTON_DPAD_LEFT)
    {
        player->stopMoving();
        player->stopRotatingSide();
    }
    else if (keyCode == Controller::Key::BUTTON_DPAD_RIGHT)
    {
        player->stopMoving();
        player->stopRotatingSide();
    }
}

void GameScene::onControllerAxis(Controller *controller, int keyCode, Event *event)
{
    if (keyCode == Controller::Key::JOYSTICK_LEFT_X || keyCode == Controller::Key::JOYSTICK_LEFT_Y)
    {
        float axisX = controller->getKeyStatus(Controller::Key::JOYSTICK_LEFT_X).value;
        float axisY = controller->getKeyStatus(Controller::Key::JOYSTICK_LEFT_Y).value;

        if (axisX > 0.5)
        {
            player->setRotateSide(2);
        }
        else if (axisX < -0.5)
        {
            player->setRotateSide(1);
        }
        else if (axisY > 0.5)
        {
            player->setMoveDirection(2);
        }
        else if (axisY < -0.5)
        {
            player->setMoveDirection(1);
        }
        else
        {
            player->stopMoving();
        }
    }
}

void GameScene::update(float delta)
{
    switch (gameState)
    {
    case GameState::init:
    {
        gameState = GameState::update;
        break;
    }

    case GameState::update:
    {
        // player
        player->updateAll(delta);

        // camera
        Vec2 playerPosition = player->getPosition();
        Vec3 currentCameraPosition = gameCamera->getPosition3D();
        Vec3 targetCameraPosition = Vec3(playerPosition.x, playerPosition.y, currentCameraPosition.z);

        // camera interpolation
        float lerpFactor = 0.2f;

        Vec3 smoothedPosition = Vec3{
            currentCameraPosition.x + (targetCameraPosition.x - currentCameraPosition.x) * lerpFactor,
            currentCameraPosition.y + (targetCameraPosition.y - currentCameraPosition.y) * lerpFactor,
            currentCameraPosition.z,
        };

        gameCamera->setPosition3D(smoothedPosition);

        break;
    }

    case GameState::pause:
    {
        /////////////////////////////
        // Add your codes below...like....
        //
        // anyPauseStuff()

        break;
    }

    case GameState::menu1:
    { /////////////////////////////
        // Add your codes below...like....
        //
        // UpdateMenu1();
        break;
    }

    case GameState::menu2:
    { /////////////////////////////
        // Add your codes below...like....
        //
        // UpdateMenu2();
        break;
    }

    case GameState::end:
    { /////////////////////////////
        // Add your codes below...like....
        //
        // CleanUpMyCrap();
        menuCloseCallback(this);
        break;
    }

    } // switch
}

void GameScene::menuCloseCallback(Ref *sender)
{
    // Close the axmol game scene and quit the application
    _director->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use
     * _director->end() as given above,instead trigger a custom event created in RootViewController.mm
     * as below*/

    // EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
}
