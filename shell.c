#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>

int flg = 1;
void prompt() {
	char s[300];
	if (flg) {
		const char *CLEAR_SCREEN = "\e[0;0H\e[2J";
		write(STDOUT_FILENO, CLEAR_SCREEN, 12);
		printf("\t\t\t\t\t\t  Welcome to My Simple shell v1.0:~\n\n\t\t\t\t\t\tCREATED BY :- RATNADEEP BHATTACHARYA\n\n\t\t\t\t\t\tNOTE:-\n\t\t\t\t\t\t1. Use 'clear' to clear screen.\n\t\t\t\t\t\t2. Here '/' does not mean root.\n\t\t\t\t\t\t3. Use '/' before specifying a path for 'cd' command.\n\t\t\t\t\t\tEnjoy......\n\n");
		flg = 0;
	}
	char disp[] = "\rUser@user-Ubuntu:~";
	strcat(disp, getcwd(s, 300));
	strcat(disp, "~$");
	printf("%s", disp);
}


void read_cmd(char cmd[], char *par[]) {
	char line[1024];
	memset(line, '\0', sizeof(line));
	int cnt = 0, i = 0, j = 0;
	char *arr[100], *pch;
	int ch;
	do {	//reading input from prompt(stdin).
		ch = fgetc(stdin);
		line[cnt++] = (char)ch;
	}while (ch != '\n');

	if (cnt == 1)	return;

	pch = strtok(line, " \n");//breaking the input into tokens.
	while (pch != NULL) {
		arr[i++] = strdup(pch);
		pch = strtok(NULL, " \n");
	}
	strcpy(cmd, arr[0]);//copying the command in cmd.

	for (int j = 0; j < i; j++) {	//copying all the parameters in par[].
		par[j] = arr[j];
		par[i] = NULL;
	}
}
// cd command.
int cd1(char path[]) {
	char s[300], curr_path[300];
	strcpy(curr_path, getcwd(s, 300));
	if (!strcmp(path, "..")) {
		chdir("..");
		return 1;
	}
	else if (!strcmp(path, ".")) return 1;
	else {
		strcat(curr_path, path);
		if (chdir(curr_path) == -1) {
			printf("Unknown Path....\n");
			return 1;
		}
		return 1;
	}
	return 0;
}
int main() {
	char cmd[100], command[100], *param[50];
	char* const env[] = {(char *) "PATH=/bin", 0};
	pid_t parpid = getpid();	//storing the parent pid for later use.
	
	while (1) {
		prompt();//Display the prompt.
		read_cmd(command, param);//Read data from the prompt and format it properly.
		
		if (!strcmp(command, "cd")) {
			if (!cd1(param[1])) {
				printf("Some error occurred....\n");
			}
			continue;
		}

		if (fork() != 0) {	//Creating a child process.
			wait(NULL);//Parent waits while child runs.
		}
		else {
			//child runs the following.
			strcpy(cmd, "/bin/");
			strcat(cmd, command);
			if (execve(cmd, param, env) == -1 && strcmp(command, "exit") && strcmp(command, "clear")) {
				printf("Unknown Command\n execution failed....\n");
				kill(getpid(), SIGKILL);

			}
		}
		
		if (getpid() != parpid)	{ //If child process then abort.
			kill(getpid(), SIGKILL);
		}
		
		if (!strcmp(command, "exit")) break;
		if (!strcmp(command, "clear"))	flg = 1;//setting the flg one if command is clear so that screen
		//gets cleaned next time when prompt() executes..
	}
	
	return 0; 
}