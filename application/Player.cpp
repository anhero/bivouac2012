/* 
 * File:   Player.cpp
 * Author: samuel
 * 
 * Created on 15 juin 2012, 03:52
 */

#include "Player.h"
#include <ios>
using namespace RedBox;

namespace Bivouac2012 {
	
	
	static const float PLAYER_SPEED = 4;
    
Player::Player(PlayState *parentState) : Sprite(), _parentState(parentState) {

}

Player::Player(const std::string& image, PlayState *parentState) : Sprite(image),
 _parentState(parentState){
    
}

void Player::onKeyHold(KeySignalData data) {
    switch (data.key) {
        case Key::W:
            move(Vector2(0,-PLAYER_SPEED));
            break;
        case Key::A:
            move(Vector2(-PLAYER_SPEED,0));
            break;
        case Key::S:
            move(Vector2(0,PLAYER_SPEED));
            break;
        case Key::D:
            move(Vector2(PLAYER_SPEED,0));
            break;
        default:
            break;
    }
}
void Player::onKeyPress(KeySignalData data) {

}
void Player::onKeyRelease(KeySignalData data) {
	
}

void Player::update() {
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
	//We check in which room the player was
	for (int i=0; i < 4; i++) {
		Sprite * room = _parentState->rooms[i];
		
		
		if (
			   (oldX > room->getXPosition() && oldX < room->getXPosition() + room->getWidth() )
			&& (oldY > room->getYPosition() && oldY < room->getYPosition() + room->getHeight() )
			) {
			last_room = room;
		}
	}
	//If the player was in a room, we check the boundaries
	//TODO: BRIDGE CHECK
	if (last_room != NULL) {
		//LEFT EDGE
		if (this->getXPositionCenter() <= last_room->getXPosition()) {
			std::cout << "TRAPPING LEFT EDGE" << std::endl;
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
	else {
		//We check if the player was on a bridge.
	}
	
	Sprite::update();
}

}