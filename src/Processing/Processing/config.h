/**
 * \file config.h
 */

#pragma once

#ifdef _MSC_VER
#pragma warning(disable : 4275 4251)
#ifdef BUILD_AstroProcessing
#define ASTRO_PROCESSING_EXPORT __declspec(dllexport)
#else
#define ASTRO_PROCESSING_EXPORT __declspec(dllimport)
#endif
#else
#define ASTRO_PROCESSING_EXPORT
#endif
