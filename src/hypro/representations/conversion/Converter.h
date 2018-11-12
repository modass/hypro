#pragma once

#ifndef INCL_FROM_GOHEADER
	static_assert(false, "This file may only be included indirectly by GeometricObject.h");
#endif
#define INCL_FROM_CONVERTERHEADER true

#include "../../flags.h"
#include "../Box/Box.h"
#include "../ConstraintSet/ConstraintSet.h"
#include "../Ellipsoids/Ellipsoid.h"
#include "../OrthogonalPolyhedron/OrthogonalPolyhedron.h"
#include "../Polytope/Polytope.h"
#include "../Polytopes/HPolytope/HPolytope.h"
#include "../Polytopes/VPolytope/VPolytope.h"
#include "../SupportFunction/SupportFunction.h"
#include "../Zonotope/Zonotope.h"
#include "../DifferenceBounds/DifferenceBounds.h"
#include "../../util/pca.h"

namespace hypro {

/**
 * @brief      enum for differentiating between exact conversion and over-/underapproximation (some functions support an alternative computation method).
 */

enum CONV_MODE {
	EXACT, OVER, UNDER, ALTERNATIVE
};

template<typename Number>
class Converter {
	public:
		/* BEGIN typedefs (do not remove this comment!) */

		using Box = BoxT<Number,Converter,BoxLinearOptimizationOn>;
		using ConstraintSet = ConstraintSetT<Number,Converter,ConstraintSetSettings>;
		using Ellipsoid = EllipsoidT<Number,Converter>;
		using HPolytope = HPolytopeT<Number,Converter,HPolytopeSetting>;
		using OrthogonalPolyhedron = OrthogonalPolyhedronT<Number,Converter,BoxLinearOptimizationOn>;
		using VPolytope = VPolytopeT<Number,Converter,VPolytopeSettings>;
		using DifferenceBounds = DifferenceBoundsT<Number,Converter,DifferenceBoundsSetting>;
		#ifdef HYPRO_USE_PPL
		using Polytope = PolytopeT<Number,Converter,PolytopeSetting>;
		#endif
		using SupportFunction = SupportFunctionT<Number,Converter,SupportFunctionSetting>;
		using Zonotope = ZonotopeT<Number,Converter,ZonotopeSetting>;

		/* END typedefs (do not remove this comment!) */

		/* BEGIN Conversion (do not remove this comment!) */

		template<typename BoxSetting = typename Box::Settings, typename InSetting>
		static BoxT<Number,Converter<Number>,BoxSetting> toBox(const BoxT<Number,Converter<Number>,InSetting>& source, const CONV_MODE = CONV_MODE::EXACT);
		template<typename BoxSetting = typename Box::Settings, typename inSetting>
		static BoxT<Number,Converter<Number>,BoxSetting> toBox(const ConstraintSetT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::EXACT);
		template<typename BoxSetting = typename Box::Settings>
		static BoxT<Number,Converter<Number>,BoxSetting> toBox(const Ellipsoid& source, const CONV_MODE = CONV_MODE::EXACT);
		template<typename BoxSetting = typename Box::Settings, typename inSetting>
		static BoxT<Number,Converter<Number>,BoxSetting> toBox(const HPolytopeT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::OVER);
		template<typename BoxSetting = typename Box::Settings, typename inSetting>
		static BoxT<Number,Converter<Number>,BoxSetting> toBox(const VPolytopeT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::OVER);
		#ifdef HYPRO_USE_PPL
		template<typename BoxSetting = typename Box::Settings, typename inSetting>
		static BoxT<Number,Converter<Number>,BoxSetting> toBox(const PolytopeT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::OVER);
		#endif
		template<typename BoxSetting = typename Box::Settings, typename inSetting>
		static BoxT<Number,Converter<Number>,BoxSetting> toBox(const SupportFunctionT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::OVER);
		template<typename BoxSetting = typename Box::Settings, typename inSetting>
		static BoxT<Number,Converter<Number>,BoxSetting> toBox(const ZonotopeT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::OVER);
		template<typename BoxSetting = typename Box::Settings, typename inSetting>
		static BoxT<Number,Converter<Number>,BoxSetting> toBox(const DifferenceBoundsT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::OVER);

		template<typename CSSetting = typename ConstraintSet::Settings, typename inSetting>
		static ConstraintSetT<Number,Converter<Number>,CSSetting> toConstraintSet(const BoxT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::EXACT);
		template<typename CSSetting = typename ConstraintSet::Settings, typename inSetting>
		static ConstraintSetT<Number,Converter<Number>,CSSetting> toConstraintSet(const ConstraintSetT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::EXACT);
		template<typename CSSetting = typename ConstraintSet::Settings>
		static ConstraintSetT<Number,Converter<Number>,CSSetting> toConstraintSet(const Ellipsoid& source, const CONV_MODE = CONV_MODE::EXACT);
		template<typename CSSetting = typename ConstraintSet::Settings, typename inSetting>
		static ConstraintSetT<Number,Converter<Number>,CSSetting> toConstraintSet(const HPolytopeT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::OVER);
		template<typename CSSetting = typename ConstraintSet::Settings, typename inSetting>
		static ConstraintSetT<Number,Converter<Number>,CSSetting> toConstraintSet(const VPolytopeT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::OVER);
		#ifdef HYPRO_USE_PPL
		template<typename CSSetting = typename ConstraintSet::Settings, typename inSetting>
		static ConstraintSetT<Number,Converter<Number>,CSSetting> toConstraintSet(const PolytopeT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::OVER);
		#endif
		template<typename CSSetting = typename ConstraintSet::Settings, typename inSetting>
		static ConstraintSetT<Number,Converter<Number>,CSSetting> toConstraintSet(const SupportFunctionT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::OVER);
		template<typename CSSetting = typename ConstraintSet::Settings, typename inSetting>
		static ConstraintSetT<Number,Converter<Number>,CSSetting> toConstraintSet(const ZonotopeT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::OVER);
	    template<typename CSSetting = typename ConstraintSet::Settings, typename inSetting>
		static ConstraintSetT<Number,Converter<Number>,CSSetting> toConstraintSet(const DifferenceBoundsT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::OVER);

		template<typename HPolySetting = typename HPolytope::Settings, typename inSetting>
		static HPolytopeT<Number,Converter<Number>,HPolySetting> toHPolytope(const BoxT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::EXACT);
		template<typename HPolySetting = typename HPolytope::Settings, typename inSetting>
		static HPolytopeT<Number,Converter<Number>,HPolySetting> toHPolytope(const ConstraintSetT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::EXACT);
		template<typename HPolySetting = typename HPolytope::Settings>
		static HPolytopeT<Number,Converter<Number>,HPolySetting> toHPolytope(const Ellipsoid& source, const CONV_MODE = CONV_MODE::EXACT);
		template<typename HPolySetting = typename HPolytope::Settings, typename inSetting>
		static HPolytopeT<Number,Converter<Number>,HPolySetting> toHPolytope(const HPolytopeT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::EXACT);
		template<typename HPolySetting = typename HPolytope::Settings, typename inSetting>
		static HPolytopeT<Number,Converter<Number>,HPolySetting> toHPolytope(const VPolytopeT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::EXACT);
		#ifdef HYPRO_USE_PPL
		template<typename HPolySetting = typename HPolytope::Settings, typename inSetting>
		static HPolytopeT<Number,Converter<Number>,HPolySetting> toHPolytope(const PolytopeT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::EXACT);
		#endif
		template<typename HPolySetting = typename HPolytope::Settings, typename inSetting>
		static HPolytopeT<Number,Converter<Number>,HPolySetting> toHPolytope(const SupportFunctionT<Number,Converter<Number>,inSetting>& source, const std::vector<vector_t<Number>>& additionalDirections = std::vector<vector_t<Number>>(), const CONV_MODE = CONV_MODE::OVER, std::size_t numberOfDirections = defaultTemplateDirectionCount );
		template<typename HPolySetting = typename HPolytope::Settings, typename inSetting>
		static HPolytopeT<Number,Converter<Number>,HPolySetting> toHPolytope(const ZonotopeT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::EXACT);
		template<typename HPolySetting = typename HPolytope::Settings, typename inSetting>
		static HPolytopeT<Number,Converter<Number>,HPolySetting> toHPolytope(const DifferenceBoundsT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::EXACT);

		template<typename VPolySetting = typename VPolytope::Settings, typename inSetting>
		static VPolytopeT<Number,Converter<Number>,VPolySetting> toVPolytope(const BoxT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::EXACT);
		template<typename VPolySetting = typename VPolytope::Settings, typename inSetting>
		static VPolytopeT<Number,Converter<Number>,VPolySetting> toVPolytope(const ConstraintSetT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::EXACT);
		template<typename VPolySetting = typename VPolytope::Settings>
		static VPolytopeT<Number,Converter<Number>,VPolySetting> toVPolytope(const Ellipsoid& source, const CONV_MODE = CONV_MODE::EXACT);
		template<typename VPolySetting = typename VPolytope::Settings, typename inSetting>
		static VPolytopeT<Number,Converter<Number>,VPolySetting> toVPolytope(const HPolytopeT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::EXACT);
		template<typename VPolySetting = typename VPolytope::Settings, typename inSetting>
		static VPolytopeT<Number,Converter<Number>,VPolySetting> toVPolytope(const VPolytopeT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::EXACT);
		#ifdef HYPRO_USE_PPL
		template<typename VPolySetting = typename VPolytope::Settings>
		static VPolytopeT<Number,Converter<Number>,VPolySetting> toVPolytope(const PolytopeT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::EXACT);
		#endif
		template<typename VPolySetting = typename VPolytope::Settings, typename inSetting>
		static VPolytopeT<Number,Converter<Number>,VPolySetting> toVPolytope(const SupportFunctionT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::UNDER, std::size_t numberOfDirections = defaultTemplateDirectionCount );
		template<typename VPolySetting = typename VPolytope::Settings, typename inSetting>
		static VPolytopeT<Number,Converter<Number>,VPolySetting> toVPolytope(const ZonotopeT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::EXACT);
		template<typename VPolySetting = typename VPolytope::Settings, typename inSetting>
		static VPolytopeT<Number,Converter<Number>,VPolySetting> toVPolytope(const DifferenceBoundsT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::EXACT);

		template<typename SFSetting = typename SupportFunction::Settings, typename inSetting>
		static SupportFunctionT<Number,Converter<Number>,SFSetting> toSupportFunction(const BoxT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::EXACT);
		template<typename SFSetting = typename SupportFunction::Settings, typename inSetting>
		static SupportFunctionT<Number,Converter<Number>,SFSetting> toSupportFunction(const ConstraintSetT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::EXACT);
		template<typename SFSetting = typename SupportFunction::Settings>
		static SupportFunctionT<Number,Converter<Number>,SFSetting> toSupportFunction(const Ellipsoid& source, const CONV_MODE = CONV_MODE::EXACT);
		template<typename SFSetting = typename SupportFunction::Settings, typename inSetting>
		static SupportFunctionT<Number,Converter<Number>,SFSetting> toSupportFunction(const HPolytopeT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::EXACT);
		template<typename SFSetting = typename SupportFunction::Settings, typename inSetting>
		static SupportFunctionT<Number,Converter<Number>,SFSetting> toSupportFunction(const VPolytopeT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::EXACT);
		#ifdef HYPRO_USE_PPL
		template<typename SFSetting = typename SupportFunction::Settings, typename inSetting>
		static SupportFunctionT<Number,Converter<Number>,SFSetting> toSupportFunction(const Polytope& source, const CONV_MODE = CONV_MODE::EXACT);
		#endif
		template<typename SFSetting = typename SupportFunction::Settings, typename inSetting>
		static SupportFunctionT<Number,Converter<Number>,SFSetting> toSupportFunction(const SupportFunctionT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::EXACT);
		template<typename SFSetting = typename SupportFunction::Settings, typename inSetting>
		static SupportFunctionT<Number,Converter<Number>,SFSetting> toSupportFunction(const ZonotopeT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::EXACT);
	    template<typename SFSetting = typename SupportFunction::Settings, typename inSetting>
		static SupportFunctionT<Number,Converter<Number>,SFSetting> toSupportFunction(const DifferenceBoundsT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::EXACT);

		template<typename ZonotopeSetting = typename Zonotope::Settings, typename inSetting>
		static ZonotopeT<Number,Converter<Number>,ZonotopeSetting> toZonotope(const BoxT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::EXACT);
		template<typename ZonotopeSetting = typename Zonotope::Settings, typename inSetting>
		static ZonotopeT<Number,Converter<Number>,ZonotopeSetting> toZonotope(const ConstraintSetT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::EXACT);
		template<typename ZonotopeSetting = typename Zonotope::Settings>
		static ZonotopeT<Number,Converter<Number>,ZonotopeSetting> toZonotope(const Ellipsoid& source, const CONV_MODE = CONV_MODE::EXACT);
		template<typename ZonotopeSetting = typename Zonotope::Settings, typename inSetting>
		static ZonotopeT<Number,Converter<Number>,ZonotopeSetting> toZonotope(const HPolytopeT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::OVER);
		template<typename ZonotopeSetting = typename Zonotope::Settings, typename inSetting>
		static ZonotopeT<Number,Converter<Number>,ZonotopeSetting> toZonotope(const VPolytopeT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::OVER);
		#ifdef HYPRO_USE_PPL
		template<typename ZonotopeSetting = typename Zonotope::Settings, typename inSetting>
		static ZonotopeT<Number,Converter<Number>,ZonotopeSetting> toZonotope(const PolytopeT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::EXACT);
		#endif
		template<typename ZonotopeSetting = typename Zonotope::Settings, typename inSetting>
		static ZonotopeT<Number,Converter<Number>,ZonotopeSetting> toZonotope(const SupportFunctionT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::OVER, std::size_t numberOfDirections = defaultTemplateDirectionCount );
		template<typename ZonotopeSetting = typename Zonotope::Settings, typename inSetting>
		static ZonotopeT<Number,Converter<Number>,ZonotopeSetting> toZonotope(const ZonotopeT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::EXACT);
		template<typename ZonotopeSetting = typename Zonotope::Settings, typename inSetting>
		static ZonotopeT<Number,Converter<Number>,ZonotopeSetting> toZonotope(const DifferenceBoundsT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::OVER);

		#ifdef HYPRO_USE_PPL
		template<typename PolytopeSetting = typename Polytope::Settings, typename inSetting>
		static PolytopeT<Number,Converter<Number>,PolytopeSetting> toPolytope(const PolytopeT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::EXACT);
		template<typename PolytopeSetting = typename Polytope::Settings, typename inSetting>
		static PolytopeT<Number,Converter<Number>,PolytopeSetting> toPolytope(const BoxT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::EXACT);
		template<typename PolytopeSetting = typename Polytope::Settings, typename inSetting>
		static PolytopeT<Number,Converter<Number>,PolytopeSetting> toPolytope(const ConstraintSetT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::EXACT);
		template<typename PolytopeSetting = typename Polytope::Settings>
		static PolytopeT<Number,Converter<Number>,PolytopeSetting> toPolytope(const Ellipsoid& source, const CONV_MODE = CONV_MODE::EXACT);
		template<typename PolytopeSetting = typename Polytope::Settings, typename inSetting>
		static PolytopeT<Number,Converter<Number>,PolytopeSetting> toPolytope(const HPolytopeT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::EXACT);
		template<typename PolytopeSetting = typename Polytope::Settings, typename inSetting>
		static PolytopeT<Number,Converter<Number>,PolytopeSetting> toPolytope(const VPolytopeT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::EXACT);
		template<typename PolytopeSetting = typename Polytope::Settings, typename inSetting>
		static PolytopeT<Number,Converter<Number>,PolytopeSetting> toPolytope(const SupportFunctionT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::OVER, std::size_t numberOfDirections = defaultTemplateDirectionCount);
		template<typename PolytopeSetting = typename Polytope::Settings, typename inSetting>
		static PolytopeT<Number,Converter<Number>,PolytopeSetting> toPolytope(const ZonotopeT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::EXACT);
		template<typename PolytopeSetting = typename Polytope::Settings, typename inSetting>
		static PolytopeT<Number,Converter<Number>,PolytopeSetting> toPolytope(const DifferenceBoundsT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::EXACT); //TODO NOT IMPLEMENTED YET
		#endif

		template<typename DBSetting = typename DifferenceBounds::Settings, typename inSetting>
		static DifferenceBoundsT<Number,Converter<Number>,DBSetting> toDifferenceBounds(const BoxT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::EXACT);
		template<typename DBSetting = typename DifferenceBounds::Settings, typename inSetting>
		static DifferenceBoundsT<Number,Converter<Number>,DBSetting> toDifferenceBounds(const ConstraintSetT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::EXACT);
		template<typename DBSetting = typename DifferenceBounds::Settings>
		static DifferenceBoundsT<Number,Converter<Number>,DBSetting> toDifferenceBounds(const Ellipsoid& source, const CONV_MODE = CONV_MODE::EXACT);
		template<typename DBSetting = typename DifferenceBounds::Settings, typename inSetting>
		static DifferenceBoundsT<Number,Converter<Number>,DBSetting> toDifferenceBounds(const HPolytopeT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::EXACT);
		template<typename DBSetting = typename DifferenceBounds::Settings, typename inSetting>
		static DifferenceBoundsT<Number,Converter<Number>,DBSetting> toDifferenceBounds(const VPolytopeT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::EXACT);
		#ifdef HYPRO_USE_PPL
		template<typename DBSetting = typename DifferenceBounds::Settings, typename inSetting>
		static DifferenceBoundsT<Number,Converter<Number>,DBSetting> toDifferenceBounds(const PolytopeT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::EXACT);// TODO NOT IMPLEMENTED YET
		#endif
		template<typename DBSetting = typename DifferenceBounds::Settings, typename inSetting>
		static DifferenceBoundsT<Number,Converter<Number>,DBSetting> toDifferenceBounds(const SupportFunctionT<Number,Converter<Number>,inSetting>& source, const std::vector<vector_t<Number>>& additionalDirections = std::vector<vector_t<Number>>(), const CONV_MODE = CONV_MODE::OVER, std::size_t numberOfDirections = defaultTemplateDirectionCount );
		template<typename DBSetting = typename DifferenceBounds::Settings, typename inSetting>
		static DifferenceBoundsT<Number,Converter<Number>,DBSetting> toDifferenceBounds(const ZonotopeT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::EXACT);
		template<typename DBSetting = typename DifferenceBounds::Settings, typename inSetting>
		static DifferenceBoundsT<Number,Converter<Number>,DBSetting> toDifferenceBounds(const DifferenceBoundsT<Number,Converter<Number>,inSetting>& source, const CONV_MODE = CONV_MODE::EXACT);

		/* END Conversion (do not remove this comment!) */
};

#include "typedefs.h"
#include "converterToBox.tpp"
#include "converterToConstraintSet.tpp"
#include "converterToHPolytope.tpp"
#include "converterToVPolytope.tpp"
#include "converterToSupportFunction.tpp"
#include "converterToZonotope.tpp"
#ifdef HYPRO_USE_PPL
#include "converterToPolytope.tpp"
#endif
//#include "settingsConversion.tpp"
#include "converterToDifferenceBounds.tpp"

} // namespace hypro

#include "convenienceOperators.h"
#include "conversionHelper.h"
