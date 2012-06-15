/* 
 * File:   Bridge.cpp
 * Author: samuel
 * 
 * Created on 15 juin 2012, 05:02
 */

#include "Bridge.h"
using namespace RedBox;
namespace Bivouac2012 {
	
	static const float ACTIVATING_RATIO = 0.2;
	static const float RETRACTING_RATIO = 0.05;
	
	static const float RETRACTING_DELAY = 0.0;
	
	static const int STICK_OUT = 14;
	
Bridge::Bridge(Vector2 pos, bool horizontal) : Sprite(), 
		_retracted(false), _retracting(true), 
		_horizontal(horizontal), _retractedRatio(0.0), _activating(false) {
	_timer.stop();
	
	part1 = new Sprite("bridge");
            part2 = new Sprite("bridge");
            part1->setScaling(Vector2(1,1.5));
            part2->setScaling(Vector2(1,1.5));
            part2->scale(Vector2(-1,-1));
            part1->rotate(-90);
            part2->rotate(-90);
	
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
	if (!_activating && !_retracted && _timer.getTime() > RETRACTING_DELAY) {
		startRetracting();
	}
	if (_retracting) {
		if (_retractedRatio > 1.0) {
			stopRetracting();
			_retractedRatio = 1.0;
		}
		else {
			_retractedRatio += RETRACTING_RATIO;
		}
	}
	
	//Updating the position according to the factor.
	if (_retracting || _activating) {
		if (_horizontal) {
			part1->setXPosition((-1 * (part1->getWidth()-STICK_OUT) * _retractedRatio - part1->getWidth()) + this->getXPosition());
			part2->setXPosition(( 1 * (part2->getWidth()-STICK_OUT) * _retractedRatio                    ) + this->getXPosition());
		}
		else {
			part1->setYPosition((-1 * (part1->getHeight()-STICK_OUT) * _retractedRatio - part1->getHeight()) + this->getYPosition());
			part2->setYPosition(( 1 * (part2->getHeight()-STICK_OUT) * _retractedRatio                     ) + this->getYPosition());
		}
	}
	if (_activating) {
		if (_timer.getTime() > 0.2) {
			_timer.addToTime(-0.2);
		}
		_retractedRatio -= ACTIVATING_RATIO;
		if (_retractedRatio < 0.0) {
			_retractedRatio = 0.0;
			_activating = false;
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

bool Bridge::checkIsOnBridge(Vector2 point, bool oldPos) {
	//I add a padding for floating point innaccuracies
	static const float padding = 1.0f;
	
	if (oldPos) {
		if (
			point.x > part1->getOldXPosition()-padding && point.x < part1->getOldXPosition()+part1->getWidth()+padding
		&&	point.y > part1->getOldYPosition()-padding && point.y < part1->getOldYPosition()+part1->getHeight()+padding
		) {
			return true;
		}
		if (
			point.x > part2->getOldXPosition()-padding && point.x < part2->getOldXPosition()+part2->getWidth()+padding
		&&	point.y > part2->getOldYPosition()-padding && point.y < part2->getOldYPosition()+part2->getHeight()+padding
		) {
			return true;
		}
	}
	else {
		if (
			point.x > part1->getXPosition() && point.x < part1->getXPosition()+part1->getWidth()
		&&	point.y > part1->getYPosition() && point.y < part1->getYPosition()+part1->getHeight()
		) {
			return true;
		}
		if (
			point.x > part2->getXPosition() && point.x < part2->getXPosition()+part2->getWidth()
		&&	point.y > part2->getYPosition() && point.y < part2->getYPosition()+part2->getHeight()
		) {
			return true;
		}
	}
	return false;
}

}