#include "HyProBox.h"

/**
 * @brief Creates an empty box
 **/
void HyProBox::emptyBox(int nlhs, mxArray *plhs[], const mxArray *prhs[]){
    if(nlhs != 1){
        mexErrMsgTxt("HyProBox - new_empty: One output expected.");
    }
    plhs[0] = convertPtr2Mat<hypro::Box<double>>(new hypro::Box<double>);
}

/**
 * @brief Creates a box from a single interval
 **/
void HyProBox::boxFromSingleInterval(int nlhs, mxArray *plhs[], const mxArray *prhs[]){
    if(nlhs != 1){
            mexErrMsgTxt("HyProBox - new_interval: One output expected."); 
    }
    mxArray *m_in_interval;
    const mwSize *dims;
    double *in;
    int dimx, dimy;

    m_in_interval = mxDuplicateArray(prhs[2]);

    dims = mxGetDimensions(prhs[2]);
    dimy = (int) dims[0];
    dimx = (int) dims[1];
    if(dimy != 1 || dimx > 2){
        mexErrMsgTxt("HyProBox - new_interval: You have to input a 1x2-matrix!");
    }

    in = mxGetPr(m_in_interval);
    carl::Interval<double> inter = ObjectHandle::mInterval2Hypro(in);
    hypro::Box<double>* box = new hypro::Box<double>(inter);
    plhs[0] = convertPtr2Mat<hypro::Box<double>>(box);
}

/**
 * @brief Creates a box from a list of intervals
 **/
void HyProBox::boxFromIntervals(int nlhs, mxArray *plhs[], const mxArray *prhs[]){
    if(nlhs != 1){
        mexErrMsgTxt("HyProBox - new_intervals: One output expected.");
    }
    mxArray *m_in_intervals;
    const mwSize *dims;
    double *in;
    int dimx, dimy;

    m_in_intervals = mxDuplicateArray(prhs[2]);

    dims = mxGetDimensions(prhs[2]);
    dimy = (int) dims[0];
    dimx = (int) dims[1];
    // Ensure that the user entered a list of intervals
    if(dimx != 2){
        mexErrMsgTxt("HyProBox - new_intervals: You have to enter a nx2-matrix! (e.g.:[lower_1 upper_1; lower_2 upper_2])");
    }

    in = mxGetPr(m_in_intervals);
    
    std::vector<carl::Interval<double>> intervals = ObjectHandle::mIntervals2Hypro(in, dimx, dimy);
    hypro::Box<double>* box = new hypro::Box<double>(intervals);
    plhs[0] = convertPtr2Mat<hypro::Box<double>>(box);
}

/**
 * @brief Creates a box from a minimal and maximal point limiting the box
 **/
void HyProBox::boxFromPoints(int nlhs, mxArray *plhs[], const mxArray *prhs[]){
    if(nlhs != 1){
        mexErrMsgTxt("HyProBox - new_points: One output expected.");
    }
    mxArray *m_in_points;
    const mwSize *dims;
    double *in;
    int dimy, dimx, numdims;

    m_in_points = mxDuplicateArray(prhs[2]);

    dims = mxGetDimensions(prhs[2]);
    numdims = mxGetNumberOfDimensions(prhs[2]);
    dimy = (int) dims[0];
    dimx = (int) dims[1];
    if(numdims > 2 || dimy != 2 || dimx != 2){
        mexErrMsgTxt("HyProBox - new_points: You have to input a 2x2-matrix containing the min. and max. point!");
    }

    in = mxGetPr(m_in_points);
    
    std::pair<hypro::Point<double>, hypro::Point<double>> pair = ObjectHandle::mPointPair2Hypro(in);
    hypro::Box<double>* box = new hypro::Box<double>(pair);
    plhs[0] =  convertPtr2Mat<hypro::Box<double>>(box);
    return;
}

/**
 * @brief Returns setting of a box
 **/
void HyProBox::getSettings(int nlhs, mxArray *plhs[], const mxArray *prhs[]){
    mexPrintf("getSettings: Not finished yet!");
    hypro::Box<double>* box = convertMat2Ptr<hypro::Box<double>>(prhs[2]);
    if(nlhs != 1)
        mexErrMsgTxt("HyProBox - getSettings: Expecting an output!");
    box->getSettings();
}

/**
 * @brief Method for the construction of an empty box of required dimension.
 **/
void HyProBox::empty(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    if(nlhs < 1)
        mexErrMsgTxt("HyProBox - empty: Expecting one output!");
    if(nrhs < 3)
        mexErrMsgTxt("HyProBox - empty: One or more arguments are missing!");

    mxArray *m_in_dim, *m_out_box;
    double *out;
    const mwSize *dims;
    int dimy, dimx;

    size_t in = (size_t) mxGetScalar(prhs[3]);

    hypro::BoxT<double, hypro::Converter<double>, hypro::BoxLinearOptimizationOn>* box = convertMat2Ptr<hypro::BoxT<double, hypro::Converter<double>, hypro::BoxLinearOptimizationOn>>(prhs[2]);

    hypro::Box<double> temp = box->Empty(in);
    hypro::Box<double>* b = new hypro::Box<double>(temp);
    plhs[0] = convertPtr2Mat<hypro::Box<double>>(b);
}

/**
 * @brief Returns the intervals from which the box is constructed
 **/
void HyProBox::intervals(int nlhs, mxArray *plhs[], const mxArray *prhs[]){
    if(nlhs < 1){
        mexErrMsgTxt("HyProBox - intervals: Expecting an output!");
    }   
    mxArray* m_out_intervals;
    double* out;

    hypro::Box<double>* box = convertMat2Ptr<hypro::Box<double>>(prhs[2]);

    std::vector<carl::Interval<double>> intervals = box->intervals();

    const int dimy = intervals.size();
    const int dimx = 2;
    m_out_intervals = plhs[0] = mxCreateDoubleMatrix(dimy, dimx, mxREAL);
    out = mxGetPr(m_out_intervals);
    vector2mVector(intervals, out, dimx, dimy);
}

/**
 * @brief Extends a box by a further interval
 **/
void HyProBox::insert(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    if(nrhs < 4){
        mexErrMsgTxt("HyProBox - insert: One argument is missing!");
    }
    mxArray *m_in_box, *m_in_interval;
    const mwSize *dims;
    double *in_box, *in_interval, *out;
    int dimy, dimx, numdims;

    hypro::Box<double>* box = convertMat2Ptr<hypro::Box<double>>(prhs[2]);

    m_in_box = mxDuplicateArray(prhs[2]);
    m_in_interval = mxDuplicateArray(prhs[3]);

    dims = mxGetDimensions(prhs[3]);
    numdims = mxGetNumberOfDimensions(prhs[3]);
    dimy = (int) dims[0];
    dimx = (int) dims[1];
    if(numdims > 2 || dimy != 1 || dimx != 2){
        mexErrMsgTxt("HyProBox - insert: You have to input a 1x2-matrix!");
    }

    in_box = mxGetPr(m_in_box);
    in_interval = mxGetPr(m_in_interval);
    carl::Interval<double> interval = ObjectHandle::mInterval2Hypro(in_interval);
    box->insert(interval);
}

/**
 * @brief Getter for the limiting points.
 **/
void HyProBox::limits(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    if(nlhs < 1){
        mexErrMsgTxt("HyProBox - limits: Expecting an output!");
    }
    if(nrhs < 3){
        mexErrMsgTxt("HyProBox - limits: One argument is missing!");
    }
    mxArray* m_out;
    double* out;

    hypro::Box<double>* box = convertMat2Ptr<hypro::Box<double>>(prhs[2]);
    std::pair<hypro::Point<double>, hypro::Point<double>> p = box->limits();
    
    m_out = plhs[0] = mxCreateDoubleMatrix(2, 2, mxREAL);
    out = mxGetPr(m_out);
    pair2matlab(p, out, 1, 2);
}

/**
 * @brief Getter for the hyperplanar representation of the current box.
 **/
void HyProBox::constraints(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    if(nlhs < 2)
        mexErrMsgTxt("HyProBox - constraints: Expecting two output values!");
    if(nrhs < 3)
        mexErrMsgTxt("HyProBox - constraints: One argument is missing!");

    hypro::Box<double>* box = convertMat2Ptr<hypro::Box<double>>(prhs[2]);
    std::vector<hypro::Halfspace<double>> hSpaces = box->constraints();

    const int dimx = hSpaces[0].dimension();
    const int dimy = hSpaces.size();

    mxArray* m_normal = plhs[0] = mxCreateDoubleMatrix(dimy,dimx, mxREAL);
    mxArray *m_offset = plhs[1] = mxCreateDoubleMatrix(dimy,1,mxREAL);

    double *out_normal, *out_offset;
    out_normal = mxGetPr(m_normal);
    out_offset = mxGetPr(m_offset);

    vector2mVector(hSpaces, out_normal, out_offset, dimx, dimy);
}

/**
 * @brief Getter for an interval representation of one specific dimension.
 **/
void HyProBox::interval(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    if(nlhs < 1){
        mexErrMsgTxt("HyProBox - interval: Expecting an output!");
    }
    if(nrhs < 4){
        mexErrMsgTxt("HyProBox - interval: One or more arguments are missing!");
    }

    mxArray *m_in_dim, *m_out;
    double *in, *out;

    size_t dim = (size_t) mxGetScalar(prhs[3]);

    hypro::Box<double>* box = convertMat2Ptr<hypro::Box<double>>(prhs[2]);
    const carl::Interval<double> inter = box->interval(dim);
    m_out = plhs[0] = mxCreateDoubleMatrix(1, 2, mxREAL);
    out = mxGetPr(m_out);
    ObjectHandle::convert2matlab(inter, out, 2, 1);
}

/**
 * @brief Getter for an interval representation of one specific dimension.
 **/
void HyProBox::at(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    if(nlhs < 1){
        mexErrMsgTxt("HyProBox - at: Expecting an output!");
    }
    if(nrhs < 4){
        mexErrMsgTxt("HyProBox - at: One or more arguments are missing!");
    }

    mxArray *m_in_dim, *m_out;
    double* out;

    size_t dim = (size_t) mxGetScalar(prhs[3]);
    hypro::Box<double>* box = convertMat2Ptr<hypro::Box<double>>(prhs[2]);
    const carl::Interval<double> inter = box->at(dim);

    m_out = plhs[0] = mxCreateDoubleMatrix(1, 2, mxREAL);
    out = mxGetPr(m_out);
    ObjectHandle::convert2matlab(inter, out, 2, 1);
}

/**
 * @brief Determines if the current box is symmetric.
 **/
void HyProBox::is_symmetric(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    if(nlhs < 1)
        mexErrMsgTxt("HyProBox - isSymmetric: Expecting an output!");
    if(nrhs < 3)
        mexErrMsgTxt("HyProBox - isSymmetric: One argument missing!");

    hypro::Box<double>* box = convertMat2Ptr<hypro::Box<double>>(prhs[2]);
    const bool empty = box->isSymmetric();
    mxLogical a = empty;
    plhs[0] = mxCreateLogicalScalar(empty);
}

/**
 * @brief Getter for the maximal point.
 **/
void HyProBox::max(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    if(nlhs < 1){
        mexErrMsgTxt("HyProBox - max: Expecting an output!");
    }
    if(nrhs < 3){
        mexErrMsgTxt("HyProBox - max: One argument missing!");
    }
    mxArray *m_out;
    double* out;

    hypro::Box<double>* box = convertMat2Ptr<hypro::Box<double>>(prhs[2]);
    hypro::Point<double> m = box->max();
    int dim = m.rawCoordinates().size();
    m_out = plhs[0] = mxCreateDoubleMatrix(1, 2, mxREAL);
    out = mxGetPr(m_out);
    ObjectHandle::convert2matlab(m, out, 1, dim);
}

/**
 * @brief Getter for the minimal point.
 **/
void HyProBox::min(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
if(nlhs < 1){
        mexErrMsgTxt("HyProBox - min: Expecting an output!");
    }
    if(nrhs < 3){
        mexErrMsgTxt("HyProBox - min: One argument missing!");
    }
    mxArray *m_out;
    double* out;

    hypro::Box<double>* box = convertMat2Ptr<hypro::Box<double>>(prhs[2]);
    hypro::Point<double> m = box->min();
    int dim = m.rawCoordinates().size();
    m_out = plhs[0] = mxCreateDoubleMatrix(1, 2, mxREAL);
    out = mxGetPr(m_out);
    ObjectHandle::convert2matlab(m, out, 1, dim);
}

/**
 * @brief Method returning the supremum point of the box, i.e. the maximal point.
 **/
void HyProBox::supremum(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    if(nlhs < 1){
        mexErrMsgTxt("HyProBox - supremum: Expecting an output!");
    }
    if(nrhs < 3){
        mexErrMsgTxt("HyProBox - supremum: One argument missing!");
    }
    hypro::Box<double>* box = convertMat2Ptr<hypro::Box<double>>(prhs[2]);
    double supremum = box->supremum();
    plhs[0] = mxCreateDoubleScalar(supremum);
}

/**
 * @brief Evaluation function (convex linear optimization).
 **/
void HyProBox::evaluate(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    if(nlhs < 1)
        mexErrMsgTxt("HyProBox - evaluate: Expecting an output!");
    if(nrhs < 5)
        mexErrMsgTxt("HyProBox - evaluate: One or more arguments are missing!");
    mxArray *m_in_vector;
    double *in_vector;
    const mwSize *dims_vec;
    int vec_len;
    bool ans;

    hypro::Box<double>* box = convertMat2Ptr<hypro::Box<double>>(prhs[2]);

    m_in_vector = mxDuplicateArray(prhs[3]);
    const double dir = (double) mxGetScalar(prhs[4]);
    in_vector = mxGetPr(m_in_vector);

    dims_vec = mxGetDimensions(prhs[3]);
    vec_len = (int) dims_vec[0];

    hypro::vector_t<double> vec = ObjectHandle::mVector2Hypro(in_vector, vec_len);

    if(dir == 0){
        ans = false;
    }else if (dir == 1){
        ans = true;
    }else{
        mexErrMsgTxt("HyProBox - evaluate: The direction must be either 0 or 1!");
    }
    
    hypro::EvaluationResult<double> eval = box->evaluate(vec, ans);
    plhs[0] = convertPtr2Mat<hypro::EvaluationResult<double>>(&eval);
}

/**
 * @brief Multi-evaluation function (convex linear optimization).
 **/
void HyProBox::multiEvaluate(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    if(nlhs < 1)
        mexErrMsgTxt("HyProBox - multiEvaluate: Expecting an output!");
    if(nrhs < 5)
        mexErrMsgTxt("HyProBox - multiEvaluate: One or more arguments are missing!");
    mxArray *m_in_vector;
    double *in_vector;
    const mwSize *dims;
    int dimx, dimy;
    bool ans;

    hypro::Box<double>* box = convertMat2Ptr<hypro::Box<double>>(prhs[2]);

    m_in_vector = mxDuplicateArray(prhs[3]);
    const double dir = (double) mxGetScalar(prhs[4]);
    in_vector = mxGetPr(m_in_vector);

    dims = mxGetDimensions(prhs[3]);
    dimy = (int) dims[0];
    dimx = (int) dims[1];

    hypro::matrix_t<double> mat = ObjectHandle::mMatrix2Hypro(in_vector, dimx, dimy);

    if(dir == 0){
        ans = false;
    }else if (dir == 1){
        ans = true;
    }else{
        mexErrMsgTxt("HyProBox - evaluate: The direction must be either 0 or 1!");
    }
    
    //Does this make sense?
    // std::vector<hypro::EvaluationResult<double>> eval = box->multiEvaluate(mat,ans);
    // plhs[0] = convertPtr2Mat<std::vector<hypro::EvaluationResult<double>>>(&eval);
}

/**
 * @brief Scaling operator.
 **/
void HyProBox::multiply(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    if(nlhs < 1)
        mexErrMsgTxt("HyProBox - *: Expecting an output!");
    if(nrhs < 4)
        mexErrMsgTxt("HyProBox - *: One or more arguments are missing!");
    hypro::Box<double>* box_1 = convertMat2Ptr<hypro::Box<double>>(prhs[2]);
    const double factor = (double) mxGetScalar(prhs[3]);
    
    hypro::Box<double> temp = box_1->operator*(factor);
    hypro::Box<double>* b = new hypro::Box<double>(temp);
    plhs[0] = convertPtr2Mat<hypro::Box<double>>(b);
}

/**
 * @brief Outstream operator.
 **/
void HyProBox::outstream(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    if(nrhs < 3)
        mexErrMsgTxt("HyProBox - <<: One or more arguments are missing!");

    hypro::Box<double>* box = convertMat2Ptr<hypro::Box<double>>(prhs[2]);
    if(!box->empty()){
        hypro::Point<double> min_p = box->min();
        hypro::Point<double> max_p = box->max();
        hypro::vector_t<double> vec_min =  min_p.rawCoordinates();
        hypro::vector_t<double> vec_max =  max_p.rawCoordinates();
        mexPrintf("Min: [");
        for(int i = 0; i < vec_min.size(); i++){
            mexPrintf("%f ", vec_min[i]);
        }
        mexPrintf("] Max: [");
        for(int i = 0; i < vec_max.size(); i++){
            mexPrintf("%f ", vec_max[i]);
        }
        mexPrintf("]\n");
    }
}

/**
 * @brief Storage size determination.
 **/
void HyProBox::box_size(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    if(nlhs < 1)
        mexErrMsgTxt("HyProBox - size: Expecting an output!");
    if(nrhs < 3)
        mexErrMsgTxt("HyProBox - size: One or more arguments are missing!");

    hypro::Box<double>* box = convertMat2Ptr<hypro::Box<double>>(prhs[2]);
    std::size_t dim = box->size();
    
    plhs[0] = mxCreateDoubleScalar(dim);
}

/**
 * @brief Returns type of representation.
 **/
void HyProBox::type(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    mexPrintf("Box");
}

/**
 * @brief Makes a symmetric box from the current box.
 **/
void HyProBox::makeSymmetric(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    if(nrhs < 3){
        mexErrMsgTxt("HyProBox - makeSymmetric: One or more arguments are missing!");
    }
    hypro::Box<double>* box = convertMat2Ptr<hypro::Box<double>>(prhs[2]);
    box->makeSymmetric();
}

/**
 * @brief Performs a Minkowski decomposition of two boxes.
 **/
void HyProBox::minkowskiDecomposition(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    if(nlhs < 1)
        mexErrMsgTxt("HyProBox - minkowskiDecomposition: Expecting one output value!");
    if(nrhs < 4)
        mexErrMsgTxt("HyProBox - minkowskiDecomposition: One or more arguments are missing!");
    
    hypro::Box<double>* box = convertMat2Ptr<hypro::Box<double>>(prhs[2]);
    hypro::BoxT<double, hypro::Converter<double>, hypro::BoxLinearOptimizationOn>* box_rhs = convertMat2Ptr<hypro::BoxT<double, hypro::Converter<double>, hypro::BoxLinearOptimizationOn>>(prhs[3]);

    hypro::Box<double> sum = box->minkowskiDecomposition(*box_rhs);
    hypro::Box<double>* b = new hypro::Box<double>(sum);
    plhs[0] = convertPtr2Mat<hypro::Box<double>>(b);
}

/**
* @brief The entry point to the HyPro Box class for Matalb.
* @param nlhs Number of items in plhs 
* @param plhs Array of mxArray pointers to the output variables
* @param nrhs Number of items in prhs
* @param prhs Array of mxArray pointers to the input variables
**/
void HyProBox::process(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){
    // Get the command string
    char cmd[64];
    if (nrhs < 1 || mxGetString(prhs[1], cmd, sizeof(cmd)))
        mexErrMsgTxt("HyProBox - First input should be a command string less than 64 characters long.");
    
    /***************************************************************************
     * Constructors
     **************************************************************************/

    if (!strcmp("new_empty", cmd) && nrhs == 2){  
        emptyBox(nlhs, plhs, prhs);
        return;
    }
    
    // Check if there is a second input, which should be the class instance handle
    if (nrhs < 2){
        mexErrMsgTxt("Second input should be a Box instance handle.");
    }
    
    if(!strcmp("copy", cmd)){
        copyObj(nlhs, plhs, nrhs, prhs);
        return;
    }

    if(!strcmp("new_interval", cmd)){
        boxFromSingleInterval(nlhs, plhs, prhs);
        return;
    }

    if(!strcmp("new_intervals", cmd)){
        boxFromIntervals(nlhs, plhs, prhs);
        return;
    }

    if(!strcmp("new_points",cmd)){
        boxFromPoints(nlhs, plhs, prhs);
        return;
    }

    if(!strcmp("new_matrix",cmd)){
        //boxFromMatrix(nlhs, plhs, nrhs, prhs);
        new_mat_vec(nlhs, plhs, nrhs, prhs);
        return;
    }

    if (!strcmp("delete", cmd)) {
        // deleteBox(nlhs, nrhs, prhs);
        deleteObject(nlhs, plhs, nrhs, prhs);
        return;
    }
    
    /***************************************************************************
     * Getters & setters
     **************************************************************************/ 

    if(!strcmp("getSettings", cmd)){
        getSettings(nlhs, plhs,  prhs);
        return;
    }

    if(!strcmp("empty", cmd)){
        empty(nlhs, plhs, nrhs, prhs);
        return;
    }

    if(!strcmp("intervals", cmd)){
        intervals(nlhs, plhs, prhs);
        return;
    }

    if(!strcmp("insert", cmd)){
        insert(nlhs, plhs, nrhs, prhs);
        return;
    }

    if(!strcmp("limits", cmd)){
        limits(nlhs, plhs, nrhs, prhs);
        return;
    }

    if(!strcmp("matrix", cmd)){
        matrix(nlhs, plhs, nrhs, prhs);
        return;
    }

    if(!strcmp("vector", cmd)){
        vector(nlhs, plhs, nrhs, prhs);
        return;
    }

    if(!strcmp("constraints", cmd)){
        constraints(nlhs, plhs, nrhs, prhs);
        return;
    }

    if(!strcmp("interval", cmd)){
        interval(nlhs, plhs, nrhs, prhs);
        return;
    }

    if(!strcmp("at", cmd)){
        at(nlhs, plhs, nrhs, prhs);
        return;
    }

    if(!strcmp("isEmpty", cmd)){
        is_empty(nlhs, plhs, nrhs, prhs);
        return;
    }

    if(!strcmp("isSymmetric", cmd)){
        is_symmetric(nlhs, plhs, nrhs, prhs);
        return;
    }

    if(!strcmp("max", cmd)){
        max(nlhs, plhs, nrhs, prhs);
        return;
    }

    if(!strcmp("min", cmd)){
        min(nlhs, plhs, nrhs, prhs);
        return;
    }

    if(!strcmp("supremum", cmd)){
        supremum(nlhs, plhs, nrhs, prhs);
        return;
    }

    if(!strcmp("vertices", cmd)){
        vertices(nlhs, plhs, nrhs, prhs);
        return;
    }

    if(!strcmp("evaluate", cmd)){
        evaluate(nlhs, plhs, nrhs, prhs);
        return;
    }

    if(!strcmp("multiEvaluate", cmd)){
        multiEvaluate(nlhs, plhs, nrhs, prhs);
        return;
    }

    if(!strcmp("==", cmd)){
        equal(nlhs, plhs, nrhs, prhs);
        return;
    }

    if(!strcmp("!=", cmd)){
        unequal(nlhs, plhs, nrhs, prhs);
        return;
    }

    if(!strcmp("*", cmd)){
        multiply(nlhs, plhs, nrhs, prhs);
        return;
    }

    if(!strcmp("<<", cmd)){
        outstream(nlhs, plhs, nrhs, prhs);
        return;
    }

    /**************************************************************************
     * General interface
    **************************************************************************/
    
    if(!strcmp("dimension", cmd)){
        dimension(nlhs, plhs, nrhs, prhs);
        return;
    }

    if(!strcmp("removeRedundancy", cmd)){
        removeRedundancy(nlhs, plhs, nrhs, prhs);
        return;
    }

    if(!strcmp("size", cmd)){
        box_size(nlhs, plhs, nrhs, prhs);
        return;
    }

    if(!strcmp("type", cmd)){
        type(nlhs, plhs, nrhs, prhs);
        return;
    }

    if(!strcmp("reduceNumberRepresentation", cmd)){
        reduceNumberRepresentation(nlhs, plhs, nrhs, prhs);
        return;
    }

    if(!strcmp("makeSymmetric", cmd)){
        makeSymmetric(nlhs, plhs, nrhs, prhs);
        return;
    }

    if(!strcmp("satisfiesHalfspace", cmd)){
        satisfiesHalfspace(nlhs, plhs, nrhs, prhs);
        return;
    }

    if(!strcmp("satisfiesHalfspaces", cmd)){
        satisfiesHalfspaces(nlhs, plhs, nrhs, prhs);
        return;
    }

    if(!strcmp("project", cmd)){
        project(nlhs, plhs, nrhs, prhs);
        return;
    }

    if(!strcmp("linearTransformation", cmd)){
        linearTransformation(nlhs, plhs, nrhs, prhs);
        return;
    }

    if(!strcmp("affineTransformation", cmd)){
        affineTransformation(nlhs, plhs, nrhs, prhs);
        return;
    }

    if(!strcmp("minkowskiSum", cmd)){
        minkowskiSum(nlhs, plhs, nrhs, prhs);
        return;
    }

    if(!strcmp("minkowskiDecomposition", cmd)){
        minkowskiDecomposition(nlhs, plhs, nrhs, prhs);
        return;
    }

    if(!strcmp("intersect", cmd)){
        intersect(nlhs, plhs, nrhs, prhs);
        return;
    }

    if(!strcmp("intersectHalfspace", cmd)){
        intersectHalfspace(nlhs, plhs, nrhs, prhs);
        return;
    }

    if(!strcmp("intersectHalfspaces", cmd)){
        reduceNumberRepresentation(nlhs, plhs, nrhs, prhs);
        return;
    }

    if(!strcmp("contains_point", cmd)){
        contains_point(nlhs, plhs, nrhs, prhs);
        return;
    }

    if(!strcmp("contains", cmd)){
        contains_object(nlhs, plhs, nrhs, prhs);
        return;
    }

    if(!strcmp("unite", cmd)){
        unite_single(nlhs, plhs, nrhs, prhs);
        return;
    }

    if(!strcmp("unite_objects", cmd)){
        unite_vec(nlhs, plhs, nrhs, prhs);
        return;
    }

    if(!strcmp("reduceRepresentation", cmd)){
        reduceRepresentation(nlhs, plhs, nrhs, prhs);
        return;
    }

    if(!strcmp("clear", cmd)){
        clear(nlhs, plhs, nrhs, prhs);
        return;
    }

    mexErrMsgTxt("HyProHyProBox - Command not recognized.");
}
