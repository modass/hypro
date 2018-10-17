#pragma once 

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <functional>
#include <type_traits>
#include <tuple>
#include <cassert>
#include "../../types.h"
#include "../../util/linearOptimization/EvaluationResult.h"
#include "LinTrafoParameters.h"

namespace hypro {

//Type of nodes. Needed to fast determine which node subclass is actually calling a function. 
enum SFNEW_TYPE { NODE = 0, TRAFO, SUMOP, LEAF };

//A node in the tree. Knows its children and its height in the tree. (Height only needed for printing)
//Base class for operations and leaves
template<typename Number>
class RootGrowNode {

protected:

	////// Members
	SFNEW_TYPE mType = NODE;												//NONE since RootGrowNode should later be an abstract class
	unsigned originCount = 0;												//Amount of children needed to function properly
	RootGrowNode* mParent = nullptr;										//parent of the current node. The topmost node has nullptr as parent
	std::vector<RootGrowNode*> mChildren = std::vector<RootGrowNode*>();	//vector of all current children

public:

	////// Constructors

	RootGrowNode(){}
	virtual ~RootGrowNode(){}	//not sure how to define deletion mechanics

	////// Getters and Setters

	virtual SFNEW_TYPE getType() const { return mType; }
	virtual unsigned getOriginCount() const { return originCount; }
	virtual RootGrowNode* getParent() const { return mParent; }
	virtual std::vector<RootGrowNode*> getChildren() const { return mChildren; }

	////// Modifiers

	void addToChildren(RootGrowNode* rhs){ mChildren.push_back(rhs); }
	void setAsParent(RootGrowNode* parent){ mParent = parent; }

	////// Displaying

	friend std::ostream& operator<<(std::ostream& ostr, const RootGrowNode& r){
		ostr << "current type: " << r.getType() << std::endl;
		if(r.getParent() != nullptr){
			ostr << "parent type: " << r.getParent()->getType() << std::endl;	
		} else {
			ostr << "parent type: nullptr" << std::endl;
		}
		ostr << "children types: ";
		for(auto c : r.getChildren()){
			ostr << c->getType() << ",";
		}
		ostr << std::endl;
		for(auto c : r.getChildren()){
			ostr << *c << std::endl;
		}
		return ostr;
	}

	////// Traversal

	// The needed functions for evaluate. Virtual s.t. they can be implemented in the Operation/Leaf classes
	// Three functions are needed: transform, compute and aggregate.
	// - transform will be called by a node for every child
	// - compute will only be called once by leaf nodes
	// - aggregate will only be called once by all non leaf nodes
	//
	// NOTE: 	All functions used as the transform must have the signature A name(A param)
	//			All functions used as the compute must have the signature B name(A param)
	//			All functions used as the aggregate must have the signature std::vector<B> name(B param)

	//For evaluate

	//For everyone - transform
	virtual matrix_t<Number> transform(const matrix_t<Number>& param) const = 0;
	//For leaves - compute
	virtual std::vector<EvaluationResult<Number>> compute(const matrix_t<Number>& param) const = 0; 
	//For operations - aggregate
	virtual std::vector<EvaluationResult<Number>> aggregate(std::vector<std::vector<EvaluationResult<Number>>>& resultStackBack, const matrix_t<Number>& currentParam) const = 0;

	//For hasTrafo - should only be called by trafoOp objects
	virtual bool hasTrafo(std::shared_ptr<const LinTrafoParameters<Number>>& ltParam, const matrix_t<Number>& A, const vector_t<Number>& b) = 0;
	
};

} //namespace hypro