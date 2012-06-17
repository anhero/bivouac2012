#include "PlayState.h"
#include "Player.h"

#include "Bacon.h"
#include <math.h>


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

//Of the game area
static const int WIDTH  = 1100;
static const int HEIGHT = 1100;
static const int BRIDGE_OFFSET_FROM_SCREEN = 200;

static const int ROOM_OFFSET_FROM_EDGE_OF_SCREEN = 0;
static const int ROOM_BACKGROUND_OFFSET_FROM_EDGE_OF_SCREEN = -75;

	PlayState::PlayState(const std::string &newName) : State(newName),
	_nbPlayers(0), _usesGamepads(true), _zRefreshCounter(0), megatimer( 60 *48) {
		Keyboard::connectKeyHold(this, &PlayState::onKeyHold);
        setBackgroundColor(Color(0, 0, 0));
		
		
		
		
        
//        Pointer::connectButtonPress(this, &PlayState::onPointerMove);
        
		//camera.setScaling(768.0f/1100.0f,768.0f/1100.0f);
		//camera.setScaling(0.5,0.5);
        initBridges();
        initPlayers();
        initGrille();
        initCrack();
        Text* myFont = new Text("font");
        myFont->setText("0123456789");
		myFont->setColor(Color::YELLOW);
        add(myFont);
        myFont->setPosition(250,250);
		
		Sprite *hud = new Sprite("hud");
		hud->setPosition(1000,20);
		add(hud);
        
		
        
	}

    void PlayState::onPointerMove(RedBox::PointerButtonSignalData data){
        std::cout << data.getPosition() << std::endl;
    }

    
    void PlayState::initCrack(){
//        Sprite * crack1;
//        Sprite * crack2;
//        Sprite * crack3;
//        
//        crack1 = new Sprite("crack_1");
//        add(crack1);
//        crack1->setPosition(264, -36);
//        
//        crack2 = new Sprite("crack_2");
//        crack2->setPosition(-36, 264);
//        add(crack2);
//
//        crack3 = new Sprite("crack_3");
//        crack3->setPosition(264, 264);
//        add(crack3);

    }
    
    void PlayState::createLavaGrill(float x, float y){
        Sprite * lavamodel = new Sprite("lava",Vector2(x,y), Vector2(107,107), Vector2(), 6);
        add(lavamodel);
        lavamodel->addAnimation("glow", 0.2, -1, 10, 0,1,2,3,4,5,4,3,2,1);
        lavamodel->startAnimation("glow");
        
        Sprite * grille = new Sprite("grille", Vector2(x - 14, y -14));
//        grille->setPosition(lava->getPosition()-Vector2(14,14));
        add(grille);
    }
      
    
    void PlayState::initGrille(){
        
        
        //plateforme1
        
        createLavaGrill(20, 20);
        
        
        //plateforme2
        createLavaGrill(950, 20);
//        createLavaGrill
        
        
        //plateforme3
        createLavaGrill(315, 665);
        
        
        //plateforme 4
        createLavaGrill(955, 665);
        createLavaGrill(955, 960);
        createLavaGrill(665, 665);

        
        
    }
    void PlayState::reset(){
        clearBacon();
        initPlayers();
    }
	void PlayState::update() {
        if (!_reset) {
        calculateCollisionButtons();
        calculateHook();
		
		if (_zRefreshCounter == 0) {
			for (int i = 0; i < players.size(); ++i) {
				players[i]->setZ(players[i]->getYPosition()+200);
				if (players[i]->_state == CARRIED) {
					players[i]->setZ(players[i]->getZ() + 92);
				}
			}
			_zRefreshCounter = 4;
		}
		_zRefreshCounter--;
        }else{
            reset();
        }
		
		megatimer--;
		if (megatimer <=0) {
			gameOver();
		}
		
		
		
	}
    void PlayState::render() {
    }
	//TODO: Remove debug "SPACE" key...?
	void PlayState::onKeyHold( KeySignalData data) {
		if (data.key == Key::V) {
            players[0]->stun();
		}
		if (players.size() > 1 && data.key == Key::B) {
            players[1]->stun();
		}
		if (players.size() > 2 && data.key == Key::N) {
            players[2]->stun();
		}
		if (players.size() > 3 && data.key == Key::M) {
            players[3]->stun();
		}
		
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
        if (data.key == Key::BACKSPACE) {
            clearBacon();

        }
		if(data.key == Key::ESCAPE){
			RedBox::Engine::exitApplication(0);
		}
        
		if (data.key == Key::ENTER) {
            for (int i = 0; i<_nbPlayers; ++i) {
                players[i]->setToBeDeleted(true);
            }
            for (int i = 0; i<_nbPlayers; ++i) {
                players.pop_back();
            }
			_reset = true;
		}
		if (data.key == Key::G) {
            gameOver();
		}
	}
    
    void PlayState::clearBacon(){
        for (std::list<Bacon*>::iterator i = bacons.begin(); i != bacons.end(); ) {
            (*i)->setToBeDeleted(true);
            i = bacons.erase(i);
            i++;
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
            players.push_back(new Player(this, i));
            players.back()->setZ(50);
            add(players.back());
			
			//TODO: Position players in the middle of the platforms.
//            players.back()->setPosition(Vector2(100,650));
        }
        _reset = false;
    }
    
//    void PlayState::baconAssplosionAt(RedBox::Vector2 coord, int baconCount){
//        for (int i = 0; i < baconCount; i++) {
//            Bacon * bacon = new Bacon(coord, this);
//            Vector2 baconVelocity;
//            baconVelocity.x =1;
//            baconVelocity.setAngle(Random::getRandomInteger(0, 360));
//            baconVelocity.normalize();
//            baconVelocity *= Random::getRandomFloat(MIN_BACON_VELOCITY, MAX_BACON_VELOCITY);
//            bacon->setVelocity(baconVelocity);
//            bacon->setGlobalDrag(30);
//            add(bacon);
//        }
//    }

	void PlayState::onGetFocus() {
//		MainWindow::getInstance().hideCursor();
	}
	
	void PlayState::onLoseFocus() {
	}
    
    void PlayState::calculateCollisionButtons(){
		//HACK
		const int button_deadzone = 40;
		bool activated[4] = {false,false,false,false};
        for (int i=0; i<players.size(); ++i) {
			
            for (int j=0; j<4; ++j) {
				if (!activated[j]) {
					if (((players[i]->getCollisionPosition() - buttons[j]->getCentroid()).getLength() + button_deadzone) < buttons[j]->getWidth()/2) {
						activated[j] = true;
						buttons[j]->activate();
					}
					else {
						buttons[j]->disengage();
					}
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

			bridge->_parentState = this;
			
			add(bridge);
			
			for (int i=0; i < Bridge::NB_EMITTERS; i++) {
				add(bridge->_emitters[i]);
			}
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
	room->setPosition((Vector2(ROOM_OFFSET_FROM_EDGE_OF_SCREEN, ROOM_OFFSET_FROM_EDGE_OF_SCREEN)));
	add(room);
	rooms[0] = room;
	room = new Sprite("floor");
	room->setPosition((Vector2(WIDTH - ROOM_OFFSET_FROM_EDGE_OF_SCREEN, ROOM_OFFSET_FROM_EDGE_OF_SCREEN)) - Vector2(room->getWidth(), 0));
	add(room);
	rooms[1] = room;
	room = new Sprite("floor");
	room->setPosition((Vector2(ROOM_OFFSET_FROM_EDGE_OF_SCREEN, HEIGHT - ROOM_OFFSET_FROM_EDGE_OF_SCREEN)) - Vector2(0, room->getHeight()));
	add(room);
	rooms[2] = room;
	room = new Sprite("floor");
	room->setPosition((Vector2(WIDTH - ROOM_OFFSET_FROM_EDGE_OF_SCREEN, HEIGHT - ROOM_OFFSET_FROM_EDGE_OF_SCREEN)) - Vector2(room->getWidth(), room->getHeight()));
	add(room);
	rooms[3] = room;

	//Add the background to the rooms.
	room = new Sprite("floor_V2(2)");
	room->setZ(-150);
	room->setPosition((Vector2(ROOM_BACKGROUND_OFFSET_FROM_EDGE_OF_SCREEN, ROOM_BACKGROUND_OFFSET_FROM_EDGE_OF_SCREEN)));
	add(room);

	room = new Sprite("floor_V2(2)");
	room->setZ(-150);
	room->setPosition((Vector2(WIDTH - ROOM_BACKGROUND_OFFSET_FROM_EDGE_OF_SCREEN, ROOM_BACKGROUND_OFFSET_FROM_EDGE_OF_SCREEN)) - Vector2(room->getWidth(), 0));
	add(room);

	room = new Sprite("floor_V2(2)");
	room->setZ(-150);
	room->setPosition((Vector2(ROOM_BACKGROUND_OFFSET_FROM_EDGE_OF_SCREEN, HEIGHT - ROOM_BACKGROUND_OFFSET_FROM_EDGE_OF_SCREEN)) - Vector2(0, room->getHeight()));
	add(room);

	room = new Sprite("floor_V2(2)");
	room->setZ(-150);
	room->setPosition((Vector2(WIDTH - ROOM_BACKGROUND_OFFSET_FROM_EDGE_OF_SCREEN, HEIGHT - ROOM_BACKGROUND_OFFSET_FROM_EDGE_OF_SCREEN)) - Vector2(room->getWidth(), room->getHeight()));
	add(room);
}
    void PlayState::calculateHook()
    {
        for (int i=0; i<_nbPlayers; ++i) {
            //Verify the grabing conditions.
            HookShot* currentHook = players[i]->getHook();
            if (currentHook->isThrown()) {
                //verify the collision with other players
                for (int j=0; j<_nbPlayers; ++j) {
                    if (players[i] != players[j] && (currentHook->getPosition() - players[j]->getPositionCenter()).getLength() < 30 &&
                        !players[j]->getIsFlicking()) {
                        
                        currentHook->grab(j);
                    }
                }
                //if there is a player hooked drags him with the hook
            }else if (currentHook->hookedPlayer()){
                players[currentHook->getTargetId()]->setPosition(currentHook->getPosition()- players[0]->getSize()/2);
            }
            players[i]->setState(MOBILE);
        }
        syncPlayerStatus();
    }
    void PlayState::syncPlayerStatus(){
        for (int i=0; i<_nbPlayers; ++i) {
            if(players[i]->getHook()->hookedPlayer()){
                players[players[i]->getHook()->getTargetId()]->setState(HOOKED);
            }
        }
        for (int i=0; i<_nbPlayers; ++i) {
            if(players[i]->getHook()->grabedPlayer()){
                players[players[i]->getHook()->getTargetId()]->setState(CARRIED);
                players[players[i]->getHook()->getTargetId()]->setGraber(players[i]);
            }
        }
    }
    void PlayState::gameOver(){
        Sprite* pixel = SpriteFactory::makePolygon(4, 1, Color::BLACK);
        pixel->scale(2000, 2000);
        pixel->setZ(500);
        pixel->setPosition(Vector2(-50,-50));
        add(pixel);
        for (int i=0; i<_nbPlayers; ++i) {
            
            Text* myFont = new Text("font");
            myFont->setZ(510);
            myFont->setText("player" + Parser::intToString(i+1) + " " + Parser::intToString(players[i]->baconCount));
            myFont->setColor(Color::YELLOW);
            add(myFont);
            myFont->setPosition(250,100*i + 100);
            players[i]->setAlpha(0);
        }
        
    }
}
