#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <ctype.h>

// A function that prints out the file descriptors of a given process id
void print_fds(pid_t pid) {
    // Open the directory /proc/<pid>/fd where the file descriptors are listed
    char path[256];
    sprintf(path, "/proc/%d/fd", pid);
    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("CANNOT OPEN");
        return;
    }

    // Loop through each entry in the directory
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        // Skip . and .. entries
        if (entry->d_name[0] == '.') continue;

        // Convert the entry name to an integer (the file descriptor number)
        int fd = atoi(entry->d_name);

        // Print out the process id and the file descriptor number
        printf("%8d %3d\n", pid, fd);
    }

    // Close the directory
    closedir(dir);
}

// A function that prints out a header for the table
void print_header() {
    printf("     PID  FD\n");
    printf("=============\n");
}

// A function that prints out a footer for the table
void print_footer() {
    printf("=============\n");
}

int main() {
    // Get the current user id
    uid_t uid = getuid();

    // Open the directory /proc where all processes are listed
    DIR *dir = opendir("/proc");
    if (dir == NULL) {
        perror("opendir");
        return 1;
    }

    // Print out a header for the table
    print_header();

    // Loop through each entry in /proc
    struct dirent *entry;
    
    while ((entry = readdir(dir)) != NULL) {
            // Skip non-numeric entries (not process ids)
            if (!isdigit(entry->d_name[0])) continue;
            // Convert the entry name to an integer (the process id)
            pid_t pid = atoi(entry->d_name);

            // Get the user id of the process owner by reading /proc/<pid>/status
            char path[256];
            
    sprintf(path, "/proc/%d/status", pid);
            
    FILE *file = fopen(path, "r");
            
    if (file == NULL) continue;

            
    char line[256];
            
    uid_t puid = -1;
            
    while (fgets(line, sizeof(line), file) != NULL) {
                // Look for a line starting with Uid:
                if (sscanf(line, "Uid: %u", &puid) == 1) break;
                
    }

            
    fclose(file);

            
    // Skip processes that are not owned by current user
            
    if (puid != uid) continue;

            
    // Print out file descriptors of this process
            
    print_fds(pid);
        
    }

        
    // Print out a footer for table
        
    print_footer();

        
    // Close /proc directory
        
    closedir(dir);
    pid_t pid = 9839;
    print_fds(pid);    
    return 0;

}