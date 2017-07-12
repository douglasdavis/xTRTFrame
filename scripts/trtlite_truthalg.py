#!/usr/bin/env python

from __future__ import print_function

import os
import argparse
import sys
import time
import datetime

parser = argparse.ArgumentParser(
    description='TRTLite python steering script'
)

## i/o arguments
parser.add_argument('-i','--infile-list',dest='infilelist',type=str)
parser.add_argument('-o','--out-dir',    dest='outdir',    type=str)
parser.add_argument('--gridDS',          dest='gridDS',    type=str)
parser.add_argument('--outDS',           dest='outDS',     type=str)

## configuration arguments
parser.add_argument('--leptons-only',dest='leps_only',action='store_true')

args = parser.parse_args()
if len(sys.argv) < 5:
    parser.print_help()
    exit(0)

def main():
    import ROOT
    ROOT.xAOD.Init().ignore()

    ROOT.EL.Algorithm()
    ROOT.asg.ToolStore()

    job = ROOT.EL.Job()
    output = ROOT.EL.OutputStream("treeOutput")
    job.outputAdd(output)
    ntuple = ROOT.EL.NTupleSvc("treeOutput")
    job.algsAdd(ntuple)

    alg = ROOT.TRTLite.TruthLoop()
    job.algsAdd(alg)
    alg.useGRLTool(True)
    alg.setFillLeptonsOnly(args.leps_only)
    alg.setTreeOutputName("treeOutput")

    if args.infilelist and args.outdir:
        sh = ROOT.SH.SampleHandler()
        ROOT.SH.readFileList(sh, "sample", args.infilelist)
        sh.setMetaString("nc_tree","CollectionTree")
        sh.print()
        job.sampleHandler(sh)

        driver = ROOT.EL.DirectDriver()
        submitDir = args.outdir
        driver.submit(job, args.outdir)
        exit(0)

    elif args.gridDS and args.outDS:
        sh = ROOT.SH.SampleHandler()
        ROOT.SH.scanRucio(sh, args.gridDS)
        sh.setMetaString("nc_tree", "CollectionTree");
        sh.setMetaString("nc_grid_filter","*.root*")
        sh.print()
        job.sampleHandler(sh)

        ts = datetime.datetime.fromtimestamp(time.time()).strftime('%Y%m%d_%H.%M.%S')
        driver = ROOT.EL.PrunDriver()
        driver.options().setString("nc_outputSampleName", args.outDS)
        driver.submitOnly(job, "runTRTLiteOnGrid_"+ts)
        exit(0)

    else:
        exit('TBD')

if __name__ == '__main__':
    main()
