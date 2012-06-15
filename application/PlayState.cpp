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

static const int WIDTH  = 900;
static const int HEIGHT = 900;
static const int BRIDGE_OFFSET_FROM_SCREEN = 200;

static const int ROOM_OFFSET_FROM_EDGE_OF_SCREEN = 0;

	PlayState::PlayState(const std::string &newName) : State(newName) {
		Keyboard::connectKeyRelease(this, &PlayState::onKeyRelease);
		Keyboard::connectKeyPress(this, &PlayState::onKeyPress);
		Keyboard::connectKeyHold(this, &PlayState::onKeyHold);
        setBackgroundColor(Color::WHITE);
        camera.setScaling(Vector2(0.88,0.88));
        initPlayers(1);
        initBridges();
	}

	void PlayState::update() {
        calculateCollisionButtons();
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
            players.push_back(new Player("player", this));
            players.back()->setZ(50);
            add(players.back());
            players.back()->setPosition(Vector2(100,450));
        }
    }
    
	void PlayState::onKeyPress( KeySignalData data) {
        players[0]->onKeyPress(data);
		
		if (data.key == Key::SPACE) {
			buttons[1]->activate();
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
			bridge->setScaling(Vector2(2,1));
			bridge->setPosition(x,y);
            
			//Adding to the bridges array
			bridges[i] = bridge;
			
			add(bridge);
		}
		
		initRooms();

        //Loop for buttons creation
		for (int i=0; i < 4; i++) {
			
            buttons[i] = new Button(Vector2(0,0));
            buttons[i]->setPosition(rooms[i]->getPositionCenter() - Vector2(buttons[i]->getWidth()/2,buttons[i]->getHeight()/2));
			add(buttons[i]);
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
		room = new Sprite("floor");
		room->setPosition(camera.screenToWorld(Vector2(ROOM_OFFSET_FROM_EDGE_OF_SCREEN, ROOM_OFFSET_FROM_EDGE_OF_SCREEN)));
		add(room);
		rooms[0] = room;
		room = new Sprite("floor");
		room->setPosition(camera.screenToWorld(Vector2(WIDTH - ROOM_OFFSET_FROM_EDGE_OF_SCREEN, ROOM_OFFSET_FROM_EDGE_OF_SCREEN)) - Vector2(room->getWidth(),0));
		add(room);
		rooms[1] = room;
		room = new Sprite("floor");
		room->setPosition(camera.screenToWorld(Vector2(ROOM_OFFSET_FROM_EDGE_OF_SCREEN, HEIGHT - ROOM_OFFSET_FROM_EDGE_OF_SCREEN)) - Vector2(0,room->getHeight()));
		add(room);
		rooms[2] = room;
		room = new Sprite("floor");
		room->setPosition(camera.screenToWorld(Vector2(WIDTH - ROOM_OFFSET_FROM_EDGE_OF_SCREEN, HEIGHT - ROOM_OFFSET_FROM_EDGE_OF_SCREEN)) -Vector2(room->getWidth(), room->getHeight()));
		add(room);
		rooms[3] = room;
	}
}
