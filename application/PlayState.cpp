#include "PlayState.h"

using namespace RedBox;

namespace Bivouac2012 {
static const int BUTTON_TOP_LEFT     = 0;
static const int BUTTON_TOP_RIGHT    = 1;
static const int BUTTON_BOTTOM_LEFT  = 2;
static const int BUTTON_BOTTOM_RIGHT = 3;
static const int BUTTON_OFFSET_FROM_SCREEN = 130;

static const int BRIDGE_TOP    = 0;
static const int BRIDGE_LEFT   = 1;
static const int BRIDGE_RIGHT  = 2;
static const int BRIDGE_BOTTOM = 3;

static const int WIDTH  = 800;
static const int HEIGHT = 600;
static const int BRIDGE_OFFSET_FROM_SCREEN = 130;

	PlayState::PlayState(const std::string &newName) : State(newName) {
		Keyboard::connectKeyRelease(this, &PlayState::onKeyRelease);
		Keyboard::connectKeyRelease(this, &PlayState::onKeyPress);
		
		
		for (int i=0; i < 4; i++) {
			//TODO Properly define offsets for the buttons...
			//Button *btn = new Button(Vector2( (i%2 + 1) * 150 , (i/2 + 1) * 125 ));
			int x = 0;
			int y = 0;

			//X
			if (i == BUTTON_TOP_LEFT || i == BUTTON_BOTTOM_LEFT) {
				x = 0 + BUTTON_OFFSET_FROM_SCREEN;
			}
			else {
				x = WIDTH - BUTTON_OFFSET_FROM_SCREEN;
			}
			//Y
			if (i == BUTTON_TOP_LEFT || i == BUTTON_TOP_RIGHT) {
				y = 0 + BUTTON_OFFSET_FROM_SCREEN;
			}
			else {
				y = HEIGHT - BUTTON_OFFSET_FROM_SCREEN;
			}
				
				
			Button *btn = new Button(Vector2(0,0));
			btn->setXPosition(x - btn->getWidth()/2);
			btn->setYPosition(y - btn->getHeight()/2);
			buttons[i] = btn;
			add(btn);
		}
		for (int i=0; i < 4; i++) {
			bool horiz = true;
			if (i == BRIDGE_LEFT || i == BRIDGE_RIGHT) {
				horiz = false;
			}
			Bridge *bridge = new Bridge(Vector2(0,0),horiz);
			
			
			int x = 0;
			int y = 0;
			// X pos branches
			if (i == BRIDGE_LEFT) {
				x = BRIDGE_OFFSET_FROM_SCREEN;
			}
			else if (i == BRIDGE_RIGHT) {
				x = WIDTH - BRIDGE_OFFSET_FROM_SCREEN;
			}
			else {
				x = WIDTH / 2;
			}
			x -= bridge->getWidth() / 2;
			
			// Y pos branches
			if (i == BRIDGE_TOP) {
				y = BRIDGE_OFFSET_FROM_SCREEN;
			}
			else if (i == BRIDGE_BOTTOM) {
				y = HEIGHT - BRIDGE_OFFSET_FROM_SCREEN;
			}
			else {
				y = HEIGHT / 2;
			}
			y -= bridge->getHeight() / 2;
			
			bridge->setPosition(x,y);
			
			add(bridge);
			bridges[i] = bridge;
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
