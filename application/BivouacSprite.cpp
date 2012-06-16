#include "BivouacSprite.h"

using namespace RedBox;
namespace Bivouac2012 {
    BivouacSprite::BivouacSprite(const std::string& image, PlayState *parentState): Sprite(image),
			isFLicking(false), _parentState(parentState), flickFrequencyCount(0), flickCount(0){}
    BivouacSprite::BivouacSprite(const std::string& image, const Vector2 &newSize, 
			const Vector2 &newTextureOffset, unsigned int nbFrames, PlayState *parentState)
			: Sprite(image, Vector2(), newSize, newTextureOffset, nbFrames),
			isFLicking(false), _parentState(parentState), flickFrequencyCount(0), flickCount(0){}

    
    void BivouacSprite::flick(){
        isFLicking = true;
        flickFrequencyCount = 0;
        flickCount =0;
    }
    
    void BivouacSprite::update(){
        Sprite::update();
        
        if(isFLicking){
            if (flickCount/(10) < 10) {
            flickFrequencyCount++;
            if(flickFrequencyCount > 5){
                this->setAlpha(100);
                if(flickFrequencyCount >10){
                    flickFrequencyCount = 0;
                }
            }
            else {
                this->setAlpha(255);
                
            }
            flickCount++;

            }
            else {
                isFLicking = false;
                this->setAlpha(255);
                finishedFlicking();
            }
        }
    }
    
    void BivouacSprite::finishedFlicking(){}
    
    void BivouacSprite::collisionsAndShits(){}
}