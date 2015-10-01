//Essa função limpa todos os arquivos usados pelo programa
void deleteFiles(){
	remove("files/users.txt");		
}

//Essa função verifica se o usename é válido
int checkUserName(char *userName){
							
	FILE *users;
	char *line;
	char *token;

	line = (char *) calloc (150,sizeof(char));
	token = (char *) calloc (150,sizeof(char));

	users = fopen("files/users.txt","r");
	
	if(!users){
		return 0;
	}

	while(feof(users) == 0){
		fscanf(users,"%s", line);
		token = strtok(line,"@");	
			
		if(strcmp(token, userName) == 0){
			fclose(users);
			return 1;
		}
	}
	
	fclose(users);
	return 0;
}

//Insere um usuário da lista de usuários
void insertUser(char *userName, char *address){
	FILE *users;
	char *putInFile;
	
	putInFile = (char *) calloc (150,sizeof(char));
	
	//Tenta abrir o arquivo de usuários, se nao existir(situação onde não existem usuários ativos) o arquivo é criado
	users = fopen("files/users.txt","a+");
	
	if(!users)
			errorOpenFile();			
			
	//Insere o nome do usuário na lista de usuários ativos
	strcat(putInFile,userName);
	strcat(putInFile,"@");
	strcat(putInFile, address);
	strcat(putInFile,"\n");
	fputs(putInFile, users);
						
	fclose(users);
	
	printf("Usuário conectado %s\n", putInFile);
	fflush(stdout);
}


//Remove um usuário da lista de usuários
void removeUser(char *userName){
	printf("Função removeUser\n");
	fflush(stdout);
	int control = 0;
	
	FILE *users;
	FILE *_users;

	char *line;
	char *_line;
	char *token;
	char *_token;

	line =(char *) calloc (150,sizeof(char));
	_line =(char *) calloc (150,sizeof(char));
	token =(char *) calloc (50,sizeof(char));
	_token =(char *) calloc (50,sizeof(char));

	users = fopen("files/users.txt","r+");
		if(!users)
			errorOpenFile();
	
	_users = fopen("files/usersTemp.txt","w+");
		if(!_users)
			errorOpenFile();
	
	while(feof(users) == 0){
		fscanf(users,"%s", line);
		strcpy(_line, line);
		token = strtok(_line,"@");	
		
		printf("Nome testado %s\n", token);
		fflush(stdout);
		
		//Isso corrige um bug implementado por satanás, não sei porque ele lê o ultimo nome do arquivo duas vezes
		if(strcmp(_token,token) != 0){
			if(strcmp(token,userName) != 0){
				strcat(line,"\n");
				fputs(line,_users);
				control = 1;
			}
		}
		
		strcpy(_token, token);
	}
	
	fclose(users);
	fclose(_users);
	
	//Verifica se existe pelo menos um usuário, se não os arquivos de usuários serão apagados
	if(control){
		remove("files/users.txt");
		rename("files/usersTemp.txt","files/users.txt");
	}else{
		remove("files/users.txt");
		remove("files/usersTemp.txt");
	}
	
	printf("Usuário desconectado : %s\n", userName);
	fflush(stdout);
}

//Essa função retorna uma lista de usuários ativos
struct userList checkActiveUsers(){
	FILE *users;
	char *line;
	char *token;
	
	line = (char *) calloc (150,sizeof(char));
	token = (char *) calloc(50,sizeof(char));

	//Lista de usuarios ativos
	int i = 0;
	struct userList _users;

	_users.size = i;
	users = fopen("files/users.txt","r");
	
	while(feof(users) == 0){
		fscanf(users,"%s", line);
		token = strtok(line,"@");	
		
		strcpy(_users.name[i], token);
		_users.size = i;
		i++;
	}
	
	fclose(users);
	return _users;
}

void putMessageChatInLog(char *userName, char *messageChat){
	int i;
	char *fileName;
	FILE *logUser;
	
	fileName = (char *) calloc(50,sizeof(char));
	
	struct userList _users = checkActiveUsers();
	
	for(i=0;i<_users.size;i++){
			
	printf("Escrevendo mensagem no log\n");
	fflush(stdout);
		
		if(strcmp(_users.name[i],userName) != 0){
			
			strcpy(fileName,"files/");
			strcat(fileName,_users.name[i]);
			strcat(fileName,".txt");
				
			printf("Nome do arquivo: %s\n", fileName);
			fflush(stdout);
			
			logUser = fopen(fileName, "a+");
			if(!logUser)	
				printf("Erro ao criar arquivo\n");
				fflush(stdout);
			
			fputs(messageChat, logUser);
			fclose(logUser);
		}
		
	}	
}

struct messageList checkLog(char *userName){
	FILE *logUser;
	char *fileName;
	char *line;
	struct messageList messages;
	
	int i = 0;
	messages.size = 0;
	
	fileName = (char *) calloc(50,sizeof(char));
	line = (char *) calloc (150,sizeof(char));
	
	strcpy(fileName,"files/");
	strcat(fileName,userName);
	strcat(fileName,".txt");
	
	logUser = fopen(fileName, "r");
	
	if(!logUser){
		return messages;
	}
	
	while(feof(logUser) == 0){
		i++;
		fscanf(logUser,"%s", line);
	
		strcpy(messages.content[i], line);
		messages.size = i;
	}
	
	fclose(logUser);
	remove(fileName);
	
	return messages;
}


