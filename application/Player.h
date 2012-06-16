/* 
 * File:   Player.h
 * Author: samuel
 *
 * Created on 15 juin 2012, 03:52
 */

#ifndef PLAYER_H
#define	PLAYER_H

#include <RedBox.h>

#include "PlayState.h"

#include "BivouacSprite.h"
namespace Bivouac2012 {
    class HookShot;

    class Player : public BivouacSprite, public sigly::HasSlots<> {
public:
	Player(const std::string& image, PlayState *parentState, int id);
	~Player();
	void onKeyPress(RedBox::KeySignalData data);
	void onKeyHold(RedBox::KeySignalData data);
	void onKeyRelease(RedBox::KeySignalData data);
	
	//void onThumbstickMove(RedBox::GamePadThumbstickSignalData data);
	void onButtonHold(RedBox::GamePadButtonSignalData data);
	void onButtonPress(RedBox::GamePadButtonSignalData data);
	
    void render();
	void update();
    void baconAssplosion();
private:
    HookShot* _hook;
	//Angle at which the player is facing.
    float facingAngle;
	
	int _playerID;
	
    
	void thumbStickMovements();
	void collisionsAndShits();
    
};

}

#endif	/* PLAYER_H */

