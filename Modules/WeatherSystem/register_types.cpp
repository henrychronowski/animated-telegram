#include "register_types.h"
#include "core/class_db.h"
#include "WeatherSystemNode.h"

void register_WeatherSystemNode_types() {
	ClassDB::register_class<WeatherNode>();
	
}

void unregister_WeatherSystemNode_types() {
}
