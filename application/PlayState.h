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
	/**
	 * 
	 */
	class PlayState : public RedBox::State {
    public:
        PlayState(const std::string &newName = std::string("PlayState"));
        void update();

    private:
		void onKeyRelease(RedBox::KeySignalData data);
		void onKeyPress(RedBox::KeySignalData data);
		void onGetFocus();
		void onLoseFocus();
		
		Button * buttons[4];
		Bridge * bridges[4];
	};
}

#endif // BV2K12_PLAYSTATE_H
