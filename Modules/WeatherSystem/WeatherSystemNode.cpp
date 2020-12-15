#include "WeatherSystemNode.h"
#include "scene/resources/particles_material.h"
#include "scene/resources/primitive_meshes.h"

WeatherNode::WeatherNode()
{
	mVolWidth = 8.0f;
	mVolHeight = 8.0f;
	mVolLength = 8.0f;
	mParticleAmt = 100;
	mParticleSpeed = 0.5f;
	mGravityStrength = 20.0f;
	mRain = false;
	mSnow = true;

	mPrevRain = false;
	mPrevSnow = false;

	ConstructVolume();
}

 WeatherNode::~WeatherNode() {
}

void WeatherNode::ConstructVolume()
{
	set_visibility_aabb(AABB(Vector3(-4, -4, -4), Vector3(mVolWidth, mVolHeight, mVolLength)));
	if (mRain)
	{
		//Init Rain particle system
		set_amount(mParticleAmt);
		set_speed_scale(mParticleSpeed); //2.0

		Ref<ParticlesMaterial> particleMat = memnew(ParticlesMaterial);
		particleMat->set_emission_shape(ParticlesMaterial::EMISSION_SHAPE_BOX);
		particleMat->set_emission_box_extents(Vector3(mVolWidth, mVolHeight, mVolLength));
		particleMat->set_gravity(Vector3(0.0f, -mGravityStrength, 0.0f)); //-50
		set_process_material(*particleMat);

		Ref<CubeMesh> rainMesh = memnew(CubeMesh);
		rainMesh->set_size(Vector3(0.02f, 0.5f, 0.02f));
		set_draw_pass_mesh(0, *rainMesh);

		Ref<SpatialMaterial> rainMat = memnew(SpatialMaterial);
		rainMat->set_flag(SpatialMaterial::FLAG_ALBEDO_FROM_VERTEX_COLOR, true);
		rainMat->set_flag(SpatialMaterial::FLAG_UNSHADED, true);
		rainMat->set_blend_mode(SpatialMaterial::BLEND_MODE_ADD);
		rainMat->set_grow(1);
		rainMat->set_albedo(Color(0.04f, 0.33f, 0.52f, 0.95f));

		set_material_override(*rainMat);
	}
	else if(mSnow)
	{
		//Init Snow particle system
		set_amount(mParticleAmt);
		mAmtChanged = false;
		set_speed_scale(mParticleSpeed); //0.5

		Ref<ParticlesMaterial> particleMat = memnew(ParticlesMaterial);
		particleMat->set_emission_shape(ParticlesMaterial::EMISSION_SHAPE_BOX);
		particleMat->set_emission_box_extents(Vector3(mVolWidth, mVolHeight, mVolLength));
		particleMat->set_gravity(Vector3(0.0f, -mGravityStrength, 0.0f)); //-20
		set_process_material(*particleMat);

		Ref<CubeMesh> snowMesh = memnew(CubeMesh);
		snowMesh->set_size(Vector3(0.02f, 0.08f, 0.02f));
		set_draw_pass_mesh(0, *snowMesh);

		Ref<SpatialMaterial> snowMat = memnew(SpatialMaterial);
		snowMat->set_flag(SpatialMaterial::FLAG_ALBEDO_FROM_VERTEX_COLOR, true);
		snowMat->set_flag(SpatialMaterial::FLAG_UNSHADED, true);
		snowMat->set_blend_mode(SpatialMaterial::BLEND_MODE_ADD);
		snowMat->set_grow(1);
		snowMat->set_albedo(Color(0.9f, 0.9f, 1.0f, 0.9f));

		set_material_override(*snowMat);
	}
}

void WeatherNode::CheckForUpdate()
{
	if (mRain && mSnow)
	{
		mRain = !mPrevRain;
		mSnow = !mPrevSnow;
	}
	else if (!mPrevRain && mRain)
	{
		ConstructVolume();
	}
	else if (!mPrevSnow && mSnow)
	{
		ConstructVolume();
	}
	if (mAmtChanged)
	{
		set_amount(mParticleAmt);
		mAmtChanged = false;
	}
	set_emitting(mRain || mSnow);
	set_visibility_aabb(AABB(Vector3(-4, -4, -4), Vector3(mVolWidth, mVolHeight, mVolLength)));
	static_cast<Ref<ParticlesMaterial> >(get_process_material())->set_emission_box_extents(Vector3(mVolWidth, mVolHeight, mVolLength));
	static_cast<Ref<ParticlesMaterial> >(get_process_material())->set_gravity(Vector3(0.0f, -mGravityStrength, 0.0f));
	//set_amount(mParticleAmt); //keeps refreshing particle system if done here
	set_speed_scale(mParticleSpeed);

	mPrevRain = mRain;
	mPrevSnow = mSnow;
}

void WeatherNode::_notification(int p_what)
{
	switch (p_what) {
		case NOTIFICATION_PROCESS:
			CheckForUpdate();
			break;
	}
}

void WeatherNode::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("ConstructVolume"), &WeatherNode::ConstructVolume);

	ClassDB::bind_method(D_METHOD("IsRainActive"), &WeatherNode::IsRainActive);
	ClassDB::bind_method(D_METHOD("IsSnowActive"), &WeatherNode::IsSnowActive);
	ClassDB::bind_method(D_METHOD("WetCamera"), &WeatherNode::WetCamera);

	ClassDB::bind_method(D_METHOD("SetRain", "rain"), &WeatherNode::SetRain);
	ClassDB::bind_method(D_METHOD("SetSnow", "snow"), &WeatherNode::SetSnow);
	ClassDB::bind_method(D_METHOD("SetWetCamera", "wetCamera"), &WeatherNode::SetWetCamera);

	//ClassDB::bind_method(D_METHOD("GetAffectedCamera"), &WeatherNode::GetAffectedCamera);
	//ClassDB::bind_method(D_METHOD("SetAffectedCamera", "camera"), &WeatherNode::SetAffectedCamera);

	ClassDB::bind_method(D_METHOD("GetVolWidth"), &WeatherNode::GetVolWidth);
	ClassDB::bind_method(D_METHOD("GetVolHeight"), &WeatherNode::GetVolHeight);
	ClassDB::bind_method(D_METHOD("GetVolLength"), &WeatherNode::GetVolLength);
	ClassDB::bind_method(D_METHOD("GetAmt"), &WeatherNode::GetAmt);
	ClassDB::bind_method(D_METHOD("GetSpeed"), &WeatherNode::GetSpeed);
	ClassDB::bind_method(D_METHOD("GetGravity"), &WeatherNode::GetGravity);

	ClassDB::bind_method(D_METHOD("SetVolWidth", "width"), &WeatherNode::SetVolWidth);
	ClassDB::bind_method(D_METHOD("SetVolHeight", "height"), &WeatherNode::SetVolHeight);
	ClassDB::bind_method(D_METHOD("SetVolLength", "length"), &WeatherNode::SetVolLength);
	ClassDB::bind_method(D_METHOD("SetAmt", "amt"), &WeatherNode::SetAmt);
	ClassDB::bind_method(D_METHOD("SetSpeed", "speed"), &WeatherNode::SetSpeed);
	ClassDB::bind_method(D_METHOD("SetGravity", "grav"), &WeatherNode::SetGravity);

	ADD_PROPERTY(PropertyInfo(Variant::REAL, "mVolWidth", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_EDITOR | PROPERTY_USAGE_STORAGE | PROPERTY_USAGE_CHECKABLE), "SetVolWidth", "GetVolWidth");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "mVolHeight", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_EDITOR | PROPERTY_USAGE_STORAGE | PROPERTY_USAGE_CHECKABLE), "SetVolHeight", "GetVolHeight");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "mVolLength", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_EDITOR | PROPERTY_USAGE_STORAGE | PROPERTY_USAGE_CHECKABLE), "SetVolLength", "GetVolLength");

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "mRain", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_EDITOR | PROPERTY_USAGE_STORAGE | PROPERTY_USAGE_CHECKABLE), "SetRain", "IsRainActive");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "mSnow", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_EDITOR | PROPERTY_USAGE_STORAGE | PROPERTY_USAGE_CHECKABLE), "SetSnow", "IsSnowActive");

	ADD_PROPERTY(PropertyInfo(Variant::INT, "mParticleAmt", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_EDITOR | PROPERTY_USAGE_STORAGE | PROPERTY_USAGE_CHECKABLE), "SetAmt", "GetAmt");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "mParticleSpeed", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_EDITOR | PROPERTY_USAGE_STORAGE | PROPERTY_USAGE_CHECKABLE), "SetSpeed", "GetSpeed");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "mGravityStrength", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_EDITOR | PROPERTY_USAGE_STORAGE | PROPERTY_USAGE_CHECKABLE), "SetGravity", "GetGravity");

}
