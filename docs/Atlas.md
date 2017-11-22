# Tiny ATLAS Software Lecture

### The ATLAS Computing environment

I'll briefly give an introduction to ATLAS Software and the Data Format

#### The software

For our purposes ATLAS software (SW) can be broken in two: Athena and
Analysis Releases (it gets more complicated but that's beyond the
scope of this small tutorial).

* Athena: The main offline software - can do everything (simulation,
  reconstruction, analyisis, ...)
* Analysis Releases: A subset of the main ATLAS code base (that exists
  in Athena) but can be used outside of Athena ("dual use
  code")... and also some analysis specific code on top of it.

All of the code is stored in a `git` repository hosted at the Athena
GitLab page [here](https://gitlab.cern.ch/atlas/athena).

People who run the core software group and the analysis software group
handle building what parts of the code are needed for Athena releases
and what parts are needed for analysis releases. The builds are called
releases and they are installed to the ATLAS CVMFS areas so that all
machines connected to the ATLAS CVMFS repositories have access to the
releases. These days, all code is built with `cmake`. We'll talk more
about that later.

The current main release of ATLAS Software is Athena release 21.0.X.

The Analysis Releases are on the numbering scheme 21.2.X. For the main
part of this workbook, we'll be setting up and using an Analysis
Release.

#### The Data Format

The data format (what is actually stored on disk in ROOT files) is
very closely related to the analysis software. The xAOD model was
developed for Run 2 of ATLAS. The xAOD model is a set of classes which
make it nice and intuitive to analyze physics objects. At it's core it
represents physics objects as C++ classes (e.g. Event, TrackParticle,
Electron, Jet, etc.). It also provides the means to analyze those
objects, store them in ROOT files, and "decorate" them with
properties.

The actual *files* stored on disks all over the world at grid sites
and in your work areas if you've downloaded some from `rucio` are
called DAOD's (Derived Analysis Object Data, some people also call the
files themselves xAOD's). A primary AOD is the starting point: Some
Athena code produces the AOD after reconstruction, and then the
**Derivation Framework** is used to produce the Derived AOD's. These
are tailored for specific purposes. Starting from the AOD which
contains everything, a derivation is made by removing whole events,
removing whole objects from events, removing some information attached
to objects in some events, or adding some information to some objects
(we do this in the TRT a lot!) The nomenclature:

* skimming: removing whole events
* thinning: removing whole objects from within an event, but keeping
  the rest of the event
* slimming: removing information from within objects, but keeping the
  rest of the object
* augmentation: adding data not found in the input data

In the TRT SW group, we use the Inner Detector (InDet) specific
derivation, with some of our own stuff built on top.

### More

That's all I'm going to say about ATLAS SW here since we need to get
onto TRT specific things -- but there's a lot more detailed
information in other places and I'd suggest starting
[here](https://atlassoftwaredocs.web.cern.ch/)!

Now you can head back to the [Intro](Intro.md).
