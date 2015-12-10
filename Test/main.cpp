#include "main.h"
//#include "obs.h"
#include "obs-module.h"
#include "obs-source.h"

//OBS_DECLARE_MODULE()

extern struct obs_source_info my_source;

bool obs_module_load(void)
{
	//obs_register_source(&my_source);
	return true;
}

void obs_module_unload(void)
{

}

main::main()
{
}


main::~main()
{
}