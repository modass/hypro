/*
 * Path.cc
 *
 *  Created on: 10.01.2017
 *      Author: j0h
 */
#include "lib/datastructures/HybridAutomaton/Path.h"

namespace hydra {

	void Path::add(const TPathElement& elem) {
		mPath.push_back(elem);
	}

	void Path::addTransition(Transition* t, const carl::Interval<Number>& enabledTime) {
		mPath.push_back(TPathElement(t,enabledTime));
		//TRACE("hydra.datastructures","Add transition " << t << " with timestamp " << enabledTime << " to path.");
	}

	void Path::addTimeStep(const carl::Interval<Number>& timeStep) {
		mPath.push_back(TPathElement(timeStep));
		//TRACE("hydra.datastructures","Add timestamp " << timeStep << " to path.");
	}

	std::pair<Transition*, carl::Interval<Number>> Path::getTransitionToJumpDepth(unsigned depth) const {
		if(depth == 0) {
			return std::make_pair(nullptr, carl::Interval<Number>::unboundedInterval());
		}

		unsigned pos = 0;
		while(pos < mPath.size() && depth > 0) {
			if(mPath.at(pos).isDiscreteStep()) {
				depth--;
				if(depth > 0)
					++pos;
			} else {
				pos++;
			}
		}
		if(pos == mPath.size()) {
			return std::make_pair(nullptr, carl::Interval<Number>::unboundedInterval());
		}
		return std::make_pair(mPath.at(pos).transition, mPath.at(pos).timeInterval);
	}

	bool Path::sharePrefix(const Path &lhs, const Path &rhs) {
        for (auto lhsIt = lhs.mPath.begin(); lhsIt != lhs.mPath.end(); ++lhsIt) {
            for (auto rhsIt = rhs.mPath.begin(); rhsIt != rhs.mPath.end(); ++rhsIt) {
                if (*lhsIt == *rhsIt) {
                    return true;
                }
            }
        }
        return false;
	}

	Path Path::sharedPrefix(const Path& lhs) const {
		Path prefix;
		for(unsigned pos = 0; pos < mPath.size() && pos < lhs.size(); ++pos) {
			if( mPath.at(pos) == lhs.at(pos)) {
				prefix.add(mPath.at(pos));
			}
		}
		return prefix;
	}

	Number Path::maximalTimeSpan(std::deque<TPathElement>::const_iterator start, std::deque<TPathElement>::const_iterator end) const {
		Number timespan = 0;
		bool validPath = true;
		auto currentPos = start;
		//std::cout << __func__ << ": start loop." << std::endl;
		while(validPath){
			if(currentPos == end) {
				if(!currentPos->isDiscreteStep()){
					timespan += currentPos->timeInterval.upper();
				}
				//std::cout << "Found end." << std::endl;
				assert(validPath);
				break;
			} else if (currentPos == mPath.end()) {
				validPath = false;
				//std::cout << "did not find end - invalid path." << std::endl;
				break;
			} else {
				assert(validPath);
				if(!(currentPos->isDiscreteStep())){
					//std::cout << "increase timespan by " << currentPos->timeInterval.upper() << std::endl;
					timespan += currentPos->timeInterval.upper();
				}
			}
			++currentPos;
		}
		//std::cout << "Exited loop." << std::endl;
		if(!validPath) {
			return -1;
		}
		return timespan;
	}

	std::vector<Transition*> Path::getTransitionSequence(std::deque<TPathElement>::const_iterator start, std::deque<TPathElement>::const_iterator end) const {
		auto currentPos = start;
		std::vector<Transition*> res;
		while(currentPos != mPath.end() && currentPos != end) {
			if(currentPos->isDiscreteStep()){
				res.push_back(currentPos->transition);
			}
			++currentPos;
		}
		if(currentPos == mPath.end() && end != mPath.end()) {
			// path end is not part of mPath or lies before start.
			return std::vector<Transition*>();
		}
		return res;
	}

	bool Path::hasChatteringZeno() const {
		// find all cycles first and store potential cycles
		// std::cout << __func__ << ": checking path: " << *this << std::endl;
		std::vector<std::vector<Transition*>> potentialCycles;
		for(auto startElemIt = mPath.begin(); startElemIt != mPath.end(); ++startElemIt) {
			// start measuring the time from a timestep -> if the startElement is not a timestep, increase it.
			if(!startElemIt->isDiscreteStep()){
				++startElemIt;
			}
			if(startElemIt == mPath.end()) {
				break;
			}
			//std::cout << __func__ << ": Starting point is: " << *startElemIt << std::endl;
			// if the startElement iterator is not pointing to the end of the path (this is the case when the last item is a transition)
			// detect cycle by finding the same location.
			if(startElemIt != mPath.end()) {
				auto nextElem = startElemIt;
				nextElem++;
				if((nextElem) == mPath.end()) {
					break;
				}
				const Location* startLoc = (startElemIt)->transition->getSource();
				// std::cout << "Dimension: " << (startElemIt)->transition->getReset().getContinuousResetMatrix().cols() << std::endl;
				std::vector<Transition*> transitionSequence;
				transitionSequence.push_back((startElemIt)->transition);
				//std::cout << __func__ << ": Starting location is " << startLoc->getId() << std::endl;
				for( ; nextElem != mPath.end(); ++nextElem) {
					//std::cout << __func__ << ": next element is " << *(nextElem) << std::endl;
					//std::cout << __func__ << ": current maximal timespan is " << maximalTimeSpan(startElemIt,nextElem) << std::endl;
					if(nextElem->isDiscreteStep() &&
						nextElem->transition->getTarget() == startLoc &&
						maximalTimeSpan(startElemIt,nextElem) == 0 ){
							transitionSequence.push_back(nextElem->transition);
							// std::cout << "Found potential path." << std::endl;
							// std::cout << "Transition sequence length: " << transitionSequence.size() << std::endl;
							potentialCycles.push_back(transitionSequence);
							break;
					} else if (nextElem->isDiscreteStep()) {
						transitionSequence.push_back(nextElem->transition);
					}
				}
			}
		}

		// std::cout << __func__ << ": Exited loop, now verifying resets. Number of potential cycles: " << potentialCycles.size() << std::endl;

		// for each of the location sequences collect the resets - if they form the identity we have found a simple case of chattering Zeno behavior.
		// TODO: Extend this for discrete and clock resets!
		// TODO: Extend this for the reset vector!
		for(auto& transitionSequence : potentialCycles) {
			assert((*transitionSequence.begin())->getSource() == transitionSequence.back()->getTarget());
			assert(!transitionSequence.empty());
			auto transitionIt = transitionSequence.begin();
			// std::cout << "Reset: " << (*transitionIt)->getReset() << std::endl;
			// std::cout << "Dimension: " << (*transitionIt)->getReset().getContinuousResetMatrix().cols() << std::endl;
			hypro::matrix_t<Number> resetMatrix = (*transitionIt)->getReset().getContinuousResetMatrix();
			++transitionIt;
			for( ; transitionIt != transitionSequence.end(); ++transitionIt) {
				// std::cout << "Reset: " << (*transitionIt)->getReset() << std::endl;
				resetMatrix = (*transitionIt)->getReset().getContinuousResetMatrix() * resetMatrix;
			}
			//std:cout << __func__ << ": Resulting reset matrix: " << resetMatrix << std::endl;
			if(resetMatrix == hypro::matrix_t<Number>::Identity(resetMatrix.rows(), resetMatrix.cols())) {
				// std::cout << __func__ << ": Found chattering Zeno path: " <<  *this << std::endl;
				return true;
			}
		}
		return false;
	}

	bool Path::operator>(const Path& r) const {
		return mPath.size() > r.mPath.size();
	}

	bool Path::operator>(unsigned r) const {
		return mPath.size() > r;
	}

	std::size_t Path::size() const {
		return mPath.size();
	}

	TPathElement Path::at(int index) const {
		return mPath.at(index);
	}

	void Path::pop_front() {
		mPath.pop_front();
	}

	void Path::push_front(const TPathElement& elem) {
		mPath.push_front(elem);
	}

	bool Path::sharesPrefix(const Path &rhs) const {
		return sharePrefix(*this, rhs);
	}

	Path::TIterator Path::begin () {
		return mPath.begin();
	}
	Path::TIterator Path::end() {
		return mPath.end();
	}

} // namespace hydra


