#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

void printAllINode() {
    DIR *dirp;
    struct dirent *dp;
    char path[1024];
    struct stat sb;

    printf("     PID     Inode\n");
    printf("==================\n");

    dirp = opendir("/proc");
    while ((dp = readdir(dirp)) != NULL) {
        if (dp->d_name[0] >= '0' && dp->d_name[0] <= '9') {
            sprintf(path, "/proc/%s/fd", dp->d_name);
            DIR *dirp2 = opendir(path);
            if (dirp2) {
                struct dirent *dp2;
                while ((dp2 = readdir(dirp2)) != NULL) {
                    if (dp2->d_name[0] >= '0' && dp2->d_name[0] <= '9') {
                        sprintf(path, "/proc/%s/fd/%s", dp->d_name, dp2->d_name);
                        lstat(path,&sb);
                        printf("%10s %10ld\n", dp->d_name,
                               sb.st_ino);
                    }
                }
                closedir(dirp2);
            }
        }
    }
    closedir(dirp);

}

void printINodeForPID(pid_t *pid) {
    DIR *dirp;
    struct dirent *dp;
    char path[1024];
    struct stat sb;

    printf("     PID     Inode\n");
    printf("==================\n");

    sprintf(path, "/proc/%s/fd", pid);
    DIR *dirp2 = opendir(path);
    if (dirp2) {
        struct dirent *dp2;
        while ((dp2 = readdir(dirp2)) != NULL) {
            if (dp2->d_name[0] >= '0' && dp2->d_name[0] <= '9') {
                sprintf(path, "/proc/%s/fd/%s", pid, dp2->d_name);
                lstat(path,&sb);
                printf("%10s %10ld\n", pid,
                       sb.st_ino);
            }
        }
        closedir(dirp2);
    }
}