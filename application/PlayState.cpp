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

static const int ROOM_OFFSET_FROM_EDGE_OF_SCREEN = 20;

	PlayState::PlayState(const std::string &newName) : State(newName),
	_nbPlayers(0), _usesGamepads(true) {
		Keyboard::connectKeyPress(this, &PlayState::onKeyPress);
        setBackgroundColor(Color::WHITE);
        initPlayers();
        initBridges();
	}

	void PlayState::update() {
        calculateCollisionButtons();
	}
    void PlayState::render() {
    }
	//TODO: Remove debug "SPACE" key...?
	void PlayState::onKeyPress( KeySignalData data) {
		if (data.key == Key::F9) {
			buttons[0]->activate();
		}
		if (data.key == Key::F10) {
			buttons[1]->activate();
		}
		if (data.key == Key::F11) {
			buttons[2]->activate();
		}
		if (data.key == Key::F12) {
			buttons[3]->activate();
		}
		if (data.key == Key::ESCAPE) {
			RedBox::Engine::exitApplication(0);
		}
	}

    void PlayState::initPlayers() {
		//We check the number of available gamepads.
		std::cout << "Initializing players..." << std::endl;
		_nbPlayers = InputManager::getInstance().getNbGamePads();
		if (_nbPlayers > 4) {
			_nbPlayers = 4;
		}
		if (_nbPlayers < 1) {
			_nbPlayers = 1;
			_usesGamepads = false;
		}
		std::cout << "Number of players: " << _nbPlayers << std::endl;
		std::cout << "Uses gamepads: " << (_usesGamepads?"yes":"no") << std::endl;
        for (int i = 0; i < _nbPlayers; ++i) {
			std::cout << "Creating player #" << i+1 << std::endl;
            players.push_back(new Player("player", this, i));
            players.back()->setZ(50);
            add(players.back());
			
			//TODO: Position players in the middle of the platforms.
            players.back()->setPosition(Vector2(100,450));
        }
    }

	void PlayState::onGetFocus() {
		MainWindow::getInstance().hideCursor();
	}
	
	void PlayState::onLoseFocus() {
	}
    
    void PlayState::calculateCollisionButtons(){
        for (int i=0; i<players.size(); ++i) {
            
            for (int j=0; j<4; ++j) {
                if ((players[i]->getCentroid() - buttons[j]->getCentroid()).getLength() < buttons[j]->getWidth()/2) {
                    buttons[j]->activate();
                }
            }
        }
    }
    
    void PlayState::initBridges(){
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
			//x -= bridge->getWidth() / 2;
			
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
			//y -= bridge->getHeight() / 2;
			
			//Move it
			bridge->setPosition(x,y);
            
			//Adding to the bridges array
			bridges[i] = bridge;
			
			add(bridge);
		}
		
		initRooms();

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
	void PlayState::initRooms() {
		//Adds the rooms.
		
		Sprite *room;
		room = new Sprite("pad_top-left");
		room->setPosition(ROOM_OFFSET_FROM_EDGE_OF_SCREEN, ROOM_OFFSET_FROM_EDGE_OF_SCREEN);
		add(room);
		rooms[0] = room;
		room = new Sprite("pad_top-right");
		room->setPosition(WIDTH - ROOM_OFFSET_FROM_EDGE_OF_SCREEN - room->getWidth(), ROOM_OFFSET_FROM_EDGE_OF_SCREEN);
		add(room);
		rooms[1] = room;
		room = new Sprite("pad_bottom-left");
		room->setPosition(ROOM_OFFSET_FROM_EDGE_OF_SCREEN, HEIGHT - ROOM_OFFSET_FROM_EDGE_OF_SCREEN - room->getHeight());
		add(room);
		rooms[2] = room;
		room = new Sprite("pad_bottom-right");
		room->setPosition(WIDTH - ROOM_OFFSET_FROM_EDGE_OF_SCREEN - room->getWidth(), HEIGHT - ROOM_OFFSET_FROM_EDGE_OF_SCREEN - room->getHeight());
		add(room);
		rooms[3] = room;
	}
}
