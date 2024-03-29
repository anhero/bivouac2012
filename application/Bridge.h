/* 
 * File:   Bridge.h
 * Author: samuel
 *
 * Created on 15 juin 2012, 05:01
 */

#ifndef BRIDGE_H
#define	BRIDGE_H

#include <RedBox.h>

namespace Bivouac2012 {
	
	class PlayState;

class Bridge : public RedBox::Sprite  {
	friend class PlayState;
public:
	static const int NB_EMITTERS = 4;
	
	Bridge(RedBox::Vector2 pos, bool horizontal);
	
	void update();
	
	void render();
	
	///Sets position for the middle of the bridge
	void setPosition(const RedBox::Vector2& newPosition);
	///Sets position for the middle of the bridge
	void setPosition(float newXPosition, float newYPosition);
	
	void rotate(float angleToAdd);
	
	float getWidth();
	float getHeight();
	
	/* Bridge movements */
	
	///To be called when the bridge is completely retracted
	void stopRetracting();
	///To be called when the bridge is titillated
	//This resets the timer on the bridge.
	void activate();
	///To be called when the bridge is to be retracted
	void startRetracting();
	///To be called to do the effects when the bridge is closed.
	void finishClosing();
	
	///Checks if the point is on a part of the bridge.
	bool checkIsOnBridge(RedBox::Vector2 point, bool oldPos = false);
	///Gets the part (Sprite) of the bridge at a certain point. Can be in the past.
	RedBox::Sprite *getPartAtPoint(RedBox::Vector2 point, bool oldPos = false);
	
	RedBox::Vector2 getOldPositionForPart(RedBox::Sprite * part);

private:
	bool _horizontal;
	RedBox::Stopwatch _timer;
	
	bool _retracting;
	bool _retracted;
	bool _activating;
	bool _finisedClosing;
	
	//0 for closed, 1 for retracted.
	float _retractedRatio;
	
	//Parts of the bridge
	RedBox::Sprite *part1;
	RedBox::Sprite *part2;
	
	RedBox::Vector2 oldPositions[2];
	
	RedBox::SpriteEmitter *_emitters[NB_EMITTERS];
	
	PlayState *_parentState;
};
}

#endif	/* BRIDGE_H */

