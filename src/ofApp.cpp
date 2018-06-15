#include "ofApp.h"

void ofApp::setup(){
	
	// Setup serial connection.
	serial.setup(1, 9600);
	serial.flush();
	
	// Setup WebSockets
	ofxLibwebsockets::ServerOptions options = ofxLibwebsockets::defaultServerOptions();
    options.port = 9092;
	options.bUseSSL = false;
    webSocketsSetup = webSocketsServer.setup( options );
    webSocketsServer.addListener(this);
	
	// Configure the background to be black and set up mapper.
	ofBackground(0);
	mapper.setup();
	
	timer = 0.0f;
}

void ofApp::update(){
	mapper.update();
	
	float now = ofGetElapsedTimef();
	if(now > timer + 5.0f){
		timer = now;
		mapper.deselect();
		mapper.setMode(ofx::piMapper::Mode::PRESENTATION_MODE);
		mapper.saveProject();
	}

	int numSerialBytes = serial.available();
	if(numSerialBytes){
		for(auto i = 0; i < numSerialBytes; ++i){
			buffer.push_back(serial.readByte());
		}
		serial.flush();
	}
	
	if(buffer.size() > 2){
		for(auto i = 0; i < buffer.size(); ++i){
		
			// Check for separator byte
			if(buffer[i] == SEPARATOR){
				
				// There must be two more bytes before
				if(i > 1){
					int direction = buffer[i-2];
					int state = buffer[i-1];
					std::cout << inputToString(direction, state) << std::endl;
					inputToMapper(direction, state);
					
					// Delete interpreted bytes from the buffer
					// Separator byte, direction and state bytes
					buffer.erase(buffer.begin(), buffer.begin() + i + 1);
					i = 0;
				}else{
					
					// If there are less than two bytes before,
					// flush them.
					buffer.erase(buffer.begin(), buffer.begin() + i + 1);
					i = 0;
				}
			}
		}
	}
}

void ofApp::draw(){
	mapper.draw();
}

void ofApp::inputToMapper(int direction, int state){
	if(state == 1){
		timer = ofGetElapsedTimef();
		mapper.setMode(ofx::piMapper::Mode::MAPPING_MODE);
		mapper.selectNextSurface();
	
		if(direction == LEFT){
			mapper.moveSelection(ofx::piMapper::Vec3(-5, 0, 0));
		}
		
		if(direction == RIGHT){
			mapper.moveSelection(ofx::piMapper::Vec3(5, 0, 0));
		}
		
		if(direction == UP){
			mapper.moveSelection(ofx::piMapper::Vec3(0, -5, 0));
		}
		
		if(direction == DOWN){
			mapper.moveSelection(ofx::piMapper::Vec3(0, 5, 0));
		}
		
		if(direction == CENTER){
			mapper.selectNextVertex();
		}
	}
}

std::string ofApp::inputToString(int direction, int state){
	string dirStr;
	string steStr;
	
	if(direction == LEFT){
		dirStr = "LEFT";
	}
	
	if(direction == RIGHT){
		dirStr = "RIGHT";
	}
	
	if(direction == UP){
		dirStr = "UP";
	}
	
	if(direction == DOWN){
		dirStr = "DOWN";
	}
	
	if(direction == CENTER){
		dirStr = "CENTER";
	}
	
	if(state == 0){
		steStr = "RELEASED";
	}
	
	if(state == 1){
		steStr = "PRESSED";
	}
	
	return dirStr + " " + steStr;
}

void ofApp::onConnect( ofxLibwebsockets::Event & args ){
	std::cout << "onConnect" << std::endl;
}

void ofApp::onOpen( ofxLibwebsockets::Event & args ){
    std::cout << "onOpen" << std::endl;
    //messages.push_back(
    //	"New connection from " + args.conn.getClientIP() +
    //	", " + args.conn.getClientName() );
}

void ofApp::onClose( ofxLibwebsockets::Event & args ){
    std::cout << "onClose" << std::endl;
    //messages.push_back("Connection closed");
}

void ofApp::onIdle( ofxLibwebsockets::Event& args ){
    std::cout << "onIdle" << std::endl;
}

void ofApp::onMessage( ofxLibwebsockets::Event & args ){
    std::cout << "onMessage: " << args.message << std::endl;
	
    // trace out string messages or JSON messages!
	/*
    if ( !args.json.isNull() ){
        messages.push_back("New message: " + args.json.toStyledString() + " from " + args.conn.getClientName() );
    } else {
        messages.push_back("New message: " + args.message + " from " + args.conn.getClientName() );
    }
    */
	
    // echo server = send message right back!
    //args.conn.send( args.message );
}

void ofApp::onBroadcast( ofxLibwebsockets::Event & args ){
    std::cout << "onBroadcast: " << args.message << std::endl;
}
