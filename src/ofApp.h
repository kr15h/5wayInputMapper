#pragma once

#include "ofMain.h"
#include "ofxPiMapper.h"

enum Direction {
	LEFT = 10,
	RIGHT = 9,
	UP = 2,
	DOWN = 3,
	CENTER = 8,
	SEPARATOR = 32
};

class ofApp : public ofBaseApp{
public:
	void setup();
	void update();
	void draw();
	
	std::string inputToString(int direction, int state);
	void inputToMapper(int direction, int state);
	ofSerial serial;
	vector<int> buffer;
	ofxPiMapper mapper;
	
	float timer;
};
