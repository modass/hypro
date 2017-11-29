#include "Transformation.h"

namespace hypro {

template <typename Number>
Transformation<Number>::Transformation (const HybridAutomaton<Number>& _hybrid) {
    const size_t CONDITION_LIMIT = 100;
    Matrix<Number> matrix_in_parser;
    Matrix<Number> matrix_calc;
    size_t m_size, i;
    DiagonalMatrixdouble Ddouble;
    Matrix<double> Vdouble;
    Matrix<double> Vinvdouble;
    Matrix<double> matrixCalcDouble;
    Matrix<Number> V;                             //backtransformation
    Matrix<Number> Vinv;                          //transformation
    Vector<Number> b_tr;                          //transformed and dumped after use
    LocationManager<Number>& locationManager = LocationManager<Number>::getInstance();
    locationSet locations;
    Location<Number>* PtrtoNewLoc;
    mTransformedHA = HybridAutomaton<Number>();
//LOCATIONS
    for (Location<Number>* LocPtr : _hybrid.getLocations() ) {
        matrix_in_parser = LocPtr->getFlow();   //copy for calculation; TODO (getsize() missing) many flows
        m_size = matrix_in_parser.cols(); //rows
        assert(m_size>=1);  //exit if location size <1-> underflow error
        m_size -= 1;
        STallValues<Number> mSTallvalues;
        declare_structures(mSTallvalues, m_size);
        b_tr        = Vector<Number>(m_size);
        matrix_calc = Matrix<Number>(m_size,m_size);
//double for rational converting here
        Ddouble     = DiagonalMatrixdouble(m_size); //formulation in .h
        Vdouble     = Matrix<double>(m_size,m_size);
        Vinvdouble  = Matrix<double>(m_size,m_size);
        matrixCalcDouble = Matrix<double>(m_size,m_size);
        V           = Matrix<Number>(m_size,m_size);
        Vinv        = Matrix<Number>(m_size,m_size);
        b_tr        = matrix_in_parser.topRightCorner(m_size,1);
        matrix_calc = matrix_in_parser.topLeftCorner(m_size,m_size);
        std::cout<<"A: "<<std::endl<<matrix_calc;
        matrixCalcDouble = convert<Number,double> (matrix_calc);
    //LOCATION TRANSFORMATION
        Eigen::EigenSolver<Matrix<double>> es(matrixCalcDouble);
        Vdouble << es.eigenvectors().real();
        Ddouble.diagonal() << es.eigenvalues().real();
        Vinvdouble = Vdouble.inverse();
        //mSTallvalues.mSTindependentFunct.D.diagonal() << es.eigenvalues().real();
        //Vinv = V.inverse();
    //ASSERTION CONDITION TODO making this faster for big/sparse matrices
        Eigen::JacobiSVD<Matrix<double>> svd(Vinvdouble);  
        double cond = svd.singularValues()(0)  / svd.singularValues()(svd.singularValues().size()-1);
        if(std::abs(cond) > CONDITION_LIMIT) {
            FATAL("hypro.eigendecomposition","condition is higher than CONDITION_LIMIT");
        }
    //TODO make assertions about V D and V^-1 and in addition to A and b if needed
        std::cout <<"Vinv(condition): ("<< cond <<")\n" << Vinv;
//TODO TESTING CONVERSION TO RATIONAL (TRACE)
        V = convert<double,Number>(Vdouble);
        Vinv = convert<double,Number>(Vinvdouble);
        TRACE("hypro.eigendecomposition","V\n" << V);
        TRACE("hypro.eigendecompositoin","Vinv\n" << Vinv);
        mSTallvalues.mSTindependentFunct.D = convert<double,Number>(Ddouble);
        //std::cout <<"D: "    << std::endl << D;
        matrix_calc = Vinv*matrix_calc;
        b_tr        = Vinv*b_tr;
        std::cout<<"Vinv*A("<<m_size<<"x"<<m_size<<"),b_tr"<<std::endl;
        matrix_in_parser.topLeftCorner(m_size,m_size) =  matrix_calc;
        matrix_in_parser.topRightCorner(m_size,1) = b_tr;   //size
        //std::cout << matrix_in_parser;
	    PtrtoNewLoc = locationManager.create(matrix_in_parser);
        locations.insert(PtrtoNewLoc);
        mLocationPtrsMap.insert(std::make_pair(LocPtr, PtrtoNewLoc));
    //SAVING STRUCT
        TRACE("hypro.eigendecomposition", "D exact:\n" << mSTallvalues.mSTindependentFunct.D.diagonal() );
        TRACE("hypro.eigendecomposition", "D approx:\n" << Ddouble.diagonal() );
        TRACE("hypro.eigendecomposition", "b_tr :\n" << b_tr );
        for( i=0; i<m_size; ++i) {
    //LINEAR BEHAVIOUR IFF D=0
    //TODO COMPARISON ALMOST EQUAL 0 [else div by 0]
            if ( mSTallvalues.mSTindependentFunct.D.diagonal()(i) == 0 ) {
                mSTallvalues.mSTindependentFunct.xinhom(i)    = b_tr(i);
            } else {
                mSTallvalues.mSTindependentFunct.xinhom(i)    = b_tr(i) / mSTallvalues.mSTindependentFunct.D.diagonal()(i);
            }
        }
    //assign values
        mSTallvalues.mSTflowpipeSegment.Vinv       = Vinv;
        mSTallvalues.mSTflowpipeSegment.V          = V;
        mLocPtrtoComputationvaluesMap.insert(std::make_pair(PtrtoNewLoc, mSTallvalues));
        //std::cout << "old loc: "<<LocPtr<<"\n";
        //std::cout << "new loc: "<<PtrtoNewLoc<<"\n";
    //INVARIANTS(TYPE CONDITION) [TODO check why assertion to Invariants fails]
        const Condition<Number>& invar1 = LocPtr->getInvariant();
        Condition<Number> invar1NEW;
        //inv: A'= A*V
        for( i=0; i<invar1.size(); ++i ) {
            invar1NEW.setMatrix(invar1.getMatrix(i)*V,i);
            invar1NEW.setVector(invar1.getVector(i)  ,i);    
        }
        PtrtoNewLoc->setInvariant(invar1NEW);
    }
    mTransformedHA.setLocations(locations); 
    //TRANSITIONS
    transitionSet transitions;
    for (Transition<Number>* TransPtr : _hybrid.getTransitions() ) {
        Transition<Number>* NewTransPtr = new Transition<Number>(*TransPtr);  //! TODO !
        //transitions not freed, shared_ptr too costly in multithreaded context
    //POINTER
        Location<Number>*   NewSourceLocPtr = mLocationPtrsMap[TransPtr->getSource()];
        Location<Number>*   NewTargetLocPtr = mLocationPtrsMap[TransPtr->getTarget()];
        const Matrix<Number> & VSource = 
          mLocPtrtoComputationvaluesMap[NewSourceLocPtr].mSTflowpipeSegment.V;
        const Matrix<Number> & VinvTarget = 
          mLocPtrtoComputationvaluesMap[NewTargetLocPtr].mSTflowpipeSegment.Vinv;

        NewTransPtr->setSource(NewSourceLocPtr);
        NewTransPtr->setTarget(NewTargetLocPtr);
    //GUARD ( when transition can be made )
        const Condition<Number>& guard1    = TransPtr->getGuard();
        Condition<Number> guard1NEW; // = NewTransPtr->getGuard();
        //inv: A'= A*V; get V from location!!
        for( i=0; i<guard1.size(); ++i ) {
            guard1NEW.setMatrix(guard1.getMatrix(i)*VSource ,i);    
            guard1NEW.setVector(guard1.getVector(i)         ,i);    
        }
        NewTransPtr->setGuard(guard1NEW);
    //RESET ( reset into new location )
        const Reset<Number>& reset1    = TransPtr->getReset();
        Reset<Number> reset1NEW;// = NewTransPtr->getReset();
        //inv: A'= V^(-1)*A
        for( i=0; i<reset1.size(); ++i ) {
            reset1NEW.setMatrix(VinvTarget * reset1.getMatrix(i),i);
            reset1NEW.setVector(VinvTarget * reset1.getVector(i),i);
        }
        NewTransPtr->setReset(reset1NEW);
        transitions.insert(NewTransPtr);
    //LOCATION HAS TRANSITIONS
        NewTargetLocPtr->addTransition(NewTransPtr);
    }
    mTransformedHA.setTransitions    (transitions);
//INITIAL STATES
    locationStateMap initialStates;
    for(typename locationStateMap::const_iterator it=_hybrid.getInitialStates().begin(); 
      it!=_hybrid.getInitialStates().end(); ++it) {
        Location<Number>* NewLocPtr = mLocationPtrsMap[it->first];
        State<Number,ConstraintSet<Number>> state1NEW = State<Number,ConstraintSet<Number>>(it->second);
        state1NEW.setTimestamp(carl::Interval<Number>(0) );
        state1NEW.setLocation(NewLocPtr);
//        initialStates.insert(make_pair(NewLocPtr, state1NEW));
        mTransformedHA.addInitialState(state1NEW);
    }
//    mTransformedHA.setInitialStates  (initialStates);
//LOCAL BAD STATES (condition [matrix,vector] matrix=matrix*V
//typename locationConditionMap::const_iterator DOES NOT WORK
    for (typename HybridAutomaton<Number>::locationConditionMap::const_iterator it = _hybrid.getLocalBadStates().begin();
      it!=_hybrid.getLocalBadStates().end(); ++it ) {
        Location<Number>* NewLocPtr = mLocationPtrsMap[it->first];
        TRACE("hypro.eigendecomposition","OldLocPtr" << it->first);
        TRACE("hypro.eigendecomposition","NewLocPtr" << NewLocPtr);
        const Condition<Number> & badState = it->second;
        TRACE("hypro.eigendecomposition","BadState" << badState);
        Condition<Number> badStateNEW;
        const Matrix<Number> & V = 
          mLocPtrtoComputationvaluesMap[NewLocPtr].mSTflowpipeSegment.V;
        for(i=0; i<badState.size(); ++i) {
            badStateNEW.setMatrix(badState.getMatrix(i)*V, i);
            badStateNEW.setVector(badState.getVector(i)  , i);
        }
        TRACE("hypro.eigendecomposition","transformed localBadState: " << badStateNEW);
        mTransformedHA.addLocalBadState(NewLocPtr, badStateNEW);
    }
    analyzeExponentialFunctions();
    //for (const auto & locBadState : mTransformedHA.getLocalBadStates() ) {
    //    TRACE("hypro.eigendecomposition","in location: " << locBadState.first)
    //    TRACE("hypro.eigendecomposition","after trafo State:" << locBadState.second);
    //}
}
template <typename Number>
void Transformation<Number>::addGlobalBadStates
  (const HybridAutomaton<Number>& _hybrid, const bool transform) {

//CHECK MATCH OF LOCATION PTRS
    assert( _hybrid.getLocations().size() == mLocationPtrsMap.size() );
    typename locationPtrMap::const_iterator locIt = mLocationPtrsMap.begin();
    typename locationPtrMap::const_iterator endLocIt = mLocationPtrsMap.end();
    for (typename locationSet::iterator origIt=_hybrid.getLocations().begin();
      origIt!=_hybrid.getLocations().end(); ++origIt) {
        //locations is set -> value is ptrs to origLoc, in map key is ptrs to origLoc
        assert( *origIt == locIt->first );
        if(locIt != endLocIt)
            ++locIt;
    }
    assert( locIt == endLocIt );
//TRANSFORMATION -> add to localBadStates
    if (transform) {
        assert (!globalBadStatesTransformed);
        //TODO MEMORY ASSERTION?
        size_t i;
        for (typename conditionVector::const_iterator it = _hybrid.getGlobalBadStates().begin(); 
          it!=_hybrid.getGlobalBadStates().end(); ++it) {
            //transform each global badstate by setting of according localBadState
           //1. loop through global states
           //2. loop through ptr to location map and create new localBadState
            for (typename locationPtrMap::iterator locMapIt = mLocationPtrsMap.begin(); 
              locMapIt!=mLocationPtrsMap.end(); ++locMapIt) {
                Condition<Number> badStateNEW;
                const Matrix<Number> & V = mLocPtrtoComputationvaluesMap[locMapIt->second].mSTflowpipeSegment.V;
                for(i=0; i<it->size(); ++i) {
                    badStateNEW.setMatrix(it->getMatrix(i)*V, i);
                    badStateNEW.setVector(it->getVector(i)  , i);
                }
                mTransformedHA.addLocalBadState(locMapIt->second, badStateNEW);
            }
        }
    globalBadStatesTransformed = true;
    } else {
//NO TRANSFORMATION -> add to globalBadStates
        size_t i;
        for (typename conditionVector::const_iterator it = _hybrid.getGlobalBadStates().begin(); 
          it!=_hybrid.getGlobalBadStates().end(); ++it) {
           //loop through global states + copy to other HybridAutomaton
            for (typename locationPtrMap::iterator locMapIt = mLocationPtrsMap.begin(); 
              locMapIt!=mLocationPtrsMap.end(); ++locMapIt) {
                Condition<Number> globalbadStateNEW;
                for(i=0; i<it->size(); ++i) {
                    globalbadStateNEW.setMatrix(it->getMatrix(i), i);
                    globalbadStateNEW.setVector(it->getVector(i), i);
                }
                mTransformedHA.addGlobalBadState(globalbadStateNEW);
            }
        }
    }
}
template <typename Number>
void Transformation<Number>::analyzeExponentialFunctions() {
//TODO Location multiple flows and states (map of states to flows)
    for ( auto &structObject : mLocPtrtoComputationvaluesMap) {
        const size_t dimension = structObject.second.mSTflowpipeSegment.V.rows();
        structObject.second.mSTindependentFunct.expFunctionType.reserve(dimension);
        TRACE("hypro.eigendecomposition","looping through " << (structObject.first));//ptr-adress of location
        //TRACE("hypro.eigendecomposition","Loc: " << V);
        for(size_t i=0; i<dimension; ++i) {
            //1. divergence D>0, 2. convergence D<0, 3. linear D=0
            //4. const [never used yet since we can then not use decomposition]
        //TODO close to 0
            if(structObject.second.mSTindependentFunct.D.diagonal()(i) == 0) {
                structObject.second.mSTindependentFunct.expFunctionType[i] = EXP_FUNCT_TYPE::LINEAR;
                //TRACE("hypro.eigendecomposition","linear behaviour of " << i);
            } else {
                if(structObject.second.mSTindependentFunct.D.diagonal()(i) < 0) {
                    structObject.second.mSTindependentFunct.expFunctionType[i] = EXP_FUNCT_TYPE::CONVERGENT;
                    //TRACE("hypro.eigendecomposition","V\n" << V);
                    //TRACE("hypro.eigendecomposition","V\n" << V);
                } else {
                    structObject.second.mSTindependentFunct.expFunctionType[i] = EXP_FUNCT_TYPE::DIVERGENT;
                    //TRACE("hypro.eigendecomposition","V\n" << V);
                    //TRACE("hypro.eigendecomposition","V\n" << V);
                }
            }
        }
    }
}

//REACHABILITY WORKER INHERITS from IWorker and MAutomaton(automaton)

//method processTask
//TASK given as shared ptr: is struct of shared_ptr, tBacktrackingInfo
//tBacktrackingInfo has hypro::Path + btLevel + currentBTPosition

template <typename Number>
void Transformation<Number>::declare_structures(STallValues<Number>& mSTallValues, const int n) {
    //mSTallValues.mSTinputVectors.x0          = Vector<Number>(n);
    //mSTallValues.mSTinputVectors.x0_2        = Vector<Number>(n);
    mSTallValues.mSTindependentFunct.D       = DiagonalMatrix<Number>(n);
    mSTallValues.mSTindependentFunct.xinhom  = Vector<Number>(n);
    //mSTallValues.mSTdependentFunct.xhom      = Matrix<Number>(n,n);
    //TODO change to multiple values, last column for transformation maxmin
    //mSTallValues.mSTdependentFunct.x_tr      = Matrix<Number>::Zero(n,3);    
    //mSTallValues.mSTdependentFunct.x_tr.col(2).array() = 0;
    //mSTallValues.mSTevalFunctions.deriv      = Matrix<Number>(n,n);
    //TODO CHANGE 2 to number of points ?! or use 2 column with numbers: 
    //[max,min] for direct [-max,-min] for indirect starting at !!1!!
    //mSTallValues.mSTevalFunctions.direct     = BoolMatrix(n,2);
    //mSTallValues.mSTevalFunctions.direct.setConstant(0);
    mSTallValues.mSTflowpipeSegment.V        = Matrix<Number>(n,n);
    mSTallValues.mSTflowpipeSegment.Vinv     = Matrix<Number>(n,n);
    //delta, deltalimit of STindependentFunc missing
    //flowpipe only V for backtransformation into original system
}
/*
//mark if in transformed system x0<x0_2 in 3rd column
template <typename Number>
void Transformation<Number>::mark_x0isMin(Matrix<Number>& x_tr, const int n) {
    for(int i=0; i<n; ++i) {   //check if x0_tr >= x0_2_tr
        if(x_tr(i,0) < x_tr(i,1)) {
            x_tr(i,2) = 1; //mark second column to recognize later
        }
    }
}
//x0<x0_2 will never change, so we can simply swap to transform systems
template <typename Number>
void Transformation<Number>::swap_x0isMax(Matrix<Number>& x_tr, const int n) {
    //std::cout << "x_tr beforeback: "<< std::endl << x_tr << std::endl;
    Vector<Number> tmp = Vector<Number>(n);
    for(int i=0; i<n; ++i) {
        if(x_tr(i,2) == 1) {
            tmp(i)    = x_tr(i,0);
            x_tr(i,0) = x_tr(i,1);
            x_tr(i,1) = tmp(i);
        }
    }
    //std::cout << "x_tr afterback: "<< std::endl << x_tr << std::endl;
}
*/
template <typename Number>
void Transformation<Number>::output_HybridAutomaton() {
    std::cout << mTransformedHA << "\n-------------- ENDOFAUTOMATA ------------------" << std::endl;
}
} //namespace hypro
