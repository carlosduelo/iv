#
# Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
# Company: Cesvima, UPM
#
# Notes: Using Builyard https://github.com/Eyescale/Buildyard
#

# Octree Constructor 
add_executable( ivOC octreeConstructor.cpp )
target_link_libraries( ivOC
                                IV
                                ${Boost_LIBRARIES}
                                )

