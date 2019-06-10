#include "MAffineFlow.h"

void MAffineFlow::new_mat_vec( int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[] ) {
	if ( nlhs != 1 ) mexErrMsgTxt( "MAffineFlow - new_mat_vec: Expecting an output." );
	if ( nrhs < 4 ) mexErrMsgTxt( "MAffineFlow - new_mat_vec: One or more arguments are missing." );
	if ( nrhs > 4 ) mexWarnMsgTxt( "MAffineFlow - new_mat_vec: One or more arguments were ignored." );

	const mwSize *mat_dims, *vec_dims;
	int mat_rows, mat_cols, len;

	mat_dims = mxGetDimensions( prhs[2] );
	vec_dims = mxGetDimensions( prhs[3] );
	mat_cols = (int)mat_dims[1];
	mat_rows = (int)mat_dims[0];
	len = (int)vec_dims[0];

	hypro::matrix_t<double> matrix = ObjectHandle::mMatrix2Hypro( prhs[2], mat_rows, mat_cols );
	hypro::vector_t<double> vector = ObjectHandle::mVector2Hypro( prhs[3], len );
	plhs[0] = convertPtr2Mat<hypro::affineFlow<double>>( new hypro::affineFlow<double>( matrix, vector ) );
}

void MAffineFlow::setTranslation( int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[] ) {
	if ( nrhs < 4 ) mexErrMsgTxt( "MAffineFlow - setTranslation: One or more arguments are missing." );
	if ( nrhs > 4 ) mexWarnMsgTxt( "MAffineFlow - setTranslation: One or more arguments were ignored." );

	const mwSize* dims;
	int len;

	dims = mxGetDimensions( prhs[3] );
	len = (int)dims[0];

	hypro::affineFlow<double>* flow = convertMat2Ptr<hypro::affineFlow<double>>( prhs[2] );
	hypro::vector_t<double> vector = ObjectHandle::mVector2Hypro( prhs[3], len );
	flow->setTranslation( vector );
}

void MAffineFlow::getTranslation( int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[] ) {
	if ( nlhs != 1 ) mexErrMsgTxt( "MAffineFlow - getTranslation: Expecting an output." );
	if ( nrhs < 3 ) mexErrMsgTxt( "MAffineFlow - getTranslation: One or more arguments are missing." );
	if ( nrhs > 3 ) mexWarnMsgTxt( "MAffineFlow - getTranslation: One or more arguments were ignored." );

	hypro::affineFlow<double>* flow = convertMat2Ptr<hypro::affineFlow<double>>( prhs[2] );
	hypro::vector_t<double> vector = flow->getTranslation();

	plhs[0] = mxCreateDoubleMatrix( vector.size(), 1, mxREAL );
	ObjectHandle::convert2Matlab( vector, plhs[0], vector.size(), 1 );
}

void MAffineFlow::hasTranslation( int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[] ) {
	if ( nlhs != 1 ) mexErrMsgTxt( "MAffineFlow - hasTranslation: Expecting an output." );
	if ( nrhs < 3 ) mexErrMsgTxt( "MAffineFlow - hasTranslation: One or more arguments are missing." );
	if ( nrhs > 3 ) mexWarnMsgTxt( "MAffineFlow - hasTranslation: One or more arguments were ignored." );

	hypro::affineFlow<double>* flow = convertMat2Ptr<hypro::affineFlow<double>>( prhs[2] );
	const bool ans = flow->hasTranslation();
	plhs[0] = mxCreateLogicalScalar( ans );
}

void MAffineFlow::outstream( int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[] ) {
	// TODO
}

void MAffineFlow::process( int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[] ) {
	int cmd = mxGetScalar( prhs[1] );

	if ( cmd == 2 ) {
		new_empty( nlhs, plhs, nrhs, prhs );
		return;
	}
	if ( cmd == 5 ) {
		new_mat_vec( nlhs, plhs, nrhs, prhs );
		return;
	}
	if ( cmd == 3 ) {
		copy( nlhs, plhs, nrhs, prhs );
		return;
	}
	if ( cmd == 1 ) {
		delete_flow( nlhs, plhs, nrhs, prhs );
		return;
	}
	if ( cmd == 6 ) {
		type( nlhs, plhs, nrhs, prhs );
		return;
	}
	if ( cmd == 18 ) {
		hasTranslation( nlhs, plhs, nrhs, prhs );
		return;
	}
	if ( cmd == 19 ) {
		setTranslation( nlhs, plhs, nrhs, prhs );
		return;
	}
	if ( cmd == 20 ) {
		getTranslation( nlhs, plhs, nrhs, prhs );
		return;
	}
	if ( cmd == 7 ) {
		isTimed( nlhs, plhs, nrhs, prhs );
		return;
	}
	if ( cmd == 8 ) {
		isDiscrete( nlhs, plhs, nrhs, prhs );
		return;
	}
	if ( cmd == 9 ) {
		equals( nlhs, plhs, nrhs, prhs );
		return;
	}
	if ( cmd == 10 ) {
		unequals( nlhs, plhs, nrhs, prhs );
		return;
	}
	if ( cmd == 11 ) {
		outstream( nlhs, plhs, nrhs, prhs );
		return;
	}
	if ( cmd == 21 ) {
		dimension( nlhs, plhs, nrhs, prhs );
		return;
	}

	mexErrMsgTxt( "MFlow - Command not recognized." );
}