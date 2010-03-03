//////////////////////////////////////////////////////////////////////////////////
// SPARK particle engine														//
// Copyright (C) 2008-2010 - Julien Fryer - julienfryer@gmail.com				//
//																				//
// This software is provided 'as-is', without any express or implied			//
// warranty.  In no event will the authors be held liable for any damages		//
// arising from the use of this software.										//
//																				//
// Permission is granted to anyone to use this software for any purpose,		//
// including commercial applications, and to alter it and redistribute it		//
// freely, subject to the following restrictions:								//
//																				//
// 1. The origin of this software must not be misrepresented; you must not		//
//    claim that you wrote the original software. If you use this software		//
//    in a product, an acknowledgment in the product documentation would be		//
//    appreciated but is not required.											//
// 2. Altered source versions must be plainly marked as such, and must not be	//
//    misrepresented as being the original software.							//
// 3. This notice may not be removed or altered from any source distribution.	//
//////////////////////////////////////////////////////////////////////////////////

#ifndef H_SPK_DEF
#define H_SPK_DEF

#include <cstdlib>

// for windows platform only
#if defined(WIN32) || defined(_WIN32)

#ifdef SPK_CORE_EXPORT
#define SPK_PREFIX __declspec(dllexport)
#elif defined(SPK_IMPORT) || defined(SPK_CORE_IMPORT)
#define SPK_PREFIX __declspec(dllimport)
#else
#define SPK_PREFIX
#endif

#ifdef _MSC_VER
#pragma warning(disable : 4251) // disables the warning about exporting STL classes in DLLs
#pragma warning(disable : 4996) // disables the deprecation of some functions
#pragma warning(disable : 4275) // disables the warning about exporting DLL classes children of non DLL classes
#endif

// other platforms than windows
#else
#define SPK_PREFIX
#endif

#define SPK_RANDOM(min,max) SPK::SPKMain::getInstance().generateRandom(min,max)
#define SPK_DEFAULT_ZONE	SPK::SPKMain::getInstance().getDefaultZone()

namespace SPK
{
	class Zone;

	enum Param
	{
		PARAM_SIZE = 0,
		PARAM_MASS = 1,
		PARAM_ANGLE = 2,
		PARAM_TEXTURE_INDEX = 3,
		PARAM_ROTATION_SPEED = 4,
	};

	class SPK_PREFIX SPKMain
	{
	public :

		static inline SPKMain& getInstance();

		inline Zone* getDefaultZone();

		template<typename T>
		inline T generateRandom(const T& min,const T& max);

	private :

		static SPKMain instance;

		Zone* defaultZone;
		unsigned int randomSeed;

		SPKMain();
		~SPKMain();
	};

	inline SPKMain& SPKMain::getInstance()
	{
		return instance;
	}

	inline Zone* SPKMain::getDefaultZone()
	{
		return defaultZone;
	}

	template<typename T>
	inline T SPKMain::generateRandom(const T& min,const T& max)
    {
		// optimized standard minimal
		long tmp0 = 16807L * (randomSeed & 0xFFFFL);
        long tmp1 = 16807L * (randomSeed >> 16);
        long tmp2 = (tmp0 >> 16) + tmp1;
        tmp0 = ((tmp0 & 0xFFFF)|((tmp2 & 0x7FFF) << 16)) + (tmp2 >> 15);

		// correction of the error
        if ((tmp0 & 0x80000000L) != 0)
			tmp0 = (tmp0 + 1) & 0x7FFFFFFFL;

		randomSeed = tmp0;

		// find a random number in the interval
        return static_cast<T>(min + ((randomSeed - 1) / 2147483646.0) * (max - min));
    }
}

#endif