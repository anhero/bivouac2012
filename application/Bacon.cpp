#include "Bacon.h"

#define BACON_TIMER_LIMIT 150
using namespace RedBox;
namespace Bivouac2012 {
    Bacon::Bacon(RedBox::Vector2 coord, PlayState *parentState) : BivouacSprite("BaconSheet", Vector2(35,67), Vector2(), 5,parentState), startedFlicking(false){
		
		addAnimation("bobuludaule", 0.06, -1, 8, 0,1,2,3,4,3,2,1);
		startAnimation("bobuludaule");
//		setDefaultFrame(4);
        _parentState = parentState;
        deleteTimer = 0;
        setPosition(coord);
		flick();
    }
    
    void Bacon::update(){
        BivouacSprite::update();
        
        
        collisionsAndShits();
//        deleteTimer++;
//        
//        if(deleteTimer > BACON_TIMER_LIMIT){
//            this->setToBeDeleted(true);
//        }
//        else if(deleteTimer > BACON_TIMER_LIMIT /2 && !startedFlicking ){
//            flick();
//            startedFlicking = true;
//        }
    }
    
    
    void Bacon::collisionsAndShits() {
		float oldX = 0;
        float oldY = 0;
        oldX = this->getOldXPosition() + this->getWidth()/2;
        oldY = this->getOldYPosition() + this->getHeight()/2;
        
        Sprite * last_room = NULL;
        //We check in which room the player was, if applicable
        for (int i=0; i < 4; i++) {
            Sprite * room = _parentState->rooms[i];
            if (   (oldX > room->getXPosition() && oldX < room->getXPosition() + room->getWidth() )
                && (oldY > room->getYPosition() && oldY < room->getYPosition() + room->getHeight() )
                ) {
                last_room = room;
            }
        }
        Bridge * last_bridge = NULL;
        //We check on which bridge the player was, if applicable.
        for (int i=0; i < 4; i++) {
            Bridge * bridge = _parentState->bridges[i];
            //Check if the player was on a bridge, at the old position of the bridge.
            if (bridge->checkIsOnBridge(Vector2(oldX, oldY),true)) {
                last_bridge = bridge;
            }
        }
        
        //If we're on a bridge
        if (last_bridge != NULL) {
            //And not in a room
            if (last_room == NULL) {
                Sprite* bridgePart = last_bridge->getPartAtPoint(this->getPositionCenter());
                if (bridgePart != NULL) {
                    this->move(bridgePart->getPosition() - last_bridge->getOldPositionForPart(bridgePart));
                }
				else {
					bridgePart = last_bridge->getPartAtPoint(Vector2(oldX, oldY), true);
					if (bridgePart != NULL) {
						float last_bridge_x = last_bridge->getXPosition() - last_bridge->getWidth()/2;
						float last_bridge_y = last_bridge->getYPosition() - last_bridge->getHeight()/2;
						
						//LEFT EDGE
						if (this->getXPositionCenter() <= last_bridge_x) {
							this->setXPosition(oldX - this->getWidth()/2);
							this->setXVelocity(this->getXVelocity() * -1);
						}
						//RIGHT EDGE
						else if (this->getXPositionCenter() >= last_bridge_x + last_bridge->getWidth()) {
							this->setXPosition(oldX - this->getWidth()/2);
							this->setXVelocity(this->getXVelocity() * -1);
						}
						//TOP EDGE
						if (this->getYPositionCenter() <= last_bridge_y) {
							this->setYPosition(oldY - this->getHeight()/2);
							this->setYVelocity(this->getYVelocity() * -1);
						}
						//BOTTOM EDGE
						else if (this->getYPositionCenter() >= last_bridge_y + last_bridge->getHeight()) {
							this->setYPosition(oldY - this->getHeight()/2);
							this->setYVelocity(this->getYVelocity() * -1);
						}
					}
					else {
						std::cout << "[BACON] YOU ARE NOT ON A BRIDGE AND WERE NOT ON A BRIDGE  OMGWTFBBQ" << std::endl;
					}
				}
            }
        }
        //If the player was in a room, we check the boundaries
        else if (last_room != NULL) {
            //LEFT EDGE
            if (this->getXPositionCenter() <= last_room->getXPosition()) {
                this->setXPosition(oldX - this->getWidth()/2);
                this->setXVelocity(this->getXVelocity() * -1);
            }
            //RIGHT EDGE
            else if (this->getXPositionCenter() >= last_room->getXPosition() + last_room->getWidth()) {
                this->setXPosition(oldX - this->getWidth()/2);
                this->setXVelocity(this->getXVelocity() * -1);
            }
            //TOP EDGE
            if (this->getYPositionCenter() <= last_room->getYPosition()) {
                this->setYPosition(oldY - this->getHeight()/2);
                this->setYVelocity(this->getYVelocity() * -1);
            }
            //BOTTOM EDGE
            else if (this->getYPositionCenter() >= last_room->getYPosition() + last_room->getHeight()) {
                this->setYPosition(oldY - this->getHeight()/2);
                this->setYVelocity(this->getYVelocity() * -1);
            }
        }
        if (last_room == NULL && last_bridge == NULL) {
            //std::cout << "BACON BE OUT" << std::endl;
        }
        else {
            //std::cout << "YOU SAFE" << std::endl;
        }
        
    }

	int Bacon::_maxFlicks() {
		return 3;
	}
}