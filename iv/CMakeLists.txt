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
    dataHandler/hdf5FileHandler.cpp
    dataHandler/factoryFileHandler.cpp
    dataHandler/lruLinkedList.cpp
    dataHandler/objectHandler.cpp
    dataHandler/cacheObject.cpp
    dataHandler/cubeCache.cpp
    )

# LIBRARIES
set(IV_LIBRARIES
    pthread
    ${HDF5_LIBRARIES}
    )

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
