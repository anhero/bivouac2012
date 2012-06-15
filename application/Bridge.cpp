/* 
 * File:   Bridge.cpp
 * Author: samuel
 * 
 * Created on 15 juin 2012, 05:02
 */

#include "Bridge.h"

#include <ios>

namespace Bivouac2012 {
	
	static const float ACTIVATING_RATIO = 0.2;
	static const float RETRACTING_RATIO = 0.05;
	
Bridge::Bridge(Vector2 pos, bool horizontal) : Sprite(), 
		_retracted(true), _retracting(false), 
		_horizontal(horizontal), _retractedRatio(1.0), _activating(false) {
	_timer.stop();
	
	part1 = new Sprite("bridge_p1");
	part2 = new Sprite("bridge_p2");
	
	if (_horizontal) {
		rotate(90);
	}

}

void Bridge::activate() {
	if (!_timer.isStarted() || _retracting) {
		_timer.start();
	}
	_retracted = false;
	_retracting = false;
	//This goes in the update...
//	_retractedRatio = 0.0;
	
	_activating = true;
}

void Bridge::stopRetracting() {
	_timer.stop();
	_retracted = true;
	_retracting = false;
	_retractedRatio = 1.0;
	_activating = false;
}
void Bridge::startRetracting() {
	_retracting = true;
	_activating = false;
}

//TODO: Intelligent tweening instead of hack...
void Bridge::update() {
	Sprite::update();
	
	//When the timer is over a certain time, we begin to retract the bridge.
	if (!_retracted && _timer.getTime() > 0.4) {
		startRetracting();
	}
	if (_retracting) {
		if (_retractedRatio > 1.0) {
			stopRetracting();
		}
		else {
			_retractedRatio += RETRACTING_RATIO;
		}
	}
	if (_activating) {
		if (_timer.getTime() > 0.2) {
			_timer.addToTime(-0.2);
		}
		_retractedRatio -= ACTIVATING_RATIO;
		if (_retractedRatio <= 0.0) {
			_retractedRatio = 0.0;
			_activating = false;
		}
	}
	
	//Updating the position according to the factor.
	if (_retracting || _activating) {
		if (_horizontal) {
			part1->setXPosition((-1 * (part1->getWidth()-10) * _retractedRatio - part1->getWidth()) + this->getXPosition());
			part2->setXPosition(( 1 * (part2->getWidth()-10) * _retractedRatio                    ) + this->getXPosition());
		}
		else {
			part1->setYPosition((-1 * (part1->getHeight()-10) * _retractedRatio - part1->getHeight()) + this->getYPosition());
			part2->setYPosition(( 1 * (part2->getHeight()-10) * _retractedRatio                     ) + this->getYPosition());
		}
	}
	
	part1->update();
	part2->update();
}

float Bridge::getHeight() {
	if ( part1->getYPosition() < part2->getYPosition() ) {
		return part2->getYPosition() - part1->getYPosition() + part2->getHeight();
	}
	else {
		return part1->getYPosition() - part2->getYPosition() + part1->getHeight();
	}
}
float Bridge::getWidth() {
	if ( part1->getXPosition() < part2->getXPosition() ) {
		return part2->getXPosition() - part1->getXPosition() + part2->getWidth();
	}
	else {
		return part1->getXPosition() - part2->getXPosition() + part1->getWidth();
	}
}

void Bridge::setPosition(const Vector2& newPosition) {
	setPosition(newPosition.x, newPosition.y);
}

void Bridge::setPosition(float newXPosition, float newYPosition) {
	Sprite::setPosition(newXPosition, newYPosition);
	
	part1->setXPosition(newXPosition);
	part1->setYPosition(newYPosition);
	
	part2->setXPosition(newXPosition);
	part2->setYPosition(newYPosition);
	
	if (_horizontal) {
		part1->moveX(-1 * part1->getWidth());
		
		part1->moveY(-1 * part1->getHeight()/2);
		part2->moveY(-1 * part2->getHeight()/2);
	}
	else {
		part1->moveY(-1 * part1->getHeight());
		
		part1->moveX(-1 * part1->getWidth()/2);
		part2->moveX(-1 * part2->getWidth()/2);
	}
}

void Bridge::rotate(float angleToAdd) {
	part1->rotate(angleToAdd);
	part2->rotate(angleToAdd);
}

void Bridge::render() {
	part1->render();
	part2->render();
}

}