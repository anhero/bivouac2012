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

	Player::Player(PlayState *parentState, int id) : BivouacSprite("spritesheet_players_eyes", Vector2(54, 92), Vector2(), 88, parentState),
	 _playerID(id), facingAngle(DOWN), _isMobile(true) {
		_hook = new HookShot("hook", "ring", this);

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
			this->addAnimation(ANIMATIONS[i+4], 0.1, -1, 8
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
	}

	Player::~Player() {
		delete _hook;
	}

	//KEYBOARD HANDLING
void Player::onKeyHold(KeySignalData data) {
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
            if (data.buttonIndex < 4) {
                _hook->throwGraplin(facingAngle);
            }
        }
	}

void Player::render(){
    BivouacSprite::render();
    _hook->render();
}

void Player::thumbStickMovements() {
	//Player movements through thumbsticks
	if (InputManager::getInstance().getNbGamePads() > 0 && _isMobile) {
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
	
	//////////////////////
	// Animation settings...
	{
		// Direction
		int animationIndex = 0;
		float angle = facingAngle;
		angle += 180.0f;
		angle += 360.0f;
		angle -=  45.0f;
		angle = fmod(angle,360);
		std::cout << angle << "°";
		if (angle < 90.0f) {
			std::cout << "right";
			animationIndex = 3;
		}
		else if (angle < 180.0f) {
			std::cout << "up";
			animationIndex = 0;
		}
		else if (angle < 270.0f) {
			std::cout << "left";
			animationIndex = 2;
		}
		else {
			std::cout << "down";
			animationIndex = 1;
		}
		
		std::cout << std::endl;
		
		if (getCurrentAnimation() != ANIMATIONS[animationIndex]) {
			startAnimation(ANIMATIONS[animationIndex]);
		}
	}
	
	BivouacSprite::update();
    _hook->update();
}

void Player::baconAssplosion(){
//    _parentState->baconAssplosionAt(this->getPosition(), 50);
    for (int i = 0; i < 50; i++) {
        Bacon * bacon = new Bacon(this->getPosition(), _parentState);
        Vector2 baconVelocity;
        baconVelocity.x =1;
        baconVelocity.setAngle(Random::getRandomInteger(0, 360));
        baconVelocity.normalize();
        baconVelocity *= Random::getRandomFloat(MIN_BACON_VELOCITY, MAX_BACON_VELOCITY);
        bacon->setVelocity(baconVelocity);
        bacon->setGlobalDrag(30);
        _parentState->add(bacon);
    }
}
    
void Player::collisionsAndShits() {
    if (_isMobile) {
        
    
		float oldX = 0;
	float oldY = 0;
	oldX = this->getOldXPosition() + this->getWidth()/2;
	oldY = this->getOldYPosition() + this->getHeight()/2;

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
			Sprite* bridgePart = last_bridge->getPartAtPoint(this->getPositionCenter());
			if (bridgePart != NULL) {
				this->move(bridgePart->getPosition() - last_bridge->getOldPositionForPart(bridgePart));
			}
		}
	}
	//If the player was in a room, we check the boundaries
	else if (last_room != NULL) {
		//LEFT EDGE
		if (this->getXPositionCenter() <= last_room->getXPosition()) {
			this->setXPosition(oldX - this->getWidth()/2);
		}
		//RIGHT EDGE
		else if (this->getXPositionCenter() >= last_room->getXPosition() + last_room->getWidth()) {
			this->setXPosition(oldX - this->getWidth()/2);
		}
		//TOP EDGE
		if (this->getYPositionCenter() <= last_room->getYPosition()) {
			this->setYPosition(oldY - this->getHeight()/2);
		}
		//BOTTOM EDGE
		else if (this->getYPositionCenter() >= last_room->getYPosition() + last_room->getHeight()) {
			this->setYPosition(oldY - this->getHeight()/2);
		}
	}
	if (last_room == NULL && last_bridge == NULL) {
		std::cout << "YOU DYING" << std::endl;
	}
	else {
		//std::cout << "YOU SAFE" << std::endl;
	}
    }
}

}