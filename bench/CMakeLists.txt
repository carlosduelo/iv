#
# Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
# Company: Cesvima, UPM
#
# Notes: Using Builyard https://github.com/Eyescale/Buildyard
#

# Cut hdf5
add_executable( benchFile file.cpp )
target_link_libraries( benchFile
                                IV
                                )

