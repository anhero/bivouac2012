#include "PlayState.h"

using namespace RedBox;

namespace Bivouac2012 {
	PlayState::PlayState(const std::string &newName) : State(newName) {
		Keyboard::connectKeyRelease(this, &PlayState::onKeyRelease);
		Keyboard::connectKeyRelease(this, &PlayState::onKeyPress);
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
