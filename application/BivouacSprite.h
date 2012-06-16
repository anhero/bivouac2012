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
    protected:
        PlayState *_parentState;

        virtual void collisionsAndShits();

	};
}

#endif // BU_BIVOUACSPRITE_H