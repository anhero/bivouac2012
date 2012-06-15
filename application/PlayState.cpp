#include "PlayState.h"
#include "Player.h"

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
        ResourceManager::loadTextureRelativePath("player", "Player.png");
		Keyboard::connectKeyRelease(this, &PlayState::onKeyRelease);
		Keyboard::connectKeyPress(this, &PlayState::onKeyPress);
		Keyboard::connectKeyHold(this, &PlayState::onKeyHold);
        setBackgroundColor(Color::WHITE);
        initPlayers(1);
		Keyboard::connectKeyRelease(this, &PlayState::onKeyPress);
        initBridges();
		
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
    void PlayState::initBridges(){
    
        //Loop for buttons creation
		for (int i=0; i < 4; i++) {
			int x = 0;
			int y = 0;
            
			//X position of the button
			if (i == BUTTON_TOP_LEFT || i == BUTTON_BOTTOM_LEFT) {
				x = 0 + BUTTON_OFFSET_FROM_SCREEN;
			}
			else {
				x = WIDTH - BUTTON_OFFSET_FROM_SCREEN;
			}
			//Y position of the button
			if (i == BUTTON_TOP_LEFT || i == BUTTON_TOP_RIGHT) {
				y = 0 + BUTTON_OFFSET_FROM_SCREEN;
			}
			else {
				y = HEIGHT - BUTTON_OFFSET_FROM_SCREEN;
			}
			
			//Creating the button
			Button *btn = new Button(Vector2(0,0));
			//Aligning and centering
			btn->setXPosition(x - btn->getWidth()/2);
			btn->setYPosition(y - btn->getHeight()/2);
			
			//Adding to the buttons array
			buttons[i] = btn;
			
			add(btn);
		}
		//Loop for bridges creation
		for (int i=0; i < 4; i++) {
			
			//Should it be an horizontal bridge?
			bool horiz = true;
			if (i == BRIDGE_LEFT || i == BRIDGE_RIGHT) {
				horiz = false;
			}
			
			//Create the bridge
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
			//Center it
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
			//Center it
			y -= bridge->getHeight() / 2;
			
			//Move it
			bridge->setPosition(x,y);
            
			//Adding to the bridges array
			bridges[i] = bridge;
			
			add(bridge);
		}
		
		//Bridges connection loop
		for (int i = 0; i < 4; i++) {
			if (i == BUTTON_TOP_LEFT) {
				buttons[i]->connectBridges(bridges[BRIDGE_TOP], bridges[BRIDGE_LEFT]);
			}
			else if (i == BUTTON_TOP_RIGHT) {
				buttons[i]->connectBridges(bridges[BRIDGE_TOP], bridges[BRIDGE_RIGHT]);
			}
			else if (i == BUTTON_BOTTOM_LEFT) {
				buttons[i]->connectBridges(bridges[BRIDGE_BOTTOM], bridges[BRIDGE_LEFT]);
			}
			else {
				buttons[i]->connectBridges(bridges[BRIDGE_BOTTOM], bridges[BRIDGE_RIGHT]);
			}
		}
    }
}
