#include "main.h"

#include "obs-module.h"
#include "obs-hotkey.h"

#include <conio.h>
#include <iostream>
#include <Windows.h>
#include <util/platform.h>
#include <sys/stat.h>
using namespace std;


// Migrate to header, plus create new Hook class
HHOOK hook_handler = NULL;			// Hook handler
HHOOK windows_kb_hook = NULL;		// Instance of Keyboard Hook
int last_key = NULL;				// Store last hooked key (try to find a way to store this in overlay struct

// Keyboard Hook 
// Currently does not handle System keys, such as CTRL. Also only does one key at a time, no CTRL+5 (%).
LRESULT CALLBACK KeyboardHook(int n, WPARAM wParam, LPARAM lParam)
{
	KBDLLHOOKSTRUCT  *virtual_kb = (KBDLLHOOKSTRUCT *)lParam;

	//Check Input originates from HID/Keyboard/Virtual Keyboard
	if ((virtual_kb->flags & LLKHF_INJECTED))
		return ::CallNextHookEx(hook_handler, n, wParam, lParam); //continue

	if (wParam == WM_KEYDOWN)
		last_key = virtual_kb->vkCode; //Store key
	if (wParam == WM_SYSKEYDOWN)
		last_key = virtual_kb->vkCode; //Store SysKey

	return ::CallNextHookEx(hook_handler, n, wParam, lParam); //continue
}

OBS_DECLARE_MODULE();
OBS_MODULE_USE_DEFAULT_LOCALE("obs-key-overlay", "en-US");

// OBS Source for Key Overlay
struct key_overlay_source {
	obs_source_t	*source;
	
	char			*file_path;
	bool			persistent;
	time_t			file_timestamp;
	float			update_time_elapsed;
	gs_texture_t	*texture;

	uint32_t		width;
	uint32_t		height;

	key_overlay_source()
	{
		source;
		file_path = "keys.png";
		width = 0;
		height = 0;
	}
};

static time_t get_modified_timestamp(const char *filename)
{
	struct stat stats;
	stat(filename, &stats);
	return stats.st_mtime;
}

static void key_overlay_source_load(struct key_overlay_source *context)
{
	char *file = context->file_path;

	obs_enter_graphics();

	//if (context->texture)
	//	gs_texture_destroy(context->texture);
	context->texture = NULL;

	if (file && *file) 
	{
		context->file_timestamp = get_modified_timestamp(file);
		context->texture = gs_texture_create_from_file(file);
		context->update_time_elapsed = 0;

		if (context->texture) 
		{
			context->width = gs_texture_get_width(context->texture);
			context->height = gs_texture_get_height(context->texture);
		} 
		else 
		{
			context->width = 0;
			context->height = 0;
		}
	}
	
	obs_leave_graphics();
}

static void key_overlay_source_unload(struct key_overlay_source *context)
{
	obs_enter_graphics();

	//if (context->texture)
	//	gs_texture_destroy(context->texture);
	context->texture = NULL;

	obs_leave_graphics();
}

static const char *key_overlay_source_get_name(void *unused)
{
	UNUSED_PARAMETER(unused);
	return obs_module_text("KeyOverlay");
}

static void *key_overlay_source_create(obs_data_t *settings, obs_source_t *source)
{
	key_overlay_source *context = new key_overlay_source();

	context->source = source;

	key_overlay_source_update(context, settings);

	return context;
}

static void key_overlay_source_destroy(void *data)
{
	key_overlay_source *context = (key_overlay_source *)data;

	key_overlay_source_unload(context);

	//if (context->file_path)
		//bfree(context->file_path);
	//bfree(context);
}

static void key_overlay_source_update(void *data, obs_data_t *settings)
{
	key_overlay_source *context = (key_overlay_source *)data;
	const char *file = obs_data_get_string(settings, "file");
	const bool unload = obs_data_get_bool(settings, "unload");

	//if (context->file_path)
		//bfree(context->file_path);
	context->file_path = bstrdup(file);
	context->persistent = !unload;

	/* Load the image if the source is persistent or showing */
	if (context->persistent || obs_source_showing(context->source))
		key_overlay_source_load(context);
	else
		key_overlay_source_unload(context);
}

static uint32_t key_overlay_source_getwidth(void *data)
{
	key_overlay_source *context = (key_overlay_source *)data;
	return context->width;
}

static uint32_t key_overlay_source_getheight(void *data)
{
	key_overlay_source *context = (key_overlay_source *)data;
	return context->height;
}

static void key_overlay_source_render(void *data, gs_effect_t *effect)
{
	key_overlay_source *context = (key_overlay_source *)data;

	if (!context->texture) return;

	gs_reset_blend_state();
	gs_effect_set_texture(gs_effect_get_param_by_name(effect, "image"), context->texture);
	gs_draw_sprite(context->texture, 0, context->width, context->height);
}

static void key_overlay_source_tick(void *data, float seconds)
{
	key_overlay_source *context = (key_overlay_source *)data;
	
	//Test key inputs
	int key = last_key; //Has the ability to store system key presses
	obs_key_t obs_key = obs_key_from_virtual_key(key); //not all keys convert to this? :S
	
	switch (key)
	{
	case 65: //A
		context->file_path = "C:/Users/mitch/OneDrive/G403 Computer Science/Level 3/Project/obs-studio/build/plugins/obs-key-overlay/Test/Source/OBS_KEY_A.png";
		break;
	case 66: //B
		context->file_path = "C:/Users/mitch/OneDrive/G403 Computer Science/Level 3/Project/obs-studio/build/plugins/obs-key-overlay/Test/Source/OBS_KEY_B.png";
		break;
	case 67: //C
		context->file_path = "C:/Users/mitch/OneDrive/G403 Computer Science/Level 3/Project/obs-studio/build/plugins/obs-key-overlay/Test/Source/OBS_KEY_C.png";
		break;
	default:
		break;
	}

	if (!obs_source_showing(context->source)) return;

	context->update_time_elapsed += seconds;

	if (context->update_time_elapsed >= 1.0f) {
		time_t t = get_modified_timestamp(context->file_path);
		context->update_time_elapsed = 0.0f;

		if (context->file_timestamp < t) {
			key_overlay_source_load(context);
		}
	}
}

static void key_overlay_source_click(void *data, const struct obs_key_event *event, bool key_up)
{
	// If successful, don't forget to implment the Focus obs-source method event. Line 346 @ obs-source.h
	// This could be used to ensure that key presses are only recognised when the source is in focus.
	// The only issue being that it is not *this* source's focus that is of interest.
	key_overlay_source *context = (key_overlay_source *)data;
	obs_key_event *key_event = (obs_key_event *)event;
	char log = (char)key_event->text;


}

static const char *file_filter =
"All formats (*.bmp *.tga *.png *.jpeg *.jpg *.gif);;"
"BMP Files (*.bmp);;"
"Targa Files (*.tga);;"
"PNG Files (*.png);;"
"JPEG Files (*.jpeg *.jpg);;"
"GIF Files (*.gif)";

static obs_properties_t *key_overlay_source_properties(void *unused)
{
	UNUSED_PARAMETER(unused);

	obs_properties_t *props = obs_properties_create();

	obs_properties_add_path(props,
		"file", obs_module_text("File"),
		OBS_PATH_FILE, file_filter, NULL);

	obs_properties_add_bool(props,
		"unload", obs_module_text("UnloadWhenNotShowing"));

	return props;
}


bool obs_module_load(void)
{
	windows_kb_hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHook, 0, 0);

	// OBS Source Info for Key Overlay
	static struct obs_source_info key_overlay_source_info = {};
	key_overlay_source_info.id = "key_overlay_source",
	key_overlay_source_info.type = OBS_SOURCE_TYPE_INPUT,
	key_overlay_source_info.output_flags = OBS_SOURCE_VIDEO,
	key_overlay_source_info.get_name = key_overlay_source_get_name,
	key_overlay_source_info.create = key_overlay_source_create,
	key_overlay_source_info.destroy = key_overlay_source_destroy,
	key_overlay_source_info.update = key_overlay_source_update,
	key_overlay_source_info.get_width = key_overlay_source_getwidth,
	key_overlay_source_info.get_height = key_overlay_source_getheight,
	key_overlay_source_info.video_render = key_overlay_source_render,
	key_overlay_source_info.video_tick = key_overlay_source_tick,
	key_overlay_source_info.get_properties = key_overlay_source_properties,
	key_overlay_source_info.key_click = key_overlay_source_click,

	/* To add next:
	.get_defaults = image_source_defaults,
	.show = image_source_show,
	.hide = image_source_hide,
	*/

	obs_register_source(&key_overlay_source_info);

	return true;
}

void obs_module_unload(void)
{
	UnhookWindowsHookEx(windows_kb_hook); //Make sure the plugin does not KeyLog to cache
	return;
}