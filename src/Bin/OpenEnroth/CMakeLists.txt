cmake_minimum_required(VERSION 3.24 FATAL_ERROR)

set(BIN_OPENENROTH_SOURCES
        OpenEnroth.cpp
        OpenEnrothOptions.cpp)

set(BIN_OPENENROTH_HEADERS
        OpenEnrothOptions.h)

if(OE_BUILD_PLATFORM STREQUAL "android")
    add_library(main SHARED)
    target_sources(main PUBLIC ${BIN_OPENENROTH_HEADERS} ${BIN_OPENENROTH_SOURCES})
    target_check_style(main)
    target_link_libraries(main PUBLIC application library_cli library_platform_main library_stack_trace Glob)
    target_link_options(main PRIVATE "-Wl,--version-script=${CMAKE_CURRENT_SOURCE_DIR}/libmain.map")
else()
    if (WIN32)
        add_executable(OpenEnroth ${BIN_OPENENROTH_HEADERS} ${BIN_OPENENROTH_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/win_OpenEnroth.rc)
    elseif(APPLE)
        # OSX Code Signing
        set(TEAM_ID "" CACHE STRING "The development team ID for code signing")
        IF(NOT ${TEAM_ID} STREQUAL "")
            message(STATUS "TEAM_ID is set so sign OSX")
            set(CMAKE_MACOSX_BUNDLE YES) # after this line, paste this:

            set(XCODE_ATTRIBUTE_CODE_SIGN_STYLE "Manual")
            set(XCODE_ATTRIBUTE_DEVELOPMENT_TEAM ${TEAM_ID})
            set(XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "Developer ID Application")
            set(XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED TRUE)
	endif()
    
        # Set apple plist items and icon
        set(MACOSX_BUNDLE_BUNDLE_NAME "OpenEnroth")
        set(MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}")
        set(MACOSX_BUNDLE_COPYRIGHT "GPLv3")
        set(MACOSX_BUNDLE_GUI_IDENTIFIER "OpenEnroth")
        set(MACOSX_BUNDLE_ICON_FILE "osx_OpenEnroth.icns")
        set(MACOSX_BUNDLE_INFO_STRING "Open reimplementation of Might and Magic 6 7 8 game engine.")
        set(MACOSX_BUNDLE_LONG_VERSION_STRING "${PROJECT_VERSION}")
        set(MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION}")
        set(OpenEnroth_ICON ${CMAKE_CURRENT_SOURCE_DIR}/osx_OpenEnroth.icns)
        set_source_files_properties(${OpenEnroth_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
        add_executable(OpenEnroth MACOSX_BUNDLE ${OpenEnroth_ICON} ${BIN_OPENENROTH_HEADERS} ${BIN_OPENENROTH_SOURCES})

	IF(NOT ${TEAM_ID} STREQUAL "")
            set_target_properties(OpenEnroth PROPERTIES
                XCODE_ATTRIBUTE_CODE_SIGN_STYLE ${XCODE_ATTRIBUTE_CODE_SIGN_STYLE}
                XCODE_ATTRIBUTE_DEVELOPMENT_TEAM ${XCODE_ATTRIBUTE_DEVELOPMENT_TEAM}
                XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY ${XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY}
                XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED ${XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED}
            ) 
	endif()
    else ()
        add_executable(OpenEnroth ${BIN_OPENENROTH_HEADERS} ${BIN_OPENENROTH_SOURCES})
    endif()
	
    target_check_style(OpenEnroth)
    target_link_libraries(OpenEnroth PUBLIC application library_cli library_platform_main library_stack_trace Glob)

    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT OpenEnroth)
endif()
