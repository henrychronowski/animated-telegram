#pragma once
//insert the Headername here
#ifndef WEATHERSYSTEMNODE_H
#define WEATHERSYSTEMNODE_H

#include "scene/3d/camera.h"
#include "scene/3d/particles.h"
#include "scene/3d/spatial.h"
#include "core/reference.h"

class WeatherNode : public Spatial {
	GDCLASS(WeatherNode, Spatial)
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

	Camera* GetAffectedCamera() const { return mCamera; }
	void SetAffectedCamera(Camera* camera) { mCamera = camera; }

protected:
	static void _bind_methods();

private:
	Particles mParticleNode;
	Camera* mCamera; //Camera affected by the WeatherNode

	float mVolWidth;
	float mVolLength;
	float mVolHeight;

	bool mRain; //Is raining
	bool mSnow; //Is snowing
	bool mWetCamera; //Should rain affect camera
};

#endif
