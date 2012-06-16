/* 
 * File:   HookShot.h
 * Author: Philippe
 *
 * Created on 15 juin 2012, 05:01
 */

#ifndef HOOKSHOT_H
#define	HOOKSHOT_H

#include <RedBox.h>


namespace Bivouac2012 {
    class Player;
    class HookShot  {
    public:
        HookShot(const std::string& hook, const std::string& chain, const Player* myCrazyFuckUser);
        void throwGraplin(float facing);
        void update();
        void render();
        bool isThrown(){return _isThrown;}
        void setThrown(bool newThrown){ _isThrown = newThrown;}
        bool grabedPlayer(){return _grabedPlayer;}
        int getGrabed(){return _playerGrabed;}
        void grab(int playerId);
        RedBox::Vector2 getPosition();
        void setPosition(RedBox::Vector2 newPos);
    private:
        const int _nbChains;
        const float _hookDelay;     
        RedBox::Stopwatch _timer;
        bool _isThrown;
        bool _grabedPlayer;
        int _playerGrabed;
        RedBox::Vector2 _targetHook;
        RedBox::Sprite* _hook;
        int steps;
        std::vector<RedBox::Sprite*> _chains;
        const Player* _myOwner;
        
    };
}

#endif	/* HOOKSHOT_H */

