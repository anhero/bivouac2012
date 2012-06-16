/* 
 * File:   Bridge.cpp
 * Author: samuel
 * 
 * Created on 15 juin 2012, 05:02
 */

#include "Bridge.h"

#include "PlayState.h"

using namespace RedBox;
namespace Bivouac2012 {
	
	static const float ACTIVATING_RATIO = 0.1;
	static const float RETRACTING_RATIO = 0.03;
	
	static const float RETRACTING_DELAY = 0.4;
	
	static const int STICK_OUT = 14;
	static const int STICK_IN = 5;

Bridge::Bridge(Vector2 pos, bool horizontal) : Sprite(),
_retracted(false), _retracting(true),
_horizontal(horizontal), _retractedRatio(0.0), _activating(false),
_finisedClosing(false) {
	_timer.stop();

	part1 = new Sprite("bridge");
	part2 = new Sprite("bridge");
	part1->setScaling(Vector2(1, 1.5));
	part2->setScaling(Vector2(1, 1.5));
	part2->scale(Vector2(-1, -1));
	part1->rotate(-90);
	part2->rotate(-90);

	for (int i=0; i < NB_EMITTERS; i++) {
		RedBox::SpriteEmitter *emitter;
		GraphicElement<Collidable> *particle;
		switch (i%NB_EMITTERS) {
			case 0:
				particle = new GraphicElement<Collidable > ("pierre_1");
				break;
			case 1:
				particle = new GraphicElement<Collidable > ("pierre_2");
				break;
			case 2:
				particle = new GraphicElement<Collidable > ("pedantic_smoke_1");
				break;
			default:
				particle = new GraphicElement<Collidable > ("pierre_3");
				break;

		}
		if (_horizontal) {
			if (i >= NB_EMITTERS / 2) {
				particle->setYAcceleration(300);
			}
			else {
				particle->setYAcceleration(130);
			}
		}
		else {
			particle->setYAcceleration(300);
		}
		emitter = SpriteFactory::makeExplosion(particle, 10, 200, Vector2(0,0));
		
		emitter->setZ(99);
		
		if (_horizontal) {
			if (i >= NB_EMITTERS / 2) {
				emitter->setShootingAngle(0.0f);
				emitter->setShootingAngleVariance(10.0f);
				emitter->setShootingForceVariance(140.0f);
			}
			else {
				emitter->setShootingAngle(0.0f);
				emitter->setShootingAngleVariance(20.0f);
				emitter->setShootingForce(100);
				emitter->setShootingForceVariance(140.0f);
			}
		}
		else {
			emitter->setShootingAngle(0);
			emitter->setShootingAngleVariance(60);
			emitter->setShootingForceVariance(100);
		}
		emitter->getPhases().push_back(ParticlePhase(0.3, 0.4, 0.0f, Vector2(), 180.0f, 0.0f, Vector2(), 90.0f));
		
		_emitters[i] = emitter;
	}

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
	_finisedClosing = false;
}

void Bridge::finishClosing() {
		for (int i=0; i < NB_EMITTERS; i++) {
			_emitters[i]->start();
		}
		_finisedClosing = true;
/*		_parentState->getCamera().shake(0.5,0.2,false//,
//			(_horizontal ? Camera::ShakeAxes::HORIZONTAL_AXIS : Camera::ShakeAxes::VERTICAL_AXIS)
		);*/
		_parentState->camera.shake(0.02,0.4,true,
			(_horizontal ? Camera::ShakeAxes::HORIZONTAL_AXIS : Camera::ShakeAxes::VERTICAL_AXIS)
		);
}

//TODO: Intelligent tweening instead of hack...
void Bridge::update() {
	Sprite::update();
	oldPositions[0] = part1->getPosition();
	oldPositions[1] = part2->getPosition();

	//When the timer is over a certain time, we begin to retract the bridge.
	if (!_activating && !_retracted && _timer.getTime() > RETRACTING_DELAY) {
		startRetracting();
	}
	if (!_finisedClosing && _retractedRatio <= 0.0 && !_retracting) {
		finishClosing();
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
			part1->setXPosition((-1 * (part1->getWidth()-STICK_OUT) * _retractedRatio - part1->getWidth()) + this->getXPosition() + STICK_IN);
			part2->setXPosition(( 1 * (part2->getWidth()-STICK_OUT) * _retractedRatio                    ) + this->getXPosition() - STICK_IN);
		}
		else {
			part1->setYPosition((-1 * (part1->getHeight()-STICK_OUT) * _retractedRatio - part1->getHeight()) + this->getYPosition() + STICK_IN);
			part2->setYPosition(( 1 * (part2->getHeight()-STICK_OUT) * _retractedRatio                     ) + this->getYPosition() - STICK_IN);
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
	
	//_emitter->update();
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
	
		for (int i=0; i < NB_EMITTERS; i++) {
			if (_horizontal) {
				_emitters[i]->setPosition(newXPosition + 4.0*(i-1.5), newYPosition);
			}
			else {
				_emitters[i]->setPosition(newXPosition, newYPosition + 14.0*(i-1.5));
			}
		}
}

void Bridge::rotate(float angleToAdd) {
	part1->rotate(angleToAdd);
	part2->rotate(angleToAdd);

	for (int i = 0; i < NB_EMITTERS; i++) {
		_emitters[i]->rotate(angleToAdd);
	}
}

void Bridge::render() {
	part1->render();
	part2->render();
	
	//_emitter->render();
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

Sprite *Bridge::getPartAtPoint(RedBox::Vector2 point, bool oldPos) {
		//I add a padding for floating point innaccuracies
	static const float padding = 1.0f;
	
	if (oldPos) {
		if (
			point.x > part1->getOldXPosition()-padding && point.x < part1->getOldXPosition()+part1->getWidth()+padding
		&&	point.y > part1->getOldYPosition()-padding && point.y < part1->getOldYPosition()+part1->getHeight()+padding
		) {
			return part1;
		}
		if (
			point.x > part2->getOldXPosition()-padding && point.x < part2->getOldXPosition()+part2->getWidth()+padding
		&&	point.y > part2->getOldYPosition()-padding && point.y < part2->getOldYPosition()+part2->getHeight()+padding
		) {
			return part2;
		}
	}
	else {
		if (
			point.x > part1->getXPosition() && point.x < part1->getXPosition()+part1->getWidth()
		&&	point.y > part1->getYPosition() && point.y < part1->getYPosition()+part1->getHeight()
		) {
			return part1;
		}
		if (
			point.x > part2->getXPosition() && point.x < part2->getXPosition()+part2->getWidth()
		&&	point.y > part2->getYPosition() && point.y < part2->getYPosition()+part2->getHeight()
		) {
			return part2;
		}
	}
	return NULL;
}

Vector2 Bridge::getOldPositionForPart(Sprite* part) {
	if (part == part1) {
		return oldPositions[0];
	}
	return oldPositions[1];
}

}