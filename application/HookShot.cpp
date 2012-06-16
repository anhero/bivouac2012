//
//  File.cpp
//  RedBoxApp
//
//  Created by Philippe Dubé-Tremblay on 12-06-15.
//  Copyright (c) 2012 Anhero Inc. All rights reserved.
//

#include "HookShot.h"

#include "Player.h"

using namespace RedBox;

namespace Bivouac2012 {

    
    HookShot::HookShot(const std::string& hook, const std::string& chain, const Player* myCrazyFuckUser): 
	_isThrown(false), steps(0), _nbChains(10), _hookDelay(0.2), _targetId(-1), _hookedPlayer(false)
	{
        _timer.stop();
        _hook = new Sprite(hook);
        _myOwner = myCrazyFuckUser;
        
        for (int i = 0; i < _nbChains; ++i) {
            _chains.push_back(new Sprite(chain));
        }
    }
    
    void HookShot::update()
    {
        if (steps == _nbChains || _hookedPlayer) {
            _isThrown = false;
            if (!_timer.isStarted()) {
                _timer.start();
            }
        }
        if (steps != 0 || _isThrown) {
            setPosition( (_destinationHook - (_destinationHook - _myOwner->getPositionCenter())/_nbChains*(_nbChains-steps)));
            for (int i=0; i< _chains.size(); ++i) {
                _chains[i]->setPosition(_myOwner->getPositionCenter() + (_hook->getPositionCenter() - _myOwner->getPositionCenter())/_chains.size()*i - _chains[i]->getSize()/2);
                _chains[i]->update();
            }
            _hook->update();
            
            if (_isThrown){
                ++steps;
            }else if(_timer.getTime() > _hookDelay) {
                --steps;
                _timer.pause();
            }
                
        }else {
            _timer.stop();
            _hookedPlayer = false;
            _grabedPlayer = true;
            
        }
    }
    void HookShot::render(){
        if (_isThrown || steps != 0) {
            
            for (int i=0; i< _chains.size(); ++i) {
                _chains[i]->render();
            }
            _hook->render();
        }
    }
    void HookShot::throwGraplin(float facing){
		Vector2 temp;

        temp = Vector2(1,0);
        temp.setAngle(facing);
        //L'endroit où le hook ira.
        _destinationHook = _myOwner->getPosition() + (2*_nbChains*temp*_chains[0]->getWidth());
        _hook->setAngle(facing);
        _isThrown = true;
        steps = 1;
        update();
    }
    
    Vector2 HookShot::getPosition(){
        return _hook->getPositionCenter();
    }
    
    void HookShot::setPosition(RedBox::Vector2 newPos){
        _hook->setPosition( newPos - _hook->getSize()/2);
        _hook->update();
        for (int i=0; i< _chains.size(); ++i) {
            _chains[i]->setPosition(_myOwner->getPositionCenter() + (_hook->getPositionCenter() - _myOwner->getPositionCenter())/_chains.size()*i - _chains[i]->getSize()/2);
            _chains[i]->update();
        }
    }
    
    void HookShot::grab(int playerId){
        _isThrown = false;
        _hookedPlayer = true;
        _targetId = playerId;
    }
}
