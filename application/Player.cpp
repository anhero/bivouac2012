/* 
 * File:   Player.cpp
 * Author: samuel
 * 
 * Created on 15 juin 2012, 03:52
 */

#include "Player.h"
using namespace RedBox;

namespace Bivouac2012 {
    
Player::Player() : Sprite() {

}

Player::Player(const std::string& image) : Sprite(image) {
    
}

void Player::onKeyHold(KeySignalData data) {
    switch (data.key) {
        case Key::W:
            move(Vector2(0,-2));
            break;
        case Key::A:
            move(Vector2(-2,0));
            break;
        case Key::S:
            move(Vector2(0,2));
            break;
        case Key::D:
            move(Vector2(2,0));
            break;
        default:
            break;
    }
}
void Player::onKeyPress(KeySignalData data) {

}
void Player::onKeyRelease(KeySignalData data) {
	
}

}