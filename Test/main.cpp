//#include "main.h"
#include "OverlaySource.h"

#include "obs-module.h"

#include <iostream>
using namespace std;

OBS_DECLARE_MODULE()
//OBS_MODULE_USE_DEFAULT_LOCALE("obs-key-overlay", "en-US")

struct obs_source_info create_overlay_source_info();
struct obs_source_info overlay_source_info;

bool obs_module_load(void)
{
	overlay_source_info = create_overlay_source_info();

	obs_register_source(&overlay_source_info);

	return true;
}

void obs_module_unload(void)
{

}

///// OBS STRUCT CONSTRUCTION //////

/*static const char *overlay_source_get_name()
{
	return obs_module_text("OverlaySource");
}*/

// OBS Source struct 
struct obs_source_info create_overlay_source_info()
{
	struct obs_source_info overlay_source_info = { 0 };

	overlay_source_info.id = "overlay_source";
	overlay_source_info.type = OBS_SOURCE_TYPE_INPUT;
	overlay_source_info.output_flags = OBS_SOURCE_VIDEO | OBS_SOURCE_INTERACTION;

#ifdef __APPLE__
	browser_source_info.output_flags |= OBS_SOURCE_CUSTOM_DRAW;
#endif

	// Interactions go here
	//overlay_source_info.get_name = overlay_source_get_name;
	//overlay_source_info.create = overlay_source_create;
	//overlay_source_info.destroy = overlay_source_destroy;

	return overlay_source_info;
}