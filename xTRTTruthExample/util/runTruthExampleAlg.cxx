#include <xTRTTruthExample/TruthAlg.h>
#include <xTRTFrame/Runner.h>

int main(int argc, char **argv) {
  xTRT::TruthAlg *alg = new xTRT::TruthAlg();
  return xTRT::Runner(argc,argv,alg);
}
