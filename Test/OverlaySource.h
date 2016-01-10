#pragma once

#include <obs-module.h>
#include <util/platform.h>
#include <obs-output.h>

class OverlaySource
{
public:
	OverlaySource(obs_data_t *settings, obs_source_t *source);
	~OverlaySource();

private:
	obs_source_t *source;

	int overlayIdentifier;
};

