

set(IV_PACKAGE_VERSION 0.1)
set(IV_DEPENDS  REQUIRED vmmlib HDF5)

set(IV_ROOT_VAR IV_ROOT)
set(IV_REPO_URL https://github.com/carlosduelo/iv.git)
set(IV_REPO_TAG master)
set(IV_FORCE_BUILD ${CI_BUILD})

if(CI_BUILD_COMMIT)
  set(IV_REPO_TAG ${CI_BUILD_COMMIT})
else()
  set(IV_REPO_TAG master)
endif()
set(IV_FORCE_BUILD ON)
set(IV_SOURCE ${CMAKE_SOURCE_DIR})