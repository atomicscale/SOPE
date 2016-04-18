#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>

int is_older(char* path1, char path2){
	//conseguesse ver facilmente qual é o mais velho se tiver o path para depois fazer hardlink para o mais antigo

	return 0;
}



int same_files(char* file1, char* file2){

	char tmp1[999];
	char tmp2[999];
 	char permissions[20];
	char name[20];
	char size[20];
	char date[20];
	char permissions2[20];
	char name2[20];
	char size2[20];
	char date2[20];
	char* p;

	//file1
	strcpy(tmp1,file1);
	p = strstr(tmp1, "  ");
	p=p+2;
	strcpy(tmp1,p);
	p = strstr(tmp1, "  ");
	*p=0;
	strcpy(permissions,tmp1);
	printf("%s\n",permissions); //debug
	*p=' ';
	p = strstr(tmp1, "  ");
	p = p+2;
	strcpy(tmp1,p);
	p = strstr(tmp1, "  ");
	memcpy(size,p, 12);
	printf("%s\n",size);//debug
	p = p+12;
	strcpy(tmp1,p);
	p = strstr(tmp1, "  ");
	*p=0;
	strcpy(date,tmp1);
	printf("%s\n",date);//debug
	*p=' ';
	p = strstr(tmp1, "  ");
	strcpy(tmp1,p);
	p = p+2;
	strcpy(name,p);
	printf("%s\n",name);//debug
	
	//file2
	strcpy(tmp2,file2);
	p = strstr(tmp2, "  ");
	p=p+2;
	strcpy(tmp2,p);
	p = strstr(tmp2, "  ");
	*p=0;
	strcpy(permissions2,tmp2);
	printf("%s\n",permissions2);//debug
	*p=' ';
	p = strstr(tmp2, "  ");
	p = p+2;
	strcpy(tmp2,p);
	p = strstr(tmp2, "  ");
	memcpy(size2,p, 12);
	printf("%s\n",size2);//debug
	p = p+12;
	strcpy(tmp2,p);
	p = strstr(tmp2, "  ");
	*p=0;
	strcpy(date2,tmp2);
	printf("%s\n",date2);//debug
	*p=' ';
	p = strstr(tmp2, "  ");
	strcpy(tmp2,p);
	p = p+2;
	strcpy(name2,p);
	printf("%s\n",name2);//debug
	
	//se puseres a fazer print do path no ficheiro files.txt depois é só guardar para um array de char em baixo fazer o hardlink ( se fizeres isso cuidado em baixo nos for's que tem condiçoes para garantir que compara só linhas de ficheiros e nao a parte do path relativo
	if (strcmp(name, name2) == 0 && strcmp(size, size2) == 0
					&& strcmp(permissions, permissions2) == 0){//pra ver o conteudo se é igual tbm é preciso o path é usar esta funçao execlp("diff","diff",path1,path2,NULL)
	// unlink(path2);
				//link(path,path2);
				// FILE* links = fopen("hlinks.txt", "a");
				      //fprintf(links, "%s now is linking to %s\n", path2, path);
		
		      //fclose(links);
	printf("%s\n","iguais");//debug
	return 0;}
	return -1;
	
}



int main (int argc, char* argv[]){
	pid_t pid;
	FILE * file;

	char tmp[999][999];
	

	pid = fork();

	if (pid == 0) {
		execlp("./lsdir", "lsdir", argv[1], NULL);

	}
	wait(NULL);
	int i = 0;
	int k,j;
	file = fopen("files.txt", "r");

	if (file) {
		while (fgets(tmp[i], 999, file) != NULL) {
			i++;
		}
		fclose(file);
	}

	for(k = 0; k < i ; k++){
		if(tmp[k] == NULL || tmp[k][0] == '\n' || tmp[k][0] == ' ' )
			continue;
		if(strchr(tmp[k],'/') != NULL)
			continue;
		for(j = k+1; j < i; j++){
		if(tmp[j] == NULL  || tmp[j][0] == '\n' || tmp[j][0] == ' ')
			continue;
		if(strchr(tmp[j],'/' )!= NULL)
			continue;

		same_files(tmp[k], tmp[j]);
}}
	return 0;
	
}





