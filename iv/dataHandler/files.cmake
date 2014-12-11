set(IV_DATA_HANDLER_PUBLIC_HEADERS
  )

set(IV_DATA_HANDLER_HEADERS
    fileHandler.h
    factoryFileHandler.h
    hdf5FileHandler.h
  )

set(IV_DATA_HANDLER_SOURCES
    hdf5FileHandler.cpp
    factoryFileHandler.cpp
  )

#list(SORT IV_DATA_HANDLER_PUBLIC_HEADERS)
list(SORT IV_DATA_HANDLER_HEADERS)
list(SORT IV_DATA_HANDLER_SOURCES)
