Weathereport is an open source CLI application to get the weather forecast of a city in the next 2 days. It is divided into several modules, and to extend the features of this application, the same interface needs to be maintained in each module. For example, if you want to use another weather service other than Yahoo! Weather API, you need to implement a "char* alloc_forecast_from_your_weather_service(char* city)" function and use it to replace the old "yahoo_alloc_forecast" one in the main() function.


BUILD

cd ./source
./m ('cli_weathereport "new york"' will be called for testing.)

The current revision is tested under Mac OS X 10.6.2.


DEPENDENCY

GNU standard libs. (libcurl and "-lcurl" was needed in previous versions, however it cannot be used directly in the current revision because the interface has had been changed. See HISTORY for details.)


HOW TO USE

./cli_weathereport CITY_NAME


TEST CASES

cd ./test
./stress


MACROS

DISABLE_CACHE:	uncomment to disable cache files, i.e. always fetching data from yahoo.


TODO

Add more weather services.
Add more command-line parameters.
(Coding style and memory leaks are always important - check them in every revisions!)


MAINTENANCE

It is recommended to add a cron job to remove the outdated cache files (or keep them as backup data if it's needed).


HISTORY

Revision 10
Split weathereport.c into several modules.

Revision 9
Removed the access version (see rev 8).
Moved the "stress" test case into the ./test directory.

Revision 8
Added a "access" version to check cache existance. However, both the access version and the fopen version take about 26 seconds to perform the 10,000 stress test, so it will be removed in the next release.

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
