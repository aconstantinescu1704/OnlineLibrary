#include "parson.h"
void display(char *response) {
    if((basic_extract_json_response(response)) != NULL) {
        puts(basic_extract_json_response(response));
    } else {
        printf("Command successful.\n");
    }


}

char* library_helper(char *url,char **cookie) {
    char *message;
    char *response;
    int sockfd;
    char* host = "ec2-3-8-116-10.eu-west-2.compute.amazonaws.com";
    char* token_jwt = malloc(1000*sizeof(char));
    sockfd = open_connection("3.8.116.10", 8080, AF_INET, SOCK_STREAM, 0);
    message = compute_get_request(host, url, NULL, cookie ,1,NULL,0); 
    //puts(message);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
     
    //puts(response);
    display(response);

    char* ret;
    ret = strstr(response,":\""); 
            
    token_jwt = strtok(ret,"}");
    token_jwt = token_jwt + 2;
    token_jwt[strlen(token_jwt)-1] = '\0';
      
    return token_jwt;

} 

void register_helper(char *command) {
    char *message;
    char *response;
    int sockfd;
   
    char* host = "ec2-3-8-116-10.eu-west-2.compute.amazonaws.com";
    char *username = (char*) malloc(sizeof(char)*100);
    char *password = (char*) malloc(sizeof(char)*100);
    sockfd = open_connection("3.8.116.10", 8080, AF_INET, SOCK_STREAM, 0);

    if(strncmp(command,"register",8) == 0) {
        printf("username=");;
        scanf("%s",username);
        printf("Username entered: %s\n",username);
        printf("password=");
        scanf("%s",password);
        printf("Password entered: %s\n",password);

        JSON_Value *root_value = json_value_init_object();
        JSON_Object *root_object = json_value_get_object(root_value);
        char *register_string = malloc(sizeof(char)*500);

        json_object_set_string(root_object, "username", username);
        json_object_set_string(root_object, "password", password);
        register_string = json_serialize_to_string_pretty(root_value);
     

        char **tosend = malloc(2 * sizeof(char*));
        tosend[0] = malloc(500*sizeof(char));
        strcpy(tosend[0],register_string);
           
           
        message = compute_post_request(host, "/api/v1/tema/auth/register", NULL, tosend, 1, NULL, 0,NULL);
            
        //puts(message);
        send_to_server(sockfd, message);
        response = receive_from_server(sockfd);
        //puts(response);
    
        display(response);
          
         
                
        }
}
char* login_helper(char *command) {
    char *message;
    char *response;
    int sockfd;
    char* host = "ec2-3-8-116-10.eu-west-2.compute.amazonaws.com";
    sockfd = open_connection("3.8.116.10", 8080, AF_INET, SOCK_STREAM, 0);

    char *username = (char*) malloc(sizeof(char)*100);
    char *password = (char*) malloc(sizeof(char)*100);


    if(strncmp(command,"login",5) == 0) {
        printf("username=");
        scanf("%s",username);
        printf("Username entered: %s\n",username);
        printf("password=");
        scanf("%s",password);
        printf("Password entered: %s\n",password);

        JSON_Value *root_value = json_value_init_object();
        JSON_Object *root_object = json_value_get_object(root_value);
        char *login_string = malloc(sizeof(char)*500);

        json_object_set_string(root_object, "username", username);
        json_object_set_string(root_object, "password", password);
        login_string = json_serialize_to_string_pretty(root_value);
          

        char **tosend = malloc(2 * sizeof(char*));
        tosend[0] = malloc(500*sizeof(char));
        strcpy(tosend[0],login_string);
           
            
           
           
        message = compute_post_request(host, "/api/v1/tema/auth/login", NULL, tosend, 1, NULL, 0,NULL);
            
        //puts(message);
        send_to_server(sockfd, message);
        response = receive_from_server(sockfd);
        //puts(response);
      
        display(response);
        char* ret;
        if((strstr(response,"connect.sid=")) != NULL) {
            ret = strstr(response,"connect.sid=");
            char* token = malloc(1000*sizeof(char));
            token = strtok(ret,";");
            return token;
        }  else return NULL;
              
        }
        return NULL;
}
void add_helper(char *token_jwt) {
    char *message;
    char *response;
    int sockfd;
    char* host = "ec2-3-8-116-10.eu-west-2.compute.amazonaws.com";

    
    char *title = (char*) malloc(sizeof(char)*100);
    printf("title=");
    scanf("%s",title);
    printf("Title entered: %s\n",title);

    char *author = (char*) malloc(sizeof(char)*100);
    printf("author=");
    scanf("%s",author);
    printf("Author entered: %s\n",author);

    char *genre = (char*) malloc(sizeof(char)*100);
    printf("genre=");
    scanf("%s",genre);
    printf("Genre entered: %s\n",genre);

    char *publisher = (char*) malloc(sizeof(char)*100);
    printf("publisher=");
    scanf("%s",publisher);
    printf("Publisher entered: %s\n",publisher);

    char *page_count = (char*) malloc(sizeof(char)*100);
    printf("page_count=");
    scanf("%s",page_count);
    printf("Page_count entered: %s\n",page_count);


   
    sockfd = open_connection("3.8.116.10", 8080, AF_INET, SOCK_STREAM, 0);
    JSON_Value *root_value = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);
    char *book_string = malloc(sizeof(char)*500);

    json_object_set_string(root_object, "title", title);
    json_object_set_string(root_object, "author", author);
    json_object_set_string(root_object, "genre", genre);
    json_object_set_number(root_object, "page_count",atoi(page_count));
    json_object_set_string(root_object, "publisher", publisher);
    book_string = json_serialize_to_string_pretty(root_value);
   

    char **tosend = malloc(2 * sizeof(char*));
    tosend[0] = malloc(500*sizeof(char));
    strcpy(tosend[0],book_string);
    
        

    message = compute_post_request(host, "/api/v1/tema/library/books", NULL, tosend, 1, NULL, 0,token_jwt);
        
    //puts(message);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    //puts(response);
    
    display(response);
}
