/**
 * \file config.h
 */

#pragma once

#ifdef _MSC_VER
#pragma warning(disable : 4275 4251)
#ifdef BUILD_AstroQtIO
#define ASTRO_QTIO_EXPORT __declspec(dllexport)
#else
#define ASTRO_QTIO_EXPORT __declspec(dllimport)
#endif
#else
#define ASTRO_QTIO_EXPORT
#endif
