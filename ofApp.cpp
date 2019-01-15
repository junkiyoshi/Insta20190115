#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetLineWidth(1.5);

	this->number_of_agent = 200;
	this->fbo.allocate(ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::update() {

	this->fbo.begin();
	ofClear(0);
	ofSetColor(39);

	for (int i = 1; i <= 4; i++) {

		vector<glm::vec2> vertices;
		for (int x = 0; x <= ofGetWidth(); x += 10) {

			vertices.push_back(glm::vec2(x, ofMap(ofNoise(i * 39, x * 0.001, ofGetFrameNum() * 0.001 * i), 0, 1, ofGetHeight() * 0.1, ofGetHeight() * 0.9)));
		}

		int count = vertices.size();
		for (int vertices_index = count - 1; vertices_index > -1; vertices_index--) {

			vertices.push_back(vertices[vertices_index] - glm::vec2(0, -60));
		}

		ofBeginShape();
		ofVertices(vertices);
		ofEndShape();
	}

	this->fbo.end();
	this->fbo.readToPixels(this->pixels);

	while (this->locations.size() < this->number_of_agent) {

		int x = ofRandom(ofGetWidth());
		int y = ofRandom(ofGetHeight());
		if (this->pixels.getColor(x, y) != ofColor(0, 0)) {

			this->locations.push_back(glm::vec2(x, y));
			this->directions.push_back(glm::normalize(glm::vec2(ofRandom(-1, 1), ofRandom(-1, 1))));
		}
	}

	for (int i = 0; i < this->number_of_agent; i++) {

		glm::vec2 future;
		future = this->locations[i] + this->directions[i] * 3;
		int x = future.x;
		int y = future.y;

		while (x < 0 || x > ofGetWidth() || y < 0 || y > ofGetHeight() || this->pixels.getColor(x, y) == ofColor(0, 0)) {

			this->directions[i] = glm::normalize(glm::vec2(ofRandom(-1, 1), ofRandom(-1, 1)));

			future = this->locations[i] + this->directions[i] * 10;
			x = future.x;
			y = future.y;
		}

		this->locations[i] = future;
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofSetColor(255);
	this->fbo.draw(0, 0);

	ofFill();
	for (int i = 0; i < this->number_of_agent; i++) {

		ofDrawCircle(this->locations[i], 3);
		for (int j = 0; j < this->number_of_agent; j++) {

			if (glm::length(this->locations[i] - this->locations[j]) < 30) {

				ofDrawLine(this->locations[i], this->locations[j]);
			}
		}
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}