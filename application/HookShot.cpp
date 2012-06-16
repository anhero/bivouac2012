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
    
    HookShot::HookShot() : _nbChains(10) {
        
    }
    
    HookShot::HookShot(const std::string& hook, const std::string& chain, const Player* myCrazyFuckUser): _isThrown(false), steps(0), _nbChains(10){
        _hook = new Sprite(hook);
        _myOwner = myCrazyFuckUser;
        
        for (int i = 0; i < _nbChains; ++i) {
            _chains.push_back(new Sprite(chain));
        }
    }
    
    void HookShot::update()
    {
        if (steps == _nbChains) {
            _isThrown = false;
        }
        if (steps != 0 || _isThrown) {
            _hook->setPosition( _targetHook - (_targetHook - _myOwner->getPosition())/_nbChains*(_nbChains-steps));
            std::cout << _hook->getPositionCenter() << std::endl;
            for (int i=0; i< _chains.size(); ++i) {
                _chains[i]->setPosition(_myOwner->getPositionCenter() + (_hook->getPositionCenter() - _myOwner->getPositionCenter())/_chains.size()*i - Vector2(_chains[i]->getWidth()/2,_chains[i]->getHeight()/2));
                _chains[i]->update();
            }
            _hook->update();
            _isThrown ? ++steps : --steps;
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
        _targetHook = _myOwner->getPosition() + (2*_nbChains*temp*_chains[0]->getWidth());
        _hook->setAngle(facing);
        _isThrown = true;
        steps = 1;
        update();
    }
    Vector2 HookShot::getPosition(){
        return _hook->getPositionCenter();
    }
}
