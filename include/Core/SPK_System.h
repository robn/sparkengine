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

#ifndef H_SPK_SYSTEM
#define H_SPK_SYSTEM

#include <vector>

#include "Core/SPK_DEF.h"
#include "Core/SPK_Logger.h"
#include "Core/SPK_Transformable.h"
#include "Core/SPK_Vector3D.h"

namespace SPK
{
	class Group;

	/**
	* @enum StepMode
	* @brief Enumeration defining how to handle the step time of particle systems
	*/
	enum StepMode
	{
		STEP_MODE_REAL,			/**< The step time is the deltatime passed by the user */
		STEP_MODE_CONSTANT,		/**< The step time is a constant time therefore 0 to many updates may occur in a call */
		STEP_MODE_ADAPTIVE,		/**< The step time is a range between 2 values therefore 0 to many updates may occur in a call */
	};

	/**
	* @brief A class defining a complete system of particles
	*/
	class SPK_PREFIX System : public Transformable
	{
	public :

		//////////////////
		// Constructors //
		//////////////////
		
		/** @brief Default constructor of system */
		inline System(bool initialize = true);

		/** 
		* @brief Copy constructor of system
		* @param system : the system to copy from
		*/
		System(const System& system);

		////////////////
		// Destructor //
		////////////////

		/** @brief Destructor of system */
		virtual ~System();

		///////////////////////
		// Groups management //
		///////////////////////

		/**
		* @brief Adds a group to the system
		* @param group : a pointer on the group to add to the system
		*/
		Group* createGroup(size_t capacity);

		/**
		* @brief Removes a group from the system
		*
		* If the group cannot be found, nothing happens.
		*
		* @param group : a pointer on the group to remove from the system
		*/
		void destroyGroup(Group* group);

		/**
		* @brief Gets the group at index
		* @param index : the index of the group to get
		* @return the group at index
		*/
		inline Group* getGroup(size_t index) const;

		/**
		* @brief Gets the number of groups in the system
		* @return the number of groups in the system
		*/
		inline size_t getNbGroups() const;

		/**
		* @brief Gets the number of particles in the system
		* @return the number of particles in the system
		*/
		size_t getNbParticles() const;

		/////////////////////////////
		// Operations on particles //
		/////////////////////////////

		/**
		* @brief Updates the particles in the system of the current time step
		*
		* Note that this method updates all groups in the system from first to last.
		*
		* @param deltaTime : the time step
		* @return true if the System is still active (has active groups)
		*/
		bool updateParticles(float deltaTime);

		/**
		* @brief Renders particles in the System
		*
		* Note that this method renders all groups in the System from first to last.
		*/
		void renderParticles() const;

		//////////////////
		// Bounding Box //
		//////////////////

		/**
		* @brief Enables or disables the computation of the axis aligned Vector for this System
		* @param AABB : true to enable the computing of the AABB of this System, false to disable it
		*/
		inline void enableAABBComputation(bool AABB);

		/**
		* @brief Tells whether the computation of the axis aligned bouding box is enabled
		* @return true if the computation of the AABB is enabled, false if it is disabled
		*/
		inline bool isAABBComputationEnabled() const;

		/**
		* @brief Gets a Vector3D holding the minimum coordinates of the AABB of this System.
		*
		* Note that this method is only useful when the AABB computation is enabled (see enableAABBComputation(bool)).
		*
		* @return a Vector3D holding the minimum coordinates of the AABB of this System
		* @since 1.01.00
		*/
		inline const Vector3D& getAABBMin() const;

		/**
		* @brief Gets a Vector3D holding the maximum coordinates of the AABB of this System.
		*
		* Note that this method is only useful when the AABB computation is enabled (see enableAABBComputation(bool)).
		*
		* @return a Vector3D holding the maximum coordinates of the AABB of this System
		* @since 1.01.00
		*/
		inline const Vector3D& getAABBMax() const;

		/////////////////////
		// Camera position //
		/////////////////////

		/**
		* @brief Sets the camera position
		*
		* Note that the camera position is only useful if the distance computation of a group is enabled (when the group must be sorted for instance).<br>
		* In that case this vector will be used as the camera position to derive the distance between the particle and the camera position.<br>
		* The camera position has to be updated before an update of the sorted group.
		*
		* @param cameraPosition the camera position
		*/
		static inline void setCameraPosition(const Vector3D& cameraPosition);

		/**
		* @brief Gets the camera position
		* @return the camera position
		*/
		static inline const Vector3D& getCameraPosition();

		///////////////
		// Step Mode //
		///////////////

		/**
		* @brief Enables or not the clamping on the deltaTime when updating systems
		*
		* This allows to limit too big delta times which may spoil your particle systems.<br>
		* Basically if the deltaTime is higher than the clamp value, the clamp value is used as the deltaTime.<br>
		* <br>
		* It allows in real step mode to avoid having too big delta times and in the other 2 modes to avoid having too
		* many updates that may slow down the application.<br>
		* <br>
		* Note that setting the clamp value too low may slow down your systems
		*
		* @param useClampStep : true to use a clamp value on the step, false not to
		* @param clamp : the clamp value
		*/
		static inline void setClampStep(bool useClampStep,float clamp = 1.0f);

		/**
		* @brief Uses a constant step to update the systems
		*
		* This tells the system to be updated with a constant time.<br>
		* Depending on the deltaTime passed for the update, 0 to many updates can occur.<br>
		* For example if the delta time is 1.0 and the constant step is 0.1 then 10 updates of time 0.1 will occur.<br>
		* <br>
		* This mode is useful when the update must be constant (accurate collisions...) but be aware it can be very computationnaly intensive.
		* 
		* @param constantStep : the value of the step
		*/
		static inline void useConstantStep(float constantStep);

		/**
		* @brief Uses an adaptive step to update the systems
		*
		* This tells the system to be updated with a time between min and max.<br>
		* If the deltaTime passed is higher than maxStep or lower than minStep then this mode operates like the constant step mode with
		* either constant time being maxStep or minStep (respectivally).<br>
		* If the deltaTime lies between minStep and maxStep then this mode performs like the real step mode.<br>
		* <br>
		* This mode is a good alternative between the other two.<br>
		* Combined with the clamp step, it allows to correctly handle the step time without being to much frame rate dependant.
		*
		* @param minStep : the minimal time step
		* @param maxStep : the maximal time step
		*/
		static inline void useAdaptiveStep(float minStep,float maxStep);

		/**
		* @brief Uses the real step to update the systems
		*
		* This is the basic mode (and the mode per default) to update the systems.<br>
		* One call to update means one update of time deltaTime.<br>
		* <br>
		* This mode is the simpler and the one that allows best performance on low end systems.<br>
		* However the update may be inaccurate (due to too big deltaTime) and it performs badly with frame rate variation.
		*/
		static inline void useRealStep();

		/**
		* @brief Gets the current step mode
		* @return the current step mode
		*/
		static inline StepMode getStepMode();

		void initialize();
		inline bool isInitialized() const;

	protected :

		virtual void propagateUpdateTransform();

	private :

		static Vector3D cameraPosition;

		// Step mode
		static StepMode stepMode;
		static float constantStep;
		static float minStep;
		static float maxStep;

		static bool clampStepEnabled;
		static float clampStep;

		float deltaStep;

		bool initialized;

		// AABB
		bool AABBComputationEnabled;
		Vector3D AABBMin;
		Vector3D AABBMax;

		std::vector<Group*> groups; // vector containing all the groups of the system

		bool innerUpdate(float deltaTime);
	};

	inline System::System(bool initialize) :
		groups(),
		deltaStep(0.0f),
		AABBComputationEnabled(false),
		AABBMin(),
		AABBMax(),
		initialized(initialize)
	{}

	inline Group* System::getGroup(size_t index) const
	{
		SPK_ASSERT(index < getNbGroups(),"System::getGroup(size_t) - Index of group is out of bounds : " << index);
		return groups[index];
	}

	inline size_t System::getNbGroups() const
	{
		return groups.size();
	}

	inline void System::enableAABBComputation(bool AABB)
	{
		AABBComputationEnabled = AABB;
	}

	inline bool System::isAABBComputationEnabled() const
	{
		return AABBComputationEnabled;
	}

	inline const Vector3D& System::getAABBMin() const
	{
		return AABBMin;
	}

	inline const Vector3D& System::getAABBMax() const
	{
		return AABBMax;
	}

	inline void System::setCameraPosition(const Vector3D& cameraPosition)
	{
		System::cameraPosition = cameraPosition;
	}

	inline const Vector3D& System::getCameraPosition()
	{
		return cameraPosition;
	}

	inline void System::setClampStep(bool enableClampStep,float clamp)
	{
		clampStepEnabled = enableClampStep;
		clampStep = clamp;
	}

	inline void System::useConstantStep(float constantStep)
	{
		stepMode = STEP_MODE_CONSTANT;
		System::constantStep = constantStep;
	}

	inline void System::useAdaptiveStep(float minStep,float maxStep)
	{
		stepMode = STEP_MODE_ADAPTIVE;
		System::minStep = minStep;
		System::maxStep = maxStep;
	}

	inline void System::useRealStep()
	{
		stepMode = STEP_MODE_REAL;
	}

	inline StepMode System::getStepMode()
	{
		return stepMode;
	}

	inline bool System::isInitialized() const
	{
		return initialized;
	}
}

#endif