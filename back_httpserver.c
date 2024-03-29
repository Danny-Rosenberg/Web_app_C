/* 
This code primarily comes from 
http://www.prasannatech.net/2008/07/socket-programming-tutorial.html
and
http://www.binarii.com/files/papers/c_sockets.txt
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

typedef struct Row{
  char class[50];
  char instructor[50];
  char size[10];
  char c_quality[10];
  char i_quality[10];
  char difficulty[10];
} row;

int start_server(int PORT_NUMBER) {

      // structs to represent the server and client
  struct sockaddr_in server_addr,client_addr;    

  int sock; // socket descriptor

      // 1. socket: creates a socket descriptor that you later use to make other system calls
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
   perror("Socket");
   exit(1);
  }
  int temp;
  if (setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&temp,sizeof(int)) == -1) {
    perror("Setsockopt");
    exit(1);
  }

      // configure the server
  server_addr.sin_port = htons(PORT_NUMBER); // specify port number
  server_addr.sin_family = AF_INET;         
  server_addr.sin_addr.s_addr = INADDR_ANY; 
  bzero(&(server_addr.sin_zero),8); 
      
      // 2. bind: use the socket and associate it with the port number
  if (bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) {
    perror("Unable to bind");
    exit(1);
  }

      // 3. listen: indicates that we want to listen to the port to which we bound; second arg is number of allowed connections
  if (listen(sock, 1) == -1) {
    perror("Listen");
    exit(1);
  }

      // once you get here, the server is set up and about to start listening
  printf("\nServer configured to listen on port %d\n", PORT_NUMBER);
  fflush(stdout);

  FILE* fp;
  char str[500];
  fp = fopen("course_evals.txt", "r");
  if (fp == NULL) {
   perror("Error opening file");
   return -1;
  }

  int http_index = 0;


  char* cp = malloc(sizeof(char) * 40000);

  //for the form
  char* cp2 = malloc(sizeof(char) * 41000);


      //x      int count_char = 0;

  char http_before[] = "<!DOCTYPE HTML><html><head><title>project1</title></head><body>";

      //     char* http_cp = {};
      //      strcpy(http_cp, http_before);
  int b  = strlen( http_before );
  // printf("the string length of http_before is : %d\n", b);

  int count_char = b;
      // include the http_before text in malloc

	//adding the header
  int i = 0;
  while (http_before[i] != '\0') {
   cp[i] = http_before[i];
   i++;
  }
// making the struct
  row* rows = malloc(sizeof(row) * 1000);
  char str_cpy[200];
  char* token;
  const char s[5] = ",\n";

  int count_line = 0; 
  while (fgets(str, 80, fp) != NULL) {
	//	        puts(str);  // test code

    strcpy(str_cpy, str);
    token = strtok(str_cpy, s);

    int counter = 0;
    while (token != NULL) {
	//  printf("%s$$\n", token);
      if (counter == 0) {
        strcpy(rows[count_line].class, token);
      }
      if (counter == 1) {
        strcpy(rows[count_line].instructor, token);
      }
//		printf("instructor is: %s\n", rows[count_line].instructor);}
      if (counter == 2) {
        strcpy(rows[count_line].size, token);
      }
      if (counter == 3) {
        strcpy(rows[count_line].c_quality, token);
      }
      if (counter == 4) {
        strcpy(rows[count_line].i_quality, token);
      }
      if (counter == 5) {
        strcpy(rows[count_line].difficulty, token);
      }
      counter++;

      token = strtok(NULL, s);

    }

    int str_len = strlen(str);
    for (int i = 0;i < str_len;i++) {
      cp[count_char + i] = str[i];
    }
    cp[count_char + str_len] = '<';
    cp[count_char + str_len + 1] = 'b';
    cp[count_char + str_len + 2] = 'r';
    cp[count_char + str_len + 3] = '>';
    count_char = count_char + str_len + 4;

// go to the next line
    count_line++;

  }     



  char after[] = "</body></html>";

  int c = strlen(after);
  // printf("the string length after is : %d\n", c);
  i = 0;

  while(after[i] != '\0'){
    cp[count_char + i] = after[i];
    i++; 
  }    

//      puts(cp);

// test code to print elements
  // for (int i = 0;i < 100;i ++) {
  //   printf("%s\n", rows[i].c_quality);
  //   printf("%s\n", rows[i].instructor);

  // }

  while (1) {
      // 4. accept: wait here until we get a connection on that port
    int sin_size = sizeof(struct sockaddr_in);
    int fd = accept(sock, (struct sockaddr *)&client_addr,(socklen_t *)&sin_size);
    if (fd != -1) {
      printf("Server got a connection from (%s, %d)\n", inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));

	// buffer to read data into
    char request[1024];

	// 5. recv: read incoming message (request) into buffer
    int bytes_received = recv(fd,request,1024,0);
	// null-terminate the string
    request[bytes_received] = '\0';
	// print it to standard out

     printf("This is the incoming request:\n%s\n", request);

	//here is where we will parse and perform logic based on the type of request
	// this is the message that we'll send back

    const char n[3] = "\n";
    char *token;
       /* get the first token */
    token = strtok(request, n);
    char* str2 = "GET /all ";
    char* str3 = "GET /index.html ";
    char* str4 = "GET /httpserver.c?category=filter ";
    char* str5 = "GET /httpserver.c?category=sort ";
    char* str6 = "GET /httpserver.c?category=calculate ";
    char* str7 = "GET /httpserver.c?keyword=";
    int parse_flag = 0;
    char keyword[50];
    int keyword_index;
   /* walk through other tokens */
    while( token != NULL ) 
    {
      printf( "%s\n", token );
      if (strncmp(str2, token, strlen(str2)) == 0) {
        parse_flag = 1; // for /all

        // printf("this is where we should code !\n");
        }
      else if (strncmp(str4, token, strlen(str4)) == 0) {
        parse_flag = 2; // for filter
      }
      else if (strncmp(str5, token, strlen(str5)) == 0) {
        parse_flag = 3; // for sort
      }
      else if (strncmp(str3, token, strlen(str3)) == 0) {
        parse_flag = 4; // for index.html
      } else if(strncmp(str6, token, strlen(str6)) == 0) {
        parse_flag = 5; // for calculate
      } else if (strncmp(str7, token, strlen(str7)) == 0) {
        parse_flag = 6; // filtered!
        int k = 0;
        keyword_index = strlen(str7);
        while (token[keyword_index] != '\0') {
          keyword[k] = token[keyword_index];
          keyword_index += 1;
          k++;
        }
      }

      token = strtok(NULL, s);
    }
    char *reply = "<form >filter by keyword : <br> <input type=\"text\" name=\"keyword\" value=\"\"> <input type=\"submit\" value=\"Submit\"> <br> </form>";
    char *reply2 = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n<html>This page should have<p>a <b>sort</b>.</html>";
    char *reply3 = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n<html>calculate!</html>";
    char* forms = "<html><form action=\"httpserver.c\" method=\"get\"><input type=\"radio\" name=\"category\" value=\"sort\" checked>sort<br><input type=\"radio\" name=\"category\" value=\"filter\"> filter<br><input type=\"radio\" name=\"category\" value=\"calculate\"> calculate<br><input type=\"submit\" value=\"Submit\"></form>";
    char* forms2 = "</html>";
    // char dest[300];

    memset(&cp2[0], '\0', sizeof(cp2));

     // send(fd, cp2, strlen(cp2), 0); 

    if (parse_flag == 1) send(fd, cp, strlen(cp), 0);   // this is for /all
    if (parse_flag == 2){
      strcat(cp2, forms);
      strcat(cp2, reply);
      strcat(cp2, cp);
      strcat(cp2, forms2);
      send(fd, cp2, strlen(cp2), 0);// for index.html

      // send(fd, reply, strlen(reply), 0);   // this is for filter
    } 
    if (parse_flag == 3) {
      strcat(cp2, forms);
      strcat(cp2, reply);
      // strcat(cp2, cp);
      strcat(cp2, forms2);
      send(fd, cp2, strlen(cp2), 0);
    } // this is for sort
    if (parse_flag == 4) {
      strcat(cp2, forms);
      strcat(cp2, cp);
      strcat(cp2, forms2);
      send(fd, cp2, strlen(cp2), 0);// for index.html
    }
    if (parse_flag == 5) send(fd, reply3, strlen(reply3), 0); // calculation
    if (parse_flag == 6) {
      strcat(cp2, forms);
      strcat(cp2, &keyword);
      strcat(cp2, forms2);
      send(fd, cp2, strlen(cp2), 0);
    }
      // code in here to make index.html!


	// 6. send: send the outgoing message (response) over the socket
	// note that the second argument is a char*, and the third is the number of chars	
	//	send(fd, reply, strlen(reply), 0);
    // send(fd, cp, strlen(cp), 0);

	// 7. close: close the connection
    close(fd);
    printf("Server closed connection\n");
    }
  }
      // 8. close: close the socket
  close(sock);
  printf("Server shutting down\n");

  return 0;

}




int main(int argc, char *argv[])
{
  // check the number of arguments
  if (argc != 2) {
    printf("\nUsage: %s [port_number]\n", argv[0]);
    exit(-1);
  }

  int port_number = atoi(argv[1]);
  if (port_number <= 1024) {
    printf("\nPlease specify a port number greater than 1024\n");
    exit(-1);
  }

  start_server(port_number);
}

