#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#define WHITESPACE " \t\n"      // We want to split our command line up into tokens
                                // so we need to define what delimits our tokens.
                                // In this case  white space
                                // will separate the tokens on our command line

#define MAX_COMMAND_SIZE 255    // The maximum command-line size

#define MAX_NUM_ARGUMENTS 5     // Mav shell only supports four arguments

char hist_cmd[15][100];
int hist_index = 0;
int hist_pid[15];
int hist_pid_index = 0;

int main()
{

  char * command_string = (char*) malloc( MAX_COMMAND_SIZE );

  while( 1 )
  {
    // Print out the msh prompt
    printf ("msh> ");

    // Read the command from the commandline.  The
    // maximum command that will be read is MAX_COMMAND_SIZE
    // This while command will wait here until the user
    // inputs something since fgets returns NULL when there
    // is no input
    while( !fgets (command_string, MAX_COMMAND_SIZE, stdin) );

    /* Parse input */
    char *token[MAX_NUM_ARGUMENTS];

    for( int i = 0; i < MAX_NUM_ARGUMENTS; i++ )
    {
      token[i] = NULL;
    }

    int   token_count = 0;         
    
    if(command_string[0] == '!')
    {
      int rep_cmd = atoi(&command_string[1]);
      if(rep_cmd>hist_index)
      {
        printf("Command not in history.\n");
        continue;
      }else
      {
        strcpy(command_string, hist_cmd[rep_cmd]);
      }
    }                        
                                                           
    // Pointer to point to the token
    // parsed by strsep
    char *argument_ptr = NULL;                                         
                                                           
    char *working_string  = strdup( command_string );                

    // we are going to move the working_string pointer so
    // keep track of its original value so we can deallocate
    // the correct amount at the end
    char *head_ptr = working_string;

    // Tokenize the input strings with whitespace used as the delimiter
    while ( ( (argument_ptr = strsep(&working_string, WHITESPACE ) ) != NULL) && 
              (token_count<MAX_NUM_ARGUMENTS))
    {
      token[token_count] = strndup( argument_ptr, MAX_COMMAND_SIZE );
      if( strlen( token[token_count] ) == 0 )
      {
        token[token_count] = NULL;
      }
        token_count++;
    }

    // Now print the tokenized input as a debug check
    // \TODO Remove this for loop and replace with your shell functionality

    //int token_index  = 0;
    //for( token_index = 0; token_index < token_count; token_index ++ ) 
    //{
    //  printf("token[%d] = %s\n", token_index, token[token_index] );  
    //}
    
    if(token[0] == NULL)
    {
      continue;
    }

    if(strcmp(token[0], "quit")==0 || strcmp(token[0], "exit")==0)
    {
      exit(0);
    }

    if(token[0]!= NULL)
    {
      if(hist_index>14)
      {
        for(int i = 0; i <14; i++)
        {
          strcpy(hist_cmd[i], hist_cmd[i+1]);
        }
      hist_index = 14;
      }
    strcpy(&hist_cmd[hist_index][0], token[0]);
    if(token[1] != NULL)
    {
      strcat(&hist_cmd[hist_index][0], " ");
      strcat(&hist_cmd[hist_index][0], token[1]);
    }
    hist_index++;
    }
    
    if(strcmp(token[0], "cd")==0)
      {
        chdir(token[1]);
        continue;
      }
    
    
    
    pid_t child = fork();
    int status;

    if(hist_pid_index>14)
    {
      for(int i = 0; i<14; i++)
      {
        hist_pid[i] = hist_pid[i+1];
      }
    hist_pid_index = 14;
    }else
    {
      hist_pid[hist_pid_index] = getpid();
      hist_pid_index++;
    }
    

    if(child == 0)
    {
      if(strcmp(token[0], "showpids")==0)
      {
        for(int i = 0; i < hist_pid_index; i++)
        {
          printf("[%d] = %d\n", i, hist_pid[i]);
        }
        _exit(0);
      }
      if(strcmp(token[0], "history")==0 && token[1] == NULL)
      {
        for(int i = 0; i<hist_index; i++)
          {
            printf("[%d] = ", i);
            //puts(hist_cmd[i]);
            printf("%s ", hist_cmd[i]);
            printf("\n");
          }
        _exit(0);
      }
      if(strcmp(token[0], "history")==0 && strcmp(token[1], "-p") == 0)
      {
          for(int i = 0; i<hist_index; i++)
          {
            printf("[%d] = ", i);
            //puts(hist_cmd[i]);
            printf("%s ", hist_cmd[i]);
            printf("-> %d\n", hist_pid[i]);
          }
          exit(0);
      }
      
     // printf("[DEBUG]: In Child\n");

      if(strcmp(token[0], "ls")==0)
      {
        execvp("ls", token);
      }
      
      if(strcmp(token[0], "mkdir")==0)
      {
        execvp("mkdir", token);
      }
      if(strcmp(token[0], "rm")==0)
      {
        execvp("rm", token);
      }
      if(strcmp(token[0], "rmdir")==0)
      {
        execvp("rmdir", token);
      }
      if(strcmp(token[0], "cp")==0)
      {
        execvp("cp", token);
      }
      if(strcmp(token[0], "quit")==0 || strcmp(token[0], "exit")==0)
      {
        exit(0);
      }
      if(execvp(token[0],token) != -1)
      {
        execvp(token[0], token);
      }
      if(execvp(token[0], token) == -1 && strcmp(token[0], "cd") && strcmp(token[0],"history"))
      {
        printf("%s: Command not found.\n", token[0]);
        exit(0);
      }
    }
    else
    {
      waitpid(child, &status, 0);
      
      //printf("[DEBUG]: In Parent\n");
    }
    // Cleanup allocated memory
    for( int i = 0; i < MAX_NUM_ARGUMENTS; i++ )
    {
      if( token[i] != NULL )
      {
        free( token[i] );
      }
    }

    free( head_ptr );

  }

  free( command_string );

  return 0;
  // e2520ca2-76f3-90d6-0242ac120003
}