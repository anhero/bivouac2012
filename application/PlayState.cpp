#include "PlayState.h"

using namespace RedBox;

namespace Bivouac2012 {
	PlayState::PlayState(const std::string &newName) : State(newName) {
	}

	void PlayState::update() {
	}

	void PlayState::onKeyRelease(RedBox::KeySignalData data) {
		if (data.key == Key::ESCAPE) {
			//TODO
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
