cmake_minimum_required(VERSION 3.0)
project(cinder)

set(CMAKE_CXX_STANDARD 14)

get_filename_component( CINDER_PATH "~/cinder/Cinder" ABSOLUTE )

include( "${CINDER_PATH}/proj/cmake/modules/cinderMakeApp.cmake" )

ci_make_app(
        APP_NAME "Cinder"
        CINDER_PATH ${CINDER_PATH}
        SOURCES main.cpp Boid.h Boid.cpp

)
