#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"
#include "parson.h" // pentru parsarea JSON
#include "help.h" //functii auxiliare folosite

int main(int argc, char *argv[])
{
   
    char *command = (char*) malloc(sizeof(char)*50);
    char **cookie_login =  malloc(2 * sizeof(char*));
  
    char* token_jwt = malloc(1000*sizeof(char));
    char* host = "ec2-3-8-116-10.eu-west-2.compute.amazonaws.com";

    while(1) {

        scanf("%s",command);
        if(strncmp(command,"exit",4) == 0  && (strlen(command) == 4)) { //exit
            return 0;
        }

        if((strncmp(command,"register",8)==0)  && (strlen(command) == 8)) {
            register_helper(command);

        }
        else if((strncmp(command,"login",5)==0)  && (strlen(command) == 5)) {
            cookie_login[0] = login_helper(command);
        
        }
        else if((strncmp(command,"enter_library",13) == 0)  && (strlen(command) == 13)) { 
            char* url = "/api/v1/tema/library/access";
            token_jwt = library_helper(url,cookie_login);
        

        } 
        else if((strncmp(command,"get_books",9)== 0) && (strlen(command) == 9)) {
            char *message;
            char *response;
            int sockfd;
        
            sockfd = open_connection("3.8.116.10", 8080, AF_INET, SOCK_STREAM, 0);
            message = compute_get_request(host, "/api/v1/tema/library/books", NULL, NULL, 0,token_jwt,0); 

            //puts(message);
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);
            //puts(response);
        
            display(response);
        
        } 
        else if(strncmp(command,"get_book",8) == 0  && (strlen(command) == 8)) {
            char *message;
            char *response;
            int sockfd;
            
            char *id = (char*) malloc(sizeof(char)*30);
            printf("id=");
            scanf("%s",id);
            printf("Id entered: %s\n",id);
        
            sockfd = open_connection("3.8.116.10", 8080, AF_INET, SOCK_STREAM, 0);
            
            char* url_tosend = (char*) malloc(sizeof(char)*100);
            strcat(url_tosend,"/api/v1/tema/library/books/");
            strcat(url_tosend,id);
        

            message = compute_get_request(host, url_tosend, NULL, NULL, 0,token_jwt,0); 
            //puts(message);
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);
            //puts(response);
        
            display(response);

        
        } 
        else if((strncmp(command,"add_book",8) == 0)  && (strlen(command) == 8)){
            add_helper(token_jwt);
        
        

        } 
        else if(strncmp(command,"delete_book",11) == 0  && (strlen(command) == 11)) { 
            char *message;
            char *response;
            int sockfd;
            
            char *id_delete = (char*) malloc(sizeof(char)*30);
            printf("id=");
            scanf("%s",id_delete);
            printf("Id entered: %s\n",id_delete);
    
            sockfd = open_connection("3.8.116.10", 8080, AF_INET, SOCK_STREAM, 0); 

            char* url_tosend = (char*) malloc(sizeof(char)*100);
            strcat(url_tosend,"/api/v1/tema/library/books/");
            strcat(url_tosend,id_delete);
        


            message = compute_get_request(host, url_tosend, NULL, NULL, 0,token_jwt,1); 
            //puts(message);
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);
            //puts(response);
        
            display(response);

        
        } 
        else if((strncmp(command,"logout",6) == 0)  && (strlen(command) == 6)){ 
            char *message;
            char *response;
            int sockfd;
    
            sockfd = open_connection("3.8.116.10", 8080, AF_INET, SOCK_STREAM, 0);
            message = compute_get_request(host, "/api/v1/tema/auth/logout", NULL, cookie_login, 1,NULL,0); 
            //puts(message);
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);
        
            //puts(response);
        
            display(response);
        
        } 
        else
        {
            printf("Please enter a valid command.\n");
        } 
      
    }
   
   free(token_jwt);
   free(cookie_login);
   free(host);
   free(command);

    return 0;
}