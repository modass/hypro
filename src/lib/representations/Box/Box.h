/**
 * The class which represents a box.
 *
 * @file Box.h
 * @author Stefan Schupp <stefan.schupp@cs.rwth-aachen.de>
 *
 * @since	2014-01-16
 * @version 2014-05-27
 */

#pragma once

#include <map>
#include <cassert>
#include "../../datastructures/Point.h"
#include "../GeometricObject.h"
#include <carl/interval/Interval.h>

namespace hypro {

template<typename Number>
class Box : hypro::GeometricObject<Number>
{
private:
        typedef carl::FLOAT_T<Number> number;
    
public:
	typedef std::map<const carl::Variable, carl::Interval<carl::FLOAT_T<Number> > > intervalMap;
        typedef std::map<const carl::Variable, carl::Interval<Number> > rawIntervalMap;
        typedef carl::Interval<carl::FLOAT_T<Number> > floatInterval;
	/***************************************************************************
	 * Members
	 **************************************************************************/
protected:
	intervalMap mBoundaries;
	
public:
	/***************************************************************************
	 * Constructors
	 **************************************************************************/
	
	Box() : mBoundaries()
	{}
        
        Box(const Box& orig) : mBoundaries(orig.boundaries())
        {}
        
        Box(const carl::Variable& var, const floatInterval& val)
        {
            mBoundaries.insert(std::make_pair(var, val));
        }
        
        Box(const intervalMap& intervals)
        {
            mBoundaries.insert(intervals.begin(), intervals.end());
        }
        
        Box(const rawIntervalMap& intervals)
        {
            for(auto& intervalPair : intervals)
            {
                mBoundaries.insert(std::make_pair(intervalPair.first, 
                        carl::Interval<carl::FLOAT_T<Number>>(intervalPair.second.lower(), intervalPair.second.lowerBoundType(), intervalPair.second.upper(), intervalPair.second.upperBoundType())));
            }
        }
        
        ~Box()
        {}
	
	/***************************************************************************
	 * Getters & setters
	 **************************************************************************/
	
	intervalMap& rBoundaries()
	{
            return mBoundaries;
	}
	
	intervalMap boundaries() const
	{
            return mBoundaries;
	}
	
	/**
	 * Inserts a new boundary for a variable.
	 * @param val Pair of Variable and Interval.
	 * @return True, if a new insertion has happened, else only update of an existing interval.
	 */
	bool insert(const std::pair<const carl::Variable, floatInterval >& val)
	{
            if(mBoundaries.find(val.first) == mBoundaries.end())
            {
                mBoundaries[val.first] = val.second;
                return true;
            }
            mBoundaries.at(val.first) = val.second;
            return false;
	}
        
        bool insert(const std::pair<const carl::Variable, carl::Interval<Number> >& val)
	{
            carl::FLOAT_T<Number> lower = carl::FLOAT_T<Number>(val.second.lower());
            carl::FLOAT_T<Number> upper = carl::FLOAT_T<Number>(val.second.upper());
            if(mBoundaries.find(val.first) == mBoundaries.end())
            {
                mBoundaries[val.first] = floatInterval(lower, val.second.lowerBoundType(), upper, val.second.upperBoundType());
                return true;
            }
            mBoundaries.at(val.first) = floatInterval(lower, val.second.lowerBoundType(), upper, val.second.upperBoundType());
            return false;
	}
	
        bool insert(const carl::Variable& var, const floatInterval& val)
	{
            if(mBoundaries.find(var) == mBoundaries.end())
            {
                mBoundaries[var] = val;
                return true;
            }
            mBoundaries.at(var) = val;
            return false;
	}
        
        bool insert(const carl::Variable& var, const carl::Interval<Number>& val)
        {
            carl::FLOAT_T<Number> lower = carl::FLOAT_T<Number>(val.lower());
            carl::FLOAT_T<Number> upper = carl::FLOAT_T<Number>(val.upper());
            if(mBoundaries.find(var) == mBoundaries.end())
            {
                mBoundaries[var] = carl::Interval<carl::FLOAT_T<Number>>(lower, val.lowerBoundType(), upper, val.upperBoundType());
                return true;
            }
            mBoundaries.at(var) = carl::Interval<carl::FLOAT_T<Number>>(lower, val.lowerBoundType(), upper, val.upperBoundType());
            return false;
        }
        
	void insert(const intervalMap& boundaries)
	{
            mBoundaries.insert(boundaries.begin(), boundaries.end());
	}
        
        void insert(const rawIntervalMap& boundaries)
        {
            for(auto& intervalPair : boundaries)
            {
                mBoundaries.insert(std::make_pair(intervalPair.first, 
                        carl::Interval<carl::FLOAT_T<Number>>(intervalPair.second.lower(), intervalPair.second.lowerBoundType(), intervalPair.second.upper(), intervalPair.second.upperBoundType())));
            }
        }
	
	bool hasDimension(const carl::Variable& var) const
	{
            return mBoundaries.find(var) != mBoundaries.end();
	}
            
        /**
         * Checks if the box has the same dimensions as this box.
         * The number of dimensions has to be equal as well as the actual
         * variables used for those dimensions.
         * 
         * @param p
         * @return True, if all dimension variables are equal
         */
        bool haveSameDimensions(const Box<Number>& b) const
        {
            if (dimension() != b.dimension())
            {
                return false;
            }
            for (auto pointIt : mBoundaries)
            {
                if ( !b.hasDimension(pointIt.first))
                {
                    return false;
                }
            }
            return true;
        }
	
	floatInterval* pInterval(const carl::Variable& var);
	floatInterval interval(const carl::Variable& var) const;
	
        bool isEmpty() const
        {
            if(mBoundaries.size() == 0)
                return true;
            for(auto interval : mBoundaries)
            {
                if(interval.second.isEmpty())
                {
                    return true;
                }
            }
            return false;
        }
        
        Point<Number> max() const
        {
            typename Point<Number>::coordinateMap coordinates;
            for(auto interval : mBoundaries)
            {
                coordinates.insert(std::make_pair(interval.first, interval.second.upper()));
            }
            return Point<Number>(coordinates);
        }
        
        Point<Number> min() const
        {
            typename Point<Number>::coordinateMap coordinates;
            for(auto interval : mBoundaries)
            {
                coordinates.insert(std::make_pair(interval.first, interval.second.lower()));
            }
            return Point<Number>(coordinates);
        }

        /**
         * @return true, if they are equal.
         */
        friend bool operator==(const Box<Number> & b1, const Box<Number> & b2)
        {
            if(b1.dimension() != b2.dimension()) return false;
            for (auto intervalIt : b1.mBoundaries)
            {
                if (!b2.hasDimension(intervalIt.first) || intervalIt.second != b2.mBoundaries.at(intervalIt.first)) return false;
            }
            return true;
        }
        
        /**
         * @return true. if they are not equal
         */
        friend bool operator!=(const Box<Number> & b1, const Box<Number> & b2)
        {
            return !(b1 == b2);
        }
        
        Box<Number>& operator= (const Box<Number>& rhs) 
        { 
            if (*this != rhs)
            {
                this->mBoundaries.clear();
                intervalMap tmp = rhs.boundaries();
                this->mBoundaries.insert(tmp.begin(), tmp.end());
            } 
            return *this;
        }
		
        /**
         *
         * @param ostr
         * @param b
         * @return
         */
        friend std::ostream & operator<< (std::ostream& ostr, const Box<Number>& b)
        {
            ostr << "{";		
            for (auto intervalIt : b.mBoundaries) {
                if (intervalIt != *(b.mBoundaries.begin())) {
                    ostr << ";";
                }
                ostr << " " << intervalIt.first << " " << intervalIt.second;
            }		
            ostr << " }";
            return ostr;
        }
        
        
	/***************************************************************************
	 * General interface
	 **************************************************************************/
	
	unsigned int dimension() const
	{
            return mBoundaries.size();
	}
	
	bool linearTransformation(Box& result /*, ... */) const;
	bool minkowskiSum(Box& result, const Box& rhs) const;
	bool intersect(Box& result, const Box& rhs) const;
	bool hull(Box& result) const;
	bool contains(const Point<Number>& point) const;
	bool unite(Box& result, const Box& rhs) const;
	
	void clear();
};

}
#include "Box.tpp"