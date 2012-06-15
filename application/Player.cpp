/* 
 * File:   Player.cpp
 * Author: samuel
 * 
 * Created on 15 juin 2012, 03:52
 */

#include "Player.h"
using namespace RedBox;

namespace Bivouac2012 {

Player::Player() : RedBox::Sprite() {
	Keyboard::connectKeyPress(this, &Player::onKeyPress);
	Keyboard::connectKeyHold(this, &Player::onKeyHold);
	Keyboard::connectKeyRelease(this, &Player::onKeyRelease);
}

void Player::onKeyHold(RedBox::KeySignalData data) {
	
}
void Player::onKeyPress(RedBox::KeySignalData data) {

}
void Player::onKeyRelease(RedBox::KeySignalData data) {
	
}

}