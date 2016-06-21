#  =============================================================================
#  Omiscid Messaging configuration
#
#  This package will prepare necessary data to work with Omiscid Messaging layer
#  using cmake.
#
#  Usage from an external project:
#    In your CMakeLists.txt, add these lines:
#
#    set(OmiscidMessaging_DIR PATH_TO_THIS_FILE)
#	 find_package( Omiscid REQUIRED )
#
#    PATH_TO_THIS_FILE represents the path to reach this configuration file
#
#    If the module is found then OmiscidMessaging_FOUND is set to TRUE.
#
#    This file will define the following variables:
#      - OmiscidMessaging_ROOT_PATH    : The Omiscid Messaging root folder.
#      - OmiscidMessaging_INCLUDE_DIRS : The Omiscid Messaging include directories.
#      - OmiscidMessaging_SRCS         : The Omiscid Messaging source files.
#      - OmiscidMessaging_HDRS         : The Omiscid Messaging header files.
#      - OmiscidMessaging_LIBS         : The Omiscid Messaging mandatory libs.
#
#  =============================================================================

set(OmiscidMessaging_FOUND TRUE CACHE BOOL "Omiscid found")
get_filename_component(ResPath ${CMAKE_CURRENT_LIST_FILE} DIRECTORY)
get_filename_component(ResPath ${ResPath} REALPATH)
set(OmiscidMessaging_ROOT_PATH ${ResPath} CACHE INTERNAL "Omiscid Messaging root folder PATH")

# manage include directory
get_filename_component(JsonFolder "${OmiscidMessaging_ROOT_PATH}/../Json/" REALPATH)
set(IncludeDirList "${OmiscidMessaging_ROOT_PATH}/;${JsonFolder}")
foreach(IncludeDir ${IncludeDirList})
	# MESSAGE( STATUS ${IncludeDir}  )
	get_filename_component(CurDir ${IncludeDir} REALPATH)
	list(APPEND ListDirs ${CurDir})
	include_directories(${CurDir})
endforeach(IncludeDir)
# MESSAGE(STATUS ${ListDirs})
set(OmiscidMessaging_INCLUDE_DIRS ${ListDirs} CACHE INTERNAL "Omiscid Messaging include PATH")

# manage source files

file(GLOB GlobRes 
	"${OmiscidMessaging_ROOT_PATH}/*.cpp"
	"${JsonFolder}/*.cpp"
)
set(OmiscidMessaging_SRCS ${GlobRes} CACHE INTERNAL "Omiscid Messaging source files.")
# MESSAGE(FATA_ERROR OmiscidMessaging_SRCS)

# manage header files
file(GLOB GlobRes  
	"${OmiscidMessaging_ROOT_PATH}/Messaging/*.h"
	"${JsonFolder}/Json/*.h"
)
set(OmiscidMessaging_HDRS ${GlobRes} CACHE INTERNAL "Omiscid Messaging header files.")

set(OmiscidMessaging_LIBS "" CACHE INTERNAL "Omsicid Messaging libraries to link with.")