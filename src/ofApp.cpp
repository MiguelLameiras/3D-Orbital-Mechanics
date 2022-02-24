#include "ofApp.h"

vector<body> bodies;
int Num_Planets;
int Num_Asteroids;

body::body(int x, int y, int z, double mass_, int hue)
{
    position = glm::vec3(x, y, z);                                                             // Initial position
    double init = 1;                                                                           // Range of possible initial velocities
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

    Num_Planets = 15;
    Num_Asteroids = 15;

    ofEnableLighting();
    light.setPosition(0, 0, 0);

    ofSetSphereResolution(5);

    gui.setup();
    gui.add(label.setup("", "Press Space to start"));
    gui.add(intSlider1.setup("Planets", 15, 1, 250));
    gui.add(intSlider2.setup("Asteroids", 15, 0, 250));
    gui.add(trails_button.setup("Show Trails", true));
    gui.add(star_button.setup("Star", true));
    gui.add(center_label.setup("", "Center of Mass:"));
    gui.add(fps_label.setup("", "fps:"));
}

//--------------------------------------------------------------
void ofApp::update()
{
    Total_mass = 1;
    double center_of_mass_x = 0;
    double center_of_mass_y = 0;
    double center_of_mass_z = 0;
    for (int i = 0; i < bodies.size(); i++)
    {
        bodies[i].update(i, show_trails);
        if (bodies[i].abs_velocity < 20 && start)
        {
            Total_mass += bodies[i].mass;
            center_of_mass_x += bodies[i].mass * bodies[i].position[0];
            center_of_mass_y += bodies[i].mass * bodies[i].position[1];
            center_of_mass_z += bodies[i].mass * bodies[i].position[2];

            std::stringstream center;
            center << "CM: (" << center_of_mass[0] << "," << center_of_mass[1] << "," << center_of_mass[2] << ")";
            center_label.setup("", center.str());
        }
    }

    if (start)
    {
        center_of_mass[0] = center_of_mass_x / Total_mass;
        center_of_mass[1] = center_of_mass_y / Total_mass;
        center_of_mass[2] = center_of_mass_z / Total_mass;

        std::stringstream center;
        center << "CM: (" << std::setprecision(3) << center_of_mass[0] << "," << center_of_mass[1] << "," << center_of_mass[2] << ")";
        center_label.setup("", center.str());
    }
    std::stringstream strm;
    strm << "Fps: " << ofGetFrameRate();
    fps_label.setup("", strm.str());
}

//--------------------------------------------------------------
void ofApp::draw()
{
    ofEnableLighting();
    ofEnableDepthTest();
    cam.begin();
    if (start && star_button)
        cam.setTarget(bodies[0].position); // Define the point where the camera rotates around
    else
        cam.setTarget(glm::vec3(0,0,0));
    // ofRotateDeg(90, 0, 0, 1); // Rotate the camera to a position
    light.enable();
    // ofSetColor(255,255,255);
    // ofDrawSphere(center_of_mass, 10);
    //  ofDrawAxis(400); // Draw axis

    for (int i = 0; i < bodies.size(); i++)
    {
        bodies[i].draw();
    }

    light.disable();
    cam.end();
    ofDisableDepthTest();
    ofDisableLighting();

    gui.draw();
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

        Num_Asteroids = intSlider2;
        Num_Planets = intSlider1;

        if (trails_button)
            show_trails = true;
        else
            show_trails = false;

        int init = 100 + Num_Planets*3;

        if (star_button)
        {
            body newbody(0, 0, 0, 1000, ofRandom(0, 40)); // Create a Giant Body / Star
            bodies.push_back(newbody);
        }
        // Create Planets
        for (int i = 1; i <= Num_Planets; i++)
        {
            body newbody(ofRandom(-init, init), ofRandom(-init, init), ofRandom(-init, init), ofRandom(10, 60), ofRandom(60, 180));
            bodies.push_back(newbody);
        }
        // Create Asteroids
        for (int i = Num_Planets + 1; i <= Num_Planets + Num_Asteroids; i++)
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