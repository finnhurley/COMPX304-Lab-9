/**
 * @author Finn Hurley
 **/

#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


//Returns the number of seconds since the last time a file was modified
int lastModified(time_t timeStamp)
{
    return (time(NULL) -
            timeStamp);
}

//Returns a years worth of seconds
int getYearInSeconds()
{
    return (60 *
            60 *
            24 * 
            365);
}

//Converts a decimal to it's octal value.
//Retrieved from: https://www.programiz.com/c-programming/examples/octal-decimal-convert
int toOctal(int value)
{
    int octalValue = 0;
    int i = 1;

    while (value != 0)
    {
        octalValue += (value % 8) * i;
        value /= 8;
        i *= 10;
    }

    return octalValue;
}

//Recursive function that prints every file inside of a specified directory.
void lsDir(char dirName[])
{
    DIR *d;
    struct dirent *dir;
    struct stat buf;

    d = opendir(dirName);

    if (d) {
        while ((dir = readdir(d)) != NULL) {
            stat (dir->d_name, &buf);

            int permissions = toOctal(buf.st_mode);

            if ((permissions % 10) == 2 ||
                (permissions % 10) == 0) {
                    printf("~Inaccessable File Skipped~\n");
                    continue;
            }

            if (dir->d_type == 4 && 
                (strncmp(dir->d_name, "..", strlen(dir->d_name)) != 0) && 
                (strncmp(dir->d_name, ".", strlen(dir->d_name)) != 0)) {

                    if (lastModified (buf.st_mtime) > getYearInSeconds()) {
                        printf("~Outdated Directory Skipped~\n");
                        continue;
                    }

                    printf("----- Directory: %s (Last modified %d seconds ago) -----\n", dir->d_name, lastModified (buf.st_mtime));
                    lsDir(dir->d_name);
            }
            else {
                printf("%s\n", dir->d_name);
            }
        }

        if ((strncmp(dirName, ".", strlen(dirName))) != 0) {
            printf("----- End of Directory: %s -----\n", dirName);
        }

        else {
            printf("----- End of Main Directory -----\n");
        }

        closedir(d);
    }
    return;
}

int main (void)
{
    char thisDir[] = ".";
    printf("----- Main Directory -----\n");
    lsDir(thisDir);
    return 0;
}