# Declare the name of this package:
atlas_subdir(xTRTFrame None)

# This package depends on other packages:
atlas_depends_on_subdirs(PUBLIC
  Control/AthToolSupport/AsgTools
  PhysicsAnalysis/D3PDTools/EventLoop
  PhysicsAnalysis/D3PDTools/EventLoopAlgs
  PhysicsAnalysis/D3PDTools/EventLoopGrid
  PhysicsAnalysis/D3P3Tools/SampleHandler
  Event/xAOD/xAODRootAccess
  Event/xAOD/xAODEventInfo
  Event/xAOD/xAODTracking
  Event/xAOD/xAODEgamma
  Event/xAOD/xAODMuon
  InnerDetector/InDetRecTools/InDetTrackSelectionTool
  PhysicsAnalysis/AnalysisCommon/PileupReweighting
  PhysicsAnalysis/MCTruthClassifier
  DataQuality/GoodRunsLists
  Trigger/TrigAnalysis/TrigDecisionTool
  Trigger/TrigAnalysis/TriggerMatchingTool
  Trigger/TrigConfiguration/TrigConfxAOD
  Tools/PathResolver
  )

# This package uses ROOT:
find_package(ROOT REQUIRED COMPONENTS Core Gpad Tree Hist RIO MathCore Graf)

# Custom definitions needed for this package:
add_definitions(-g -O3 -std=c++14)

# Generate a CINT dictionary source file:
atlas_add_root_dictionary(xTRTFrame _cintDictSource
  ROOT_HEADERS xTRTFrame/Config.h xTRTFrame/TNPAlgorithm.h xTRTFrame/Algorithm.h xTRTFrame/xTRTFrameDict.h
  EXTERNAL_PACKAGES ROOT
  )

# Build a library that other components can link against:
atlas_add_library(xTRTFrame Root/*.cxx xTRTFrame/*.h xTRTFrame/*.icc xTRTFrame/*.hpp ${_cintDictSource}
  PUBLIC_HEADERS xTRTFrame
  LINK_LIBRARIES
  AsgTools
  GoodRunsListsLib
  PileupReweightingLib
  MCTruthClassifierLib
  EventLoop
  EventLoopAlgs
  EventLoopGrid
  SampleHandler
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
  INCLUDE_DIRS ${ROOT_INCLUDE_DIRS}
  )

atlas_install_data(data/*)
