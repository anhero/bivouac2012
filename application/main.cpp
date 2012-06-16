#include <RedBox.h>
#include "PlayState.h"

#define TEX(s) RedBox::ResourceManager::loadTextureRelativePath(s, s ".png");

//TODO: Transfer in a "initState"
void initResources() {
	TEX("button");
	TEX("button_on");
	TEX("button_off");
	TEX("bridge");
	TEX("pierre_1");
	TEX("pierre_2");
	TEX("pierre_3");
	
	TEX("pedantic_smoke_1");
	
	TEX("floor");
	TEX("floor_V2(2)")
    TEX("spritesheet_players_eyes");
    TEX("hook0");
    TEX("chain0");
    TEX("hook1");
    TEX("chain1");
    TEX("hook2");
    TEX("chain2");
    TEX("hook3");
    TEX("chain3");
    TEX("bacon_glow");
    TEX("bacon_strip");
    TEX("grille");
    TEX("lava_square");
    
    
    TEX("crack_1");
    TEX("crack_2");
    TEX("crack_3");

    
}

int main(int argc, char *argv[]) {
	// Initialize RedBox
	RedBox::Engine::application(argc, argv, "Bivouac2012");

	// We set the number of updates per second to be executed.
	RedBox::Engine::setUpdatesPerSecond(48);

	// We set the minimum number of frames per second that are
	// tolerated before sacrificing on the number of updates per second.
	RedBox::Engine::setMinFps(5);

	// We initialize the engine with a screen resolution.
	RedBox::Engine::initializeEngine(900, 900);

	initResources();

	RedBox::Engine::addState(new Bivouac2012::PlayState());
	RedBox::Engine::playState("PlayState");
	
	// Then start everything.
	RedBox::Engine::showMainWindow();

	return 0;
}


