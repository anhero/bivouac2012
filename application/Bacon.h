/**
 * @file
 */
#ifndef BU_BACON_H
#define BU_BACON_H



#include <RedBox.h>
#include "PlayState.h"

#include "BivouacSprite.h"
namespace Bivouac2012 {
	/**
	 * 
	 */
	class Bacon : public BivouacSprite {
    public:
        Bacon(RedBox::Vector2 coord, PlayState *parentState);
        
        void update();
		
	protected:
		int _maxFlicks();
		
    private:
        bool startedFlicking;
        PlayState *_parentState;

        int deleteTimer;
        
        void baconAssplosion();
        void collisionsAndShits();
        
	};
}

#endif // BU_BACON_H