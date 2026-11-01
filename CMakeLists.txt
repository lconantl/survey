cmake_minimum_required(VERSION 3.21)

project(
	survey
	VERSION 1.0.0
	DESCRIPTION "Interactive console surveys: arrows, colors, text and password input"
	LANGUAGES CXX)

file(GLOB_RECURSE SURVEY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_library(survey STATIC ${SURVEY_SOURCES})
add_library(survey::survey ALIAS survey)

target_include_directories(
	survey
	PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/lib"
	PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")

target_compile_features(survey PUBLIC cxx_std_20)

if(MSVC)
	target_compile_options(survey PRIVATE /utf-8 /W4 /EHsc /Zc:__cplusplus)
else()
	target_compile_options(survey PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(PROJECT_IS_TOP_LEVEL)
	option(SURVEY_BUILD_TESTS "Build GoogleTest tests" ON)

	if(MSVC)
		target_compile_options(survey PRIVATE /WX)
	else()
		target_compile_options(survey PRIVATE -Werror)
	endif()

	add_executable(survey-example main.cpp)
	target_link_libraries(survey-example PRIVATE survey::survey)

	if(MSVC)
		target_compile_options(survey-example PRIVATE /utf-8)
	endif()

	if(MINGW)
		target_link_options(survey-example PRIVATE -static)
	endif()

	if(SURVEY_BUILD_TESTS)
		include(FetchContent)
		FetchContent_Declare(
			googletest
			GIT_REPOSITORY https://github.com/google/googletest.git
			GIT_TAG v1.15.2
			GIT_SHALLOW TRUE)
		set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
		set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
		FetchContent_MakeAvailable(googletest)

		enable_testing()
		file(GLOB_RECURSE SURVEY_TEST_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.test.cpp")
		add_executable(survey-tests ${SURVEY_TEST_FILES})
		target_include_directories(survey-tests PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
		target_link_libraries(survey-tests PRIVATE survey::survey GTest::gtest_main)

		if(MSVC)
			target_compile_options(survey-tests PRIVATE /utf-8)
		endif()

		if(MINGW)
			target_link_options(survey-tests PRIVATE -static)
		endif()

		include(GoogleTest)
		gtest_discover_tests(survey-tests DISCOVERY_MODE PRE_TEST)
	endif()
endif()
