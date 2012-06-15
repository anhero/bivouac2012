#include "PlayState.h"
#include "Player.h"

using namespace RedBox;

namespace Bivouac2012 {
	PlayState::PlayState(const std::string &newName) : State(newName) {
        ResourceManager::loadTextureRelativePath("player", "Player.png");
		Keyboard::connectKeyRelease(this, &PlayState::onKeyRelease);
		Keyboard::connectKeyPress(this, &PlayState::onKeyPress);
		Keyboard::connectKeyHold(this, &PlayState::onKeyHold);
        setBackgroundColor(Color::WHITE);
        initPlayers(1);
	}

	void PlayState::update() {
	}
    void PlayState::render() {
    }
    
	void PlayState::onKeyRelease( KeySignalData data) {
		players[0]->onKeyRelease(data);
	}
    
	void PlayState::onKeyHold( KeySignalData data) {
		players[0]->onKeyHold(data);
	}
    
    void PlayState::initPlayers(int nbPlayers){
        for (int i = 0; i < nbPlayers; ++i) {
            players.push_back(new Player("player"));
            add(players.back());
            players.back()->setPosition(Vector2(100,100));
        }
    }
    
	void PlayState::onKeyPress( KeySignalData data) {
        players[0]->onKeyPress(data);
	}

	void PlayState::onGetFocus() {
		MainWindow::getInstance().hideCursor();
	}
	
	void PlayState::onLoseFocus() {
	}
}
