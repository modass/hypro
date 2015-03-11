/**
 * Implementation of the box.
 *
 * @file Box.tpp
 * @author Stefan Schupp <stefan.schupp@cs.rwth-aachen.de>
 *
 * @since   2014-01-16
 * @version 2015-02-27
 */

#include "Box.h"


namespace hypro {

template<typename Number>
Box<Number>::Box(const std::set<Point<Number>>& _points) {
	if(_points.size() > 0) {
		unsigned dim = _points.begin()->dimension();
		for(unsigned d = 0; d < dim; ++d) {
			mBoundaries.push_back(carl::Interval<Number>(_points.begin()->at(d)));
		}
 		if(_points.size() > 1) {
			auto pointIt = _points.begin();
			++pointIt;
			for(; pointIt != _points.end(); ++pointIt ) {
				for(unsigned d = 0; d < pointIt->dimension(); ++d) {
					if(mBoundaries[d].lower() > pointIt->at(d)) 
						mBoundaries[d].setLower(pointIt->at(d));

					if(mBoundaries[d].upper() < pointIt->at(d)) 
						mBoundaries[d].setUpper(pointIt->at(d));
				}
			}
		}
	}
}
	
	
template<typename Number>
carl::Interval<Number> Box<Number>::interval(const carl::Variable& var) const
{
    int pos = hypro::VariablePool::getInstance().dimension(var);
	if( pos < 0)
		return carl::Interval<Number>::emptyInterval();
	
    return mBoundaries.at(pos);
}

template<typename Number>
carl::Interval<Number>& Box<Number>::rInterval(const carl::Variable& var) {
	int pos = hypro::VariablePool::getInstance().dimension(var);
	assert(pos >= 0);
    return mBoundaries.at(pos);
}

template<typename Number>
std::set<Point<Number>> Box<Number>::corners() const {
	std::set<Point<Number>> result;
	unsigned limit = int(pow(mBoundaries.size(),2));
	
	for(unsigned bitCount = 0; bitCount < limit ; ++bitCount) {
		vector_t<Number> coord = vector_t<Number>(dimension());
		for(unsigned dimension = 0; dimension < this->dimension(); ++dimension) {
			unsigned pos = (1 << dimension);
			if(bitCount & pos)
				coord(dimension) = mBoundaries[dimension].upper();
			else
				coord(dimension) = mBoundaries[dimension].lower();
		}
		result.insert(Point<Number>(coord));
	}
	return result;
}

template<typename Number>
Box<Number> Box<Number>::linearTransformation(const matrix_t<Number>& A, const vector_t<Number>& b) const
{
    std::set<Point<Number>> corners = this->corners();
	std::set<Point<Number>> transformedCorners;
	if(b != vector_t<Number>()) {
		for(auto& point : corners) {
			transformedCorners.insert(Point<Number>(A*point.rawCoordinates() + b));
		}
	} else {
		for(auto& point : corners) {
			transformedCorners.insert(Point<Number>(A*point.rawCoordinates()));
		}
	}
	return Box<Number>(transformedCorners);
}

template<typename Number>
Box<Number> Box<Number>::minkowskiSum(const Box<Number>& rhs) const
{
	assert(dimension() == rhs.dimension());
	Box<Number> result;
    for (unsigned i = 0; i < dimension(); ++i)
    {
        result.insert(i, mBoundaries[i].add(rhs[i]));
    }
    return result;
}

template<typename Number>
Box<Number> Box<Number>::intersect(const Box<Number>& rhs) const
{
	assert(dimension() == rhs.dimension());
    Box<Number> result;
    for (unsigned i = 0; i < dimension(); ++i)
    {
//        std::cout << rhs.at(i) << std::endl;
        carl::Interval<Number > res = mBoundaries[i].intersect(rhs[i]);
        result.insert(i, res );
    }
    return result;
}

template<typename Number>
bool Box<Number>::contains(const Point<Number>& point) const
{
    if( this->dimension() > point.dimension() )
        return false;
    
    for(unsigned i = 0; i < dimension(); ++i)
    {
        if( !mBoundaries[i].contains(point.at(i)))
            return false;
    }
    return true;
}

template<typename Number>
Box<Number> Box<Number>::unite(const Box<Number>& rhs) const
{
	assert(dimension() == rhs.dimension());
	Box<Number> result;
    for(unsigned i = 0; i < dimension(); ++i)
    {
        Number lowerMin = mBoundaries[i].lower() < rhs.at(i).lower() ? mBoundaries[i].lower() : rhs.at(i).lower();
        Number upperMax = mBoundaries[i].upper() > rhs.at(i).upper() ? mBoundaries[i].upper() : rhs.at(i).upper();
        carl::BoundType lowerType = carl::getWeakestBoundType(mBoundaries[i].lowerBoundType(), rhs.at(i).lowerBoundType());
        carl::BoundType upperType = carl::getWeakestBoundType(mBoundaries[i].upperBoundType(), rhs.at(i).upperBoundType());
        result.insert( i, carl::Interval<Number>(lowerMin, lowerType, upperMax, upperType) );
    }
    return result;
}

template<typename Number>
void Box<Number>::clear()
{
    mBoundaries.erase(mBoundaries.begin(), mBoundaries.end());
}
	
} // namespace
