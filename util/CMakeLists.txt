#
# Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
# Company: Cesvima, UPM
#
# Notes: Using Builyard https://github.com/Eyescale/Buildyard
#

# Cut hdf5
add_executable( cuthdf5 cutHDF5.cpp )
target_link_libraries( cuthdf5
                                IV
                                )

# Octree Constructor
add_executable( ivOC octreeConstructor.cpp )
target_link_libraries( ivOC
                                IV
                                )

# hdf5 to ivD
add_executable( ivh5ToivD hdf5ToivD.cpp )
target_link_libraries( ivh5ToivD
                                IV
                                )
