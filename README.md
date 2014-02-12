# JAV / Porn / Hentai Organizer
a XBMC for porn

The purpose of this project is to have a client that can download metadata about a JAV / Porn / Hentai video.  It will consist of a server and a client.

There will be a mechanism to sync tags and actress based on file hash and video title.

## Features working
* Tagging
* Tag normalization
* Tag autocompletetion
* Add actress to video
* Actress autocompletion
* Select directories to import into library
* Filtering by tag or actress

## Features planned
* Sync local database of tags to server
* Sync tags from server to local database
* Sync covers of videos in your library
* Thumbnail generation
* Actress videography and info
* Duplicate detection
* Recommendations
* OpenCV face detection
* Subtitle sharing for JAV

## Requirements
* [Qt5] (http://qt-project.org)
* [sqlite] (http://www.sqlite.org)
* [boost] (http://www.boost.org)

## Installation
* git submodule init
* git submodule update
* qmake
* make
* g++ -std=c++11 import.cpp -lboost_system -lboost_filesystem -lboost_regex -lsqlite3 -o import

## Importing files into library
To import files into the library, edit dirs to include the folders that you want imported.  It's recursive so just specify the top level, one directory per line:

* /mnt/disk1/porn
* /mnt/disk2/jav

Compile either or both:
* import-jav.cpp to import JAV videos.
* import-porn.cpp to import Porn videos.

the only difference is a regex to detect correctly named JAV files.
