#pragma once
//insert the Headername here
#ifndef WEATHERSYSTEMNODE_H
#define WEATHERSYSTEMNODE_H

#include "scene/3d/camera.h"
#include "scene/3d/particles.h"
#include "scene/3d/cpu_particles.h"
#include "scene/3d/spatial.h"

class WeatherNode : public Particles {
	GDCLASS(WeatherNode, Particles)
public:
	WeatherNode();
	~WeatherNode();

	void ConstructVolume();

	bool IsRainActive() const { return mRain; }
	bool IsSnowActive() const { return mSnow; }
	bool WetCamera() const { return mWetCamera; }

	void SetRain(bool raining) { mRain = raining; }
	void SetSnow(bool snowing) { mSnow = snowing; }
	void SetWetCamera(bool wetCamera) { mWetCamera = wetCamera; }

	//Camera GetAffectedCamera() const { return mCamera; }
	//void SetAffectedCamera(Camera camera) { mCamera = camera; }

	float GetVolWidth() const { return mVolWidth; }
	float GetVolHeight() const { return mVolHeight; }
	float GetVolLength() const { return mVolLength; }

	int GetAmt() const { return mParticleAmt; }
	float GetSpeed() const { return mParticleSpeed; }
	float GetGravity() const { return mGravityStrength; }

	void SetVolWidth(float width) { mVolWidth = width; }
	void SetVolHeight(float height) { mVolHeight = height; }
	void SetVolLength(float length) { mVolLength = length; }

	void SetAmt(int amt) {
		mParticleAmt = amt;
		mAmtChanged = true;
	}
	void SetSpeed(float speed) { mParticleSpeed = speed; }
	void SetGravity(float grav) { mGravityStrength = grav; }

	void CheckForUpdate();

	void _notification(int p_what);

protected:
	static void _bind_methods();

private:
	//Camera mCamera = Camera(); //Camera affected by the WeatherNode
	int mParticleAmt;
	bool mAmtChanged;

	float mVolWidth;
	float mVolLength;
	float mVolHeight;
	float mParticleSpeed;
	float mGravityStrength;

	bool mRain; //Is raining
	bool mSnow; //Is snowing
	bool mWetCamera; //Should rain affect camera

	bool mPrevRain;
	bool mPrevSnow;
};

#endif
