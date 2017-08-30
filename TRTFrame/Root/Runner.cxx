#include <TRTFrame/Runner.h>
#include <TRTFrame/Algo.h>
#include <TRTFrame/Externals/CLI11.hpp>

#include <EventLoopGrid/PrunDriver.h>
#include <EventLoopGrid/GridDriver.h>
#include <EventLoop/Job.h>
#include <EventLoop/DirectDriver.h>
#include <EventLoopAlgs/NTupleSvc.h>
#include <EventLoop/OutputStream.h>
#include <SampleHandler/ScanDir.h>
#include <SampleHandler/SampleHandler.h>
#include <SampleHandler/ToolsDiscovery.h>

#include "xAODRootAccess/Init.h"

namespace xTRT {
  int Runner(int argc, char **argv, xTRT::Algo* alg) {
    CLI::App app("TRTFrame Job");

    std::string configFile;
    std::string inputTextFile;
    std::string outputDir;
    std::string gridDS;
    std::string outDS;
    bool        printConf;
    app.add_option("-c,--config",configFile,"Config file name")->required();
    app.add_option("-o,--out-dir",outputDir,"Name for output directory")->required();
    app.add_flag("--print-config",printConf,"Print YAML config");
    auto o_infile = app.add_option("-i,--in-file",inputTextFile,"List of samples in a txt file");
    auto o_gridds = app.add_option("--gridDS",gridDS,"Grid sample name to process");
    auto o_outds  = app.add_option("--outDS", outDS, "Output sample name to for grid job");

    try {
      app.parse(argc,argv);
    } catch(const CLI::ParseError& err) {
      return app.exit(err);
    }

    xAOD::Init().ignore();

    EL::Job job;
    EL::OutputStream output("treeOutput");
    job.outputAdd(output);
    EL::NTupleSvc *ntuple = new EL::NTupleSvc("treeOutput");
    job.algsAdd(ntuple);

    alg->feedConfig(configFile.c_str(),printConf);
    job.algsAdd(alg);
    alg->setTreeOutputName("treeOutput");

    SH::SampleHandler sh;
    sh.setMetaString("nc_tree","CollectionTree");

    if ( o_infile->count() ) {
      SH::readFileList(sh,"sample",inputTextFile);
      sh.print();
      job.sampleHandler(sh);
      EL::DirectDriver driver;
      driver.submit(job,outputDir);
      return 0;
    }
    else if ( o_gridds->count() && o_outds->count() ) {
      SH::scanRucio(sh,gridDS);
      sh.print();
      job.sampleHandler(sh);
      EL::PrunDriver driver;
      driver.options()->setString("nc_outputSampleName",outDS);
      driver.submitOnly(job,"TRTFrameGridJob_"+outputDir);
    }
    else {
      std::cout << "Require -i and -o OR --gridDS and --outDS and -o!" << std::endl;
      std::cout << app.help() << std::endl;
    }

    return 0;
  }
}
