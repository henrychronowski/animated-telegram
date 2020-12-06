#include "register_types.h"
#include "core/class_db.h"
#include "WeatherSystemNode.h"

void register_WeatherSystem_types() {
	ClassDB::register_class<WeatherNode>();
}

void unregister_WeatherSystem_types() {
}
