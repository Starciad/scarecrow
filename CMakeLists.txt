cmake_minimum_required(VERSION 3.16)

project(scarecrow VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_subdirectory(lib/sdl)
add_subdirectory(lib/sdl_image)
add_subdirectory(lib/sdl_mixer)

add_executable(${PROJECT_NAME}
	src/game.c
	src/graphics.c
	src/input.c
	src/main.c
	src/mathematics.c
	src/player.c
	src/scenario.c
	src/tilemap.c
	src/transition.c
	src/world.c
)

add_custom_target(copy_resources ALL
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		${CMAKE_CURRENT_SOURCE_DIR}/res
		$<TARGET_FILE_DIR:${PROJECT_NAME}>/res
	DEPENDS ${PROJECT_NAME}
)

target_include_directories(${PROJECT_NAME}
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(${PROJECT_NAME}
	PRIVATE
		$<$<C_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic -Werror>
		$<$<C_COMPILER_ID:MSVC>:/W4 /WX>
)

target_link_libraries(${PROJECT_NAME}
	PRIVATE
		SDL3::SDL3
		SDL3_image::SDL3_image
		SDL3_mixer::SDL3_mixer
)
