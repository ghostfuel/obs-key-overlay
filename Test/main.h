#pragma once
//#include "obs.h"
#include "obs-module.h"

class main
{
public:
	main();
	~main();
};

static const char *key_overlay_source_get_name(void *unused);
static void *key_overlay_source_create(obs_data_t *settings, obs_source_t *source);
static void key_overlay_source_destroy(void *data);
static void key_overlay_source_update(void *data, obs_data_t *settings);