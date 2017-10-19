#include <xTRTUseTNP/TNPAlg.h>
#include <xTRTFrame/Runner.h>

int main(int argc, char **argv) {
  xTRT::TNPAlg *alg = new xTRT::TNPAlg();
  return xTRT::Runner(argc,argv,alg);
}
