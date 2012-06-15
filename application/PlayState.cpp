#include "PlayState.h"

using namespace RedBox;

namespace Bivouac2012 {
	PlayState::PlayState(const std::string &newName) : State(newName) {
		Keyboard::connectKeyRelease(this, &PlayState::onKeyRelease);
		Keyboard::connectKeyRelease(this, &PlayState::onKeyPress);
		
		for (int i=0; i < 4; i++) {
			//TODO Properly define offsets for the buttons...
			Button *btn = new Button(Vector2( (i%2 + 1) * 150 , (i/2 + 1) * 125 ));
			buttons[i] = btn;
			add(btn);
		}
	}

	void PlayState::update() {
	}

	void PlayState::onKeyRelease(RedBox::KeySignalData data) {
		if (data.key == Key::ESCAPE) {
			RedBox::Engine::exitApplication(0);
		}
	}

	void PlayState::onKeyPress(RedBox::KeySignalData data) {
	}

	void PlayState::onGetFocus() {
		MainWindow::getInstance().hideCursor();
	}
	
	void PlayState::onLoseFocus() {
	}
}
