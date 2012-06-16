/**
 * @file
 */
#ifndef BU_BIVOUACSPRITE_H
#define BU_BIVOUACSPRITE_H

#include "PlayState.h"

namespace Bivouac2012 {
	/**
	 * 
	 */
	class BivouacSprite : public RedBox::Sprite {
    public:
        BivouacSprite(const std::string& image, PlayState *parentState);
        void update();
        void flick();
    protected:
        
        virtual void finishedFlicking();
        PlayState *_parentState;

        virtual void collisionsAndShits();
        
        int flickCount;
        int flickFrequencyCount;
        bool isFLicking;

	};
}

#endif // BU_BIVOUACSPRITE_H