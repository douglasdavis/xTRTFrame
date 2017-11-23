# Algorithm Development Part 1

### Table of Contents
* [(Back to the Intro)](Intro.md)
* [Package Overview](#package-overview)
* [Adding a histogram](#adding-a-histogram)
  + [Look at the Histogram](#look-at-the-histogram)
* [Adding ntuple Output](#adding-ntuple-output)
* [Summary](#summary)

## Package Overview

So I'm calling my project `AnaProject`, the package that we're going
to develop is `AnaProjectLooper`. The structure:

```
AnaProjectLooper/
├── AnaProjectLooper
│   ├── AnaProjectLooperAlg.h
│   └── AnaProjectLooperDict.h
├── CMakeLists.txt
├── Root
│   └── AnaProjectLooperAlg.cxx
├── data
└── util
    └── runAnaProjectLooperAlg.cxx
```

The main C++ class that we're going to work on is the
`AnaProjectLooperAlg`. The class is defined in the header file
`AnaProjectLooper/AnaProjectLooperAlg.h` and implemented in the source
file `Root/AnaProjectLooperAlg.cxx`

If you look at the header file, you'll that your class inherits from
`xTRT::Algorithm`. You'all also see some functions:

* `histInitialize()`
* `execute()`
* `finalize()` 

These are functions which are part of the `EventLoop` API. `EventLoop`
is a package developed and supported by ATLAS ASG (Analysis Software
Group). If you look at the AnalysisBase tutorial (Linked in the
[Intro](Intro.md)), they go into more details about `EventLoop`. The
`xTRT::Algorithm` class inherits from the `EL::Algorithm` class. So
`xTRTFrame` itself is just an _extension_ to `EventLoop`. **This is
one of the most important ideas of xTRTFrame: It is a structure built
around EventLoop for the purpose of making life easier as a TRT
analyzer.** `xTRTFrame` provides an algorithm with many helper/utility
functions to bootstrap your project.


All of the EventLoop API functions are listed, some are commented out
since they might be needed for more advanced tasks (beyond the scope
of this tutorial).

If you look at the source file, you'll see a description of how these
functions are called. We'll be spending most of our time in the
tutorial in `histInitialize()` (called before any events are
processed) and `execute()` (called once per event).

## Adding a Histogram

Alright, let's add some output. Starting with a histogram. We want a
histogram of the pileup, or average mu, distribution We're going to
add a single line to the source, `Root/AnaProjectLooperAlg.cxx`. In
the `histInitialize()` function, before the return statement, we'll
use the `create(...)` function (part of `xTRT::Algorithm`):

```cpp
EL::StatusCode AnaProjectLooperAlg::histInitialize() {
  // ...
  // ...

  create(TH1F("h_avgMu",h_avgMu",100,0,100));

  return EL::StatusCode::SUCCESS;
}
```

Now in the `execute()` function we'll use the `averageMu()` function
(part of `xTRT::Algorithm`) to retrieve the value and fill the
histogram with the `grab<T>(...)` function (part of
`xTRT::Algorithm`). We'll also make sure to get the event weight.

```cpp
EL::StatusCode AnaProjectLooperAlg::execute() {
  // ...
  // ...

  float w     = eventWeight();
  float avgMu = averageMu();
  grab<TH1F>("h_avgMu")->Fill(avgMu,w);
  
  return EL::StatusCode::SUCCESS;
}
```

Notice how we "grab" the histogram object with the name used when
creating it. That's it! That's how you create and fill histograms.

### Look at the Histogram

Now that we've made some changes to the code, we need to recompile.
```sh
$ cd /path/to/workarea/build
$ make
```

Notice that we did not need to run `cmake` again -- this would only be
necessary if you added _new_ files. If we just modify _existing_ files,
then all we need to do is recompile, the build files generated by
`cmake` are already there from before.

Let's run again:
```sh
$ cd ../run
$ runAnaProjectLooper -c default.cfg -i sample_files.txt -o job_output_with_hist
....
....
....
$ root -l job_output_with_hist/hist-sample.root
[0] new TBrowser
```

Now you ought to be able to check out your new `h_avgMu` histogram.

## Adding ntuple Output

We can also make your algorithm output ntuples. This is a bit more
complex than adding histograms, but still pretty
straightforward. First, we need to add a `TTree` as a class member
variable, and a new class member variable that will be a branch. Let's
go to the header, and in the private section of the class definition
make our variables:

```cpp
// ...
private:

  TTree* m_event_tree; //!
  
  float m_eventWeight; //!

  //...
public:
  //...
```

As you can see, we're going to save the event weight to a new tree
called. Now let's head over to the source, in `histInitialize()`
again. We're going to use a macro provided by `xTRTFrame` which will
initialize the output tree and make sure it gets saved properly.

```cpp
EL::StatusCode::histInitialize() {

  // ...
  // ...
  
  SETUP_OUTPUT_TREE(m_event_tree,"event_tree");
  m_event_tree->Branch("eventWeight",&m_eventWeight);

  return EL::StatusCode::SUCCESS
}
```

Now, in `execute()`, we'll make sure to set `m_eventWeight` on each event and fill the
`m_event_tree`.

```cpp
EL::StatusCode::execute() {
  // ...
  // ...

  float m_eventWeight = eventWeight();
  float avgMu         = averageMu();
  grab<TH1F>("h_avgMu")->Fill(avgMu,m_eventWeight);

  m_event_tree->Fill();

  return EL::StatusCode::SUCCESS;
}
```

Run your job again (now with `-o your_output`), and now you'll find
the new ntuple:

```sh
$ root -l your_output/data-xTRTFrameTreeOutput/sample.root
[0] new TBrowser
```

## Summary

Alright, so we've learned how to actually make our algorithm do
something. We made a histogram and an ntuple. Only, nothing has been
TRT related. Let's move on to [Algorithm Development Part
2](AlgDevelopment2.md) for that.