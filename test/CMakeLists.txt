#
# Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
# Company: Cesvima, UPM
#
# Notes: Using Builyard https://github.com/Eyescale/Buildyard
#

# Test fileHandler
add_executable( testFileHandler fileHandler.cpp )
target_link_libraries( testFileHandler
                                IVdataHandler
                                ${HDF5_HL_LIBRARIES}
                                ${Boost_LIBRARIES}
                                )

# Test Priority LinkedList
add_executable( testLRULinkedList lruLinkedList.cpp )
target_link_libraries( testLRULinkedList
                                IVdataHandler
                                pthread)
