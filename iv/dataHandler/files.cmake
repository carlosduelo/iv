set(IV_DATA_HANDLER_PUBLIC_HEADERS
  )

set(IV_DATA_HANDLER_HEADERS
    fileHandler.h
    factoryFileHandler.h
    hdf5FileHandler.h
    lruLinkedList.h
    cubeCache.h
  )

set(IV_DATA_HANDLER_SOURCES
    hdf5FileHandler.cpp
    factoryFileHandler.cpp
    lruLinkedList.cpp
    cubeCache.cpp
  )

#list(SORT IV_DATA_HANDLER_PUBLIC_HEADERS)
list(SORT IV_DATA_HANDLER_HEADERS)
list(SORT IV_DATA_HANDLER_SOURCES)
