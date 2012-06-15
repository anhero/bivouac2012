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
    
Player::Player(PlayState *parentState) : Sprite(), _parentState(parentState) {

}

Player::Player(const std::string& image, PlayState *parentState) : Sprite(image), _parentState(parentState){
    _hook = new HookShot("hook","chain", this);
}
Player::~Player(){
    delete _hook;
}

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

void Player::render(){
    Sprite::render();
    _hook->render();
}

void Player::update() {
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