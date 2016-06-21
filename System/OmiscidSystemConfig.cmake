#  =============================================================================
#  Omiscid System configuration
#
#  This package will prepare necessary data to work with Omiscid System layer
#  using cmake.
#
#  Usage from an external project:
#    In your CMakeLists.txt, add these lines:
#
#    set(OmiscidSystem_DIR PATH_TO_THIS_FILE)
#	 find_package( Omiscid REQUIRED )
#
#    PATH_TO_THIS_FILE represents the path to reach this configuration file
#
#    If the module is found then OmiscidSystem_FOUND is set to TRUE.
#
#    This file will define the following variables:
#      - OmiscidSystem_ROOT_PATH    : The Omiscid System root folder.
#      - OmiscidSystem_INCLUDE_DIRS : The Omiscid System include directories.
#      - OmiscidSystem_SRCS         : The Omiscid source files.
#      - OmiscidSystem_HDRS         : The Omiscid header files.
#      - OmiscidSystem_LIBS         : The Omiscid mandatory libs.
#
#  =============================================================================

set(OmiscidSystem_FOUND TRUE CACHE BOOL "Omiscid found")
get_filename_component(ResPath ${CMAKE_CURRENT_LIST_FILE} DIRECTORY)
get_filename_component(ResPath ${ResPath} REALPATH)
set(OmiscidSystem_ROOT_PATH ${ResPath} CACHE INTERNAL "Omiscid System root folder PATH")

# manage include directory
set(IncludeDirList "${OmiscidSystem_ROOT_PATH}")
foreach(IncludeDir ${IncludeDirList})
	# MESSAGE( STATUS ${IncludeDir}  )
	# get_directory_property(CurDir DIRECTORY ${IncludeDir} DIRECTORY)
	get_filename_component(CurDir ${IncludeDir} REALPATH)
	list(APPEND ListDirs ${CurDir})
	include_directories(${CurDir})
endforeach(IncludeDir)

set(OmiscidSystem_INCLUDE_DIRS ${ListDirs} CACHE INTERNAL "Omiscid System include PATH")

# manage source files
file(GLOB GlobRes 
	"${OmiscidSystem_ROOT_PATH}/*.cpp"
)
set(OmiscidSystem_SRCS ${GlobRes} CACHE INTERNAL "Omiscid System source files.")

# manage header files
file(GLOB GlobRes  
	"${OmiscidSystem_ROOT_PATH}/System/*.h"
)
set(OmiscidSystem_HDRS ${GlobRes} CACHE INTERNAL "Omiscid System header files.")

if ( MSVC )
	# Add socket lirbrary
	set(OmiscidSystem_LIBS "Ws2_32.lib" CACHE INTERNAL "Omsicid System libraries to link with.")
else()
	set(OmiscidSystem_LIBS "" CACHE INTERNAL "Omsicid System libraries to link with.")
endif()