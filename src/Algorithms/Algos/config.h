/**
 * \file config.h
 */

#pragma once

#ifdef _MSC_VER
#pragma warning(disable : 4275 4251)
#ifdef BUILD_AstroAlgorithms
#define ASTRO_ALGORITHMS_EXPORT __declspec(dllexport)
#else
#define ASTRO_ALGORITHMS_EXPORT __declspec(dllimport)
#endif
#else
#define ASTRO_ALGORITHMS_EXPORT
#endif

#ifndef ITK_UNEQUAL_OPERATOR_MEMBER_FUNCTION
#if __cplusplus < 202002L
// For C++14 and C++17, this macro defines an operator!= member function that
// just calls the corresponding operator== member function.
#define ITK_UNEQUAL_OPERATOR_MEMBER_FUNCTION(TypeName)                                                                 \
    bool operator!=(const TypeName& other) const                                                                       \
    {                                                                                                                  \
        return !(this->operator==(other));                                                                             \
    }
#else
// With C++20, operator!= is defined implicitly, calling the corresponding operator==.
#define ITK_UNEQUAL_OPERATOR_MEMBER_FUNCTION(TypeName)
#endif
#endif
