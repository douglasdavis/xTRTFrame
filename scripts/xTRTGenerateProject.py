#!/usr/bin/env python

from __future__ import print_function
import sys
import os
import subprocess

if len(sys.argv) < 2:
    print("Give a package name!")
    exit(0)

package_name   = sys.argv[1]
package_looper = package_name+'Looper'

subprocess.call('mkdir -p '+package_name+'/'+package_looper+'/'+package_looper,shell=True)
subprocess.call('mkdir -p '+package_name+'/'+package_looper+'/Root',shell=True)
subprocess.call('mkdir -p '+package_name+'/'+package_looper+'/data',shell=True)
subprocess.call('mkdir -p '+package_name+'/'+package_looper+'/util',shell=True)
subprocess.call('git init '+package_name,shell=True)
subprocess.call('git clone https://gitlab.cern.ch/atlas-trt-software/xTRTFrame.git '+package_name+'/xTRTFrame',shell=True)

submodules_file = package_name+'/.gitmodules'
submodules_content = """[submodule "xTRTFrame"]
    path = xTRTFrame
    url = https://gitlab.cern.ch/atlas-trt-software/xTRTFrame.git
"""
with open(submodules_file,'w') as subout:
    subout.write(submodules_content)

cmake_file = """cmake_minimum_required( VERSION 3.4 FATAL_ERROR )

# Find the AnalysisBase project. This is what, amongst other things, pulls
# in the definition of all of the "atlas_" prefixed functions/macros.
find_package( AnalysisBase 21.2 REQUIRED )

# Set up CTest. This makes sure that per-package build log files can be
# created if the user so chooses.
atlas_ctest_setup()

atlas_project( {0} 0.1
   USE AnalysisBase ${{AnalysisBase_VERSION}} )

# Set up the runtime environment setup script. This makes sure that the
# project's "setup.sh" script can set up a fully functional runtime environment,
# including all the externals that the project uses.
lcg_generate_env( SH_FILE ${{CMAKE_BINARY_DIR}}/${{ATLAS_PLATFORM}}/env_setup.sh )
install( FILES ${{CMAKE_BINARY_DIR}}/${{ATLAS_PLATFORM}}/env_setup.sh
   DESTINATION . )

# Set up CPack. This call makes sure that an RPM or TGZ file can be created
# from the built project. Used by Panda to send the project to the grid worker
# nodes.
atlas_cpack_setup()
""".format(package_name)

looper_cmake = """# Declare the name of this package:
atlas_subdir({0} None)

# This package depends on other packages:
atlas_depends_on_subdirs(PUBLIC
  xTRTFrame
  )

# This package uses ROOT:
find_package(ROOT REQUIRED COMPONENTS Core Gpad Tree Hist RIO MathCore Graf)

# Custom definitions needed for this package:
add_definitions(-g -O3 -std=c++14)

# Generate a CINT dictionary source file:
atlas_add_root_dictionary({0} _cintDictSource
  ROOT_HEADERS {0}/{0}Alg.h {0}/{0}Dict.h
  EXTERNAL_PACKAGES ROOT
  )

# Build a library that other components can link against:
atlas_add_library({0} Root/*.cxx {0}/*.h {0}/*.icc {0}/*.hpp ${{_cintDictSource}}
  PUBLIC_HEADERS {0}
  LINK_LIBRARIES
  xTRTFrame
  ${{ROOT_LIBRARIES}}
  INCLUDE_DIRS ${{ROOT_INCLUDE_DIRS}}
  )

atlas_add_executable(run{0}Alg
  util/run{0}Alg.cxx
  LINK_LIBRARIES ${{ROOT_LIBRARIES}} {0}
  )
""".format(package_looper)

with open(package_name+'/CMakeLists.txt','w') as cmake_out:
    cmake_out.write(cmake_file)

with open(package_name+'/'+package_looper+'/CMakeLists.txt','w') as cmake_looper_out:
    cmake_looper_out.write(looper_cmake)

dictionary_file = package_name+'/'+package_looper+'/'+package_looper+'/'+package_looper+'Dict.h'
dictionary_file_contents = """// -*- c++ -*-

#include <{0}/{0}Alg.h>

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclass;

#endif

#ifdef __CINT__
#pragma link C++ class {0}Alg+;
#endif
""".format(package_looper)

with open(dictionary_file,'w') as dict_out:
    dict_out.write(dictionary_file_contents)


header_file = package_name+'/'+package_looper+'/'+package_looper+'/'+package_looper+'Alg.h'
header_contents = """#ifndef {0}_{0}Alg_h
#define {0}_{0}Alg_h

// xTRTFrame
#include <xTRTFrame/Algorithm.h>

class {0}Alg : public xTRT::Algorithm {{
private:

public:
  {0}Alg();
  virtual ~{0}Alg();

  /// EventLoop API function
  virtual EL::StatusCode histInitialize() override;
  /// EventLoop API function
  virtual EL::StatusCode execute() override;
  /// EventLoop API function
  virtual EL::StatusCode finalize() override;

  //// EventLoop API function
  // virtual EL::StatusCode initialize() override;
  //// EventLoop API function for advanced users
  // virtual EL::StatusCode setupJob(EL::Job& job) override;
  //// EventLoop API function for advanced users
  // virtual EL::StatusCode fileExecute() override;
  //// EventLoop API function for advanced users
  // virtual EL::StatusCode changeInput(bool firstFile) override;
  //// EventLoop API function for advanced users
  // virtual EL::StatusCode postExecute() override;
  //// EventLoop API function for advanced users
  // virtual EL::StatusCode histFinalize() override;

  ClassDefOverride({0}Alg, 1);

}};

#endif
""".format(package_looper)

with open(header_file,'w') as hf:
    hf.write(header_contents)

source_file = package_name+'/'+package_looper+'/Root/'+package_looper+'Alg.cxx'
source_contents="""#include <{0}/{0}Alg.h>

{0}Alg::{0}Alg() : xTRT::Algorithm() {{}}

{0}Alg::~{0}Alg() {{}}

EL::StatusCode {0}Alg::histInitialize() {{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.
  ANA_CHECK_SET_TYPE(EL::StatusCode);
  ANA_CHECK(xTRT::Algorithm::histInitialize());

  return EL::StatusCode::SUCCESS;
}}

EL::StatusCode {0}Alg::execute() {{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.
  ANA_CHECK_SET_TYPE(EL::StatusCode);
  ANA_CHECK(xTRT::Algorithm::execute());

  return EL::StatusCode::SUCCESS;
}}

EL::StatusCode {0}Alg::finalize() {{
  // This method is the mirror image of initialize(), meaning it gets
  // called after the last event has been processed on the worker node
  // and allows you to finish up any objects you created in
  // initialize() before they are written to disk.  This is actually
  // fairly rare, since this happens separately for each worker node.
  // Most of the time you want to do your post-processing on the
  // submission node after all your histogram outputs have been
  // merged.  This is different from histFinalize() in that it only
  // gets called on worker nodes that processed input events.
  ANA_CHECK_SET_TYPE(EL::StatusCode);
  ANA_CHECK(xTRT::Algorithm::finalize());

  return EL::StatusCode::SUCCESS;
}}
""".format(package_looper)

with open(source_file,'w') as sf:
    sf.write(source_contents)


util_file = """#include <{0}/{0}Alg.h>
#include <xTRTFrame/Runner.h>

int main(int argc, char **argv) {{
  {0}Alg *alg = new {0}Alg();
  return xTRT::Runner(argc,argv,alg);
}}
""".format(package_looper)

util_file_path = package_name+'/'+package_looper+'/util/run{0}Alg.cxx'.format(package_looper)
with open(util_file_path,'w') as uf:
    uf.write(util_file)

ci_file = """image: atlas/analysisbase:latest

variables:
  GIT_SUBMODULE_STRATEGY: recursive
  GIT_SSL_NO_VERIFY: "true"

build:
  script:
    - source /home/atlas/release_setup.sh
    - mkdir -p build
    - cd build
    - cmake ../
    - make
    - cd ../
    - source build/*/setup.sh
"""
ci_file_path = package_name+'/.gitlab-ci.yml'
with open(ci_file_path,'w') as cif:
    cif.write(ci_file)

readme_file_path = package_name+'/README.md'
readme_text = """# {0}

Put information about your package here!
""".format(package_name)

with open(readme_file_path,'w') as rmf:
    rmf.write(readme_text)
