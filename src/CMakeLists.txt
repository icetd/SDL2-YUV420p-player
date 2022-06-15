set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )

set(SDL2_libs_DIR /home/tiandeng/share/Dev/Thirlib/lib/SDL2/lib )
set(SDL2_headers_DIR /home/tiandeng/share/Dev/Thirlib/lib/SDL2/include )


add_library( SDL2 SHARED IMPORTED )
set_target_properties( SDL2 PROPERTIES IMPORTED_LOCATION ${SDL2_libs_DIR}/libSDL2.so )

include_directories(${SDL2_headers_DIR} )
link_directories(${SDL2_libs_DIR})

add_executable(SDL_mouse_move
	SDL_mouse_move.c)
target_link_libraries( SDL_mouse_move  SDL2)

add_executable(SDL_YUV420_player
	SDL_YUV420_player.c)
target_link_libraries( SDL_YUV420_player SDL2)


