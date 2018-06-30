#include "player.h"

#include <QDebug>
#include <QGraphicsScene>
#include <QVector2D>
#include <QPaintEngine>
#include <qmath.h>

#include "game/game.h"
#include "core/inputmanager.h"
#include "game/misc/animationframework.h"
#include "game/misc/gun.h"
#include "core/window.h"
#include "game/multiplayergame.h"

extern Window* w;
Player::Player(QPointF position) : Mob(position, MobType::PLAYER)
{
    isMoving = false;
    shooting = false;
    reverseAnim = false;
    resetAnim = false;
    infected = false;
    degrees = 180.0f;
    // FRAME_TIME tells us how many game frames does each animation frame last
    static const float FRAME_TIME = 7.0f;
    static const int NUM_OF_FRAMES = 4;

    // Load the player sprite map from res.qrc
    QString path = ":/images/textures/player_sprite.png";
    if(!pixmap.load(path)) qDebug() << "Player>failed to load texture with path: " << path;

    // Initialize all animFWs
    for(int i = 0; i < 8; i++)
        animFW[i] = new AnimationFramework(FRAME_TIME, NUM_OF_FRAMES); // 0-2>FORWARD/LEFT/RIGHT, 3-5>BACKWARD/LEFT/RIGHT, 6>LEFT, 7>RIGHT
    shootingAnimFW = new AnimationFramework(FRAME_TIME, 2);
    walkState = MobWalkState::WALK_FORWARD;
    state = PlayerWalkState::WALK_FORWARD;

    gun = new Gun(this);
    // We make the nameTag child of the player so it's position is relative to the player position
    nameTag = new QGraphicsTextItem(this);

    w->centerOn(this->pos());
}

Player::~Player()
{
    // Delete all things that are not covered by the QT framework
    for(int i = 0; i < 8; i++)
        delete animFW[i];
    delete shootingAnimFW;
    delete gun;
}

void Player::update()
{
    gun->update();
}

void Player::entityCollision()
{
    // Standard mobCollision plus bullet collision
    Mob::mobCollision();
    gun->handleBulletCollision();
}

void Player::handleInput()
{
    InputManager* inputManager = w->getGame()->inputManager;
    // We convert playerPos coords from scene coords to view coords
    QPointF playerPos = w->mapFromScene(this->getCenterPosition());
    QPointF mousePos = w->getGame()->inputManager->mousePosition;
    // Get the relative angle between the player center position and the mouse position
    float angle = atan2(playerPos.y() - mousePos.y(), playerPos.x() - mousePos.x());
    angle = angle * 180.0f / M_PI;
    angle += 180.0f; // clock wise degrees
    degrees = angle;

    setPlayerWalkState();

    isMoving = false;
    reverseAnim = false;
    if(inputManager->isKeyPressed(Qt::Key_Down) || inputManager->isKeyPressed(Qt::Key_S))
    {
        shooting = false;
        isMoving = true;
        // Check if the walking direction is opposite to the facing direction reverse the animation player order
        if(angle >= 0.0f && angle <= 180.0f) reverseAnim = true;
        this->setPos(this->pos().x(), this->pos().y()+10.0f);
    }
    if(inputManager->isKeyPressed(Qt::Key_Right) || inputManager->isKeyPressed(Qt::Key_D))
    {
        shooting = false;
        isMoving = true;
        if(angle >= 90.0f && angle <= 270.0f) reverseAnim = true;
        this->setPos(this->pos().x()+10.0f, this->pos().y());
    }
    if(inputManager->isKeyPressed(Qt::Key_Left) || inputManager->isKeyPressed(Qt::Key_A))
    {
        shooting = false;
        isMoving = true;
        if(!(angle >= 90.0f && angle <= 270.0f)) reverseAnim = true;
        this->setPos(this->pos().x()-10.0f, this->pos().y());
    }
    if(inputManager->isKeyPressed(Qt::Key_Up) || inputManager->isKeyPressed(Qt::Key_W))
    {
        shooting = false;
        isMoving = true;
        if(!(angle >= 0.0f && angle <= 180.0f)) reverseAnim = true;
        this->setPos(this->x(), this->y() - 10.0f);
    }

    if(inputManager->mouseButtonPressed)
    {
        // Try to fire a gun if it succeeds we set the shooting to true for the shootingAnim to play
        if(gun->fire())
        {
            emit shotBullet();
            shooting = true;
        }
        // If the shooting is true we reset the shootingAnim frame to 0
        if(shooting) resetAnim = true;
    }
    if(inputManager->isKeyPressed(Qt::Key_C))
    {
        qDebug() << "-----------------------------------------------------------------------------";
        qDebug() << "pos playerCoords: " << this->pos().x() << "|" << this->pos().y();
        qDebug() << "-----------------------------------------------------------------------------";
    }

    w->centerOn(this->pos());
}

void Player::setPlayerWalkState()
{
    // Base on degrees we decide what PlayerWalkState we will have
    if(degrees >= 22.5f && degrees <= 67.5f) state = PlayerWalkState::WALK_FORWARD_RIGHT;
    else if(degrees >= 67.5f && degrees <= 112.5f) state = PlayerWalkState::WALK_FORWARD;
    else if(degrees >= 112.5f && degrees <= 157.5f) state = PlayerWalkState::WALK_FORWARD_LEFT;
    else if(degrees >= 157.5f && degrees <= 202.5f) state = PlayerWalkState::WALK_LEFT;
    else if(degrees >= 202.5f && degrees <= 247.5f) state = PlayerWalkState::WALK_BACKWARD_LEFT;
    else if(degrees >= 247.5f && degrees <= 292.5f) state = PlayerWalkState::WALK_BACKWARD;
    else if(degrees >= 292.5f && degrees <= 337.5f) state = PlayerWalkState::WALK_BACKWARD_RIGHT;
    else state = PlayerWalkState::WALK_RIGHT;
}

void Player::enableNameTag(QString username)
{
    nameTag->setFont(QFont("times", 15));
    nameTag->setDefaultTextColor(QColor("white"));
    nameTag->setPlainText(username);
    updateNameTag();
}

void Player::updateNameTag()
{
    // Set the nameTag position above the player relative to him and in the middle of him (we must factor in the size of the name)
    QFontMetrics fM(nameTag->font());
    float x = MOB_WIDTH/2.0 - fM.width(nameTag->toPlainText())/2.0f;
    nameTag->setPos(x, -30.0f);
}

void Player::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // Each sprite has this dimensions
    static const QSizeF dims(36.0f, 57.0f);
    // We do the standard mob pain which only paints a rect around the boundingBox and shape if the debug is true
    Mob::paint(painter, option, widget);

    QRectF targetRect, sourceRect; // targetRect is where we will paint (boundingRect()) and sourceRect is the sprite that we will draw from the sprite map
    targetRect = this->boundingRect();
    float x; // We only change need the x coord of the sprite then we just set the y based on the animation time
    int index; // We set the index to the appropriate animFW index

    // This just handles specifics of the sprite map which can't be seen here but are visable in the sprite map
    switch(state)
    {
        case PlayerWalkState::WALK_FORWARD: x = 0.0f; index = 0; break;
        case PlayerWalkState::WALK_FORWARD_LEFT:x = 41.0f; index = 1; break;
        case PlayerWalkState::WALK_FORWARD_RIGHT: x = 341.0f; index = 2; break;
        case PlayerWalkState::WALK_BACKWARD: x = 178.0f; index = 3; break;
        case PlayerWalkState::WALK_BACKWARD_LEFT: x = 131.0f; index = 4; break;
        case PlayerWalkState::WALK_BACKWARD_RIGHT: x = 232.0f; index = 5; break;
        case PlayerWalkState::WALK_LEFT:
        {
            x = 91.0f;
            index = 6;
            if(shooting)
            {
                x = 76.0f;
                break;
            }
            break;
        }
        case PlayerWalkState::WALK_RIGHT: x = 285.0f; index = 7; break;
    }

    if(shooting) // If the player is shooting this animation plays
    {
        // This is a little cheat for fitting all the sprite in the boundingBox
        float expand1 = 0.0f;
        float expand2 = 0.0f;
        if(index == 6 || index == 7)
        {
            targetRect.setWidth(targetRect.width() + 10.0f);
            expand1 = 7.0f;
            expand2 = 5.0f;
        }
        // If the player has just shot a bullet the shooting animation plays
        if(shootingAnimFW->getAnimationNum() == 1 || resetAnim)
        {
            sourceRect.setRect(x, 375.0f, dims.width() + expand1, dims.height());
            shootingAnimFW->update();
            resetAnim = false;
        }
        else // When the animation finishes and as long as the player doesn't move the player with a gun pointing will show
        {
            sourceRect.setRect(x, 312.0f, dims.width() + expand2, dims.height());
            // We don't advance with update so the animation frame stays the same
            shootingAnimFW->update(false);
        }
    }
    else if(!isMoving) // If the player isn't moving we just show the idle standing player
    {
        sourceRect.setRect(x, 250.0f, dims.width(), dims.height() + 2.0f);
    }
    else // If the player just walks the normal walking animation is played
    {
        // We get the animation with the index and it's animation frame (num) which should be playing
        int animNum = animFW[index]->getAnimationNum(reverseAnim);
        if(animNum == 1) sourceRect.setRect(x, 0.0f, dims.width(), dims.height());
        else if(animNum == 2) sourceRect.setRect(x, 59.0f, dims.width(), dims.height());
        else if(animNum == 3) sourceRect.setRect(x, 125.0f, dims.width(), dims.height());
        else sourceRect.setRect(x, 187.0f, dims.width(), dims.height());

        // Then we update the animation framework on that index so that the animation flows and the animationNum changes every NUM_OF_FRAMES
        animFW[index]->update();
    }

    // We update the position of the nametag
    updateNameTag();

    // We just draw a correct sprite of the player (sourceRect) in the boundingBox(targetRect)
    painter->drawPixmap(targetRect, pixmap, sourceRect);
}
