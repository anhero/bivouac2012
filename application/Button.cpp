/* 
 * File:   Button.cpp
 * Author: samuel
 * 
 * Created on 15 juin 2012, 04:10
 */

#include "Button.h"

#include "Bridge.h"

using namespace RedBox;

namespace Bivouac2012 {
	
Button::Button(Vector2 pos) : Sprite("button", pos, Vector2(223,224), Vector2(), 2) {
}

void Button::connectBridges(Bridge* bridge1, Bridge* bridge2) {
	_bridges[0] = bridge1;
	_bridges[1] = bridge2;
//    isOn = false;
    setDefaultFrame(1);
}

void Button::activate() {

//    isOn = true;
	for (int i=0; i < 2; i++) {
		_bridges[i]->activate();
	}
    
    if(getDefaultFrame() != 1){
        setDefaultFrame(1);
    }
}
    
    void Button::disengage(){
        if(getDefaultFrame() != 0){
            setDefaultFrame(0);
        }
    }
    
    
}
