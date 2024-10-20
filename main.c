#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h> 
#include <pthread.h>
#include <fcntl.h>
#define MAX_LINE 100
char *history; 
struct thread_node{//como temos inputs infinitos, struct thread_node é criada como a lista dos pthread_t
  pthread_t valor;
  struct thread_node *next;
};
typedef struct Node {
    char *data;
    struct Node *next;
} Node;

//comando de tamanho máximo para uma instrução
pid_t pid;
pid_t child(pid){
  if (pid == 0){
  return 1;
  }else{
    return 0;
  }
}
pid_t father(pid){
  if (pid>0){
    return 1;
  }else{
    return 0;
  }
}
int hasotherthingy (char *string){
  for (int i = 0;string[i]!='\0';i++) {
    //tem q usar diferente de \0 pq com strlen vc estaria assumindo que está terminado com \0 q pode nao ser o caso com getline
     if(string[i]=='>'){
       return 1;
     }
    
}return 0; 
}
int hasdoublegreater(char *string) {
    for (int i = 0; string[i] != '\0'; i++) {
        if (string[i] == '>' && string[i + 1] == '>') {
            return 1;
        }
    }
    return 0;
}


void splitStringByRedirect(const char *input, char **string1, char **string2) {
    char *token;
    char *inputCopy = strdup(input);
    token = strtok(inputCopy, ">");  
    *string1 = strdup(token);
    token = strtok(NULL, ">");
    if (token != NULL) {
      *string2 = strdup(token);
    }else{
      *string2 = NULL;
    }

    free(inputCopy);
}
void splitStringByPipe(const char *input, char **string1, char **string2) {
    char *token;
    char *inputCopy = strdup(input);
    token = strtok(inputCopy, "|");  
    *string1 = strdup(token);
    token = strtok(NULL, "|");
    if (token != NULL) {
      *string2 = strdup(token);
    }else{
      *string2 = NULL;
    }

    free(inputCopy);
}
void splitStringByDoubleRedirect(const char *input, char **string1, char **string2) {
    char *token;
    char *inputCopy = strdup(input);
    char *doubleRedirect = strstr(inputCopy, ">>");

    if (doubleRedirect != NULL) {
        *doubleRedirect = '\0';  
        *string1 = strdup(inputCopy);
        *string2 = strdup(doubleRedirect + 2); 
    } else {
        *string1 = strdup(inputCopy);
        *string2 = NULL;
    }

    free(inputCopy);
}
char* formatString(char *string) {
  int j=0;
  for (int i = 0;string[i]!='\0';i++) {
    if (string[i]!= ' ' &&string[i]!='\n') {//tem q usar diferente de \0 pq com strlen vc estaria assumindo que está terminado com \0 q pode nao ser o caso com getline
      string[j] = string[i];
      j++;
    }
  }
  string[j] = '\0';
  return(strdup(string));
}
void insertAtBeginning(struct thread_node ** head, pthread_t newData) {
    struct thread_node* newNode = (struct thread_node*)malloc(sizeof(struct thread_node));
    if (newNode == NULL) {
        printf("Erro: Falha na alocação de memória.\n");
        exit(1); //encerra o programa no caso de erro na alocacao
    }
    newNode->valor = newData;
    newNode->next = *head;
    *head = newNode;
}
void *functionforthread(void* arg) {
    char* command = (char*)arg;
    system(command);   
    pthread_exit(NULL);//assegura que a thread vai ser acbada, e que a memória que utilizou será desalocada. pode retornar um status para o thread que o espera. pode ter em seus argumentos um ponteiro para o que desejas que retorne
   
}

int hasthingy (char *string){
  for (int i = 0;string[i]!='\0';i++) {
    //tem q usar diferente de \0 pq com strlen vc estaria assumindo que está terminado com \0 q pode nao ser o caso com getline
     if(string[i]=='|'){
       return 1;
     }
    
}return 0; 
}
int executeSeq (char comando[MAX_LINE]){//função que divide uma grande string em várias substrings, e executa cada uma. vc pode usar returns dessa funcao para tratamentos de erro onde tem esse exit(1) implementar break para se o token for igual a exit 
  //usamos o comando strtok para separar a string
          pid = fork();
          if(pid == 0){//CHILD PROCESS
                        char *string = strtok(comando,";");
                        while(string!=NULL && strcmp(string,"exit\n")!=0){
                          if(hasthingy(string) && !hasotherthingy(string)){//|
                          printf("iniciating pipe operation\n");
                          fflush(stdout);
                              char *token = strtok(string, "|");
                              int fileDescriptors[2];
                              if (pipe(fileDescriptors) == -1) {//faço o pipe com o array
                                      printf("Error in pipe function\n");
                                      exit(1);
                                  }
                              int pidpid;
                              for (int i = 0; token!=NULL; i++) {
                                  char command[MAX_LINE];//crio comando, string de 100 char
                                  strcpy(command, token);//recebe token
                                  
                                  pidpid = fork();//fork no pid 
                                  if (pidpid < 0) {//se der errado, sai do programa
                                      printf("Error when forking process\n");
                                      exit(1);
                                  }
                                  if (pidpid == 0) {//se for filho
                                      if(i == 0){//WRITE se i = 0(primeira iteracao é pra escrever)
                                          fflush(stdout);
                                          dup2(fileDescriptors[1], STDOUT_FILENO);
                                          close(fileDescriptors[0]);
                                          close(fileDescriptors[1]);
                                          execlp("/bin/sh", "sh", "-c", command, NULL);
                                          
                                          
                                      }else if (i==1){
                                          dup2(fileDescriptors[0], STDIN_FILENO);
                                          close(fileDescriptors[0]);
                                          close(fileDescriptors[1]);
                                          fflush(stdout);
                                          execlp("/bin/sh", "sh", "-c", command, NULL);
                                          fflush(stdout);
       
                                      }
                                      fflush(stdout);
                                      printf("Error when executing process\n");
                                      fflush(stdout);
                                  
                                  }

                  
          

                                  
                                  fflush(stdout);
                                  token = strtok(NULL, "|");
                                  
                                  
                                  
                                
                              }
                              close(fileDescriptors[0]);//why close for the third time? Answer: liberar a memória do pai
                              close(fileDescriptors[1]);
                              wait(NULL); 
                              wait(NULL);  
                              
                              //espera o filho acaba
                              
                          }
                          else if (!hasotherthingy(string)&&!hasthingy(string)){        
                            fflush(stdout) ;          
                            pid_t pidid = fork();
                            size_t length = strlen(string);
                            if (length > 0 && string[length - 1]=='\n') {
                              string[length - 1] = '\0';                             
                          }
                          if(pidid ==0 ){//depois fazer uso do child
                            if(execlp("/bin/sh","sh","-c",string,NULL)==-1)//sempre vai rodar ent isso é redundante.
                              exit(1);
                            
                          }else{
                            wait(NULL);
                          }
                          }
                          else if((hasotherthingy(string))&&!hasthingy(string))
                          {
                            printf("iniciating redirect.\n");
                            char *whatToDo;
                            char *whereToGo;
                            splitStringByRedirect(string,&whatToDo,&whereToGo);
                            formatString(whereToGo);//inserido aqui para não diferenciar "oi.txt" de " oi.txt "
                            pid_t pidid = fork();
                           
                                
                            if(pidid ==0 ){//depois fazer uso da funcao child
                             FILE *file;
                              if(!hasdoublegreater(string)){
                              fflush(stdout);
                              file = fopen(whereToGo,"w");
                              }
                              else if(hasdoublegreater(string)){
                              fflush(stdout);
                              file = fopen(whereToGo,"a");
                              }
                              int fd = fileno(file);
                              int file2 = dup2(fd,STDOUT_FILENO);
                              fclose(file);
                              execlp("/bin/sh","sh","-c",whatToDo,NULL);
                              free(file);
                            }
                            else{
                            wait(NULL);
                          }                          
                          } //executavel do shell, nome do seu executavel ( o proprio shell) , comando a ser executado, em seguida o que vc quer desejar e null que indica q acabou os args 
                          string=strtok(NULL,";");
                          
          }exit(1);
          }else if (pid>0){//pai espera o filho acabar
            wait(NULL);   
          }else{
            printf("Error when forking.\n");
          }
         
           
  //}
  return 1;
  //
}


void executePar(char *comando) {
    struct thread_node *percorrer = NULL;
    struct thread_node *join = NULL;
    struct thread_node *head = NULL;
    int contadorDeThread = 0;
    char *string = strtok(comando, ";");
    while (string != NULL) {
      
      //formatString(string);
       
        pthread_t thread;

        if (pthread_create(&thread, NULL, functionforthread, string) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
        insertAtBeginning(&head, thread);

        contadorDeThread++;
        string = strtok(NULL, ";");
    }
  

    join = head;

    for (int i = 0;i<contadorDeThread;i++) {
        if (pthread_join(join->valor, NULL) != 0) {
            perror("pthread_join");
            exit(EXIT_FAILURE);
        }
        join = join->next;
    }
  
    percorrer = head;
    while (percorrer!=NULL) {
        struct thread_node *temp=percorrer;
        percorrer =percorrer->next;
        free(temp);
    }
//método para evitar falta de sincronização nas threasd MUTEX (mutual exclusion)
//assegura que cada thread pode acessar 
    //pthread_mutex_destroy(&mutex);

}

int main(int argc, char *argv[]) {
    char style[20] = "seq";
    if (argc <= 1) {
        size_t buffer_size = MAX_LINE;
        char *comando = malloc(sizeof(char) * buffer_size);   
        while(1){
            printf("capvf %s>",style);
            getline(&comando,&buffer_size,stdin);
            if (strcmp(comando,"exit\n")==0) {
              printf("Exiting...\n");
              free(comando);
              free(history);
              exit(0);
            }else if(feof(stdin)){
              printf("CTRL D input detected. Exiting...\n");
              exit(0);
            }else if (strcmp(comando,"!!\n") == 0) {
                if(history!= NULL){
                  printf("History:%s", history);
                  if(strcmp(style,"seq")==0){
                    executeSeq(history);
                  }else{
                    executePar(history);
                  }
                }else{
                    printf("No commands\n");
                }
            }else if(strcmp(comando,"style parallel\n")==0){
              strcpy(style,"par");
            }else if (strcmp(comando,"style sequential\n")==0) {
              strcpy(style,"seq");
            }else{
              if(history != NULL){
                free(history);
              }
                 history = strdup(comando);               
                if(strcmp(style,"seq")==0) {
                  executeSeq(comando);             
                }else if(strcmp(style,"par")==0) {
                  executePar(comando);     
                }
            }
            }
    }else if(argc == 2) {
        FILE *file = fopen(argv[1], "r");
        char styles[5] = "seq";      
        if (file != NULL) {
            char *line = NULL;
            size_t line_length = 0; 
            pid_t finalpid = fork();   
            if (finalpid ==0){
           while (getline(&line, &line_length, file) != -1) {
        char *token = strtok(line, ";");
        while (token != NULL) {
        //printf("line: %s\n", token);
        //printf("HERE I AM\n");
        fflush(stdout);
        pid_t finalfinalpid = fork();
        if (finalfinalpid == 0) {
          if (!hasotherthingy(token) && !hasthingy(token)){

            if(strcmp(style,"seq")==0 ){
            //printf("olha eu aqui\n");
            execlp("/bin/sh", "sh", "-c", token, NULL);
            perror("execlp");
            exit(1);
            }
          }else if(hasotherthingy(token) && !hasthingy(token)){
            FILE *file;
            printf("iniciating redirect.\n");
                            char *whatToDo;
                            char *whereToGo;
                            splitStringByRedirect(token,&whatToDo,&whereToGo);
                            if(hasdoublegreater(token)){
                              file = fopen(whereToGo,"a");
                            }else{
                              file =fopen(whereToGo,"w");
                            }
                              int file2 = dup2(fileno(file),STDOUT_FILENO);
                              fclose(file);
                              execlp("/bin/sh","sh","-c",whatToDo,NULL);
                              free(file);
                              
          }
          else if(hasthingy(token)&& !hasotherthingy(token)){//se tem | e n tem
                              printf("iniciating pipe operation");
                              fflush(stdout);
                              char *whatToDo;
                              char *whereToGo;
                              splitStringByPipe(token,&whatToDo,&whereToGo);
                              //printf("%s %s", whatToDo, whereToGo);
                              int fileDescriptors[2];
                             
                              if (pipe(fileDescriptors) == -1){
                                printf("pipe operation unsuccessful");
                                fflush(stdout);
                                exit(1);
                              }
                              int pidWrite = fork(); 
                              if(pidWrite==0){
                                printf("\nNOW EXECUTING: %s in write operation\n",whatToDo);
                                fflush(stdout);
                                dup2(fileDescriptors[1],STDOUT_FILENO);
                                close(fileDescriptors[1]);
                                close(fileDescriptors[0]);
                                execlp("/bin/sh","sh","-c",whatToDo,NULL);
                                
                              }
                              int pidRead = fork();
                              if(pidRead==0){
                                printf("NOW EXECUTING: %s in read operation\n",whereToGo);
                                fflush(stdout);
                                dup2(fileDescriptors[0],STDIN_FILENO);
                                close(fileDescriptors[1]);
                                close(fileDescriptors[0]);
                                execlp("/bin/sh","sh","-c",whereToGo,NULL);
                                
                              }
                              close(fileDescriptors[1]);
                              close(fileDescriptors[0]);
                              waitpid(pidWrite,NULL,0);
                              waitpid(pidRead,NULL,0);
                              
                                
                              

                            }
            
          
           
        } else if (finalfinalpid < 0) {
            perror("fork"); 
            exit(1);
        
        } else {
            wait(NULL);
        }
        token = strtok(NULL, ";");
    }
}
            
            }else{
                      wait(NULL);
                     }
                       free(line); 
                       fclose(file);
                    
                
            
        } else {
            printf("Error: File does not exist or is corrupted.\n");
        }
    } else {
        printf("Too many commands: select only one file.\n");
    }

    return 0;
}