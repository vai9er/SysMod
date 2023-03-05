#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <limits.h>

void print_header() {
    printf("    PID  FD  Filename\n");
    printf("    =========================\n");
}

// A function that prints out a footer for the table
void print_footer() {
    printf("    =========================\n");
}

// A function that prints out the file descriptors of a given process id
void printFDPath(pid_t pid) {
    // Open the directory /proc/<pid>/fd where the file descriptors are listed
    char path[256];
    sprintf(path, "/proc/%d/fd", pid);
    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("CANNOT OPEN");
        return;
    }
    print_header();
    // Loop through each entry in the directory
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        // Skip . and .. entries
        if (entry->d_name[0] == '.') continue;
        
        // Convert the entry name to an integer (the file descriptor number)
        int fd = atoi(entry->d_name);
        
        // Get the path of the file descriptor using readlink()
        char fd_path[256];
        sprintf(fd_path, "/proc/%d/fd/%s", pid, entry->d_name);
        char path_buf[PATH_MAX];
        ssize_t len = readlink(fd_path, path_buf, sizeof(path_buf));
        if (len == -1) {
            perror("CANNOT READLINK");
            continue;
        }
        path_buf[len] = '\0';
        // Print out the process id, file descriptor number, and path
        printf("%8d %3d %s\n", pid, fd, path_buf);
    }

    // Close the directory
    closedir(dir);
    print_footer();
}

int main(){
    printFDPath(8591);
    return 0;
}

