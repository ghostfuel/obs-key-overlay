#include "main.h"
#include "OverlaySource.h"

#include "obs-module.h"

#include <memory>
#include <iostream>
using namespace std;

// OBS Source for Key Overlay
struct key_overlay_source {
	obs_source_t	*source;
	
	size_t			size;
	uint32_t		width = 1024;
	uint32_t		height = 768;
};

static const char *key_overlay_source_get_name(void *unused)
{
	UNUSED_PARAMETER(unused);
	return obs_module_text("KeyOverlay");
}

static void *key_overlay_source_create(obs_data_t *settings, obs_source_t *source)
{
	//struct key_overlay_source *context = bzalloc(sizeof(struct key_overlay_source));
	//unique_ptr<key_overlay_source> context;
	key_overlay_source context;
	context.source = source;

	return context.source;
}

static void key_overlay_source_destroy(void *data)
{
	
}

static void key_overlay_source_update(void *data, obs_data_t *settings)
{

}

static uint32_t key_overlay_source_getwidth(void *data)
{
	key_overlay_source context;
	return context.width;
}

static uint32_t key_overlay_source_getheight(void *data)
{
	key_overlay_source context;
	return context.height;
}


// OBS Source Info for Key Overlay
static struct obs_source_info key_overlay_source_info = {
	key_overlay_source_info.id = "key_overlay_source",
	key_overlay_source_info.type = OBS_SOURCE_TYPE_INPUT,
	key_overlay_source_info.output_flags = OBS_SOURCE_VIDEO,
	key_overlay_source_info.get_name = key_overlay_source_get_name,
	key_overlay_source_info.create = key_overlay_source_create,
	key_overlay_source_info.destroy = key_overlay_source_destroy,
	//key_overlay_source_info.update = key_overlay_source_update,
	key_overlay_source_info.get_width = key_overlay_source_getwidth,
	key_overlay_source_info.get_height = key_overlay_source_getheight,
	/*.update = image_source_update,
	.get_defaults = image_source_defaults,
	.show = image_source_show,
	.hide = image_source_hide,
	.video_render = image_source_render,
	.video_tick = image_source_tick,
	.get_properties = image_source_properties*/
};

OBS_DECLARE_MODULE();
OBS_MODULE_USE_DEFAULT_LOCALE("obs-key-overlay", "en-US");

bool obs_module_load(void)
{
	//overlay_source_info = create_overlay_source_info();
	obs_register_source(&key_overlay_source_info);

	return true;
}

void obs_module_unload(void)
{

}