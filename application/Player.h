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
    enum PlayerState{
        MOBILE,
        IMMUNE,
        HOOKED,
        CARRIED,
        STUNTED
    };
    class Player : public BivouacSprite, public sigly::HasSlots<> {
		friend class PlayState;
public:
	Player(PlayState *parentState, int id);
	~Player();
	void onKeyPress(RedBox::KeySignalData data);
	void onKeyHold(RedBox::KeySignalData data);
	void onKeyRelease(RedBox::KeySignalData data);
	
	//void onThumbstickMove(RedBox::GamePadThumbstickSignalData data);
	void onButtonHold(RedBox::GamePadButtonSignalData data);
	void onButtonPress(RedBox::GamePadButtonSignalData data);
        HookShot* getHook();
    void render();
	void update();
    bool isMobile(){return _state <= IMMUNE;}
    void setState(PlayerState newState){ _state = newState;}
    void setGraber(Player* graber);
    void baconAssplosion();
    
    void refreshGrabed();    
    void resetPosition();
	
	static const std::string ANIMATIONS[9];
	
	RedBox::Vector2 positionToCollisionPoint(RedBox::Vector2 point);
	
	void setYCollisionPosition(float newY);
	void setXCollisionPosition(float newX);
	void setCollisionPosition(RedBox::Vector2 position);
	void setCollisionPosition(float newX, float newY);
	
	float getYCollisionPosition();
	float getXCollisionPosition();
        
    int baconCount;
		
		RedBox::Text * escapeText;
        
	RedBox::Vector2 getCollisionPosition();
	
	void stun();

private:
    HookShot* _hook;
	//Angle at which the player is facing.
    float facingAngle;
	
	int _playerID;
    Player* _graber;
    PlayerState _state;
	
		RedBox::Sprite * buttonHud;
        bool canHarvestBacon;
    
	void thumbStickMovements();
	void collisionsAndShits();
    
    void harvestBacon();    
     
        RedBox::Sprite * debugCircle;    
		
		bool _stunned;
		unsigned int _stunned_counter;
};

}

#endif	/* PLAYER_H */

