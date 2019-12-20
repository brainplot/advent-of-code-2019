if (MSVC)
	add_compile_options(/utf-8 /W4)
	add_compile_definitions(UNICODE)
	add_compile_definitions(_UNICODE)
else()
	add_compile_options(-Wall -Wextra -pedantic)
endif()

add_library(common STATIC common.cpp)
add_library(intcode STATIC intcode.cpp)

macro(add_challenge challenge)
	string(REGEX REPLACE "\\.[^.]*$" "" BASENAME ${challenge})
	add_executable(${BASENAME})
	target_sources(${BASENAME} PRIVATE ${BASENAME}.cpp)
	target_link_libraries(${BASENAME} common)
endmacro()

file(GLOB CHALLENGES LIST_DIRECTORIES false RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} day_*.cpp)

foreach(CHALLENGE ${CHALLENGES})
	add_challenge(${CHALLENGE})
endforeach()

set(INTCODE_CHALLENGES 2;5;7;9)
foreach(CHL ${INTCODE_CHALLENGES})
	target_link_libraries(day_${CHL} intcode)
endforeach()
