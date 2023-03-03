#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <errno.h>

void FDTABLE(pid_t pid) {
    char path[1024];
    sprintf(path, "/proc/%d/fd", pid);
    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("opendir");
        return;
    }
    printf(" PID  FD\n");
    printf("=============\n");
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.') continue;
        printf("%5d%4s\n", pid, entry->d_name);
    }
}

int isThresh(int threshold) {
    DIR *proc = opendir("/proc");
    if (proc == NULL) {
        perror("opendir");
        return 1;
    }
    
    int offending = 0;
    
    struct dirent *entry;
    
    while ((entry = readdir(proc)) != NULL) {
        if (entry->d_name[0] == '.') continue;
        
        char *endptr;
        pid_t pid = strtol(entry->d_name, &endptr, 10);
        
        if (*endptr != '\0') continue;

        char path[1024];
        sprintf(path, "/proc/%d", pid);

        struct stat sb;
        
        if (stat(path, &sb) == -1) {
            perror("stat");
            continue;
        }

        if (sb.st_uid != getuid()) continue;

        sprintf(path, "/proc/%d/fd", pid);

        DIR *fd = opendir(path);
        
        if (fd == NULL) continue;

        int count = 0;

        while ((entry = readdir(fd)) != NULL)
            count += entry->d_name[0] != '.';

            closedir(fd);

            if (count > threshold){
                offending++;
                printf("%5d(%2d)",pid,count);
            }
            
     }

     closedir(proc);

     return offending > 0 ? 1 : 0; 
}

int main(int argc,char **argv){
   int thresh=atoi(argv[1]);
   FDTABLE(getpid());
   printf("\nOffending processes : ");
   isThresh(thresh);
}