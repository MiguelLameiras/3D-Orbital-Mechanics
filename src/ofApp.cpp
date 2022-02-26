#include "ofApp.h"

vector<body> bodies;
int Num_Stars;
int Num_Planets;
int Num_Asteroids;
double Initial_Max_Velocity;

body::body(int x, int y, int z, double mass_, int hue)
{
    position = glm::vec3(x, y, z);                                                             // Initial position
    double init = Initial_Max_Velocity;                                                                           // Range of possible initial velocities
    velocity = glm::vec3(ofRandom(-init, init), ofRandom(-init, init), ofRandom(-init, init)); // Initial random velocity
    mass = mass_;
    if (mass < 100)
    {
        size = mass / 10;
    }
    else
    {
        size = 20;
    }
    color.setHsb(hue, 255, 255, 255);
}

body::~body()
{
}

void body::update(int body_num, bool show_trails)
{
    acceleration_calculate(body_num);
    velocity += acceleration;
    abs_velocity = sqrt(velocity[0] * velocity[0] + velocity[1] * velocity[1] + velocity[2] * velocity[2]);
    // std::cout << abs_velocity << std::endl;
    position += velocity;
    if (show_trails)
    {
        trail.push_back(position);
        if (trail.size() > 200)
        {
            trail.erase(trail.begin());
        }
    }
}

void body::draw()
{
    ofSetColor(color);
    ofDrawSphere(position, size);
    ofPolyline trails(trail);
    trails.draw();
}

void body::acceleration_calculate(int body_num)
{
    double ax = 0;
    double ay = 0;
    double az = 0;
    double G = 0.5;

    for (int i = 0; i < 1 + Num_Asteroids + Num_Planets; i++)
    {
        if (i != body_num)
        {

            double r = pow((sqrt(pow((position[0] - bodies[i].position[0]), 2) + pow((position[1] - bodies[i].position[1]), 2) + pow((position[2] - bodies[i].position[2]), 2))), 3);
            ax -= G * bodies[i].mass * (position[0] - bodies[i].position[0]) / r;
            ay -= G * bodies[i].mass * (position[1] - bodies[i].position[1]) / r;
            az -= G * bodies[i].mass * (position[2] - bodies[i].position[2]) / r;
        }
    }
    acceleration[0] = ax;
    acceleration[1] = ay;
    acceleration[2] = az;
}

//--------------------------------------------------------------
void ofApp::setup()
{
    ofSetVerticalSync(true);
    ofSetBackgroundColor(0, 0, 0);
    center_of_mass = glm::vec3(0, 0, 0);
    start = false;
    show_trails = true;
    show_axis = false;
    show_center = false;
    star_relative_mass = 10000;

    Temp_Num_Planets = 15;
    Temp_Num_Asteroids = 15;
    Temp_Num_Stars = 1;
    Initial_Max_Velocity = 4;

    ofEnableLighting();
    light.setPosition(0, 0, 0);

    ofSetSphereResolution(10);

    // instantiate and position the gui //
    UI = new ofxDatGui(ofxDatGuiAnchor::TOP_LEFT);

    UI->addFRM();
    UI->addBreak();

    // add a folder to group a few components together //
    ofxDatGuiFolder *folder = UI->addFolder("Stars", ofColor::orange);
    folder->addSlider("Number of Stars", 0, 5, 1);
    folder->addSlider("Relative Mass", 10000, 100000, 10000);

    folder->onSliderEvent(this, &ofApp::onSliderEvent);

    UI->addBreak();

    // add a couple range sliders //
    UI->addSlider("Planets", 0, 250, 15);
    UI->addSlider("Asteroids", 0, 250, 15);
    UI->addSlider("Proximity", 0, 1, 0.2);
    UI->addSlider("Kinetic Energy", 0, 10, 4);

    UI->addBreak();

    // and a couple of simple buttons //
    UI->addToggle("Show Trails", true);
    UI->addToggle("Show Axis", false);
    UI->addToggle("Center of Mass", false);
    UI->addButton("Start Simulation");

    // adding the optional header allows you to drag the gui around //
    UI->addHeader(":: drag me to reposition ::");

    // adding the optional footer allows you to collapse/expand the gui //
    UI->addFooter();

    // once the gui has been assembled, register callbacks to listen for component specific events //
    UI->onButtonEvent(this, &ofApp::onButtonEvent);
    UI->onToggleEvent(this, &ofApp::onToggleEvent);
    UI->onSliderEvent(this, &ofApp::onSliderEvent);
    UI->onDropdownEvent(this, &ofApp::onDropdownEvent);
    UI->onColorPickerEvent(this, &ofApp::onColorPickerEvent);

    UI->setOpacity(0);
    //UI->setLabelAlignment(ofxDatGuiAlignment::CENTER);
}

//--------------------------------------------------------------
void ofApp::update()
{
    Total_mass = 1;
    double center_of_mass_x = 0;
    double center_of_mass_y = 0;
    double center_of_mass_z = 0;
    for (int i = 0; i < (int)bodies.size(); i++)
    {
        bodies[i].update(i, show_trails);
        if (bodies[i].abs_velocity < 20 && start)
        {
            Total_mass += bodies[i].mass;
            center_of_mass_x += bodies[i].mass * bodies[i].position[0];
            center_of_mass_y += bodies[i].mass * bodies[i].position[1];
            center_of_mass_z += bodies[i].mass * bodies[i].position[2];
        }
    }

    if (start)
    {
        center_of_mass[0] = center_of_mass_x / Total_mass;
        center_of_mass[1] = center_of_mass_y / Total_mass;
        center_of_mass[2] = center_of_mass_z / Total_mass;
    }
}

//--------------------------------------------------------------
void ofApp::draw()
{
    ofEnableLighting();
    ofEnableDepthTest();
    light.enable();
    cam.begin();
    cam.setTarget(glm::vec3(0, 0, 0));
    if (start && Num_Stars == 1)
        cam.setTarget(bodies[0].position); // Define the point where the camera rotates around
    else if(start)
        cam.setTarget(center_of_mass);
    //ofRotateDeg(90, 0, 0, 1); // Rotate the camera to a position

    if(show_center)
    {
        ofSetColor(255,255,255);
        ofDrawSphere(center_of_mass, 10);
    }
    if (show_axis)
    {
        ofDrawGrid(1000); // Draw axis
    }

    for (int i = 0; i < (int)bodies.size(); i++)
    {
        bodies[i].draw();
    }

    cam.end();
    light.disable();
    ofDisableDepthTest();
    ofDisableLighting();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    switch (key)
    {
    case 'f':
    case 'F':
        ofToggleFullscreen();
        break;
    case ' ':
        bodies.clear();

        Num_Planets = Temp_Num_Planets;
        Num_Asteroids = Temp_Num_Asteroids;
        Num_Stars = Temp_Num_Stars;

        int init = 500 * (1 - proximity) + Num_Planets;

        std::cout << Num_Planets << " " << Num_Asteroids << " " << Num_Stars << std::endl;

        // Create Planets
        for (int i = 0; i < Num_Stars; i++)
        {
            body newbody(ofRandom(-init, init), ofRandom(-init, init), ofRandom(-init, init), ofRandom(star_relative_mass - 5000, star_relative_mass + 5000), ofRandom(10, 50));
            bodies.push_back(newbody);
        }
        // Create Planets
        for (int i = Num_Stars; i < Num_Planets; i++)
        {
            body newbody(ofRandom(-init, init), ofRandom(-init, init), ofRandom(-init, init), ofRandom(10, 60), ofRandom(60, 180));
            bodies.push_back(newbody);
        }
        // Create Asteroids
        for (int i = Num_Planets + 1; i < Num_Planets + Num_Asteroids; i++)
        {
            body newbody(ofRandom(-init, init), ofRandom(-init, init), ofRandom(-init, init), ofRandom(5, 10), ofRandom(180, 255));
            bodies.push_back(newbody);
        }

        start = true;
        break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{
}

void ofApp::onButtonEvent(ofxDatGuiButtonEvent e)
{
    if (e.target->is("Start Simulation"))
        keyPressed(' ');
}

void ofApp::onToggleEvent(ofxDatGuiToggleEvent e)
{
    // cout << "onToggleEvent: " << e.target->getLabel() << " " << e.checked << endl;
    if (e.target->is("Show Axis"))
        if (show_axis)
            show_axis = false;
        else
            show_axis = true;
    if (e.target->is("Show Trails"))
        if (show_trails)
            show_trails = false;
        else
            show_trails = true;
    if (e.target->is("Center of Mass"))
        if (show_center)
            show_center = false;
        else
            show_center = true;
}

void ofApp::onSliderEvent(ofxDatGuiSliderEvent e)
{
    // cout << "onSliderEvent: " << e.target->getLabel() << " " << e.target->getValue() << endl;
    if (e.target->is("Planets"))
        Temp_Num_Planets = (int)e.target->getValue();
    if (e.target->is("Asteroids"))
        Temp_Num_Asteroids = (int)e.target->getValue();
    if (e.target->is("Proximity"))
        proximity = e.target->getValue();
    if (e.target->is("Number of Stars"))
        Temp_Num_Stars = (int)e.target->getValue();
    if (e.target->is("Relative Mass"))
        star_relative_mass = e.target->getValue();
    if (e.target->is("Kinetic Energy"))
        Initial_Max_Velocity = e.target->getValue();
}

void ofApp::onDropdownEvent(ofxDatGuiDropdownEvent e)
{
    cout << "onDropdownEvent: " << e.target->getLabel() << " Selected" << endl;
}

void ofApp::onColorPickerEvent(ofxDatGuiColorPickerEvent e)
{
    cout << "onColorPickerEvent: " << e.target->getLabel() << " " << e.target->getColor() << endl;
    ofSetBackgroundColor(e.color);
}
