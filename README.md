# WSAE #
The Worlds Scariest Archive Extractor is a linux tool for extracting "World's Scariest Police Chases" Game Files.

* The tool requires to files to function:
 	* _police.toc_ - this is the table of contents file found on the root directory of the disc
 	* _police.img_ - this are the files themselves one dumped after another. This is also on the root of the disc.
  
* I recommend ripping/copying the disc to a folder in your home directory.
 
## Installing ##
* build from source by opening the teminal in the WSAE folder and typing  `make`
* copy the tool to 1 of 2 places
1. your bin folder (/home/user/bin) <--ubuntu, does this work on other systems?
2. in the same directory as the .toc and .img files

## Using ##
 * open the terminal in your WSPC rip directory and depending on where you copied the tool,type
 1. `wsae -h`   (option 1)
 2. `./wsae -h` (option 2)
 
* if you run the tool with no arguments, it will extract from police.toc and police.img in the current directory.
