/**
 * @file
 */
#ifndef BV2K12_PLAYSTATE_H
#define BV2K12_PLAYSTATE_H

#include <RedBox.h>


namespace Bivouac2012 {
    class Player;
	/**
	 * 
	 */
	class PlayState : public RedBox::State {
    public:
        PlayState(const std::string &newName = std::string("PlayState"));
        void update();
        void render();

    private:
        void initPlayers(int nbPlayers);
		void onKeyRelease(RedBox::KeySignalData data);
		void onKeyPress(RedBox::KeySignalData data);
        void onKeyHold(RedBox::KeySignalData data);
		void onGetFocus();
		void onLoseFocus();
        std::vector<Player*> players;
	};
}

#endif // BV2K12_PLAYSTATE_H
