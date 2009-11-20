BUILD

cd ./source
./m ('cli_weathereport "new york"' will be called for testing.)

The current revision is tested under Mac OS X 10.6.2.

DEPENDENCY

libcurl (downloading weather data online.)

HOW TO USE

./cli_weathereport CITY_NAME

TEST CASES

cd ./source
./stress

MACROS

DISABLE_CACHE:	uncomment to disable cache files.

TODO

Move the new test case into the ./test directory.
Find more memory leaks (checked with valgrind since rev 5).
Improve coding style.
Use access and lock to check cache existence to improve performance.

MAINTENANCE

It is recommended to add a cron job to remove the outdated cache files (and the downloaded data files, before the unlink feature is added).

HISTORY

Revision 7
Added a new test case at ./source/stress.

Revision 6
Removed curlget.h from weathereport.h, which was forgotten in the last commit.

Revision 5
Fixed a leak.
Code cleanup.
Removed the curlget module from Makefile.

Revision 4
Fixed a leak.
Split the code into several modules.
Replaced the module curlget with httpget, a light-weight http client based on TCP.
Improved performance by avoiding save data.
Added some comments.
Added a simple Makefile.

Revision 3
The first rough version. Functions initially implemented.

Revision 2
Initial checkout & commit.

Revision 1
Repository created.
