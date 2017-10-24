cmake_minimum_required( VERSION 3.4 FATAL_ERROR )

# Find the AnalysisBase project. This is what, amongst other things, pulls
# in the definition of all of the "atlas_" prefixed functions/macros.
find_package( AnalysisBase 21.2 REQUIRED )

# Set up CTest. This makes sure that per-package build log files can be
# created if the user so chooses.
atlas_ctest_setup()

atlas_project( WorkDir 1.0.0
   USE AnalysisBase ${AnalysisBase_VERSION} )

# Set up the runtime environment setup script. This makes sure that the
# project's "setup.sh" script can set up a fully functional runtime environment,
# including all the externals that the project uses.
lcg_generate_env( SH_FILE ${CMAKE_BINARY_DIR}/${ATLAS_PLATFORM}/env_setup.sh )
install( FILES ${CMAKE_BINARY_DIR}/${ATLAS_PLATFORM}/env_setup.sh
   DESTINATION . )

# Set up CPack. This call makes sure that an RPM or TGZ file can be created
# from the built project. Used by Panda to send the project to the grid worker
# nodes.
atlas_cpack_setup()
