# CMake module to search for the yaml-cpp library
# If it's found it sets YamlCpp_FOUND to TRUE
# and following variables are set:
#    YamlCpp_INCLUDE_DIRS
#    YamlCpp_LIBRARIES

find_path(YamlCpp_INCLUDE_DIRS NAMES yaml-cpp/yaml.h PATHS $ENV{YamlCpp_ROOT}/include)
find_library(YamlCpp_LIBRARIES NAMES yaml-cpp libyaml-cpp libyaml-cppmdd PATHS $ENV{YamlCpp_ROOT}/lib $ENV{YamlCpp_ROOT}/build/Debug)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(YamlCpp DEFAULT_MSG YamlCpp_LIBRARIES YamlCpp_INCLUDE_DIRS)
