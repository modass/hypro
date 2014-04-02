/* 
 * Contains utilities to wrap the ppl polyhedra to our datastructure.
 * @file   util.h
 * @author Stefan Schupp <stefan.schupp@cs.rwth-aachen.de>
 *
 * @since       2014-04-01
 * @version     2014-04-01
 */

#pragma once

#include <ppl.hh>
#include <carl/core/Variable.h>

#include "VariablePool.h"
#include "../../datastructures/Point.h"

namespace hypro
{
namespace polytope
{
    template<typename Number>
    Parma_Polyhedra_Library::Generator pointToGenerator(const Point<Number>& point)
    {
        using namespace Parma_Polyhedra_Library;
        Linear_Expression ls;
        for(auto pointIt = point.begin(); pointIt != point.end(); ++pointIt)
        {
            Linear_Expression tmp = (*pointIt).second.toDouble() * VariablePool::getInstance().variable((*pointIt).first);
            ls += tmp;
        }
        Generator result = Generator::point(ls);
        return result;
    }
}
}
