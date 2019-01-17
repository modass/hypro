#pragma once
#include "Condition.h"
#include "flow/operations.h"
#include "flow/typetraits.h"
#include "flow/visitors.h"
#include "../../types.h"
#include <boost/variant.hpp>
#include <iostream>
#include <string>
#include "mex.h"
#include "matrix.h"
#include "../ClassHandle.h"
#include "../ObjectHandle.h"
#include "decomposition/Decomposition.h"
#include "../../representations/ConstraintSet/ConstraintSet.h"
#include "../../representations/types.h"

class MCondition{
    public:
        static void process(int, mxArray**, int, const mxArray**);
    private:
        static void new_empty(int, mxArray**, int, const mxArray**);
        static void new_mat_vec(int, mxArray**, int, const mxArray**);
        static void new_constr_set(int, mxArray**, int, const mxArray**);
        static void copy(int, mxArray**, int, const mxArray**);
        static void delete_condition(int, mxArray**, int, const mxArray**);
        static void size(int, mxArray**, int, const mxArray**);
        static void isempty(int, mxArray**, int, const mxArray**);
        static void getMatrix(int, mxArray**, int, const mxArray**);
        static void getVector(int, mxArray**, int, const mxArray**);
        static void isAxisAligned(int, mxArray**, int, const mxArray**);
        static void isAxisAligned_at(int, mxArray**, int, const mxArray**);
        static void setMatrix(int, mxArray**, int, const mxArray**);
        static void setVector(int, mxArray**, int, const mxArray**);
        static void constraints(int, mxArray**, int, const mxArray**);
        static void hash(int, mxArray**, int, const mxArray**);
        static void getDotRepresentation(int, mxArray**, int, const mxArray**);
        static void decompose(int, mxArray**, int, const mxArray**);
        static void equals(int, mxArray**, int, const mxArray**);
        static void unequals(int, mxArray**, int, const mxArray**);
        static void outstream(int, mxArray**, int, const mxArray**);
        static void combine(int, mxArray**, int, const mxArray**);
};

#include "MCondition.tpp"