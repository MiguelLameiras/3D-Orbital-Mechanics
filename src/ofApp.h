#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxDatGui.h"

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

	void onButtonEvent(ofxDatGuiButtonEvent e);
	void onToggleEvent(ofxDatGuiToggleEvent e);
	void onSliderEvent(ofxDatGuiSliderEvent e);
	void onDropdownEvent(ofxDatGuiDropdownEvent e);
	void onColorPickerEvent(ofxDatGuiColorPickerEvent e);

	uint tIndex;

	bool start;
	bool show_trails;
	bool show_axis;
	bool show_center;
	ofEasyCam cam;
	ofLight light;
	ofxDatGui *UI;

	glm::vec3 center_of_mass; // center of mass position
	double Total_mass;
	double proximity;
	int star_relative_mass;

	int Temp_Num_Planets;
	int Temp_Num_Asteroids;
	int Temp_Num_Stars;
};

class body
{
public:
	// Constructor and destructor
	body(int x, int y, int z, double mass_, int hue); // Initial coordinates and hue
	~body();

	void update(int body_num, bool show_trails);
	void draw();

	void acceleration_calculate(int body_num);

	ofColor color;
	double size;
	double mass;
	double abs_velocity;
	glm::vec3 position, velocity, acceleration; // 3D vectors for particles physics
	vector<glm::vec3> trail;
};
