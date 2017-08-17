#!/usr/bin/env python

from __future__ import print_function

import os
import argparse
import sys
import time
import datetime

parser = argparse.ArgumentParser(
    description='TRTFrame python steering script'
)

## i/o arguments
parser.add_argument('-c','--config',     dest='config',    type=str, required=True)
parser.add_argument('-i','--infile-list',dest='infilelist',type=str)
parser.add_argument('-o','--out-dir',    dest='outdir',    type=str)
parser.add_argument('--gridDS',          dest='gridDS',    type=str)
parser.add_argument('--outDS',           dest='outDS',     type=str)

args = parser.parse_args()
if len(sys.argv) < 5:
    parser.print_help()
    exit(0)

def main():
    ts = datetime.datetime.fromtimestamp(time.time()).strftime('%Y%m%d_%H.%M.%S')

    import ROOT
    ROOT.xAOD.Init().ignore()

    ROOT.EL.Algorithm()
    ROOT.asg.ToolStore()

    job = ROOT.EL.Job()
    output = ROOT.EL.OutputStream("treeOutput")
    job.outputAdd(output)
    ntuple = ROOT.EL.NTupleSvc("treeOutput")
    job.algsAdd(ntuple)

    alg = ROOT.xTRT.TruthLoop()
    alg.feedConfig(args.config)

    job.algsAdd(alg)

    alg.setTreeOutputName("treeOutput")

    sh = ROOT.SH.SampleHandler()
    if args.infilelist and args.outdir:
        ROOT.SH.readFileList(sh, "sample", args.infilelist)
        sh.setMetaString("nc_tree","CollectionTree")
        sh.print()
        job.sampleHandler(sh)

        driver = ROOT.EL.DirectDriver()
        outDir = args.outdir
        if outDir == 'ts':
            outDir = 'TRTFrame_TruthLoop_'+ts
        driver.submit(job, outDir)
        exit(0)

    elif args.gridDS and args.outDS:
        ROOT.SH.scanRucio(sh, args.gridDS)
        sh.setMetaString("nc_tree", "CollectionTree")
        sh.setMetaString("nc_grid_filter","*.root*")

        sh.print()
        job.sampleHandler(sh)

        driver = ROOT.EL.PrunDriver()
        driver.options().setString("nc_outputSampleName", args.outDS)
        driver.submitOnly(job, "runTRTFrameOnGrid_"+ts)
        exit(0)

    else:
        exit('TBD')

if __name__ == '__main__':
    main()
