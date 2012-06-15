/* 
 * File:   Player.cpp
 * Author: samuel
 * 
 * Created on 15 juin 2012, 03:52
 */

#include "Player.h"
#include "HookShot.h"
using namespace RedBox;

namespace Bivouac2012 {
    
Player::Player() : Sprite() {

}

Player::Player(const std::string& image) : Sprite(image),isFacing(UP) {
    _hook = new HookShot("hook","chain", this);
}
Player::~Player(){
    delete _hook;
}

void Player::onKeyHold(KeySignalData data) {
    switch (data.key) {
        case Key::W:
            move(Vector2(0,-2));
            isFacing = UP;
            break;
        case Key::A:
            move(Vector2(-2,0));
            isFacing = LEFT;
            break;
        case Key::S:
            move(Vector2(0,2));
            isFacing = DOWN;
            break;
        case Key::D:
            move(Vector2(2,0));
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

void Player::update(){
    Sprite::update();
    _hook->update();
}

}