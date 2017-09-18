#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/dir.h>
#include <unistd.h>
#include <libgen.h>
#include <dirent.h>
//^^ thats a lot of includes :S


/*World's Scariest Archive Extractor 4 linux by RooMan*/
//version 0.1.1
//FIXED create directory tree to avoid seg' faults :(
//TODO print file tree
//FIXED fix command line arguments


#define VERSION_Major   0
#define VERSION_Minor   1
#define VERTION_RelType 'A'


#define oDEBUG      'd'
#define oVERBOSE    'v'
#define oSILENT     's'
#define oLISTONLY   'l'
#define oHELP       'h'
#define oTOC        't'
#define oIMG        'i'


int DEBUG   =0;
int VERBOSE =0;
int SILENT  =0;
int LIST    =0;
int argi=0;







void printHelp()
{
    printf("\nWorlds Scariest Archive Extractor %d.%d%c\nUsage: [options] [-t] [path to.TOC file] [-i] [path to .IMG file]\nOptions:\n\t-l\tList files only\n\t-v\tDisplay verbose\n\t-h\tDisplays this screen\n",VERSION_Major,VERSION_Minor,VERTION_RelType);
    exit(0);
}




int main(int argc,char *argv[])
{
char buff[2024]; //buffer to hold the current dir #oversized

getcwd(buff,sizeof(buff)); //copy the current working dir into buffer
char tocFileLocation[1000]; //buffer for the tocfile path #oversized
char imgFileLocation[1000]; // ditto 
strcpy(tocFileLocation,buff); // copy the CWD path into the toc file path
strcpy(imgFileLocation,buff); // ditto
strcat(tocFileLocation,"/police.toc"); //append the filename
strcat(imgFileLocation,"/police.img"); // ditto


for (argi=1;argi<argc;argi++) // scanning arguments
{
    char *ret=strpbrk(argv[argi],"-"); // lokking for "-"
        if (ret) //if a "-" was found scan for known arguments
        {
            char *option=strpbrk(argv[argi],"dvslhti");//scanning...

                if (option) //if a valid argument was found...
                {
                    switch (*option) // which one exactly?
                    {
                        case oDEBUG: //debug?
                            DEBUG=1;
                        break;

                        case oLISTONLY: //list only mode?
                            LIST=1;
                        break;

                        case oVERBOSE: //verbose mode?
                            VERBOSE=1;
                        break;

                        case oSILENT: //shh
                            SILENT=1;
                        break;

                        case oHELP: //need a hand?
                            printHelp();
                        break;

                        case oTOC: // im looking for the toc file in the next argument
                        sprintf(tocFileLocation,"%s",argv[argi+1]);
                        break;

                        case oIMG: // im looking for the img file in the next argument
                        sprintf(imgFileLocation,"%s",argv[argi+1]);
                        break;

                        default: // none of the above?
                            printHelp();
                        break;
                    }
                }

            else {printf("<PANIC> unknown argument %s\n",argv[argi]);printHelp();} // no argument seemed to mach ones known
        }
}





    FILE *tocfp=fopen(tocFileLocation,"rb"); // open the toc file
    FILE *imgfp=fopen(imgFileLocation,"rb"); // open the img file

        if (tocfp!=NULL && imgfp!=NULL) // are they open?
        {

            int i=0,x=0,count=0,offOfFile [1000],sizeOfFile [1000]; // arrays for the file name/size/offset
            char nameOfFile[1000][1000],fileBuffer[1000];

            if(DEBUG||VERBOSE){if (!SILENT){printf("TOC file loaded at %p\nIMG file loaded at %p\n",&tocfp,&imgfp);}} // are we debugging?

        //begin find data blocks
            while (!feof(tocfp))
            {
                int ch,i=0;

                /*bitbang the file name into buffer and convert to linux friendly names*/
                while ((ch = fgetc(tocfp)) != '\0' && ch != EOF)    //while not null or end of file...
                    {
                        if (ch=='\\') {nameOfFile[count][i++]='/';} // linux name conversion '\' into '/'

                        else{nameOfFile[count][i++] = ch;}          //anything else place in buffer
                    }
                        nameOfFile[count][i] = '\0';                //append null when we are finished


                //TODO make this section cpu agnostic
                // lets find the offset where the file is located
                fread(&offOfFile[count],4,1,tocfp);
                if (ferror(tocfp)&&!SILENT)
                {printf("<PANIC> file segment %X unreadable",count);exit(EXIT_FAILURE);}// if reading from disc some parts may be unreadable
                //lets find its size
                fread(&sizeOfFile[count],4,1,tocfp);
                if(ferror(tocfp)&&!SILENT)
                {printf("<PANIC> file segment %X unreadable",count);exit(EXIT_FAILURE);}// if reading from disc some parts may be unreadable
                //increment our file counter and loop back to top
                count++;
                // warning!! count= totalfiles+1
            }
        //end find data blocks

        //begin 'GUI' and backend

                // fancy debug info
                if (DEBUG||VERBOSE)
                {
                    if (!SILENT)
                    {
                        for (i=0;i<count-1;i++)
                        {
                            printf("found file %s\n",nameOfFile[i]);
                        }
                    }
                }





                if (!LIST)
                    {
                    //super-duper complex file extration comming...
                        for(i=0;i<count-1;i++)
                        {

                            char *pathname=strdup(nameOfFile[i]); //duplicate name of file to a buffer
                            char *bname=dirname(pathname);// this destroys the original string (no idea why)
                            sprintf(fileBuffer,"%s",nameOfFile[i]);

                            fseek(imgfp,offOfFile[i],SEEK_SET);

                                //'GUI' clear screen and display the current file being extracted
                            if(!VERBOSE)
                            {
                                if (system("clear")) {printf("unable to execute clear screen system function\n");}
                            }


                            if (strlen(bname)>1)
                            {
                                if(!opendir(bname)){mkdir(bname,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);} //if dir doesn't exist create it(uncondionaly)
                            }

                            if (!SILENT){printf("Extracting:\n%s\n\t\tOffset:%x\n\t\tSize: %dB\n",nameOfFile[i],offOfFile[i],sizeOfFile[i]);}



                            FILE *of=fopen(fileBuffer,"w+b"); //open a file to write into
                            for(x=0;x<sizeOfFile[i];x++)
                            {
                                fputc(fgetc(imgfp),of); //dump file from img <--speedy, glitchy, exciting!
                            }

                            fclose(of);
                        }
                         //close current file and loop back
            }

        }

else {printf("<PANIC> one or more files could be loaded\n");exit(EXIT_FAILURE);}

    free(tocfp);
    free(imgfp);
    return 0;
}
