#pragma once
//#include "obs.h"
#include "obs-module.h"

class main
{
public:
	main();
	~main();
};

// Define a private messages. This message is sent when key is down/up 
#define WM_KEYSTROKE (WM_USER + 101) 
#define WM_KEYINPUT  (WM_USER + 102) 

static const char *key_overlay_source_get_name(void *unused);
static void *key_overlay_source_create(obs_data_t *settings, obs_source_t *source);
static void key_overlay_source_destroy(void *data);
static void key_overlay_source_update(void *data, obs_data_t *settings);