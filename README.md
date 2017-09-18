# WSAE
The Worlds Scariest Archive Extractor is a linux tool for extracting "World's Scariest Police Chases" Game Files

#The tool requires to files to function:
  *police.toc - this is the table of contents file found on the root directory of the disc
  *police.img - this are the files themselves one dumped after another. This is also on the root of the disc.
#I recommend ripping/copying the disc to a folder in your home directory.
 
 ?? How To Use ??
 #build from source
 
 #copy the tool to 1 of 2 places
    1) your bin folder (/home/user/bin) <--ubuntu, does this work on other systems?
    2) In the same directory as the .toc and .img files
    
 #open the terminal in your WSPC rip directory and depending on where you copied the tool,type 
  "wsae -h"   (option 1) 
  "./wsae -h" (option 2)
  
#upon success you will see this screen;

Worlds Scariest Archive Extractor 0.1A
Usage: [options] [-t] [path to.TOC file] [-i] [path to .IMG file]
Options:
	-l	List files only
	-v	Display verbose
	-h	Displays this screen
  
#if you run the tool with no arguments, it will extract from police.toc and police.img in the current directory.

  
 
