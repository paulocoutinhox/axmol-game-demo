#include "characters/Player.h"

Player *Player::createPlayer(const std::string &name)
{
    Player *player = static_cast<Player *>(Player::create("characters/" + name + "/rifle/move/survivor-move_rifle_0.png"));

    if (player)
    {
        auto physicsBody = PhysicsBody::createBox(Vec2(320, 120), PhysicsMaterial(1.0f, 0.1f, 1.0f), Vec2(10, -20));
        physicsBody->setDynamic(true);
        physicsBody->setMass(1.0f);
        physicsBody->setGravityEnable(false);
        physicsBody->setRotationEnable(false);
        player->addComponent(physicsBody);

        player->setSpeed(1000);
        player->setAnchorPoint(Vec2(0.5f, 0.5f));
        player->setStartAngle(90);
        player->setRotateSpeed(180);
        player->rotate(0);

        player->stopRotatingSide();
        player->stopMoving();

        player->autorelease();
        player->retain();

        return player;
    }

    AX_SAFE_DELETE(player);

    return nullptr;
}

bool Player::init()
{
    if (!Sprite::init())
    {
        return false;
    }

    return true;
}

void Player::updateAll(float dt)
{
    if (moveDirection == 1)
    {
        moveForward(dt);
    }
    else if (moveDirection == 2)
    {
        moveBackward(dt);
    }
    else if (rotateSide == 1)
    {
        rotate(angle - rotateSpeed * dt);
    }
    else if (rotateSide == 2)
    {
        rotate(angle + rotateSpeed * dt);
    }
}

void Player::setSpeed(float value)
{
    this->speed = value;
}

void Player::rotate(float angle)
{
    this->angle = angle;

    if (this->angle > 360.0f)
    {
        this->angle -= 360.0f;
    }

    if (this->angle < 0.0f)
    {
        this->angle += 360.0f;
    }

    this->setRotation(angle + startAngle);
}

void Player::moveForward(float dt)
{
    float angleRadians = AX_DEGREES_TO_RADIANS(getRotation() + startAngle);
    Vec2 direction = Vec2(sin(angleRadians), cos(angleRadians));
    direction.normalize();
    getPhysicsBody()->setVelocity(direction * speed);
}

void Player::moveBackward(float dt)
{
    float angleRadians = AX_DEGREES_TO_RADIANS(getRotation() + startAngle);
    Vec2 direction = Vec2(sin(angleRadians), cos(angleRadians));
    direction.normalize();
    getPhysicsBody()->setVelocity(direction * -speed);
}

void Player::stopRotatingSide()
{
    rotateSide = 0;
}

void Player::stopMoving()
{
    moveDirection = 0;
    rotateSide = 0;
    getPhysicsBody()->setVelocity(Vec2::ZERO);
}

void Player::setRotateSide(int value)
{
    rotateSide = value;
}

void Player::setMoveDirection(int value)
{
    moveDirection = value;
}

void Player::setRotateVelocity(float value)
{
    rotateSpeed = value;
}

void Player::setStartAngle(float value)
{
    startAngle = value;
}

void Player::setRotateSpeed(float value)
{
    rotateSpeed = value;
}
