
#pragma once

#ifndef INCL_FROM_GOHEADER
	static_assert(false, "This file may only be included indirectly by GeometricObject.h");
#endif

#include "SupportFunctionNewSetting.h"
#include "../../util/linearOptimization/Optimizer.h"
#include "../../util/logging/Logger.h"

namespace hypro {


/**
 * @brief      The class which represents a SupportFunctionNew.
 * @tparam     Number     The used number type.
 * @tparam     Converter  The used converter.
 * @tparam	   Settings   The used settings.
 * \ingroup geoState @{
 */
template <typename Number, typename Converter, class Setting>
class SupportFunctionNewT : public GeometricObject<Number, SupportFunctionNewT<Number,Converter,Setting>> {
  private:
  public:
  protected:

	/***************************************************************************
	 * Members
	 **************************************************************************/

  public:
	/***************************************************************************
	 * Constructors
	 **************************************************************************/

	/**
	 * @brief      Creates an empty SupportFunctionNew.
	 */
	SupportFunctionNewT();

	/**
	 * @brief      Copy constructor.
	 * @param[in]  orig  The original.
	 */
	SupportFunctionNewT( const SupportFunctionNewT& orig );

	/**
	 * @brief      Settings conversion constructor.
	 * @param[in]  orig  The original.
	 */
	template<typename SettingRhs, carl::DisableIf< std::is_same<Setting, SettingRhs> > = carl::dummy>
	SupportFunctionNewT(const SupportFunctionNewT<Number,Converter,SettingRhs>& orig) {

	}

	/**
	 * @brief      Move constructor.
	 * @param[in]  orig  The original.
	 */
	SupportFunctionNewT( SupportFunctionNewT&& orig );

	/**
	 * @brief Destructor.
	 */
	~SupportFunctionNewT() {}

	/***************************************************************************
	 * Getters & setters
	 **************************************************************************/

	Setting getSettings() const { return Setting{}; }

	 /**
	  * @brief Static method for the construction of an empty SupportFunctionNew of required dimension.
	  * @param dimension Required dimension.
	  * @return Empty SupportFunctionNew.
	  */
	static SupportFunctionNewT<Number,Converter,Setting> Empty(std::size_t dimension = 1) {
		return SupportFunctionNewT<Number,Converter,Setting>();
	}

	/**
	 * @brief Determines if the current SupportFunctionNew is empty.
	 * @return True, if SupportFunctionNew is empty. False otherwise.
	 */
	bool empty() const;

	/**
	 * @brief Method returning the supremum point of the SupportFunctionNew, i.e. the maximal point.
	 * @return A point representing the supremum of the current SupportFunctionNew.
	 */
	Number supremum() const;

	/**
	 * @brief Getter for a vertex-representation of the current SupportFunctionNew.
	 * @return A vector of points.
	 */
	std::vector<Point<Number>> vertices( const matrix_t<Number>& = matrix_t<Number>::Zero(0,0) ) const;

	/**
	 * @brief      Evaluation function (convex linear optimization).
	 * @param[in]  _direction  The direction/cost function.
	 * @return     Maximum towards _direction.
	 */
	EvaluationResult<Number> evaluate( const vector_t<Number>& _direction, bool ) const;

	/**
	 * @brief      Multi-evaluation function (convex linear optimization).
	 * @param[in]  _directions  The directions/cost functions.
	 * @return     A set of maxima towards the respective directions.
	 */
	std::vector<EvaluationResult<Number>> multiEvaluate( const matrix_t<Number>& _directions, bool useExact = true ) const;

	/**
	 * @brief Checks if two SupportFunctionNews are equal.
	 * @param b1 Contains the first SupportFunctionNew.
	 * @param b2 Contains the second SupportFunctionNew.
	 * @return True, if they are equal.
	 */
	template<class SettingRhs>
	friend bool operator==( const SupportFunctionNewT<Number,Converter,Setting>& b1, const SupportFunctionNewT<Number,Converter,SettingRhs>& b2 ) {
		return false;
	}

	/**
	 * @brief Determines inequality of two SupportFunctionNewes.
	 * @param b1 A SupportFunctionNew.
	 * @param b2 A SupportFunctionNew.
	 * @return False, if both SupportFunctionNewes are equal.
	 */
	friend bool operator!=( const SupportFunctionNewT<Number,Converter,Setting>& b1, const SupportFunctionNewT<Number,Converter,Setting>& b2 ) { return !( b1 == b2 ); }

	/**
	 * @brief Assignment operator.
	 * @param rhs A SupportFunctionNew.
	 */
	SupportFunctionNewT<Number,Converter,Setting>& operator=( const SupportFunctionNewT<Number,Converter,Setting>& rhs ) = default;

	/**
	 * @brief Move assignment operator.
	 * @param rhs A SupportFunctionNew.
	 */
	SupportFunctionNewT<Number,Converter,Setting>& operator=(SupportFunctionNewT<Number,Converter,Setting>&& rhs) = default;

	/**
	 * @brief Outstream operator.
	 * @param ostr Outstream.
	 * @param b A SupportFunctionNew.
	 */
#ifdef HYPRO_LOGGING
	friend std::ostream& operator<<( std::ostream& ostr, const SupportFunctionNewT<Number,Converter,Setting>& b ) {
		// Put outstream operations here.
#else
	friend std::ostream& operator<<( std::ostream& ostr, const SupportFunctionNewT<Number,Converter,Setting>& ) {
#endif
		return ostr;
	}

	/***************************************************************************
	 * General interface
	 **************************************************************************/

	 /**
	  * @brief      Getter for the space dimension.
	  * @return     The dimension of the space.
	  */
	std::size_t dimension() const;

	/**
	 * @brief      Removes redundancy.
	 */
	void removeRedundancy();

	/**
	 * @brief      Storage size determination.
	 * @return     Size of the required memory.
	 */
	std::size_t size() const;

	static representation_name type() { return representation_name::SFN; }

	/**
	 * @brief      Function to reduce the number representation (over-approximate).
	 * @param[in]  limit      The limit
	 */
	const SupportFunctionNewT<Number,Converter,Setting>& reduceNumberRepresentation();


	std::pair<CONTAINMENT, SupportFunctionNewT> satisfiesHalfspace( const Halfspace<Number>& rhs ) const;
	std::pair<CONTAINMENT, SupportFunctionNewT> satisfiesHalfspaces( const matrix_t<Number>& _mat, const vector_t<Number>& _vec ) const;
	SupportFunctionNewT<Number,Converter,Setting> project(const std::vector<std::size_t>& dimensions) const;
	SupportFunctionNewT<Number,Converter,Setting> linearTransformation( const matrix_t<Number>& A ) const;
	SupportFunctionNewT<Number,Converter,Setting> affineTransformation( const matrix_t<Number>& A, const vector_t<Number>& b ) const;
	SupportFunctionNewT<Number,Converter,Setting> minkowskiSum( const SupportFunctionNewT<Number,Converter,Setting>& rhs ) const;

	/**
	 * @brief      Computes the intersection of two SupportFunctionNewes.
	 * @param[in]  rhs   The right hand side SupportFunctionNew.
	 * @return     The resulting SupportFunctionNew.
	 */
	SupportFunctionNewT<Number,Converter,Setting> intersect( const SupportFunctionNewT<Number,Converter,Setting>& rhs ) const;

	SupportFunctionNewT<Number,Converter,Setting> intersectHalfspace( const Halfspace<Number>& hspace ) const;
	SupportFunctionNewT<Number,Converter,Setting> intersectHalfspaces( const matrix_t<Number>& _mat, const vector_t<Number>& _vec ) const;
	bool contains( const Point<Number>& point ) const;

	/**
	 * @brief      Containment check for a SupportFunctionNew.
	 * @param[in]  SupportFunctionNew   The SupportFunctionNew.
	 * @return     True, if the given SupportFunctionNew is contained in the current SupportFunctionNew, false otherwise.
	 */
	bool contains( const SupportFunctionNewT<Number,Converter,Setting>& SupportFunctionNew ) const;

	/**
	 * @brief      Computes the union of two SupportFunctionNewes.
	 * @param[in]  rhs   The right hand side SupportFunctionNew.
	 * @return     The resulting SupportFunctionNew.
	 */
	SupportFunctionNewT<Number,Converter,Setting> unite( const SupportFunctionNewT<Number,Converter,Setting>& rhs ) const;

	/**
	 * @brief      Computes the union of the current SupportFunctionNew with a set of SupportFunctionNewes.
	 * @param[in]  SupportFunctionNewes  The SupportFunctionNewes.
	 * @return     The resulting SupportFunctionNew.
	 */
	static SupportFunctionNewT<Number,Converter,Setting> unite( const std::vector<SupportFunctionNewT<Number,Converter,Setting>>& SupportFunctionNewes );

	/**
	 * @brief      Reduces the representation of the current SupportFunctionNew.
	 */
	void reduceRepresentation();

	/**
	 * @brief      Makes this SupportFunctionNew equal to the empty SupportFunctionNew.
	 */
	void clear();

};
/** @} */


} // namespace hypro

#include "SupportFunctionNew.tpp"
