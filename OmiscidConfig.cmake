#  =============================================================================
#  Omiscid configuration
#
#  This package will prepare necessary data to work with Omiscid using cmake.
#  it will call configuration for System, Messaging, Com, Service Control.
#
#  Usage from an external project:
#    In your CMakeLists.txt, add these lines:
#
#    set(Omiscid_DIR PATH_TO_THIS_FILE)
#	 find_package( Omiscid REQUIRED )
#
#    PATH_TO_THIS_FILE represents the path to reach this configuration file
#
#    If the module is found then Omiscid_FOUND is set to TRUE.
#
#    This file will define the following variables:
#      - Omiscid_ROOT_PATH    : The Omiscid root folder.
#      - Omiscid_INCLUDE_DIRS : The Omiscid include directories.
#      - Omiscid_LIBS         : The Omiscid mandatory libraries.
#      -          : The Omiscid sources files.
#      - Omiscid_HDRS         : The Omiscid header files.
#
#  =============================================================================

function(CollapseVariable VariablePrefix SubModule VariableSuffix)
	set(VariableName "Temp_${VariablePrefix}${VariableSuffix}")
	set(SubmoduleVariableName "${VariablePrefix}${SubModule}${VariableSuffix}")
	# MESSAGE( STATUS ${VariableName} ", " ${${SubmoduleVariableName}} )
	set(${VariableName} "${${VariableName}};${${SubmoduleVariableName}}" PARENT_SCOPE)
endfunction(CollapseVariable)

set(Omiscid_FOUND TRUE CACHE BOOL "Omiscid found")
get_filename_component(ResPath ${CMAKE_CURRENT_LIST_FILE} DIRECTORY)
set(Omiscid_ROOT_PATH ${ResPath} CACHE INTERNAL "Omiscid root PATH")

if ( NOT Omiscid_FIND_COMPONENTS )
    list(APPEND Omiscid_FIND_COMPONENTS "All")
endif()

string(TOLOWER ${Omiscid_FIND_COMPONENTS} Omiscid_FIND_COMPONENTS)
if ( ${Omiscid_FIND_COMPONENTS} MATCHES "all" )
	MESSAGE(ERROR "*****************************************")
	MESSAGE(ERROR "*")
	MESSAGE(ERROR "* ServiceControl and Com Layer are not")
	MESSAGE(ERROR "* avaiblable for the moment in Omiscid 3.0b.")
	MESSAGE(ERROR "*")
	MESSAGE(FATAL_ERROR "*****************************************")
	list(APPEND ModulesToInclude "ServiceControl" "Com" "Messaging" "System" )
else()
	if ( ${Omiscid_FIND_COMPONENTS} MATCHES "com" )
		MESSAGE(ERROR "*****************************************")
		MESSAGE(ERROR "*")
		MESSAGE(ERROR "* ServiceControl and Com Layers are not")
		MESSAGE(ERROR "* avaiblable for the moment in Omiscid 3.0b.")
		MESSAGE(ERROR "*")
		MESSAGE(FATAL_ERROR "*****************************************")
		list(APPEND ModulesToInclude "Com" "Messaging" "System" )
	else()
		if ( ${Omiscid_FIND_COMPONENTS} MATCHES "messaging" )
			list(APPEND ModulesToInclude "Messaging" "System" )
		else()
			if ( ${Omiscid_FIND_COMPONENTS} MATCHES "system" )
				list(APPEND ModulesToInclude "System" )
			endif()
		endif()
	endif()
endif()

if ( NOT ModulesToInclude )
	MESSAGE( FATAL_ERROR "Wrong omiscid module list " ${Omiscid_FIND_COMPONENTS})
endif()

set(ListSuffixes "_INCLUDE_DIRS;_LIBS;_SRCS;_HDRS")
# Call all cmake configs
foreach(OmiscidSubModule ${ModulesToInclude})
	# create module variable
	set(SubModuleName "Omiscid${OmiscidSubModule}")
	set(SubModuleDir "${SubModuleName}_DIR")
	# MESSAGE( STATUS "${SubModuleVar}" )
	set("${SubModuleDir}" "${Omiscid_ROOT_PATH}/${OmiscidSubModule}")
	# MESSAGE( STATUS ${SubModuleDir} " = " ${${SubModuleDir}})
	find_package( ${SubModuleName} REQUIRED )

	# collapse all submodules variables to Omiscid ones
	foreach(Suffix ${ListSuffixes})
		CollapseVariable("Omiscid" ${OmiscidSubModule} ${Suffix})
	endforeach(Suffix)
endforeach(OmiscidSubModule)

set(Omiscid_INCLUDE_DIRS ${Temp_Omiscid_INCLUDE_DIRS} CACHE INTERNAL "The Omiscid include directories.")
set(Omiscid_LIBS ${Temp_Omiscid_LIBS} CACHE INTERNAL "The Omiscid mandatory libraries.")
set(Omiscid_SRCS ${Temp_Omiscid_SRCS} CACHE INTERNAL "The Omiscid sources files.")
set(Omiscid_HDRS ${Temp_Omiscid_HDRS} CACHE INTERNAL "The Omiscid header files.")
