#include "Reset.h"

namespace hypro {

	template<typename Number>
	Reset<Number>::Reset(const matrix_t<Number>& mat, const vector_t<Number>& vec) {
		mResets.emplace_back(mat,vec);
	}

	template<typename Number>
	void Reset<Number>::setVector(const vector_t<Number>& in, std::size_t I) {
		while (mResets.size() < I+1) {
			mResets.push_back(ConstraintSet<Number>());
		}
		mResets[I].rVector() = in;
	}

	template<typename Number>
	void Reset<Number>::setMatrix(const matrix_t<Number>& in, std::size_t I) {
		while (mResets.size() < I+1) {
			mResets.push_back(ConstraintSet<Number>());
		}
		mResets[I].rMatrix() = in;
	}

	template<typename Number>
	template<typename Representation, typename ...Rargs>
	State<Number,Representation, Rargs...> Reset<Number>::applyReset(const State<Number,Representation,Rargs...>& inState) const {
		TRACE("hydra.datastructures","inState #sets: " << inState.getNumberSets() << " and #resets: " << this->size());
		assert(this->empty() || inState.getNumberSets() == this->size());
		if(this->empty()) {
			return inState;
		}
		return inState.applyTransformation(mResets);
	}

	template<typename Number>
	Reset<Number> combine(
		const Reset<Number>& lhs, const Reset<Number>& rhs,
		const std::vector<std::string> haVar, const std::vector<std::string> lhsVar, const std::vector<std::string> rhsVar) {

		matrix_t<Number> newMat;
		vector_t<Number> newVec;

		if(lhs.size() == 0 && rhs.size() != 0) {
			newMat = combine(matrix_t<Number>(0,0), rhs.getMatrix(), haVar, lhsVar, rhsVar);
			newVec = combine(vector_t<Number>(0), rhs.getVector());
		} else if(lhs.size() != 0 && rhs.size() == 0) {
			newMat = combine(lhs.getMatrix(), matrix_t<Number>(0,0), haVar, lhsVar, rhsVar);
			newVec = combine(lhs.getVector(), vector_t<Number>(0));
		} else if(lhs.size() == 0 && rhs.size() == 0) {
			return Reset<Number>();
		} else {
			assert(lhs.size() != 0);
			assert(rhs.size() != 0);
			newMat = combine(lhs.getMatrix(), rhs.getMatrix(), haVar, lhsVar, rhsVar);
			newVec = combine(lhs.getVector(), rhs.getVector());
		}

		Reset<Number> re;
		re.setMatrix(newMat);
		re.setVector(newVec);

		return re;
	}

} // namespace
