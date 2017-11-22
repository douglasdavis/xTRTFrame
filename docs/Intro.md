# Introduction

### Table of Contents
* [ATLAS Software](#atlas-software)
* [Getting setup](#getting-setup)
* [Compiling the project](#compiling-the-project)
* [Config file and IO](#config-file-and-io)
* [Pushing to a GitLab repository](#pushing-to-a-gitlab-repository)
  + [Handling the xTRTFrame subrepo in your repo](#handling-the-xtrtframe-subrepo-in-your-repo)
* [Refreshing your existing working environment](#refreshing-your-existing-working-environment)

## ATLAS Software

If you're unfamiliar with ATLAS Software - [give this a
read](ATLASSW.md). All `xTRTFrame` documentation is going to be related
to ATLAS "Analysis Releases." Not Athena! If these sentences didn't
make sense to you, click that link.

I'm also going to assume that you're working on `lxplus` or something
similar. (Something with ATLAS CVMFS directories mounted... where you
can run `setupATLAS`).

## Getting setup

First, let's define a few things:

* **Working Area**: Pretty self explanatory - where you'll be doing
  your work; in this area we will store code, build files, and launch
  jobs. I'm going to call the path `/path/to/workarea`
* **Source Area**: This is where code you develop is going to live. It
  will be a `git` repository and will be the name of your project. For
  the purpose of this tutorial I'm going to call mine
  `AnaProject`. You should call yours something related to your
  project! I'm going to call the path `/path/to/workarea/AnaProject`.
* **Build Area**: This is where we'll compile code, you won't actually
  edit any files here, you'll just be using `cmake` to build and compile
  here. I'm going to call the path `/path/to/workarea/build`.
* **Run Area**: This is where we'll execute our code (run our
  algorithm). I'm going to call the path `/path/to/workarea/run`.

Let's get to it, create a working area:

```sh
$ cd /path/to/workarea
$ mkdir -p /path/to/workarea/run
$ mkdir -p /path/to/workarea/build
```

Now let's setup an ATLAS Analysis Release

```sh
$ cd build
$ setupATLAS # if you haven't done this already
$ asetup 21.2.10,AnalysisBase
$ lsetup git
```

You now have an analysis environment using the 21.2.10 release (the
latest release at the time of writing this).

Were now going to generate a skeleton project which will take
advantage of the `xTRTFrame` package:

```sh
$ cd ../
$ curl https://gitlab.cern.ch/atlas-trt-software/xTRTFrame/raw/master/scripts/xTRTGenerateProject.py | python - AnaProject
```

Remember, I'm calling my project `AnaProject` -- you should call yours
something related to your project!

You should now have the following structure in `/path/to/workarea`:
```
.
├── AnaProject
│   ├── AnaProjectLooper
│   ├── CMakeLists.txt
│   ├── README.md
│   └── xTRTFrame
├── build
└── run
```

The sript you just downloaded with `curl` and ran with Python created
a skeleton project and checked out `xTRTFrame` as a `git
submodule`. This is a git repository within a git repository.  Your
project is the top level git repository. `xTRTFrame` is a package
within it. We also created a package called `AnaProjectLooper`. This
is going to be your package which inherits from `xTRTFrame`. The
reason why we call it [ProjectName]Looper is because the package is
going to be home to a "loop algorithm."

Right now the project does _nothing_. It's a skeleton. Where going to
develop code in the new package to actually do something.


## Compiling the project

Since we have some code, let's compile it.

```sh
$ cd /path/to/workarea/build
$ cmake ../AnaProject
$ make -j4
$ source $AnalysisBase_PLATFORM/setup.sh
```

The `cmake` command generates the files required for the build, and
then we compile with `make`. CMake also generates a setup script that
we source (this sets up the necessary environment variables to use our
now compiled library and executable). You shoud now have a new
executable at your disposal. Mine is called
`runAnaProjectLooperAlg`. If you run it, you'll get a message telling
you that you're missing some command line arguments, specifically a
config file. Let's talk about the config file.

## Config file and IO

Everytime you use `xTRTFrame`, you'll be required to use a config
file. We supply a default one: `xTRTFrame/data/default.cfg`. Let's use
it.

```sh
$ cd /path/to/workarea/run
$ cp ../AnaProject/xTRTFrame/data/default.cfg .
```

There's a dedicated documentation page which describes all of the
config parameters (and how to add custom ones). That's
[here](Config.md). For now we'll continue with the tutorial while
using the default one, we'll talk about making some changes in a later
section.

The whole purpose of the algorithm is to process some DAODs. Let's
talk input/output (IO). There are two ways to process DAODs with
`xTRTFrame`: either a plain text file with a list of DAOD files, or a
grid dataset. For this tutorial, we have some sample files saved in
EOS for easy use with the plain text file input. We'll talk about the
grid later.

```sh
$ cp ../AnaProject/xTRTFrame/data/sample_files.txt .
```

Now we can actually run the algorithm (we always define an output
directory with the `-o` flag):

```sh
$ runAnaProjectLooper -c default.cfg -i sample_files.txt -o job_output
```

Congrats! You've just run your first `xTRTFrame` based algorithm. The
output files are in the new directory `job_output`. Unfortunately, the
algorithm does nothing... we will change that soon!

## Pushing to a GitLab repository

One of the things that python script did was make your project a `git`
repository. Let's make your first commit.

```sh
$ cd /path/to/workarea/AnaProject
$ git status # see all the files
$ git add . # stage them for commiting
$ git commit -m "first gimmit" # make the commit.
```

Now we want to push them to GitLab. Sign into [CERN's
GitLab](https://gitlab.cern.ch/). Create a new repository with the
name of your project. Then follow the instructions for pushing an
existing repository.

### Handling the xTRTFrame subrepo in your repo

`xTRTFrame` exists as a submodule in _your_ repository. Think of it as
just a single file in your repository, but when you `cd` into it, it
is an isolated `git` repository. If you checkout a branch in the
`xTRTFrame` git repo, then your git repo will see the `xTRTFrame`
"file" as changed, because the repo is now in a different state.

Let's say you checkout a new tag of `xTRTFrame` (tag vX.Y for example):
```sh
$ cd /path/to/workarea/AnaProject/xTRTFrame
$ git fetch origin
$ git checkout -b vX.Y vX.Y
```

Now if you run `git status` in _your_ repo it will tell you that
`xTRTFrame` has changed. You can make your submodule clone of
`xTRTFrame` line up with the vX.Y tag by just treating `xTRTFrame`
like a changed file:

```sh
$ cd /path/to/workarea/AnaProject
$ git add xTRTFrame
$ git commit -m "sync xTRTFrame with tag vX.Y"
$ git push origin master
```

## Refreshing your existing working environment

When you exit your shell and come back, you'll need to re-setup your
environment. Do that with:

```sh
$ cd /path/to/workarea
$ cd build
$ setupATLAS
$ asetup 21.2.10,AnalysisBase
$ lsetup git
$ cmake ../AnaProject # not necessarily needed but good for completion
$ make                # not necessarily needed but good for completion
$ source $AnalysisBase_PLATFORM/setup.sh
```

## Moving on

Alright, now that we have a project that compiles and runs, let's
actually start making your algorithm do something. [Next
phase! (Algorithm Development)](AlgDevelopment1.md)
