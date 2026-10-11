#pragma once
#include "framework.h"
#include "h3functions.hpp"
#include "JSH3ApiCompat.h"

constexpr UINT16 H3_MAX_OBJECTS = h3::limits::OBJECTS;

// Compatibility surface for object implementations ported from the JS fork.
// Registration and callbacks use the current ObjectExtender API; these helpers
// preserve the source-level names used by those implementations.
#include "BaseClasses.h"
#include "ObjectExtenders/H3MapItemNew.h"
#include "ObjectExtenders/WaterObjects.h"
#include "ObjectExtenders/FlagsExtender.h"
#include "ObjectExtenders/TeamVisitFlags.h"

