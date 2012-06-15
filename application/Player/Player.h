/* 
 * File:   Player.h
 * Author: samuel
 *
 * Created on 15 juin 2012, 03:52
 */

#ifndef PLAYER_H
#define	PLAYER_H

#include <RedBox.h>

namespace Bivouac2012 {

class Player : RedBox::Sprite, public sigly::HasSlots<> {
public:
	Player();
	
	void onKeyPress(RedBox::KeySignalData data);
	void onKeyHold(RedBox::KeySignalData data);
	void onKeyRelease(RedBox::KeySignalData data);
private:

};

}

#endif	/* PLAYER_H */

