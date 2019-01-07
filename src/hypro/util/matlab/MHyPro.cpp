#include "HyProBox.h"
#include "HyProConstraintSet.h"
#include "HyProTaylorModel.h"
#include "HyProEllipsoid.h"
#include <iostream>
#include "mex.h"
#include "matrix.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){

    // Get type of the Matlab object
    char type[64];
    mxGetString(prhs[0], type, sizeof(type));

    if(!strcmp(type, "Box")){
        HyProBox::process(nlhs, plhs, nrhs, prhs);
    }else if(!strcmp(type, "Ellipsoid")){
        HyProEllipsoid::process(nlhs, plhs, nrhs, prhs);
    }else if(!strcmp(type, "ConstraintSet")){
        HyProConstraintSet::process(nlhs, plhs, nrhs, prhs);
    }else if(!strcmp(type, "SupportFunction")){
       // HyProSupportFunction::process(nlhs, plhs, nrhs, prhs);
    }else{
        mexErrMsgTxt("MHyPro - Unknown type.");
    }
    
}