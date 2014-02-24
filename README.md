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
* Thumbnail generation

## Features planned
* Sync local database of tags to server
* Sync tags from server to local database
* Sync covers of videos in your library
* Actress videography and info
* Duplicate detection
* Recommendations
* OpenCV face detection
* Subtitle sharing for JAV

## Requirements
* [Qt5] (http://qt-project.org)
* [sqlite] (http://www.sqlite.org)
* [ffmpegthumbnailer] (https://code.google.com/p/ffmpegthumbnailer)

## Installation
* qmake
* make

## Importing Videos into library
To import videos into the database

* menu > general > options > select a folder under "Images Folder" (this folder is where thumbnails are stored)
* menu > general > options > add top level directory to either JAV, Porn, Hentai
* menu > database > import videos

The import will not copy or move any files, it will just insert file name, file path, and file hash into the database.

## Add Tags to Videos

* click a video in the file listing
* select a tag under the tag combobox / type a new tag into the tag combobox
* the new tag will appear under the tag list
* double click a tag in the tag list to remove a tag from a video

## Add Actress to Videos

* click a video in the file listing
* select an actress under the Actress combobox / type a new actress into the actress combobox
* the new actress will appear in the actress list
* double click an actress in the actress list to remove an actress from a video

