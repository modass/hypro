#include "rectangularResetHandler.h"

namespace hypro {
template <typename State>
void rectangularResetHandler<State>::handle() {
	TRACE( "hypro.worker", "Applying Reset trafo for subspace " << mIndex );
	assert( mState->getSetType( mIndex ) == representation_name::carl_polytope );
	auto res = mState->partialIntervalAssignment( mIntervalResets, mIndex );

	mState->setSetDirect( res.getSet( mIndex ), mIndex );
	mState->setSetType( res.getSetType( mIndex ), mIndex );
}

template <typename State>
void rectangularResetHandler<State>::rectangularIntersectReset( const State& state, const HybridAutomaton<Number>& automaton ) {
	assert( !state.getTimestamp().isEmpty() );

	// TRACE( "hydra.worker.discrete", "Applying handler " << this->handlerName() );

	for ( auto& location : automaton.getLocations() ) {
		for ( auto& transitionPtr : location->getTransitions() ) {
			if ( state.getLocation() == transitionPtr->getTarget()) {
				CONTAINMENT containmentResult = CONTAINMENT::BOT;
				State resultingSet(state);
				if ( !transitionPtr->getReset().empty() ){
					for ( size_t i = 0; i < resultingSet.getNumberSets(); i++ ) {
						if ( resultingSet.getSetType( i ) == representation_name::carl_polytope ) {
							IntervalAssignment<Number> intervalReset = transitionPtr->getReset().getIntervalReset( i );
							resultingSet = resultingSet.partialIntervalAssignment( intervalReset.mIntervals, i );
						} else {
							AffineTransformation<Number> reset = transitionPtr->getReset().getAffineReset( i );
							resultingSet = resultingSet.partiallyApplyTransformation( reset.mTransformation, i );
						}
					}
				}

				// determine full vs. partial containment
				if ( resultingSet == state ) {
					containmentResult = CONTAINMENT::FULL;
				}

				// reduction
				resultingSet.removeRedundancy();

				// return containment information
				if ( resultingSet.isEmpty() ) {
					containmentResult = CONTAINMENT::NO;
				} else if ( containmentResult != CONTAINMENT::FULL ) {
					containmentResult = CONTAINMENT::PARTIAL;
				} else {
					containmentResult = CONTAINMENT::FULL;
				}

				if ( containmentResult != CONTAINMENT::NO ) {
					mResetSatisfyingStates[transitionPtr.get()].emplace_back( std::move( resultingSet ) );
				}

				std::cout << "find transition reset result: " << containmentResult << std::endl;
				std::cout << resultingSet << std::endl;
				mResetContainment[transitionPtr.get()] = containmentResult;
			}
		}
	}
}
}  // namespace hypro
