#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(Hadr01)

#----------------------------------------------------------------------------
# Prepend our own CMake Modules to the search path
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules ${CMAKE_MODULE_PATH})

#Define install directories
if("${CMAKE_INSTALL_PREFIX}" STREQUAL "/usr/local")
    #Overwrite the default
    set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR})
endif()

# Find and configure BOOST
# select boost libraries to be used
set(BOOST_SUFFIX $ENV{BOOSTCOMP}$ENV{BOOSTVER})
message(STATUS "Using BOOST at ${BOOST_SUFFIX}") 
set(BOOST_LIB boost_thread${BOOST_SUFFIX} boost_wave${BOOST_SUFFIX} boost_serialization${BOOST_SUFFIX} boost_filesystem${BOOST_SUFFIX} boost_system${BOOST_SUFFIX} boost_timer${BOOST_SUFFIX} boost_program_options${BOOST_SUFFIX})

include_directories($ENV{BOOST}/include/boost$ENV{BOOSTVER}) #old versions
include_directories($ENV{BOOST}/include)
link_directories($ENV{BOOST}/lib)

if(DEFINED ENV{VECGEOMDIR})
    find_package(VecGeom QUIET REQUIRED)
    if( ${VecGeom_FOUND} )
        include_directories(${VECGEOM_INCLUDE_DIRS})
        if( ${VecCore_FOUND} )
            add_definitions(-DVECCORE_ENABLE_VC)
            include_directories(${VecCore_INCLUDE_DIRS})
        endif()
        message (STATUS "Found VecGeom include dirs ${VECGEOM_INCLUDE_DIRS}")
    else()
        message (FATAL_ERROR "VecGeom not found")
    endif()
endif()

if(DEFINED ENV{XERCESCDIR})
    find_package(XercesC REQUIRED)
    if( ${XercesC_FOUND} )
        include_directories(${XercesC_INCLUDE_DIRS})
        message (STATUS "Found XercesC include dirs ${XercesC_INCLUDE_DIRS}")
    else()
        message (FATAL_ERROR "XercesC not found")
    endif()
endif()

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
#
include(${Geant4_USE_FILE})


# root libraries
find_package(ROOT 6.16 CONFIG REQUIRED)
#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})

# compilation flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g -Wno-shadow=local -ggdb ${ROOT_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O2 -pipe ${ROOT_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS ${ROOT_LD_FLAGS})
#----------------------------------------------------------------------------
# Locate sources and headers for this project
#
include_directories(${CRMC_INCLUDE_DIR} 
		    ${PROJECT_SOURCE_DIR}/include 
        ${Geant4_INCLUDE_DIR}
		    ${ROOT_INCLUDE_DIRS} 
)
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${CRMC_INCLUDE_DIR}/*.h ${PROJECT_SOURCE_DIR}/include/*.hh)
#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#

ROOT_GENERATE_DICTIONARY(G__Particle ${PROJECT_SOURCE_DIR}/include/ScoredParticle.hh LINKDEF ScoringParticle_LinkDef.h)

add_library(MainObjLib OBJECT ${sources})
set_property(TARGET ${MainObjLib} PROPERTY POSITION_INDEPENDENT_CODE 1)

add_library(scorePartLib SHARED ${PROJECT_SOURCE_DIR}/src/ScoredParticle.cc G__Particle.cxx)
set_property(TARGET ${scorePartLib} PROPERTY POSITION_INDEPENDENT_CODE 1)
target_link_libraries(scorePartLib ${ROOT_LIBRARIES})

link_directories(${CRMC_LIBRARY_DIR})

add_executable(Hadr01 Hadr01.cc $<TARGET_OBJECTS:MainObjLib>)
target_link_libraries(Hadr01 ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} scorePartLib)

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build Hadr01. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#

# set(Hadr01_SCRIPTS
#     hadr01.in vis.mac muons.in
#   )

# foreach(_script ${Hadr01_SCRIPTS})
#   configure_file(
#     ${PROJECT_SOURCE_DIR}/${_script}
#     ${PROJECT_BINARY_DIR}/${_script}
#     COPYONLY
#     )
# endforeach()

# #----------------------------------------------------------------------------
# # Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
# #
install(TARGETS scorePartLib DESTINATION lib-$ENV{SYSTEMINSTALL})
install(FILES build-$ENV{SYSTEMINSTALL}/libParticle_rdict.pcm DESTINATION lib-$ENV{SYSTEMINSTALL})
install(TARGETS Hadr01 DESTINATION bin-$ENV{SYSTEMINSTALL})
