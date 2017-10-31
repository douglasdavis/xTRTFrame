#include <xTRTFrame/Runner.h>
#include <xTRTFrame/Algorithm.h>
#include <xTRTFrame/Externals/CLI11.hpp>

#include <AsgTools/MsgLevel.h>
#include <EventLoopGrid/PrunDriver.h>
#include <EventLoopGrid/GridDriver.h>
#include <EventLoop/Job.h>
#include <EventLoop/DirectDriver.h>
#include <EventLoopAlgs/NTupleSvc.h>
#include <EventLoop/OutputStream.h>
#include <SampleHandler/ScanDir.h>
#include <SampleHandler/SampleHandler.h>
#include <SampleHandler/ToolsDiscovery.h>

#include <xAODRootAccess/Init.h>

namespace xTRT {
  int Runner(int argc, char **argv, xTRT::Algorithm* alg) {
    CLI::App app("xTRTFrame Job");

    std::string configFile;
    app.add_option("-c,--config",configFile,"Config file name")->required();
    std::string outputDir;
    app.add_option("-o,--out-dir",outputDir,"Name for output directory")->required();
    std::string inputTextFile;
    auto o_infile = app.add_option("-i,--in-file",inputTextFile,"List of samples in a txt file");
    std::string gridDS;
    auto o_gridds = app.add_option("--gridDS",gridDS,"Grid sample name to process");
    std::string outDS;
    auto o_outds  = app.add_option("--outDS", outDS, "Output sample name to for grid job");
    bool printConf;
    app.add_flag("--print-config",printConf,"Print configuration options");
    bool debugMode;
    app.add_flag("--debug",debugMode,"Flag to run in debug mode");

    CLI11_PARSE(app, argc, argv);

    xAOD::Init().ignore();

    EL::Job job;
    job.options()->setDouble(EL::Job::optCacheSize, 10*1024*1024);

    EL::OutputStream output("xTRTFrameTreeOutput");
    job.outputAdd(output);
    EL::NTupleSvc *ntuple = new EL::NTupleSvc("xTRTFrameTreeOutput");
    job.algsAdd(ntuple);

    alg->feedConfig(configFile.c_str(),printConf);
    alg->setTreeOutputName("xTRTFrameTreeOutput");
    if ( debugMode ) {
      alg->setMsgLevel(MSG::DEBUG);
    }
    job.algsAdd(alg);

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
      driver.submitOnly(job,"xTRTFrameGridJob_"+outputDir);
    }
    else {
      std::cout << "Require -i and -o OR --gridDS and --outDS and -o!" << std::endl;
      std::cout << app.help() << std::endl;
    }

    return 0;
  }
}
