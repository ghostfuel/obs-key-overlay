#include "main.h"
#include "obs-module.h"

OBS_DECLARE_MODULE()
//OBS_MODULE_USE_DEFAULT_LOCALE("obs-key-overlay", "en-US")

//extern struct obs_source_info create_overlay_info_source;

bool obs_module_load(void)
{
	//create_overlay_info_source = { 0 };
	//create_overlay_info_source.id = "overlay_source";
	//obs_register_source(&create_overlay_info_source);
	//obs_register_source(&my_source);
	return true;
}

void obs_module_unload(void)
{

}