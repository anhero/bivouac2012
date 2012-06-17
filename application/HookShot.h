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
        bool hookedPlayer(){return _hookedPlayer;}
        bool grabedPlayer(){return _grabedPlayer;}
        int getTargetId(){return _targetId;}
        void releasePlayer();
        void grab(int playerId);
        void grabedshacle();
        RedBox::Vector2 getPosition();
        void setPosition(RedBox::Vector2 newPos);
    private:
        const int _nbChains;
        const float _hookDelay; 
        int _grabedToughtess;
        RedBox::Stopwatch _timer;
        bool _isThrown;
        bool _hookedPlayer;
        bool _grabedPlayer;
        int _targetId;
        RedBox::Vector2 _destinationHook;
        RedBox::Sprite* _hook;
        int steps;
        void shackle();
        std::vector<RedBox::Sprite*> _chains;
        const Player* _myOwner;
        
    };
}

#endif	/* HOOKSHOT_H */

