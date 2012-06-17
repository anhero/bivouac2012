/**
 * @file
 */
#ifndef BV2K12_PLAYSTATE_H
#define BV2K12_PLAYSTATE_H

#include <RedBox.h>

#include "Button.h"
#include "Bridge.h"
#include <list>
#include <vector>

namespace Bivouac2012 {
    class Bacon;
    class Player;
	/**
	 * 
	 */
	class PlayState : public RedBox::State {
		friend class Player;
		//Hack because of getCamera being const...
		friend class Bridge;
        friend class Bacon;
    public:
        PlayState(const std::string &newName = std::string("PlayState"));
        void update();
        void render();
        std::vector<Player*> getPlayers(){return players;}		
		void stun();

    private:
        
        void createLavaGrill(float x, float y);
        
        
        void onPointerMove(RedBox::PointerButtonSignalData data);
        
        void initPlayers();
		void onKeyHold(RedBox::KeySignalData data);
		void onGetFocus();
		void onLoseFocus();
        void initBridges();
		void initRooms();
        void calculateCollisionButtons();
        void calculateHook();
        void calculateGrabing();
        void syncPlayerStatus();
        void clearBacon();
        void createLava();
		
		void makeTheHud();
        
        
        void initCrack();
        void initGrille();
//        void baconAssplosionAt(RedBox::Vector2 coord, int baconCount);
        
		
		
		int megatimer;
		
        std::vector<Player*> players;
		
        void reset();
        bool _reset = false;
        
		Button * buttons[4];
		Bridge * bridges[4];
		
		RedBox::Sprite * rooms[4];
		
		int _nbPlayers;
		bool _usesGamepads;
        std::list<Bacon*> bacons;
		
		float _screenHeight;

		unsigned int _zRefreshCounter;
		
		
		std::vector<RedBox::Text*> playerScores;
		RedBox::Text *countDown;
        
	};
}

#endif // BV2K12_PLAYSTATE_H
