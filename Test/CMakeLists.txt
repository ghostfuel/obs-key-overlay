project(Test)

set(Test_HEADERS
	main.h)

set(Test_SOURCES
	main.cpp)

add_library(win-capture MODULE
	${win-capture_SOURCES}
	${win-capture_HEADERS})
target_link_libraries(Test
	libobs)

install_obs_plugin(Test)
