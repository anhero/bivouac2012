/* 
 * File:   Player.cpp
 * Author: samuel
 * 
 * Created on 15 juin 2012, 03:52
 */

#include "Player.h"
#include "HookShot.h"
#include <ios>
using namespace RedBox;

namespace Bivouac2012 {
	
	
	static const float PLAYER_SPEED = 4;

	Player::Player(const std::string& image, PlayState *parentState, int id) : Sprite(image),
	_parentState(parentState), _playerID(id), isFacing(DOWN) {
		_hook = new HookShot("hook", "ring", this);

		//Player 1 is controllable via keyboar, always
		if (_playerID == 0) {
			Keyboard::connectKeyPress(this, &Player::onKeyPress);
			Keyboard::connectKeyHold(this, &Player::onKeyHold);
			Keyboard::connectKeyRelease(this, &Player::onKeyRelease);
		}
		if (InputManager::getInstance().getNbGamePads() > 0) {
			//InputManager::getInstance().getGamePad(_playerID)->connectThumbstickMove(this, &Player::onThumbstickMove);
			InputManager::getInstance().getGamePad(_playerID)->connectButtonHold(this, &Player::onButtonHold);
			InputManager::getInstance().getGamePad(_playerID)->connectButtonPress(this, &Player::onButtonPress);
		}
	}

	Player::~Player() {
		delete _hook;
	}

	//KEYBOARD HANDLING
void Player::onKeyHold(KeySignalData data) {
    switch (data.key) {
        case Key::W:
            move(Vector2(0,-PLAYER_SPEED));
            isFacing = UP;
            break;
        case Key::A:
            move(Vector2(-PLAYER_SPEED,0));
            isFacing = LEFT;
            break;
        case Key::S:
            move(Vector2(0,PLAYER_SPEED));
            isFacing = DOWN;
            break;
        case Key::D:
            move(Vector2(PLAYER_SPEED,0));
            isFacing = RIGHT;
            break;
        default:
            break;
    }
}
void Player::onKeyPress(KeySignalData data) {
    switch (data.key) {
        case Key::SPACE:
            _hook->throwGraplin(isFacing);
            break;
            
        default:
            break;
    }
}
void Player::onKeyRelease(KeySignalData data) {
    
}
/*
	//GAMEPAD HANDLING
	void Player::onThumbstickMove(RedBox::GamePadThumbstickSignalData data) {
		//std::cout << data.thumbstickIndex << "::" << data.gamePadState.getThumbstick(data.thumbstickIndex) << std::endl;
		if (data.thumbstickIndex == 0) {
			if (data.gamePadState.getThumbstick(0) < -0.2) {
				move(Vector2(-PLAYER_SPEED,0));
				isFacing = LEFT;
			}
			else if (data.gamePadState.getThumbstick(0) > 0.2) {
				move(Vector2(PLAYER_SPEED,0));
				isFacing = RIGHT;
			}
		}
	}
 */
	void Player::onButtonHold(RedBox::GamePadButtonSignalData data) {
		
	}
	void Player::onButtonPress(RedBox::GamePadButtonSignalData data) {
		if (data.buttonIndex >= 0 && data.buttonIndex < 4) {
			_hook->throwGraplin(isFacing);
		}
	}

void Player::render(){
    Sprite::render();
    _hook->render();
}

void Player::update() {
	//Player movements through thumbsticks
	if (InputManager::getInstance().getNbGamePads() > 0) {
		GamePadState gamePadState = InputManager::getInstance().getGamePad(_playerID)->getState();
		if (gamePadState.getThumbstick(0) < -0.2) {
			move(Vector2(-PLAYER_SPEED,0));
			isFacing = LEFT;
		}
		else if (gamePadState.getThumbstick(0) > 0.2) {
			move(Vector2(PLAYER_SPEED,0));
			isFacing = RIGHT;
		}
		if (gamePadState.getThumbstick(1) < -0.2) {
			move(Vector2(0,-PLAYER_SPEED));
			isFacing = UP;
		}
		else if (gamePadState.getThumbstick(1) > 0.2) {
			move(Vector2(0,PLAYER_SPEED));
			isFacing = DOWN;
		}
	}

    ////////////////////////////////////////
    //
    // fait une fonction à part.
    //
    ////////////////////////////////////////
	//We assume first update
	if (this->getOldXPosition() == 0 && this->getOldYPosition() == 0) {
		Sprite::update();
		return;
	}
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
	
	if (last_bridge != NULL) {
		/*//If the player quits the bridge, on current bridge position.
		if (!last_bridge->checkIsOnBridge(this->getPositionCenter())) {
			//And not in a room.
			std::cout << "YOU NOT ON BRIDGE ANYMORE" << std::endl;
		}*/
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
	
	Sprite::update();
    _hook->update();
}

}