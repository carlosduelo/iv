#
# Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
# Company: Cesvima, UPM
#
# Notes: Using Builyard https://github.com/Eyescale/Buildyard
#

# Test fileHandler
add_executable( testFileHandler fileHandler.cpp )
target_link_libraries( testFileHandler
                                IV
                                ${HDF5_HL_LIBRARIES}
                                ${Boost_LIBRARIES}
                                )

# Test fileHandler
add_executable( testIVDFileHandler ivdFileHandler.cpp )
target_link_libraries( testIVDFileHandler
                                IV
                                )

# Test Priority LinkedList
add_executable( testLRULinkedList lruLinkedList.cpp )
target_link_libraries( testLRULinkedList
                                IV
                                )

# Test Cube Cache
add_executable( testCubeCache cubeCache.cpp )
target_link_libraries( testCubeCache
                                IV
                                )
# Test Brick Cache
add_executable( testBrickCache brickCache.cpp )
target_link_libraries( testBrickCache
                                IV
                                )
