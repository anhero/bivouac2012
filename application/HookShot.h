/* 
 * File:   HookShot.h
 * Author: Philippe
 *
 * Created on 15 juin 2012, 05:01
 */

#ifndef HOOKSHOT_H
#define	HOOKSHOT_H

#include <RedBox.h>
#include "Player.h"


namespace Bivouac2012 {
    
    class HookShot  {
    public:
        HookShot();
        HookShot(const std::string& hook, const std::string& chain, const Player* myCrazyFuckUser);
        void throwGraplin(float facing);
        void update();
        void render();
        
    private:
        const int _nbChains = 10;
        bool _isThrown;
        RedBox::Vector2 _targetHook;
        RedBox::Sprite* _hook;
        int steps;
        std::vector<RedBox::Sprite*> _chains;
        const Player* _myOwner;
        
    };
}

#endif	/* HOOKSHOT_H */

