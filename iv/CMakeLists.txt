#
# Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
# Company: Cesvima, UPM
#
# Notes: Using Builyard https://github.com/Eyescale/Buildyard
#

# PUBLIC HEADERS
#set(IV_PUBLIC_HEADERS )

# SOURCES
set(IV_SOURCES
    dataHandler/fileReader/hdf5FileHandler.cpp
    dataHandler/fileReader/testFileHandler.cpp
    dataHandler/fileReader/factoryFileHandler.cpp
    dataHandler/util/lruLinkedList.cpp
    dataHandler/cache/objectHandler.cpp
    dataHandler/cache/cacheObject.cpp
    dataHandler/cache/controlCache.cpp
    dataHandler/cache/cubeCacheFactory.cpp
    dataHandler/cache/cubeCacheSimple.cpp
    dataHandler/cache/brickCache.cpp
    dataHandler/cache/cache.cpp
    dataHandler/octree/factoryOctreeConstructor.cpp
    dataHandler/octree/octreeConstructorMemory.cpp
    )

# LIBRARIES
set(IV_LIBRARIES
    pthread
    ${HDF5_LIBRARIES}
    ${CUDA_LIBRARIES}
    )

include_directories( ${CUDA_INCLUDE_DIRS} )

#list(SORT IV_PUBLIC_HEADERS)
list(SORT IV_SOURCES)

add_library(IV SHARED
            ${IV_PUBLIC_HEADERS}
            ${IV_SOURCES}
           )
target_link_libraries(IV ${IV_LIBRARIES} pthread)
set_target_properties(IV PROPERTIES VERSION ${VERSION} SOVERSION ${VERSION_ABI})

#install(FILES ${IV_PUBLIC_HEADERS}
#        DESTINATION include/iv/dataHandler COMPONENT dev)

#install(TARGETS IV
#        ARCHIVE DESTINATION lib COMPONENT dev
#        RUNTIME DESTINATION bin COMPONENT lib
#        LIBRARY DESTINATION lib COMPONENT lib)
