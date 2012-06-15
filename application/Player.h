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

namespace Bivouac2012 {

class Player : public RedBox::Sprite, public sigly::HasSlots<> {
public:
	Player(PlayState *parentState);
	Player(const std::string& image, PlayState *parentState);
	
	void onKeyPress(RedBox::KeySignalData data);
	void onKeyHold(RedBox::KeySignalData data);
	void onKeyRelease(RedBox::KeySignalData data);
	
	void update();
private:
	PlayState *_parentState;
};

}

#endif	/* PLAYER_H */

