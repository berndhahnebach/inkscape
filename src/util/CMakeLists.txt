
set(util_SRC
	ege-appear-time-tracker.cpp
	ege-tags.cpp
	expression-evaluator.cpp
	share.cpp
	units.cpp


	# -------
	# Headers
	accumulators.h
	compose.hpp
	copy.h
	ege-appear-time-tracker.h
	ege-tags.h
	enums.h
	expression-evaluator.h
	filter-list.h
	find-if-before.h
	find-last-if.h
	fixed_point.h
	format.h
	forward-pointer-iterator.h
	function.h
	glib-list-iterators.h
	list-container-test.h
	list-container.h
	list-copy.h
	list.h
	longest-common-suffix.h
	map-list.h
	mathfns.h
	reference.h
	reverse-list.h
	share.h
	tuple.h
	ucompose.hpp
	units.h
	unordered-containers.h
)

# add_inkscape_lib(util_LIB "${util_SRC}")
add_inkscape_source("${util_SRC}")
