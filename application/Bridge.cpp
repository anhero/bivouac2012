/* 
 * File:   Bridge.cpp
 * Author: samuel
 * 
 * Created on 15 juin 2012, 05:02
 */

#include "Bridge.h"

namespace Bivouac2012 {
	
Bridge::Bridge(Vector2 pos, bool horizontal) : Sprite("bridge", pos) {
	if (horizontal) {
		rotate(90);
	}
}

void Bridge::activate() {
	//TODO: reset the timer
}

}