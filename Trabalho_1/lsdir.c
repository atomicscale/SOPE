#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
/* "readdir" etc. are defined here. */
#include <dirent.h>
/* limits.h defines "PATH_MAX". */
#include <limits.h>
/* List the files in "dir_name". */
#include <pwd.h>
/* Finding usernames from uid. */

void print_file(const char* path, int output) {
	struct stat file_info;
	struct passwd *pwd;

	if(stat(path, &file_info) == 1) {
		perror(strerror(errno));
		perror("Error retrieving data from file.");
		return;
	}

	char inode[20], size[10], last_modification[20], file_permissions[20];

	sprintf(inode, "%u",(unsigned int) file_info.st_ino);
	write(output, inode, strlen(inode));
	write(output, "  ", strlen("  "));

    sprintf(file_permissions,"%s", (S_ISDIR(file_info.st_mode)) ? "d" : "-");
    write(output, file_permissions, strlen(file_permissions));
    sprintf(file_permissions, "%s", (file_info.st_mode & S_IRUSR) ? "r" : "-");
    write(output, file_permissions, strlen(file_permissions));
    sprintf(file_permissions, "%s", (file_info.st_mode & S_IWUSR) ? "w" : "-");
    write(output, file_permissions, strlen(file_permissions));
    sprintf(file_permissions, "%s", (file_info.st_mode & S_IXUSR) ? "x" : "-");
    write(output, file_permissions, strlen(file_permissions));
    sprintf(file_permissions, "%s", (file_info.st_mode & S_IRGRP) ? "r" : "-");
    write(output, file_permissions, strlen(file_permissions));
    sprintf(file_permissions, "%s", (file_info.st_mode & S_IWGRP) ? "w" : "-");
    write(output, file_permissions, strlen(file_permissions));
    sprintf(file_permissions, "%s", (file_info.st_mode & S_IXGRP) ? "x" : "-");
    write(output, file_permissions, strlen(file_permissions));
    sprintf(file_permissions, "%s", (file_info.st_mode & S_IROTH) ? "r" : "-");
    write(output, file_permissions, strlen(file_permissions));
    sprintf(file_permissions, "%s", (file_info.st_mode & S_IWOTH) ? "w" : "-");
    write(output, file_permissions, strlen(file_permissions));
    sprintf(file_permissions, "%s", (file_info.st_mode & S_IXOTH) ? "x" : "-");
    write(output, file_permissions, strlen(file_permissions));

	

	write(output, "  ", strlen("  "));
	if ((pwd = getpwuid(file_info.st_uid)) != NULL){
		write(output, pwd->pw_name, strlen(pwd->pw_name));
		write(output, "  ", strlen("  "));
	}
	sprintf(size, "%6u", (unsigned int) file_info.st_size);
	write(output, "  ", strlen("  "));
	write(output, size, strlen(size));
	write(output, "  ", strlen("  "));
	strftime(last_modification, 20, "%e %b %g %H:%M", localtime(&file_info.st_mtime));
	write(output, last_modification, strlen(last_modification));
	write(output, "  ", strlen("  "));
}

void list_dir (const char * dir_name,int output)
{
	DIR * d;
    /* Open the directory specified by "dir_name". */
	d = opendir (dir_name);

    /* Check if it was open. */
	if (! d) {
		fprintf (stderr, "Cannot open directory '%s': %s\n",
			dir_name, strerror (errno));
		exit (EXIT_FAILURE);
	}

	struct dirent * entry;
	const char * d_name;
      /* "Readdir" gets subsequent entries from "d". 
      Note: The sorting is based in the order in which they are stored by the filesystem */

	while ((entry = readdir(d)) != NULL) {

		d_name = entry->d_name;
        /* Print the name of the file and directory. */
		//printf ("%s/%s\n", dir_name, d_name);

		if (entry->d_type == DT_DIR) {

            /* Check that teh directory is not "d" or d's parent. */

			if (strcmp (d_name, "..") != 0 && strcmp (d_name, ".") != 0) {
				int path_length;
				char path[PATH_MAX];

				path_length = snprintf (path, PATH_MAX,
					"%s%s", dir_name, d_name);
                /*Concatenating file path to get the full path.
                Only way to access subdirectories*/
				strcat(path,"/");


				if (path_length >= PATH_MAX) {
					fprintf (stderr, "Path length has got too long.\n");
					exit (EXIT_FAILURE);
				}
				write(output, path, strlen(path));
				write(output, "\n", strlen("\n"));
                /* Recursively call "list_dir" with the new path. */
				list_dir (path,output);
			}
		}
		else if(entry->d_type == DT_REG){

			if (strcmp (d_name, "..") != 0 && strcmp (d_name, ".") != 0) {
				int path_length;
				char path[PATH_MAX];

				path_length = snprintf (path, PATH_MAX,
					"%s%s", dir_name, d_name);

				if (path_length >= PATH_MAX) {
					fprintf (stderr, "Path length has got too long.\n");
					exit (EXIT_FAILURE);
				}

				print_file(path, output);
				write(output, d_name, strlen(d_name));
				write(output, "\n", strlen("\n"));

			}

		}
	}
    /* After going through all the entries, close the directory. */
	if (closedir (d)) {
		fprintf (stderr, "Could not close '%s': %s\n",
			dir_name, strerror (errno));
		exit (EXIT_FAILURE);
	}
	
}

int main (int argc, char* argv[])
{
	char* filepath = "./files.txt";
	int file = open(filepath, O_CREAT | O_RDWR | O_TRUNC, 0666);

	if(file == -1) {
		perror(strerror(errno));
		perror("Redirecting to console...\n");
		file = STDOUT_FILENO;
	}

	list_dir (argv[1],file);
	return 0;
}