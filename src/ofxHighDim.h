#pragma once

#include "ofMain.h"
#include "tsne.h"
#include "LargeVis.h"

class ofxHighDim
{
public:
    std::vector<ofVec2f> run(vector<vector<float> > & data, vector<int> & categories, int dims=2, double perplexity=30, double theta=0.5, bool normalize=true, bool runManually=false);
    std::vector<ofVec2f> iterate(const map<int, ofVec2f> & tsne_fixed_points = map<int, ofVec2f>());
    ofMesh mesh;

private:
    void finish();
    
    TSNE tsne;
    LargeVis largeVis;
    
    vector<vector<float> > data;
    vector<ofVec2f> embeddedPoints;
    vector<ofColor> colors;

    int dims;
    double perplexity;
    double theta;
    bool normalize;
    bool runManually;
    
    int N, D;
    double *X, *Y;
    int *fixedPointsIndexes;

    int iter, max_iter, N_fixed_points;
};
