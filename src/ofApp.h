#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp
{
public:
	void setup();
	void update();
	void draw();
	

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	ofEasyCam cam;
	ofLight light;
	ofxPanel gui;

	ofxIntSlider intSlider1;
	ofxIntSlider intSlider2;
	ofxLabel label;
	ofxLabel fps_label;
};

class body
{
public:
	//Constructor and destructor
	body(int x, int y, int z, double mass_, int hue);//Initial coordinates and hue
	~body();

	void update(int body_num);
	void draw();

	void acceleration_calculate(int body_num);

	ofColor color;
	double size;
	double mass;
	glm::vec3 position, velocity, acceleration; // 3D vectors for particles physics
	vector<glm::vec3> trail;
};
