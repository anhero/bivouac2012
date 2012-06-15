/* 
 * File:   Button.cpp
 * Author: samuel
 * 
 * Created on 15 juin 2012, 04:10
 */

#include "Button.h"

using namespace RedBox;

namespace Bivouac2012 {
	
Button::Button(Vector2 pos) : Sprite("button", pos) {
}

void Button::connectBridges(Bridge* bridge1, Bridge* bridge2) {
	_bridges[0] = bridge1;
	_bridges[1] = bridge2;
}

void Button::activate() {
	for (int i=0; i < 2; i++) {
		_bridges[i]->activate();
	}
}

}
