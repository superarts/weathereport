BUILD

cd ./source
./m ("cli_weathereport tianjin" will be called for testing.)

The current revision is tested under Mac OS X 10.6.2.

DEPENDENCY

libcurl (downloading weather data online.)

HOW TO USE

./cli_weathereport CITY_NAME

TEST CASES

cd ./source
php ../test/stress.php
*1. 10,000 times testing. Takes 56 seconds on my computer. Cache mode enabled.
*2. Above 80 seconds if cache mode is disabled.

MACROS

DISABLE_CACHE:	uncomment to disable cache files. 
OFFLINE_TEST:	uncomment to enable offline test (doesn't fetch data online).

TODO

enable space support, e.g. 'new york'
unlink data files
use access and lock to check cache existence

MAINTENANCE

It is recommended to add a cron job to remove the outdated cache files (and the downloaded data files, before the unlink feature is added).
