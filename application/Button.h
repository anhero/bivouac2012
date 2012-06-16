/* 
 * File:   Button.h
 * Author: samuel
 *
 * Created on 15 juin 2012, 04:10
 */

#ifndef BUTTON_H
#define	BUTTON_H

#include <RedBox.h>

namespace Bivouac2012 {
	class Bridge;
class Button : public RedBox::Sprite {
public:
	Button(RedBox::Vector2 pos);
	
	//Connects the bridges to the button
	void connectBridges(Bridge *bridge1, Bridge *bridge2);
	
	//Activates a bridge. The bridge itself decides how to handle that "signal".
	void activate();
private:
	Bridge *_bridges[2];
};
}

#endif	/* BUTTON_H */

