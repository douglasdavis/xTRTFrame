#include <TRTFrame/TruthLoop.h>
#include <TRTFrame/Runner.h>

int main(int argc, char **argv) {
  xTRT::TruthLoop *alg = new xTRT::TruthLoop();
  return xTRT::Runner(argc,argv,alg);
}