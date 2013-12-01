/** 
 * @file Vertex.h
 * 
 * @author Stefan Schupp
 * @author Sebastian Junges
 * 
 * @since 2011-03-24
 * @version 2013-11-25
 */

#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "Point.h"

namespace hypro {
/*
    class Vertex;
    typedef std::vector<Vertex> vVec;
    typedef std::vector<Vertex>::iterator vVecIt; 
    typedef std::list<Vertex> vList;
    typedef std::list<Vertex>::iterator vListIt;
    typedef std::set<Vertex> vSet;
    typedef std::set<Vertex>::iterator vSetIt;*/ 
    
    template<class numbertype>
    class Vertex : public Point<numbertype> 
    {
        private:
            bool mColor;

        public:
            /**
             *
             * @param dimension
             * @param color
             * @return
             */
            Vertex(unsigned dimension = 2, bool color = false) : Point<numbertype>(dimension) 
            {
                mColor = color;
            }

            /**
             *
             * @param coordinates
             * @param color
             * @return
             */
            Vertex(std::vector<numbertype> coordinates, bool color) : Point<numbertype>(coordinates) 
            {
                mColor = color;
            }

            /**
             *
             * @param p
             * @param color
             * @return
             */
            Vertex(const Point<numbertype> & p, bool color) : Point<numbertype>(p) 
            {
                mColor = color;
            }

            /**
             *
             * @return the color of the vertex.
             */
            bool color() const
            {
                return mColor;
            }

            /**
             *
             * @param c the new value for this vertex' color.
             */
            void setColor(bool c = true) 
            {
                mColor = c;
            }

            /**
             * Inverts the color.
             */
            void invertColor() 
            {
                mColor = !mColor;
            }

            friend std::ostream& operator<<(std::ostream& ostr, const Vertex& v) 
            {
                ostr << v.toString();
                return ostr;
            }

            friend std::istream& operator>>(std::istream& istr, Vertex& v) 
            {
                for (unsigned d = 0; d < v.dimension(); d++) {
                    istr >> v.mCoordinates.at(d);
                }
                istr >> v.mColor; 
                return istr;
            }

            std::string toString(bool parentheses = true) const 
            {
                if (parentheses) 
                    return Point<numbertype>::toString(true) + " " + "[" + mColor +"]";
                else 
                    return Point<numbertype>::toString(false) + " " + mColor;
            }
    };
}