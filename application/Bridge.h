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

class Bridge : public RedBox::Sprite  {
public:
	Bridge(RedBox::Vector2 pos, bool horizontal);
	
	//This resets the timer on the bridge.
	void activate();
	
private:
	bool _horizontal;
};
}

#endif	/* BRIDGE_H */

