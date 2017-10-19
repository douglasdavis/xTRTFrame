# Declare the name of this package:
atlas_subdir(xTRTTruthExample None)

# This package depends on other packages:
atlas_depends_on_subdirs(PUBLIC
  xTRTFrame
  )

# This package uses ROOT:
find_package(ROOT REQUIRED COMPONENTS Core Gpad Tree Hist RIO MathCore Graf)

# Custom definitions needed for this package:
add_definitions(-g -O3 -std=c++14)

# Generate a CINT dictionary source file:
atlas_add_root_dictionary(xTRTTruthExample _cintDictSource
  ROOT_HEADERS Root/LinkDef.h
  EXTERNAL_PACKAGES ROOT
  )

# Build a library that other components can link against:
atlas_add_library(xTRTTruthExample Root/*.cxx xTRTTruthExample/*.h xTRTTruthExample/*.icc xTRTTruthExample/*.hpp ${_cintDictSource}
  PUBLIC_HEADERS xTRTTruthExample
  LINK_LIBRARIES
  xTRTFrame
  ${ROOT_LIBRARIES}
  INCLUDE_DIRS ${ROOT_INCLUDE_DIRS}
  )

atlas_add_executable(runTruthExampleAlg
  util/runTruthExampleAlg.cxx
  LINK_LIBRARIES ${ROOT_LIBRARIES} xTRTTruthExample
  )

atlas_install_scripts(scripts/*.py)

atlas_install_data(data/*)
