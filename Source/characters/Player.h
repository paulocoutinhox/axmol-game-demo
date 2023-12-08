#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "axmol-ext.h"
#include "axmol.h"

USING_NS_AX;
USING_NS_AX_EXT;

class Player : public Sprite
{
public:
    static Player *createPlayer(const std::string &name);

    virtual bool init() override;
    void updateAll(float dt);

    void setSpeed(float value);
    void rotate(float value);
    void moveForward(float dt);
    void moveBackward(float dt);
    void stopRotatingSide();
    void stopMoving();
    void setRotateSide(int value);
    void setMoveDirection(int value);
    void setRotateVelocity(float value);
    void setStartAngle(float value);
    void setRotateSpeed(float value);

private:
    int rotateSide = 0;    // 0 = none, 1 = left, 2 = right
    int moveDirection = 0; // 0 = none, 1 = forward, 2 = backward
    float speed;
    float angle;
    float startAngle;
    float rotateSpeed;

    Vec2 calculateForce(float dt, bool forward);
};

#endif // __TANK_H__
