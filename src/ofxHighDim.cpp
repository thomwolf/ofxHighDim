#include "ofxHighDim.h"


std::vector<ofVec2f> ofxHighDim::run(vector<vector<float> > & data, vector<int> & categories, int dims, double perplexity, double theta, bool normalize, bool runManually) {
    this->data = data;
    this->dims = dims;
    this->perplexity = perplexity;
    this->theta = theta;
    this->normalize = normalize;
    this->runManually = runManually;
    
    max_iter = 1000;
    iter = 0;
    
    N = data.size();
    D = data[0].size();
    
    if (N - 1 < 3 * perplexity) {
        ofLog(OF_LOG_WARNING, "Perplexity too large for number of data points, setting to max");
        perplexity = (float) (N-1) / 3.0 - 1.0;
    }
    
    X = (double*) malloc(D * N * sizeof(double));
    fixedPointsIndexes = (int*) malloc(N * sizeof(int));
    
    // Get categories range
    int min_cat = numeric_limits<int>::max();
    int max_cat = numeric_limits<int>::min();
    for (int i=0; i<N; i++) {
        if (categories[i] < min_cat)  min_cat = categories[i];
        if (categories[i] > max_cat)  max_cat = categories[i];
    }
    // Prepare data and colors
    int idx = 0;
    for (int i=0; i<N; i++) {
        ofColor clr;
        clr.setHsb((unsigned char)(float(categories[i]-min_cat)/float(max_cat-min_cat)*255), 255, 255);
        colors.push_back(clr);
        for (int j=0; j<D; j++) {
            X[idx] = data[i][j];
            idx++;
        }
    }

    // t-SNE
    //Y = (double*) malloc(dims * N * sizeof(double));
    //tsne.run(X, N, D, Y, fixedPointsIndexes, dims, perplexity, theta, 1000, runManually);
    
    //LargeVis
    largeVis.load_from_data(X, N, D);
    largeVis.run();
    Y = largeVis.get_ans();
    
    if (runManually) {
        return embeddedPoints;
    }
    return iterate();
}

std::vector<ofVec2f> ofxHighDim::iterate(const std::map<int, ofVec2f> & tsne_fixed_points){
    N_fixed_points = tsne_fixed_points.size();

    if (iter > max_iter && N_fixed_points == 0) {
        return embeddedPoints;
    }
    
    if (runManually) {
        //double *X_Fixed_points = (double*) malloc(D * N_fixed_points * sizeof(double));
        //int *I_Fixed_points = (int*) malloc(N_fixed_points * sizeof(int));
        
        int idx = 0;
        for(std::map<int,ofVec2f>::const_iterator ii=tsne_fixed_points.begin(); ii!=tsne_fixed_points.end(); ++ii, ++idx)
        {
            int index = (*ii).first;
            fixedPointsIndexes[idx] = index;
            Y[2*index] = (*ii).second.x;
            Y[2*index+1] = (*ii).second.y;
        }
        tsne.runIteration(N_fixed_points);
    }
    
    // keep track of min for normalization
    vector<double> min_, max_;
    min_.resize(dims);
    max_.resize(dims);
    for (int i=0; i<dims; i++) {
        min_[i] = numeric_limits<double>::max();
        max_[i] = numeric_limits<double>::min();
    }
    
    // unpack Y into embeddedPoints
    embeddedPoints.clear();
    mesh.clear();
    int idxY = 0;
    for (int i=0; i<N; i++) {
        //vector<double> tsnePoint;
        //tsnePoint.resize(dims);
        ofVec2f tsnePoint;
        mesh.addVertex(ofVec2f(Y[idxY], Y[idxY+1]));
        mesh.addColor(colors[i]);
        for (int j=0; j<dims; j++) {
            tsnePoint[j] = Y[idxY];
            if (normalize) {
                if (tsnePoint[j] < min_[j])  min_[j] = tsnePoint[j];
                if (tsnePoint[j] > max_[j])  max_[j] = tsnePoint[j];
            }
            idxY++;
        }
        embeddedPoints.push_back(tsnePoint);
    }
    
    // normalize if requested
    if (normalize) {
        for (int i=0; i<embeddedPoints.size(); i++) {
            for (int j=0; j<dims; j++) {
                embeddedPoints[i][j] = (embeddedPoints[i][j] - min_[j]) / (max_[j] - min_[j]);
            }
        }
    }
    
    iter++;
//    if (iter == max_iter) {
//        finish();
//    }
    
    return embeddedPoints;
}

void ofxHighDim::finish() {
    tsne.finish();
    delete(X);
    delete(Y);
    delete(fixedPointsIndexes);
}
