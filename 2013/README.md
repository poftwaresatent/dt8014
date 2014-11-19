Halmstad Algorithms Course 2013 (DT8014)
========================================

* The [syllabus][] (PDF) is the official document describing the
  format and intended learning outcomes.
* The [Blackboard site][] contains further details, instructions, and
  links.
* The [notes][] are a "living" Google document that grows as we
  currently teach this course for the first time.

[syllabus]: http://www.hh.se/sitevision/proxy/utbildning/hittautbildning/kursplaner.4677.html/svid12_70cf2e49129168da015800074301/752680950/se_proxy/utb_kursplan.asp?kurskod=DT8014&revisionsnr=1&format=pdf&lang=en
[Blackboard site]: https://hh.blackboard.com/webapps/portal/frameset.jsp?tab_tab_group_id=_2_1&url=%2Fwebapps%2Fblackboard%2Fexecute%2Flauncher%3Ftype%3DCourse%26id%3D_5092_1%26url%3D
[notes]: https://docs.google.com/document/d/1C1Gs8qaBFTV1PkHLRUuLx_WAx8tviem3PDRAPJqlCFw/edit?usp=sharing

This repository contains example code, starting points for project,
etc.  The directories are named after the course modules they are
relevant for.  As this is the first time we are teaching this course,
everything here should be considered "under construction."


Setup for Module 3 - Computational Geometry
-------------------------------------------

The code in the `m3-computational-geometry` subdirectory serves as
starting point for [project 2][].  It relies on [GTK+][] (version 2)
to provide a graphical user interface.  In order to minimize the
learning curve, a simplified wrapper is provided via the
`m3-computational-geometry/gfx.hpp` file.  Installing GTK+ is trivial
on Linux, easy on Mac OS X, and apparently quite easy on Windows.

* Most if not all Linux distribution provide GTK+ in their package
  manager.  You will need the development version in case your
  distribution distinguishes it from the runtime package.  For
  example, under Debian just run `sudo apt-get install libgtk2.0-dev`.
  Notice that the workstations in Halmstad University's room B231c
  have all the necessary packages to work with this.

* For Mac OS X, the recommended way is to use [Macports][].  This
  simplifies the GTK+ setup, all you then need is `sudo port install
  gtk2`.

* Under Windows, there appear to be two commonly used setups.  The
  most promising (it has been used successfully by at least one of our
  students with Codeblocks) approach is to follow the official [Win32
  instructions][] on the GTK homepage (the 64 bit version can be
  reached via their download tab at the top of the page).  Note that
  you will need to scroll down the page to the **GTK+ 2.x** files, and
  that there is an all-in-one bundle that should give you all the
  dependencies as well.  The other alternative is to rely on
  [Cygwin][] which appears to be used behind the scenes if you have
  set up Eclipse with CDT.  Googling for "cygwin install gtk+" seems
  to turn up some potentially useful pages if you want to try this
  option.

Try to first compile and run the `test-gfx.cpp` and the
`test-polydraw.cpp` examples from the `m3-computational-geometry`
directory.  To make sense of the code, read the comments there and in
`gfx.hpp`.

The line intersection program illustrated during the lecture is called
`example-line-intersection.cpp`.  It allows you to drag line segment
end points and also add new lines by dragging in empty areas.  This
source code is also intended to serve as starting point for your
project, illustrating how to manage user interaction.

Finally, note that everything you find here has been developed on OS X
under Macports using the [clang++][] compiler, so you may very well
need to adjust some things here and there.  Especially if you work
under Windows.  Please do feel free to submit [issues][], [patches][],
or preferably [pull requests][].

[project 2]: https://docs.google.com/document/d/1C1Gs8qaBFTV1PkHLRUuLx_WAx8tviem3PDRAPJqlCFw/edit?pli=1#heading=h.d4zdbfz995a2
[GTK+]: http://www.gtk.org/
[Macports]: http://www.macports.org/
[Cygwin]: http://www.cygwin.com/
[Win32 instructions]: http://www.gtk.org/download/win32.php
[clang++]: http://clang.llvm.org/
[issues]: https://github.com/poftwaresatent/algocourse/issues
[patches]: http://en.wikipedia.org/wiki/Patch_(Unix)
[pull requests]: https://help.github.com/articles/using-pull-requests
