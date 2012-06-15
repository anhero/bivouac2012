/* 
 * File:   Bridge.h
 * Author: samuel
 *
 * Created on 15 juin 2012, 05:01
 */

#ifndef BRIDGE_H
#define	BRIDGE_H

#include <RedBox.h>
using namespace RedBox;
namespace Bivouac2012 {

class Bridge : public RedBox::Sprite  {
public:
	Bridge(Vector2 pos, bool horizontal);
	
	void update();
	
	void render();
	
	///Sets position for the middle of the bridge
	void setPosition(const Vector2& newPosition);
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

private:
	bool _horizontal;
	Stopwatch _timer;
	
	bool _retracting;
	bool _retracted;
	bool _activating;
	
	//0 for closed, 1 for retracted.
	float _retractedRatio;
	
	//Parts of the bridge
	Sprite *part1;
	Sprite *part2;
};
}

#endif	/* BRIDGE_H */

