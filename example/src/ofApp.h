#pragma once

#include <map>
#include "ofMain.h"
#include "ofxHighDim.h"
#include "ofxSpatialHash.h"
#include "ofxInfiniteCanvas.h"

class ofApp : public ofBaseApp{

	public:
        ofApp();
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        void addTrackedPoint(int index, ofVec2f point);
    
    struct TestPoint {
        int class_;
        ofColor color;
        vector<float> point;
//        ofPoint tsnePoint;
    };
    struct FixedPoint {
        int index;
        ofVec2f position;
        //        ofPoint tsnePoint;
        bool operator() (const FixedPoint & left, int right)
        {
            return left.index < right;
        }
    };

    
    ofxHighDim highDim;
    ofxInfiniteCanvas cam;

    vector<TestPoint> testPoints;
//    vector<vector<float> > data;
//    vector<vector<double> > tsnePoints;
    
    /// \brief Our point collection.
    ///
    /// These points MUST be initialized BEFORE initing the hash.
    std::vector<ofVec2f> tsnePoints;
//    ofMesh mesh;
    
    /// \brief The spatial hash specialized for ofVec2f.
    ofx::KDTree<ofVec2f> hash;
    
    /// \brief The search results specialized for ofVec2f.
    ofx::KDTree<ofVec2f>::SearchResults searchResults;

    bool runManually;
    
    // Point tracking
    bool pointTracked;
    FixedPoint draggedPoint;
    //std::vector<FixedPoint> tsne_fixed_points;
    std::map<int, ofVec2f> tsne_fixed_points;
//    std::vector<int> tsne_fixed_points_index;
//    std::vector<ofVec2f> tsne_fixed_points_position;
};
