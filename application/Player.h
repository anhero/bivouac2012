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
#include "HookShot.h"

#include "BivouacSprite.h"
namespace Bivouac2012 {

    class Player : public BivouacSprite, public sigly::HasSlots<> {
public:
	Player(PlayState *parentState, int id);
	~Player();
	void onKeyPress(RedBox::KeySignalData data);
	void onKeyHold(RedBox::KeySignalData data);
	void onKeyRelease(RedBox::KeySignalData data);
	
	//void onThumbstickMove(RedBox::GamePadThumbstickSignalData data);
	void onButtonHold(RedBox::GamePadButtonSignalData data);
	void onButtonPress(RedBox::GamePadButtonSignalData data);
	HookShot* getHook(){return _hook;}
    void render();
	void update();
    bool isMobile(){return _isMobile;}
    void setMobile(bool newHooked){ _isMobile = newHooked;}
    void baconAssplosion();
	
	static const std::string ANIMATIONS[9];
	
private:
    HookShot* _hook;
	//Angle at which the player is facing.
    float facingAngle;
	
	int _playerID;
    bool _isMobile;
	
    
	void thumbStickMovements();
	void collisionsAndShits();
    
};

}

#endif	/* PLAYER_H */

