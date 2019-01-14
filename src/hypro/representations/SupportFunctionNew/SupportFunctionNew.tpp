#pragma once

#include "SupportFunctionNew.h"

namespace hypro {

	/***************************************************************************
	 * Constructors
	 **************************************************************************/

	template<typename Number, typename Converter, typename Setting>
	SupportFunctionNewT<Number,Converter,Setting>::SupportFunctionNewT() {
		//do nothing
	}

	//copy constructor
	template<typename Number, typename Converter, typename Setting>
	SupportFunctionNewT<Number,Converter,Setting>::SupportFunctionNewT( const SupportFunctionNewT<Number,Converter,Setting>& orig ) : mRoot(orig.getRoot()) {
		//handled by initializer list
	}

	//move constructor
	template<typename Number, typename Converter, typename Setting>
	SupportFunctionNewT<Number,Converter,Setting>::SupportFunctionNewT( SupportFunctionNewT<Number,Converter,Setting>&& orig ) {
		mRoot = std::move(orig.getRoot());
		orig.clear();
	}

	/***************************************************************************
	 * Getters & setters
	 **************************************************************************/

	template<typename Number, typename Converter, typename Setting>
	void SupportFunctionNewT<Number,Converter,Setting>::addOperation(RootGrowNode<Number,Setting>* newRoot) const {
		assert(newRoot->getOriginCount() == 1);
		if(newRoot){
			newRoot->addToChildren(mRoot);
			assert(newRoot->getChildren().size() == 1);
		}
	}

	template<typename Number, typename Converter, typename Setting>
	void SupportFunctionNewT<Number,Converter,Setting>::addOperation(RootGrowNode<Number,Setting>* newRoot, const std::vector<SupportFunctionNewT<Number,Converter,Setting>>& rhs) const {
		assert(newRoot != nullptr);
		assert(newRoot->getOriginCount() <= rhs.size() + newRoot->getChildren().size() + 1);
		newRoot->addToChildren(mRoot);
		for(const auto& sf : rhs){
			newRoot->addToChildren(sf.getRoot());
		}
		assert(newRoot->getChildren().size() >= newRoot->getOriginCount());
	}

	/***************************************************************************
	 * Tree Traversal
	 **************************************************************************/

	//When Result type and Param type = void
	template<typename Number, typename Converter, typename Setting>
	void SupportFunctionNewT<Number,Converter,Setting>::traverse( 	
		std::function<void(RootGrowNode<Number,Setting>*)>& transform,
		std::function<void(RootGrowNode<Number,Setting>*)>& compute, 	
		std::function<void(RootGrowNode<Number,Setting>*)>& aggregate) const 
	{
		std::function<Parameters<>(RootGrowNode<Number,Setting>*, Parameters<>)> tNotVoid = [&](RootGrowNode<Number,Setting>* n, Parameters<> p) -> Parameters<> { transform(n); return Parameters<>(); };
		std::function<Parameters<>(RootGrowNode<Number,Setting>*, Parameters<>)> cNotVoid = [&](RootGrowNode<Number,Setting>* n, Parameters<> p) -> Parameters<> { compute(n); return Parameters<>(); };
		std::function<Parameters<>(RootGrowNode<Number,Setting>*, std::vector<Parameters<>>, Parameters<>)> aNotVoid = [&](RootGrowNode<Number,Setting>* n, std::vector<Parameters<>> v, Parameters<> p) -> Parameters<> { aggregate(n); return Parameters<>(); };
		traverse(tNotVoid, cNotVoid, aNotVoid, Parameters<>());
	}

	//When Param type = void, but Result type not
	template<typename Number, typename Converter, typename Setting>
	template<typename Result>
	Result SupportFunctionNewT<Number,Converter,Setting>::traverse(	
		std::function<void(RootGrowNode<Number,Setting>*)>& transform,
		std::function<Result(RootGrowNode<Number,Setting>*)>& compute, 
		std::function<Result(RootGrowNode<Number,Setting>*, std::vector<Result>)>& aggregate) const 
	{	
		std::function<Parameters<>(RootGrowNode<Number,Setting>*, Parameters<>)> tNotVoid = [&](RootGrowNode<Number,Setting>* n, Parameters<> ) -> Parameters<> { transform(n); return Parameters<>(); };
		std::function<Result(RootGrowNode<Number,Setting>*, Parameters<>)> cNotVoid = [&](RootGrowNode<Number,Setting>* n, Parameters<> ) -> Result { return compute(n); };
		std::function<Result(RootGrowNode<Number,Setting>*, std::vector<Result>, Parameters<>)> aWithParams = [&](RootGrowNode<Number,Setting>* n, std::vector<Result> v, Parameters<> ) -> Result { return aggregate(n,v); };
		Parameters<> noInitParams = Parameters<>();
		return traverse(tNotVoid, cNotVoid, aWithParams, noInitParams);
	}

	//When Result type = void, but Param type not
	template<typename Number, typename Converter, typename Setting>
	template<typename ...Rargs>
	void SupportFunctionNewT<Number,Converter,Setting>::traverse(	
		std::function<Parameters<Rargs...>(RootGrowNode<Number,Setting>*, Parameters<Rargs...>)>& transform,
		std::function<void(RootGrowNode<Number,Setting>*, Parameters<Rargs...>)>& compute, 
		std::function<void(RootGrowNode<Number,Setting>*, Parameters<Rargs...>)>& aggregate,
		Parameters<Rargs...>& initParams) const 
	{
		std::function<Parameters<>(RootGrowNode<Number,Setting>*, Parameters<Rargs...>)> cNotVoid = [&](RootGrowNode<Number,Setting>* n, Parameters<Rargs...> p) -> Parameters<>{ compute(n,p); return Parameters<>(); };
		std::function<Parameters<>(RootGrowNode<Number,Setting>*, std::vector<Parameters<>>, Parameters<Rargs...>)> aNotVoid = [&](RootGrowNode<Number,Setting>* n, std::vector<Parameters<>> v, Parameters<Rargs...> p) -> Parameters<> { aggregate(n,p); return Parameters<>(); };
		traverse(transform, cNotVoid, aNotVoid, initParams);
	}

	//Actual traverse function. Result type and Param type not void
	template<typename Number, typename Converter, typename Setting>
	template<typename Result, typename ...Rargs>
	Result SupportFunctionNewT<Number,Converter,Setting>::traverse(
		std::function<Parameters<Rargs...>(RootGrowNode<Number,Setting>*, Parameters<Rargs...>)>& transform,
		std::function<Result(RootGrowNode<Number,Setting>*, Parameters<Rargs...>)>& compute, 
		std::function<Result(RootGrowNode<Number,Setting>*, std::vector<Result>, Parameters<Rargs...>)>& aggregate, 
		Parameters<Rargs...>& initParams) const
	{ 
		//Usings
		using Node = RootGrowNode<Number,Setting>*;
		using Param = Parameters<Rargs...>;
		using Res = Result;

		//Prepare Stacks
		std::vector<Node> callStack;
		std::vector<Param> paramStack;
		std::vector<std::pair<int,std::vector<Res>>> resultStack;  
		callStack.push_back(mRoot.get());
		paramStack.push_back(initParams);
		resultStack.push_back(std::make_pair(-1, std::vector<Res>()));

		//Traversal loop
		while(!callStack.empty()){

			Node cur = callStack.back();
			Param currentParam = paramStack.back();

			if(cur->getOriginCount() == 0){

				std::pair<int,std::vector<Res>> currentResult = resultStack.back();

				//If leaf and end of stack is reached
				if(currentResult.first == -1){

					return std::apply(compute, std::make_pair(cur, currentParam));
					
				//If leaf and not end of stack is reached	
				} else {

					resultStack.at(currentResult.first).second.emplace_back(std::apply(compute, std::make_pair(cur, currentParam)));

				}

				// delete result frame and close recursive call
				callStack.pop_back();
				paramStack.pop_back();
				resultStack.pop_back();

			} else {

				//If enough arguments for operation of node and #arguments != 0
				//if(resultStack.back().second.size() == cur->getOriginCount()) {
				if(resultStack.back().second.size() >= cur->getOriginCount()) {

					Res accumulatedResult = std::apply(aggregate, std::make_tuple(cur, resultStack.back().second, currentParam));

					// we reached the top, exit
					if(resultStack.back().first == -1) {
						return accumulatedResult;
					}

					// forward result.
					resultStack.at(resultStack.back().first).second.emplace_back(accumulatedResult);

					// delete result frame and close recursive call
					callStack.pop_back();
					paramStack.pop_back();
					resultStack.pop_back();

				//Every other case (recursive call)
				} else {

					// here we create the new stack levels.
					std::size_t callingFrame = callStack.size() - 1;
					for(auto c : cur->getChildren()){
						callStack.push_back(c.get());
						paramStack.push_back(std::apply(transform, std::make_pair(cur, currentParam)));
						resultStack.push_back(std::make_pair(callingFrame, std::vector<Res>()));
					}
				}
			}
		}
		std::cout << "THIS SHOULD NOT HAPPEN." << std::endl;
		return Res();
	}

	/***************************************************************************
	 * Evaluation
	 **************************************************************************/

	template<typename Number, typename Converter, typename Setting>
	bool SupportFunctionNewT<Number,Converter,Setting>::empty() const {

		if(mRoot == nullptr) return true;
		
		//first function - parameters are not transformed
		std::function<void(RootGrowNode<Number,Setting>*)> doNothing = [](RootGrowNode<Number,Setting>* ){ };

		//if leaf - call empty function of representation
		std::function<bool(RootGrowNode<Number,Setting>*)> leafEmpty = 
			[](RootGrowNode<Number,Setting>* n) -> bool {
				return n->empty();
			};

		//if not leaf - not empty if all children not empty
		std::function<bool(RootGrowNode<Number,Setting>*, std::vector<bool>)> childrenEmpty =
			[](RootGrowNode<Number,Setting>* n, std::vector<bool> childrenEmpty) -> bool {
				return n->empty(childrenEmpty);
				//for(auto child : childrenEmpty){
				//	if(child) return true;
				//}
				//return false;
			};

		return traverse(doNothing, leafEmpty, childrenEmpty);
	}

	template<typename Number, typename Converter, typename Setting>
	Number SupportFunctionNewT<Number,Converter,Setting>::supremum() const {

		if(mRoot == nullptr) return Number(0);

		//first function - parameters are not transformed
		std::function<void(RootGrowNode<Number,Setting>*)> doNothing = [](RootGrowNode<Number,Setting>* ){ };

		//leaves compute the supremum point
		std::function<Point<Number>(RootGrowNode<Number,Setting>*)> supremumPointLeaf = 
			[](RootGrowNode<Number,Setting>* n) -> Point<Number> {
				assert(n->getType() == SFNEW_TYPE::LEAF);
				return n->supremumPoint();
			};

		//operations call their own supremum functions
		std::function<Point<Number>(RootGrowNode<Number,Setting>*, std::vector<Point<Number>>)> supremumPointOp =
			[](RootGrowNode<Number,Setting>* n, std::vector<Point<Number>> v) -> Point<Number> {
				return n->supremumPoint(v);
			};
	
		Point<Number> res = traverse(doNothing, supremumPointLeaf, supremumPointOp);
		Number resNorm = Point<Number>::inftyNorm(res);
		return resNorm;
	}

	template<typename Number, typename Converter, typename Setting>
	std::vector<Point<Number>> SupportFunctionNewT<Number,Converter,Setting>::vertices( const matrix_t<Number>& additionalDirections ) const {
		std::vector<vector_t<Number>> additionalDirectionVector;
		for(unsigned rowIndex = 0; rowIndex < additionalDirections.rows(); ++rowIndex){
			additionalDirectionVector.push_back(vector_t<Number>(additionalDirections.row(rowIndex)));
		}
		auto tmp = Converter::toHPolytope(*this, additionalDirectionVector);
		return tmp.vertices();
	}

	template<typename Number, typename Converter, typename Setting>
	EvaluationResult<Number> SupportFunctionNewT<Number,Converter,Setting>::evaluate( const vector_t<Number>& _direction, bool useExact) const {
		if(mRoot == nullptr) return EvaluationResult<Number>();
		matrix_t<Number> dirAsMatrix = matrix_t<Number>::Zero(1,_direction.rows());
		dirAsMatrix.row(0) = _direction;
		return multiEvaluate(dirAsMatrix, useExact).front();
	}

	template<typename Number, typename Converter, typename Setting>
	std::vector<EvaluationResult<Number>> SupportFunctionNewT<Number,Converter,Setting>::multiEvaluate( const matrix_t<Number>& _directions, bool useExact ) const {

		if(mRoot == nullptr) return std::vector<EvaluationResult<Number>>();

		//Define lambda functions that will call the functions transform, compute and aggregate dependent on the current node type
		std::function<Parameters<matrix_t<Number>>(RootGrowNode<Number,Setting>*, Parameters<matrix_t<Number>>)> trans = 
			[](RootGrowNode<Number,Setting>* n, Parameters<matrix_t<Number>> param) -> Parameters<matrix_t<Number>> { 
				return Parameters<matrix_t<Number>>(n->transform(std::get<0>(param.args))); 
			};
		std::function<std::vector<EvaluationResult<Number>>(RootGrowNode<Number,Setting>*, Parameters<matrix_t<Number>>)> comp = 
			[&](RootGrowNode<Number,Setting>* n, Parameters<matrix_t<Number>> dir) -> std::vector<EvaluationResult<Number>> { 
				return n->compute(std::get<0>(dir.args), useExact); 
			};
		std::function<std::vector<EvaluationResult<Number>>(RootGrowNode<Number,Setting>*, std::vector<std::vector<EvaluationResult<Number>>>, Parameters<matrix_t<Number>>)> agg = 
			[](RootGrowNode<Number,Setting>* n, std::vector<std::vector<EvaluationResult<Number>>> resultStackBack, Parameters<matrix_t<Number>> currentParam) -> std::vector<EvaluationResult<Number>> { 
				return n->aggregate(resultStackBack, std::get<0>(currentParam.args)); 
			};

		//Traverse the underlying SupportFunctionNewT with the three functions and given directions as initial parameters.
		Parameters<matrix_t<Number>> params = Parameters<matrix_t<Number>>(_directions);
		return traverse(trans, comp, agg, params);
	}

	//Find out if tree has at least one trafoOp and if yes, update the linTrafoParameters
	template<typename Number, typename Converter, typename Setting>
	bool SupportFunctionNewT<Number,Converter,Setting>::hasTrafo(std::shared_ptr<const LinTrafoParameters<Number,Setting>>& ltParam, const matrix_t<Number>& A, const vector_t<Number>& b) const {

		if(mRoot == nullptr) return false;

		//first function - parameters are not transformed
		std::function<void(RootGrowNode<Number,Setting>*)> doNothing = [](RootGrowNode<Number,Setting>* ){ };

		//second function - leaves cannot be operations
		std::function<bool(RootGrowNode<Number,Setting>*)> leavesAreNotTrafoOps =
			[](RootGrowNode<Number,Setting>* ) -> bool {
				return false;
			};

		//third function - if current node type or given result is TRAFO, then update and return true
		std::function<bool(RootGrowNode<Number,Setting>*, std::vector<bool>)> checkAndUpdateTrafo =
			[&](RootGrowNode<Number,Setting>* n, std::vector<bool> haveSubtreesTrafo) -> bool {
				if(n->getType() == SFNEW_TYPE::TRAFO){
					return static_cast<TrafoOp<Number,Converter,Setting>*>(n)->hasTrafo(ltParam, A, b);
				} else {
					for(auto hasSubTreeTrafo : haveSubtreesTrafo){
						if(hasSubTreeTrafo){
							return true;	
						} 
					}
				}
				return false;
			};

		return traverse(doNothing, leavesAreNotTrafoOps, checkAndUpdateTrafo);
	}

	/***************************************************************************
	 * General Interface
	 **************************************************************************/

	template<typename Number, typename Converter, typename Setting>
	std::size_t SupportFunctionNewT<Number,Converter,Setting>::dimension() const {
		if(mRoot == nullptr) return std::size_t(0);
		return mRoot->getDimension();
	}

	//template<typename Number, typename Converter, typename Setting>
	//void SupportFunctionNewT<Number,Converter,Setting>::removeRedundancy() {
	//	// Support functions are already non-redundant (Polytope support functions are made non-redundant upon construction).
	//}

	template<typename Number, typename Converter, typename Setting>
	std::size_t SupportFunctionNewT<Number,Converter,Setting>::size() const {

		if(mRoot == nullptr) return std::size_t(0);
		
		//first function - parameters are not transformed
		std::function<void(RootGrowNode<Number,Setting>*)> doNothing = [](RootGrowNode<Number,Setting>* ){ };

		//leaves compute their storage size
		std::function<std::size_t(RootGrowNode<Number,Setting>*)> sizeofLeaf = 
			[](RootGrowNode<Number,Setting>* n) -> std::size_t {
				assert(n->getType() == SFNEW_TYPE::LEAF);
				return sizeof(*n);
			};

		//operations compute their storage size and add it to the ones of their children
		std::function<std::size_t(RootGrowNode<Number,Setting>*, std::vector<std::size_t>)> sizeofOp =
			[](RootGrowNode<Number,Setting>* n, std::vector<std::size_t> v) -> std::size_t {
				std::size_t storage = 0;
				for(const auto& childStorageSize : v){
					storage += childStorageSize;
				}
				return storage + sizeof(*n);
			};
	
		return traverse(doNothing, sizeofLeaf, sizeofOp);
	}

	//template<typename Number, typename Converter, typename Setting>
	//const SupportFunctionNewT<Number,Converter,Setting>& SupportFunctionNewT<Number,Converter,Setting>::reduceNumberRepresentation() { }

	template<typename Number, typename Converter, typename Setting>
	std::pair<CONTAINMENT, SupportFunctionNewT<Number,Converter,Setting>> SupportFunctionNewT<Number,Converter,Setting>::satisfiesHalfspace( const Halfspace<Number>& rhs ) const {
		bool limiting = false;
    	EvaluationResult<Number> planeEvalRes = this->evaluate(rhs.normal(), false);
    	if(planeEvalRes.errorCode == SOLUTION::INFEAS){
			//std::cout << "Is infeasible (should not happen)." << std::endl;
			//std::cout << "Set is (Hpoly): " << std::endl << Converter::toHPolytope(*this) << std::endl;
			//assert(Converter::toHPolytope(*this).empty());
    		return std::make_pair(CONTAINMENT::NO, *this);
    	} else if(planeEvalRes.supportValue > rhs.offset()){
			//std::cout << "Object will be limited. " << std::endl;
    		// the actual object will be limited by the new plane
    		limiting = true;
			// std::cout << "evaluate(" << convert<Number,double>(-(_mat.row(rowI))) << ") <=  " << -(_vec(rowI)) << ": " << this->evaluate(-(_mat.row(rowI))).supportValue << " <= " << -(_vec(rowI)) << std::endl;
    		// std::cout << __func__ <<  ": Limiting plane " << convert<Number,double>(_mat.row(rowI)).transpose() << " <= " << carl::toDouble(_vec(rowI)) << std::endl;
            if(this->evaluate(-(rhs.normal()), false ).supportValue < -(rhs.offset())){
				//std::cout << "fullyOutside" << std::endl;
                // the object lies fully outside one of the planes -> return false
                return std::make_pair(CONTAINMENT::NO, this->intersectHalfspace(rhs));
            }
    	}
    	if(limiting){
        	return std::make_pair(CONTAINMENT::PARTIAL, this->intersectHalfspace(rhs));
    	} else {
    		return std::make_pair(CONTAINMENT::FULL, *this);
    	}
	}

	template<typename Number, typename Converter, typename Setting>
	std::pair<CONTAINMENT, SupportFunctionNewT<Number,Converter,Setting>> SupportFunctionNewT<Number,Converter,Setting>::satisfiesHalfspaces( const matrix_t<Number>& _mat, const vector_t<Number>& _vec ) const {
		TRACE("hypro.representations.supportFunction","Matrix: " << _mat << std::endl << " <= " << _vec );
		if(_mat.rows() == 0) {
			return std::make_pair(CONTAINMENT::FULL, *this);
		}
		assert(_mat.rows() == _vec.rows());
        std::vector<unsigned> limitingPlanes;
        for(unsigned rowI = 0; rowI < _mat.rows(); ++rowI) {
        	TRACE("hypro.representations.supportFunction", "Evaluate against plane " << rowI );
        	EvaluationResult<Number> planeEvalRes = this->evaluate(_mat.row(rowI), false);
        	TRACE("hypro.representations.supportFunction", "Return from evaluate." );
        	if(planeEvalRes.errorCode == SOLUTION::INFEAS){
				TRACE("hypro.representations.supportFunction", "Is infeasible (should not happen)." );
				//TRACE("hypro.representations.supportFunction", "Set is (Hpoly): " << std::endl << Converter::toHPolytope(*this) );
        		return std::make_pair(CONTAINMENT::NO, *this);
        	//} else if(!carl::AlmostEqual2sComplement(planeEvalRes.supportValue, _vec(rowI), 2) && planeEvalRes.supportValue > _vec(rowI)){
        	} else if(planeEvalRes.supportValue > _vec(rowI)){
				TRACE("hypro.representations.supportFunction", "Object will be limited, as " << planeEvalRes.supportValue << " > " << _vec(rowI));
        		// the actual object will be limited by the new plane
        		limitingPlanes.push_back(rowI);
				Number invDirVal = this->evaluate(-(_mat.row(rowI)), false).supportValue;
				//TRACE("hypro.representations.supportFunction", "evaluate(" << -(_mat.row(rowI)) << ") <=  " << -(_vec(rowI)) << ": " << invDirVal << " <= " << -(_vec(rowI)));
        		//TRACE("hypro.representations.supportFunction", ": Limiting plane " << _mat.row(rowI).transpose() << " <= " << carl::toDouble(_vec(rowI)));

	            //if(!carl::AlmostEqual2sComplement(invDirVal, Number(-(_vec(rowI))), 2) && invDirVal < -(_vec(rowI))) {
	            if(invDirVal < -(_vec(rowI))) {
	            	// exact verification in case the values are close to each other
	            	if(carl::AlmostEqual2sComplement(Number(-invDirVal), planeEvalRes.supportValue, 16)) {
	            		EvaluationResult<Number> secndPosEval = this->evaluate(_mat.row(rowI), true);
	            		if(secndPosEval.supportValue > _vec(rowI)) {
	            			EvaluationResult<Number> secndNegEval = this->evaluate(-(_mat.row(rowI)), true);
	            			if(secndNegEval.supportValue < -(_vec(rowI))) {
	            				TRACE("hypro.representations.supportFunction", "fullyOutside" );
				                // the object lies fully outside one of the planes -> return false
				                return std::make_pair(CONTAINMENT::NO, this->intersectHalfspaces(_mat,_vec) );
	            			}
	            		}
	            	} else {
	            		// the values are far enough away from each other to make this result a false negative.
	            		TRACE("hypro.representations.supportFunction", "fullyOutside, as " << invDirVal << " >= " << -(_vec(rowI)) );
		                // the object lies fully outside one of the planes -> return false
		                return std::make_pair(CONTAINMENT::NO, this->intersectHalfspaces(_mat,_vec) );
	            	}
	            }
        	}
        }
    	if(limitingPlanes.size() < unsigned(_mat.rows())){
    		if(limitingPlanes.size() == 0 ){
    			TRACE("hypro.representations.supportFunction", " Object will stay the same");
    			return std::make_pair(CONTAINMENT::FULL, *this);
    		}
    		TRACE("hypro.representations.supportFunction", " Object will be limited but not empty (" << limitingPlanes.size() << " planes)");
    		// if the result is not fullyOutside, only add planes, which affect the object
        	matrix_t<Number> planes = matrix_t<Number>(limitingPlanes.size(), _mat.cols());
        	vector_t<Number> distances = vector_t<Number>(limitingPlanes.size());
        	for(unsigned i = 0; i < distances.rows(); ++i){
        		// std::cout << "Set row " << i << " to plane " << limitingPlanes.back() << std::endl;
        		planes.row(i) = _mat.row(limitingPlanes.back());
        		distances(i) = _vec(limitingPlanes.back());
        		limitingPlanes.pop_back();
        	}
			assert(limitingPlanes.empty());
        	TRACE("hypro.representations.supportFunction", "Intersect with " << planes << ", " << distances);
        	return std::make_pair(CONTAINMENT::PARTIAL, this->intersectHalfspaces(planes,distances));
    	} else {
    		TRACE("hypro.representations.supportFunction", " Object will be fully limited but not empty");
    		assert(limitingPlanes.size() == unsigned(_mat.rows()));
    		TRACE("hypro.representations.supportFunction", "Intersect with " << _mat << ", " << _vec);
    		return std::make_pair(CONTAINMENT::PARTIAL, this->intersectHalfspaces(_mat,_vec));
    	}
	}

	template<typename Number, typename Converter, typename Setting>
	SupportFunctionNewT<Number,Converter,Setting> SupportFunctionNewT<Number,Converter,Setting>::project(const std::vector<std::size_t>& dimensions) const {
		
		if(mRoot == nullptr) return SupportFunctionNewT<Number,Converter,Setting>();

		// check for full projection
		bool fullProjection = true;
		if(dimensions.size() == this->dimension()) {
			for(unsigned i = 0; i < this->dimension(); ++i) {
				if(dimensions.at(i) != i) {
					fullProjection = false;
					break;
				}
			}
		} else {
			fullProjection = false;
		}
		if(!fullProjection){
			DEBUG("hypro.representations.supportFunction", "No full projection, create.");
			std::shared_ptr<ProjectOp<Number,Converter,Setting>> proj = std::make_shared<ProjectOp<Number,Converter,Setting>>(*this, dimensions);
			std::shared_ptr<RootGrowNode<Number,Setting>> projPtr = std::static_pointer_cast<RootGrowNode<Number,Setting>>(proj);
			SupportFunctionNewT<Number,Converter,Setting> sf = SupportFunctionNewT<Number,Converter,Setting>(projPtr);
			return sf;
		}
		DEBUG("hypro.representations.supportFunction", "Full projection, copy.");
		return *this;
	}

	template<typename Number, typename Converter, typename Setting>
	SupportFunctionNewT<Number,Converter,Setting> SupportFunctionNewT<Number,Converter,Setting>::linearTransformation( const matrix_t<Number>& A ) const {
		std::shared_ptr<TrafoOp<Number,Converter,Setting>> trafo = std::make_shared<TrafoOp<Number,Converter,Setting>>(*this, A, vector_t<Number>::Zero(A.rows()));
		std::shared_ptr<RootGrowNode<Number,Setting>> trafoPtr = std::static_pointer_cast<RootGrowNode<Number,Setting>>(trafo);
		SupportFunctionNewT<Number,Converter,Setting> sf = SupportFunctionNewT<Number,Converter,Setting>(trafoPtr);
		return sf;
	}

	template<typename Number, typename Converter, typename Setting>
	SupportFunctionNewT<Number,Converter,Setting> SupportFunctionNewT<Number,Converter,Setting>::affineTransformation( const matrix_t<Number>& A, const vector_t<Number>& b ) const {
		std::shared_ptr<TrafoOp<Number,Converter,Setting>> trafo = std::make_shared<TrafoOp<Number,Converter,Setting>>(*this, A, b);
		std::shared_ptr<RootGrowNode<Number,Setting>> trafoPtr = std::static_pointer_cast<RootGrowNode<Number,Setting>>(trafo);
		SupportFunctionNewT<Number,Converter,Setting> sf = SupportFunctionNewT<Number,Converter,Setting>(trafoPtr);
		return sf;
	}

	template<typename Number, typename Converter, typename Setting>
	SupportFunctionNewT<Number,Converter,Setting> SupportFunctionNewT<Number,Converter,Setting>::minkowskiSum( const SupportFunctionNewT<Number,Converter,Setting>& rhs ) const {
		std::shared_ptr<SumOp<Number,Converter,Setting>> sum = std::make_shared<SumOp<Number,Converter,Setting>>(*this, rhs);
		std::shared_ptr<RootGrowNode<Number,Setting>> sumPtr = std::static_pointer_cast<RootGrowNode<Number,Setting>>(sum);
		SupportFunctionNewT<Number,Converter,Setting> sf = SupportFunctionNewT<Number,Converter,Setting>(sumPtr);
		return sf;
	}

	template<typename Number, typename Converter, typename Setting>
	SupportFunctionNewT<Number,Converter,Setting> SupportFunctionNewT<Number,Converter,Setting>::minkowskiSum( const std::vector<SupportFunctionNewT<Number,Converter,Setting>>& rhs ) const {
		std::shared_ptr<SumOp<Number,Converter,Setting>> sum = std::make_shared<SumOp<Number,Converter,Setting>>(*this, rhs);
		std::shared_ptr<RootGrowNode<Number,Setting>> sumPtr = std::static_pointer_cast<RootGrowNode<Number,Setting>>(sum);
		SupportFunctionNewT<Number,Converter,Setting> sf = SupportFunctionNewT<Number,Converter,Setting>(sumPtr);
		return sf;	
	}

	template<typename Number, typename Converter, typename Setting>
	SupportFunctionNewT<Number,Converter,Setting> SupportFunctionNewT<Number,Converter,Setting>::scale( const Number& factor ) const {
		std::shared_ptr<ScaleOp<Number,Converter,Setting>> scale = std::make_shared<ScaleOp<Number,Converter,Setting>>(*this, factor);
		std::shared_ptr<RootGrowNode<Number,Setting>> scalePtr = std::static_pointer_cast<RootGrowNode<Number,Setting>>(scale);
		SupportFunctionNewT<Number,Converter,Setting> sf = SupportFunctionNewT<Number,Converter,Setting>(scalePtr);
		return sf;	
	}

	template<typename Number, typename Converter, typename Setting>
	SupportFunctionNewT<Number,Converter,Setting> SupportFunctionNewT<Number,Converter,Setting>::intersect( const SupportFunctionNewT<Number,Converter,Setting>& rhs ) const {
		std::shared_ptr<IntersectOp<Number,Converter,Setting>> intersec = std::make_shared<IntersectOp<Number,Converter,Setting>>(*this, rhs);
		std::shared_ptr<RootGrowNode<Number,Setting>> intersecPtr = std::static_pointer_cast<RootGrowNode<Number,Setting>>(intersec);
		SupportFunctionNewT<Number,Converter,Setting> sf = SupportFunctionNewT<Number,Converter,Setting>(intersecPtr);
		return sf;	
	}

	template<typename Number, typename Converter, typename Setting>
	SupportFunctionNewT<Number,Converter,Setting> SupportFunctionNewT<Number,Converter,Setting>::intersect( const std::vector<SupportFunctionNewT<Number,Converter,Setting>>& rhs ) const {
		std::shared_ptr<IntersectOp<Number,Converter,Setting>> intersec = std::make_shared<IntersectOp<Number,Converter,Setting>>(*this, rhs);
		std::shared_ptr<RootGrowNode<Number,Setting>> intersecPtr = std::static_pointer_cast<RootGrowNode<Number,Setting>>(intersec);
		SupportFunctionNewT<Number,Converter,Setting> sf = SupportFunctionNewT<Number,Converter,Setting>(intersecPtr);
		return sf;
	}

	template<typename Number, typename Converter, typename Setting>
	SupportFunctionNewT<Number,Converter,Setting> SupportFunctionNewT<Number,Converter,Setting>::intersectHalfspace( const Halfspace<Number>& hspace ) const {
		matrix_t<Number> mat = matrix_t<Number>(1,hspace.normal().rows());
    	for(unsigned i = 0; i < hspace.normal().rows(); ++i){
    		mat(0,i) = hspace.normal()(i);
    	}
    	vector_t<Number> vec = vector_t<Number>(1);
    	vec(0) = hspace.offset();
    	typename Converter::HPolytope* hpoly = new typename Converter::HPolytope(mat,vec);
        return intersect(SupportFunctionNewT<Number,Converter,Setting>(hpoly));
	}

	template<typename Number, typename Converter, typename Setting>
	SupportFunctionNewT<Number,Converter,Setting> SupportFunctionNewT<Number,Converter,Setting>::intersectHalfspaces( const matrix_t<Number>& _mat, const vector_t<Number>& _vec ) const {
		typename Converter::HPolytope* hpoly = new typename Converter::HPolytope(_mat,_vec);
		return intersect(SupportFunctionNewT<Number,Converter,Setting>(hpoly));	
	}

	template<typename Number, typename Converter, typename Setting>
	bool SupportFunctionNewT<Number,Converter,Setting>::contains( const vector_t<Number>& point ) const {

		if(mRoot == nullptr) return false;

		//first function - parameters are backtransformed into the domain space of the given operation
		std::function<Parameters<vector_t<Number>>(RootGrowNode<Number,Setting>*, Parameters<vector_t<Number>>)> reverseOp = 
			[](RootGrowNode<Number,Setting>* n, Parameters<vector_t<Number>> p) -> Parameters<vector_t<Number>> { 
				return Parameters<vector_t<Number>>(n->reverseOp(std::get<0>(p.args)));
			};

		//second function - calls contains() of leaf
		std::function<bool(RootGrowNode<Number,Setting>*, Parameters<vector_t<Number>>)> doesLeafContain = 
			[](RootGrowNode<Number,Setting>* n, Parameters<vector_t<Number>> p) -> bool {
				return n->contains(std::get<0>(p.args));
			};

		std::function<bool(RootGrowNode<Number,Setting>*, std::vector<bool>, Parameters<vector_t<Number>>)> aggregate =
			[](RootGrowNode<Number,Setting>* n, std::vector<bool> v, Parameters<vector_t<Number>> ){
				return n->contains(v);
			};

		Parameters<vector_t<Number>> initParams = Parameters<vector_t<Number>>(point);
		return traverse(reverseOp, doesLeafContain, aggregate, initParams);
	}

	template<typename Number, typename Converter, typename Setting>
	bool SupportFunctionNewT<Number,Converter,Setting>::contains( const Point<Number>& point ) const {
		return contains(point.rawCoordinates());
	}

	template<typename Number, typename Converter, typename Setting>
    bool SupportFunctionNewT<Number,Converter,Setting>::contains( const SupportFunctionNewT<Number,Converter,Setting>& rhs, std::size_t directions) const {
    	std::vector<vector_t<Number>> templateDirections = computeTemplate<Number>(this->dimension(), directions);
    	for(const auto& direction : templateDirections) {
    		if(this->evaluate(direction,true).supportValue < rhs.evaluate(direction,true).supportValue){
				return false;
			}
    	}
    	return true;
    }

	template<typename Number, typename Converter, typename Setting>
	bool SupportFunctionNewT<Number,Converter,Setting>::contains( const SupportFunctionNewT<Number,Converter,Setting>& rhs ) const {
		return contains(rhs, 8);
	}

	template<typename Number, typename Converter, typename Setting>
	SupportFunctionNewT<Number,Converter,Setting> SupportFunctionNewT<Number,Converter,Setting>::unite( const SupportFunctionNewT<Number,Converter,Setting>& rhs ) const {
		std::shared_ptr<UnionOp<Number,Converter,Setting>> scale = std::make_shared<UnionOp<Number,Converter,Setting>>(*this, rhs);
		std::shared_ptr<RootGrowNode<Number,Setting>> scalePtr = std::static_pointer_cast<RootGrowNode<Number,Setting>>(scale);
		SupportFunctionNewT<Number,Converter,Setting> sf = SupportFunctionNewT<Number,Converter,Setting>(scalePtr);
		return sf;
	}

	template<typename Number, typename Converter, typename Setting>
	SupportFunctionNewT<Number,Converter,Setting> SupportFunctionNewT<Number,Converter,Setting>::unite( const std::vector<SupportFunctionNewT<Number,Converter,Setting>>& rhs ) const {
		std::shared_ptr<UnionOp<Number,Converter,Setting>> scale = std::make_shared<UnionOp<Number,Converter,Setting>>(*this, rhs);
		std::shared_ptr<RootGrowNode<Number,Setting>> scalePtr = std::static_pointer_cast<RootGrowNode<Number,Setting>>(scale);
		SupportFunctionNewT<Number,Converter,Setting> sf = SupportFunctionNewT<Number,Converter,Setting>(scalePtr);
		return sf;
	}

	//template<typename Number, typename Converter, typename Setting>
	//void SupportFunctionNewT<Number,Converter,Setting>::reduceRepresentation() {
	//	//Do nothing
	//}

	//template<typename Number, typename Converter, typename Setting>
	//void SupportFunctionNewT<Number,Converter,Setting>::clear() {
	//	mRoot = nullptr;
	//}

	template<typename Number, typename Converter, typename Setting>
	std::vector<std::size_t> SupportFunctionNewT<Number,Converter,Setting>::collectProjections() const {

		//TODO: Check if right
		if(mRoot == nullptr) return std::vector<std::size_t>();
		
		//first function - do nothing
		std::function<void(RootGrowNode<Number,Setting>*)> doNothing = [](RootGrowNode<Number,Setting>* ){ };

		//second function - leaves return a vector of ascending dimensions
		std::function<std::vector<std::size_t>(RootGrowNode<Number,Setting>*)> collectLeafDimensions = 
			[](RootGrowNode<Number,Setting>* n) -> std::vector<std::size_t> {
				std::vector<std::size_t> res;
				for(std::size_t i = 0; i < n->getDimension(); ++i){ 
					res.emplace_back(i);
				}
				return res;
			};

		//third function - call the respective function of the node which sorts out unwanted dimensions
		std::function<std::vector<std::size_t>(RootGrowNode<Number,Setting>*, std::vector<std::vector<std::size_t>>)> intersectDims =
			[](RootGrowNode<Number,Setting>* n, std::vector<std::vector<std::size_t>> dims) -> std::vector<std::size_t> {
				return n->intersectDims(dims);
			};

		return traverse(doNothing, collectLeafDimensions, intersectDims);
	}

} // namespace hypro
