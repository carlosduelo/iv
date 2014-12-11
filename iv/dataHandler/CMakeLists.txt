#
# Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
# Company: Cesvima, UPM
#
# Notes: Using Builyard https://github.com/Eyescale/Buildyard
#

include(files.cmake)

# IV DATA HANDLER
add_library(IVdataHandler SHARED
            ${IV_COMMON_HEADERS}
            ${IV_DATA_HANDLER_PUBLIC_HEADERS}
            ${IV_DATA_HANDLER_SOURCES}
            ${IV_DATA_HANDLER_HEADERS})
target_link_libraries(IVdataHandler ${HDF5_LIBRARIES} pthread)
set_target_properties(IVdataHandler
        PROPERTIES VERSION ${VERSION} SOVERSION ${VERSION_ABI})

install(FILES ${IV_DATA_HANDLER_PUBLIC_HEADERS}
        DESTINATION include/iv/dataHandler COMPONENT dev)

#install(TARGETS IVdataHandler
#        ARCHIVE DESTINATION lib COMPONENT dev
#        RUNTIME DESTINATION bin COMPONENT lib
#        LIBRARY DESTINATION lib COMPONENT lib)
