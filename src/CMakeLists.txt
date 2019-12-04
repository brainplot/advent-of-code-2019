if (MSVC)
	add_compile_options(/utf-8 /W4)
	add_compile_definitions(UNICODE)
	add_compile_definitions(_UNICODE)
else()
	add_compile_options(-Wall -Wextra -pedantic)
endif()

macro(add_challenge challenge)
	add_executable(${challenge})
	target_sources(${challenge} PRIVATE ${challenge}.cpp)
endmacro()

add_challenge(day_2)
