/* 
 * File:   Player.cpp
 * Author: samuel
 * 
 * Created on 15 juin 2012, 03:52
 */

#include "Player.h"
#include "HookShot.h"
#include "Bacon.h"
#include <ios>
#include <math.h>


#define MIN_BACON_VELOCITY 100
#define MAX_BACON_VELOCITY 150

#define FRAME_OF_DIRECTION(x) (x + frameOffset + i*5)

using namespace RedBox;

namespace Bivouac2012 {

const std::string Player::ANIMATIONS[9] =  {
		"standing_up","standing_down","standing_left","standing_right",
		"walking_up","walking_down","walking_left","walking_right",
		"shocked"
};

	static const float UP = 0.0;
	static const float LEFT = 90.0;
	static const float DOWN = 180.0;
	static const float RIGHT = 270.0;
	
	static const float PLAYER_SPEED = 5;
	
	static const float PLAYER_Y_COLLISION_POINT = 75;
	static const float PLAYER_COLLISION_CIRCLE = 11;
	
	static const int STUN_LENGTH = 100;

    static const int DEFAULT_BACON_COUNT = 300;
    
	Player::Player(PlayState *parentState, int id) : BivouacSprite("spritesheet_players_eyes", Vector2(54, 92), Vector2(), 88, parentState),
    _playerID(id), facingAngle(DOWN), _state(MOBILE),
	_stunned(false), _stunned_counter(0), _graber(0), baconCount(DEFAULT_BACON_COUNT){
        std::string hookStr = "hook";
        hookStr.append(Parser::intToString(id));
        std::string ringStr = "chain";
        ringStr.append(Parser::intToString(id));
		_hook = new HookShot(hookStr, ringStr, this);

		//Player 1 is controllable via keyboar, always
		if (_playerID == 0) {
			Keyboard::connectKeyPress(this, &Player::onKeyPress);
			Keyboard::connectKeyHold(this, &Player::onKeyHold);
			Keyboard::connectKeyRelease(this, &Player::onKeyRelease);
		}
		if (InputManager::getInstance().getNbGamePads() > 0) {
			//InputManager::getInstance().getGamePad(_playerID)->connectThumbstickMove(this, &Player::onThumbstickMove);
			InputManager::getInstance().getGamePad(_playerID)->buttonHold.connect(this, &Player::onButtonHold);
			InputManager::getInstance().getGamePad(_playerID)->buttonPress.connect(this, &Player::onButtonPress);
		}
		
		//Animation construction!
		int frameOffset = _playerID * 22;
		for (int i=0; i < 4; i++) {
			this->addAnimation(ANIMATIONS[i], 0.1, 1, 1, FRAME_OF_DIRECTION(0));
		}
		for (int i=0; i < 4; i++) {
			this->addAnimation(ANIMATIONS[i+4], 0.07, -1, 8
				,FRAME_OF_DIRECTION(0) ,FRAME_OF_DIRECTION(1) ,FRAME_OF_DIRECTION(2) ,FRAME_OF_DIRECTION(1) 
				,FRAME_OF_DIRECTION(0) ,FRAME_OF_DIRECTION(3) ,FRAME_OF_DIRECTION(4) ,FRAME_OF_DIRECTION(3)
			);
		}
		{
			this->addAnimation(ANIMATIONS[8], 0.1, -1, 2
				, frameOffset + 20
				, frameOffset + 21
			);
		}
		
		this->startAnimation("standing_down");
         
		escapeText = new Text("font", "TO ESCAPE");
		
		escapeText->setColor(Color::WHITE);
         canHarvestBacon = true;
         
         debugCircle = SpriteFactory::makePolygon(4, 1, Color::WHITE);
        
		buttonHud = new Sprite("hud_button2", Vector2(), Vector2(33,29), Vector2(), 2);
		buttonHud->addAnimation("boblol", 0.08, -1, 2, 0,1);
		buttonHud->startAnimation("boblol");
        resetPosition();
	}

	Player::~Player() {
		delete _hook;
		delete buttonHud;
		delete escapeText;
	}

	//KEYBOARD HANDLING
void Player::onKeyHold(KeySignalData data) {
	if (!_stunned) {
    switch (data.key) {
        case Key::W:
            move(Vector2(0,-PLAYER_SPEED));
            facingAngle = UP;
            break;
        case Key::A:
            move(Vector2(-PLAYER_SPEED,0));
            facingAngle = LEFT;
            break;
        case Key::S:
            move(Vector2(0,PLAYER_SPEED));
            facingAngle = DOWN;
            break;
        case Key::D:
            move(Vector2(PLAYER_SPEED,0));
            facingAngle = RIGHT;
            break;
        default:
            break;
    }
	}
}
    
void Player::onKeyPress(KeySignalData data) {
    switch (data.key) {
        case Key::SPACE:
            _hook->throwGraplin(facingAngle);
            break;
            
        default:
            break;
    }
}
    
void Player::onKeyRelease(KeySignalData data) {
    
}

	void Player::onButtonHold(RedBox::GamePadButtonSignalData data) {
		
	}
	void Player::onButtonPress(RedBox::GamePadButtonSignalData data) {
        if (data.gamePadIndex == _playerID) {
            if (data.buttonIndex < 4 && !_stunned) {
                if (_hook->getTargetId() != -1  && _hook->grabedPlayer()) {
                    Player* target = _parentState->getPlayers()[_hook->getTargetId()];
                    Vector2 temp = Vector2(-300,0);
                    temp.setAngle(facingAngle);
                    target->setVelocity(temp);
                    target->setGlobalDrag(200);
                    target->stun();
                    _hook->releasePlayer();
                }else if(_state == CARRIED || _state == HOOKED){
                    _graber->getHook()->grabedshacle();
                }
                else{
                    _hook->throwGraplin(facingAngle);
                }
            }
        }
	}

void Player::render(){
    BivouacSprite::render();
    _hook->render();
	
	if(_graber){
		buttonHud->render();
		escapeText->render();
	}
}
    
    void Player::resetPosition(){
        if(_playerID == 0){
            setCollisionPosition(Vector2(240,240));
        }
        else if(_playerID == 1){
            setCollisionPosition(Vector2(860,240));

        }
        else if(_playerID == 2){
            setCollisionPosition(Vector2(860,860));
        }
        else if(_playerID == 3){
            setCollisionPosition(Vector2(240,860));

        }
    }

void Player::thumbStickMovements() {
	//Player movements through thumbsticks
	if (InputManager::getInstance().getNbGamePads() > 0 && _state <= IMMUNE && !_stunned) {
		GamePadState gamePadState = InputManager::getInstance().getGamePad(_playerID)->getState();
		float x = gamePadState.getThumbstick(0);
		float y = gamePadState.getThumbstick(1);
		if (fabs(x) < 0.2) {
			x = 0.0;
		}
		if (fabs(y) < 0.2) {
			y = 0.0;
		}
		Vector2 direction = Vector2(x,y);
        if (direction.getLength()) {
            facingAngle = direction.getAngle();
        }
		move(Vector2(x * PLAYER_SPEED, y * PLAYER_SPEED));
	}
}

void Player::update() {
	thumbStickMovements();

	//We assume first update
	if (this->getOldXPosition() == 0 && this->getOldYPosition() == 0) {
		BivouacSprite::update();
		return;
	}
	collisionsAndShits();
    
    if(!isFLicking)harvestBacon();
	
	//////////////////////
	// Stunned player...
	if (_stunned) {
		if (flickCount >= 100) {
			flick();
		}
		if (--_stunned_counter == 0) {
			_stunned = false;
		}
	}
	
	//////////////////////
	// Animation settings...
	if (_stunned) {
		if (getCurrentAnimation() != "shocked") {
			startAnimation("shocked");
		}
	}
    else if (_state == CARRIED) {
        startAnimation("shocked");
        setAngle(90);
    }else{
		// Direction
        setAngle(0);
		int animationIndex = 0;
		float angle = facingAngle;
		angle += 180.0f;
		angle += 360.0f;
		angle -=  45.0f;
		angle = fmod(angle,360);
		if (angle < 90.0f) {
			animationIndex = 3;
		}
		else if (angle < 180.0f) {
			animationIndex = 0;
		}
		else if (angle < 270.0f) {
			animationIndex = 2;
		}
		else {
			animationIndex = 1;
		}
		
		if (getOldPosition() != getPosition()) {
			animationIndex += 4;
		}
		
		if (getCurrentAnimation() != ANIMATIONS[animationIndex]) {
			startAnimation(ANIMATIONS[animationIndex]);
		}
	}
	
	BivouacSprite::update();
    _hook->update();
    refreshGrabed();
    debugCircle->update();
	buttonHud->update();
	escapeText->update();
	escapeText->setPosition(getPosition() + Vector2(80, -50));
	buttonHud->setPosition(getPosition() + Vector2(40, -50));
//    std::cout << "bacon count : " << baconCount << std::endl;
}
    
    
void Player::harvestBacon(){
    Vector2 colCirclePosition(getPosition().x +getWidth()/2 , getPosition().y +getHeight() - 50);
    int radius = 45;
    
    debugCircle->setPosition(colCirclePosition);
    debugCircle->setScaling(Vector2(radius,radius));
    for (std::list<Bacon*>::iterator i = _parentState->bacons.begin(); i != _parentState->bacons.end(); ) {
        if (!(*i)->getIsFlicking() && ((*i)->getPositionCenter()  - colCirclePosition).getLength() < radius) {
				(*i)->setToBeDeleted(true);
				i = _parentState->bacons.erase(i);
				baconCount++;
        }
        else{
            i++;
        }
    }
}

void Player::baconAssplosion(){
//    _parentState->baconAssplosionAt(this->getPosition(), 50);
    
    int baconAssplosionCount =  MathHelper::roundUpDivision(baconCount, 2);
    
    if (baconAssplosionCount < 10) {
        baconAssplosionCount = baconCount;
    }
    baconCount -= baconAssplosionCount;
    
    for (int i = 0; i < baconAssplosionCount; i++) {
        Bacon * bacon = new Bacon(this->getCollisionPosition(), _parentState);
        Vector2 baconVelocity;
        baconVelocity.x =1;
        baconVelocity.setAngle(Random::getRandomInteger(0, 360));
        baconVelocity.normalize();
        baconVelocity *= Random::getRandomFloat(MIN_BACON_VELOCITY, MAX_BACON_VELOCITY);
        bacon->setVelocity(baconVelocity);
        bacon->setGlobalDrag(30);
		bacon->moveX(- bacon->getWidth()/2);
		bacon->moveY(- bacon->getHeight()/2);
        _parentState->add(bacon);
        _parentState->bacons.push_back(bacon);
    }
}

//Transforms a point from the top-left to its collision point.
Vector2 Player::positionToCollisionPoint(Vector2 point) {
	return Vector2(
				point.x + getWidth()/2,
				point.y + PLAYER_Y_COLLISION_POINT
		);
}
void Player::setYCollisionPosition(float newY) {
	setYPosition(newY - PLAYER_Y_COLLISION_POINT);
}
void Player::setXCollisionPosition(float newX) {
	setXPosition(newX - getWidth()/2);
}
void Player::setCollisionPosition(RedBox::Vector2 position) {
	setCollisionPosition(position.x, position.y);
}
void Player::setCollisionPosition(float newX, float newY) {
	setYCollisionPosition(newY);
	setXCollisionPosition(newX);
}

Vector2 Player::getCollisionPosition() {
	return positionToCollisionPoint(getPosition());
}
float Player::getXCollisionPosition() {
	return getCollisionPosition().x;
}
float Player::getYCollisionPosition() {
	return getCollisionPosition().y;
}

void Player::collisionsAndShits() {
    if (_state <= IMMUNE) {
        
		//The collision point.
		Vector2 collisionPoint =  positionToCollisionPoint(getPosition());
		
		Vector2 oldPosition = positionToCollisionPoint(getOldPosition());
	float oldX = oldPosition.x;
	float oldY = oldPosition.y;

	Sprite * last_room = NULL;
	//We check in which room the player was, if applicable
	for (int i=0; i < 4; i++) {
		Sprite * room = _parentState->rooms[i];
		if (   (oldX > room->getXPosition() && oldX < room->getXPosition() + room->getWidth() )
			&& (oldY > room->getYPosition() && oldY < room->getYPosition() + room->getHeight() )
			) {
			last_room = room;
		}
	}
	Bridge * last_bridge = NULL;
	//We check on which bridge the player was, if applicable.
	for (int i=0; i < 4; i++) {
		Bridge * bridge = _parentState->bridges[i];
		//Check if the player was on a bridge, at the old position of the bridge.
		if (bridge->checkIsOnBridge(Vector2(oldX, oldY),true)) {
			last_bridge = bridge;
		}
	}
	
	//If we're on a bridge
	if (last_bridge != NULL) {
		//And not in a room
		if (last_room == NULL) {
			Sprite* bridgePart = last_bridge->getPartAtPoint(collisionPoint);
			if (bridgePart != NULL) {
				this->move(bridgePart->getPosition() - last_bridge->getOldPositionForPart(bridgePart));
			}
		}
	}
	//If the player was in a room, we check the boundaries
	else if (last_room != NULL) {
		//LEFT EDGE
		if (collisionPoint.x - PLAYER_COLLISION_CIRCLE <= last_room->getXPosition()) {
			this->setXCollisionPosition(oldX + 1);
		}
		//RIGHT EDGE
		else if (collisionPoint.x + PLAYER_COLLISION_CIRCLE >= last_room->getXPosition() + last_room->getWidth()) {
			this->setXCollisionPosition(oldX - 1);
		}
		//TOP EDGE
		if (collisionPoint.y - PLAYER_COLLISION_CIRCLE <= last_room->getYPosition()) {
			this->setYCollisionPosition(oldY + 1);
		}
		//BOTTOM EDGE
		else if (collisionPoint.y + PLAYER_COLLISION_CIRCLE >= last_room->getYPosition() + last_room->getHeight()) {
			this->setYCollisionPosition(oldY - 1);
		}
	}
	if (last_room == NULL && last_bridge == NULL) {
        resetPosition();
		stun();
	}
	else {
		//std::cout << "YOU SAFE" << std::endl;
	}
    }    
}
    
    void Player::refreshGrabed(){
        if (_state == CARRIED && _graber != 0) {
            setPosition(_graber->getPositionCenter() - Vector2(0,30) - getSize()/2);
        }
    }

	void Player::stun() {
		if (!_stunned) {
            std::cout << _playerID << std::endl;
			baconAssplosion();
			flick();
			_stunned = true;
			_stunned_counter = STUN_LENGTH;
		}
	}
	
    void Player::setGraber(Player* graber){
        _graber = graber;
    }
    HookShot* Player::getHook(){
        return _hook;
    }
}