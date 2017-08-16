# Declare the name of this package:
atlas_subdir(TRTFrame None)

# This package depends on other packages:
atlas_depends_on_subdirs(PUBLIC
  Control/AthToolSupport/AsgTools
  PhysicsAnalysis/D3PDTools/EventLoop
  PhysicsAnalysis/D3PDTools/EventLoopAlgs
  PhysicsAnalysis/D3PDTools/EventLoopGrid
  Event/xAOD/xAODRootAccess
  Event/xAOD/xAODEventInfo
  Event/xAOD/xAODTracking
  Event/xAOD/xAODEgamma
  Event/xAOD/xAODMuon
  InnerDetector/InDetRecTools/InDetTrackSelectionTool
  PhysicsAnalysis/AnalysisCommon/PileupReweighting
  DataQuality/GoodRunsLists
  Trigger/TrigAnalysis/TrigDecisionTool
  Trigger/TrigAnalysis/TriggerMatchingTool
  Trigger/TrigConfiguration/TrigConfxAOD
  Tools/PathResolver
  )

# This package uses ROOT:
find_package(ROOT REQUIRED COMPONENTS Core Gpad Tree Hist RIO MathCore Graf)

# This packages uses yaml-cpp
find_package(yamlcpp REQUIRED)

# Custom definitions needed for this package:
add_definitions(-g -O3 -std=c++14)

# Generate a CINT dictionary source file:
atlas_add_root_dictionary(TRTFrame _cintDictSource
  ROOT_HEADERS Root/LinkDef.h
  EXTERNAL_PACKAGES ROOT
  )

# Build a library that other components can link against:
atlas_add_library(TRTFrame Root/*.cxx TRTFrame/*.h TRTFrame/*.hpp ${_cintDictSource}
  PUBLIC_HEADERS TRTFrame
  LINK_LIBRARIES
  AsgTools
  GoodRunsListsLib
  PileupReweightingLib
  EventLoop
  EventLoopAlgs
  EventLoopGrid
  xAODRootAccess
  xAODEventInfo
  xAODTracking
  xAODEgamma
  xAODMuon
  InDetTrackSelectionToolLib
  TrigConfxAODLib
  TrigDecisionToolLib
  TriggerMatchingToolLib
  PathResolver
  ${ROOT_LIBRARIES}
  ${YAMLCPP_LIBRARIES}
  INCLUDE_DIRS ${ROOT_INCLUDE_DIRS} ${YAMLCPP_INCLUDE_DIR}
  )

#atlas_add_executable(runLoopAlg
#  util/runLoopAlg.cxx
#  LINK_LIBRARIES ${ROOT_LIBRARIES} TRTFrame
#  )

atlas_install_scripts(scripts/*.py)
atlas_install_data(data/*)
