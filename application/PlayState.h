/**
 * @file
 */
#ifndef BV2K12_PLAYSTATE_H
#define BV2K12_PLAYSTATE_H

#include <RedBox.h>

#include "Button.h"
#include "Bridge.h"

#include <vector>

namespace Bivouac2012 {
    class Player;
	/**
	 * 
	 */
	class PlayState : public RedBox::State {
		friend class Player;
    public:
        PlayState(const std::string &newName = std::string("PlayState"));
        void update();
        void render();

    private:
        void initPlayers();
		void onKeyHold(RedBox::KeySignalData data);
		void onGetFocus();
		void onLoseFocus();
        void initBridges();
		void initRooms();
        void calculateCollisionButtons();
        
        std::vector<Player*> players;
		
		Button * buttons[4];
		Bridge * bridges[4];
		
		RedBox::Sprite * rooms[4];
		
		int _nbPlayers;
		bool _usesGamepads;
	};
}

#endif // BV2K12_PLAYSTATE_H
