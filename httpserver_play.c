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
#include <math.h>

typedef struct Row{
  char class[50];
  char instructor[50];
  char size[10];
  char c_quality[10];
  char i_quality[10];
  char difficulty[10];
} row;
 
// reverses a string 'str' of length 'len'
void reverse(char *str, int len)
{
    int i=0, j=len-1, temp;
    while (i<j)
    {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++; 
        j--;
    }
}
 
 // Converts a given integer x to string str[].  d is the number
 // of digits required in output. If d is more than the number
 // of digits in x, then 0s are added at the beginning.
int intToStr(int x, char str[], int d)
{
    int i = 0;
    while (x)
    {
        str[i++] = (x%10) + '0';
        x = x/10;
    }
 
    // If number of digits required is more, then
    // add 0s at the beginning
    while (i < d)
        str[i++] = '0';
 
    reverse(str, i);
    str[i] = '\0';
    return i;
}

// 
void ftoa(float n, char *res, int afterpoint)
{
    // Extract integer part
    int ipart = (int)n;
 
    // Extract floating part
    float fpart = n - (float)ipart;
 
    // convert integer part to string
    int i = intToStr(ipart, res, 0);
 
    // check for display option after point
    if (afterpoint != 0)
    {
        res[i] = '.';  // add dot
 
        // Get the value of fraction part upto given no.
        // of points after dot. The third parameter is needed
        // to handle cases like 233.007
        fpart = fpart * pow(10, afterpoint);
 
        intToStr((int)fpart, res + i + 1, afterpoint);
    }
}

void addHeader(char* file){
  
     char http_before[] = "<!DOCTYPE HTML><html><head><title>project1</title></head><body><form><b>Penn CS course Evalutaions</b><br>Sort by course quality : <input type=\"submit\" name=\"sort\" value=\"sort1\"><br>Sort by difficulty :   <input type=\"submit\" name=\"sort\" value=\"sort2\"></form> <br><form>filter by keyword : <br> <input type=\"text\" name=\"keyword\" value=\"\"> <input type=\"submit\" value=\"Submit\"> </form><br> <br><form> Average course difficulty :  <input type=\"submit\" name=\"average\" value=\"average1\"><br> Average instructor quality :   <input type=\"submit\" name=\"average\" value=\"average2\"><br></form>";
     int i = 0;
     while (http_before[i] != '\0') {
     file[i] = http_before[i];
     i++;
     }
}

void addFooter(char* file){
  
  char after[] = "</body></html>";
  strcat(file, after);

}


void addBreaks(row rows[], char* file, int num_lines){
 //  int size = sizeof(rows);
 //  int start_point = size/sizeof(rows);

//probably also need to add ','
    strcat(file, "<table>");
    strcat(file, "<tr>");
    strcat(file, "<td>");
    strcat(file, "course number");
    strcat(file, "</td>");
    strcat(file, "<td>");
    strcat(file, "instructor");
    strcat(file, "</td>");
    strcat(file, "<td>");
    strcat(file, "class size");
    strcat(file, "</td>");
    strcat(file, "<td>");
    strcat(file, "class quality");
    strcat(file, "</td>");
    strcat(file, "<td>");
    strcat(file, "instructor quality");
    strcat(file, "</td>");
    strcat(file, "<td>");
    strcat(file, "difficulty");
    strcat(file, "</td>");
    strcat(file, "</tr>");

    for(int i = 0; i < num_lines; i++) {
//      char* line = malloc(sizeof(char) * 1000);
    strcat(file, "<tr>");
    strcat(file, "<td>");
    strcat(file, rows[i].class);
    strcat(file, "</td>");
    strcat(file, "<td>");
    strcat(file, rows[i].instructor);
    strcat(file, "</td>");
    strcat(file, "<td>");
    strcat(file, rows[i].size);
    strcat(file, "</td>");
    strcat(file, "<td>");
    strcat(file, rows[i].c_quality);
    strcat(file, "</td>");
    strcat(file, "<td>");
    strcat(file, rows[i].i_quality);
    strcat(file, "</td>");
    strcat(file, "<td>");
    strcat(file, rows[i].difficulty);
    strcat(file, "</td>");
    strcat(file, "</tr>");
//  strcat(file, line); //adding lines to the file.
    }
    strcat(file, "</table>");
//printf("The modified file is %s: \n", file);

}


void sort_1(row rows[], int num_lines){
    int i;
    int j;
    
  for (i = 1; i < num_lines + 1; i++){
        
        j = i - 1;
        while(j > 0 && atof(rows[j].c_quality) < atof(rows[j-1].c_quality)){
    row temp;
        memcpy(&temp, &rows[j], sizeof(row));
        memcpy(&rows[j], &rows[j-1], sizeof(row));
        memcpy(&rows[j-1], &temp, sizeof(row));
        j--;
      }
  }
}

//Insertion sort based on course difficulty
void sort_2(row rows[], int num_lines){
  int i;
  int j;
    
  for (i = 1; i < num_lines + 1; i++){        
    j = i - 1;
    while(j > 0 && atof(rows[j].difficulty) < atof(rows[j-1].difficulty)){
      row temp;
      memcpy(&temp, &rows[j], sizeof(row));
      memcpy(&rows[j], &rows[j-1], sizeof(row));
      memcpy(&rows[j-1], &temp, sizeof(row));
      j--;
    }
  }
}

double average_cd(row rows[], int num_lines) {
  double sum = 0.0;
  for (int i = 0;i < num_lines;i++) {
    sum += atof(rows[i].difficulty);
  }
  sum = sum / (double) num_lines;
  return sum;
}

double average_iq(row rows[], int num_lines) {
  double sum = 0.0;
  for (int i = 0;i < num_lines;i++) {
    sum += atof(rows[i].i_quality);
  }
  sum = sum / (double) num_lines;
  return sum;
}


//Insertion sort low to high based on
int initialize(row rows[], char* html, int x, int num_lines){
  int num_l = num_lines;
  addHeader(html);
  if (x == 2) { // sort course_quality
    sort_1(rows, num_lines); 
  } 
  if (x == 3) { // sort Two
    sort_2(rows, num_lines);
  }
  else if (x == 4) { // filter

  }
  else if (x == 5) { // average course difficulty

    strcat(html, "<br><br>");
    strcat(html, "the average of course difficulty would be : ");
    char temp_cp[20];
    ftoa(average_cd(rows, num_lines), temp_cp, 5);
    strcat(html, temp_cp);
    strcat(html, "<br><br>");
  }
  else if (x == 6) { //average of instructor quality
    strcat(html, "<br><br>");
    strcat(html, "the average of instructor quality would be : ");
    char temp_cp[20];
    ftoa(average_iq(rows, num_lines), temp_cp, 5);
    strcat(html, temp_cp);
    strcat(html, "<br><br>");

  }
  
  addBreaks(rows, html, num_l);

  addFooter(html);
  return 0;

}


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

  // opening file
  FILE* fp;
  char str[500];
  fp = fopen("course_evals.txt", "r");
  if (fp == NULL) {
   perror("Error opening file");
   return -1;
  }

  //for the form
  char* cp2 = malloc(sizeof(char) * 41000);

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

// go to the next line
    count_line++;

  }     


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
    const int for_all = 0;
    const int index_html = 1;
    const int sort_cq = 2;
    const int sort_dif = 3;
    const int filtered = 4;
    const int average_cd = 5;
    const int average_iq = 6;
       /* get the first token */
    token = strtok(request, n);
    char* str2 = "GET / ";
    char* str3 = "GET /index.html ";
    char* str4 = "GET /?sort=sort1 ";
    char* str5 = "GET /?sort=sort2 ";
    char* str6 = "GET /?keyword=";
    char* str7 = "GET /?average=average1 ";
    char* str8 = "GET /?average=average2 ";
    int parse_flag = 0;
    char keyword[50];
    int keyword_index;
   /* walk through other tokens */
    while( token != NULL ) 
    {
      printf( "%s\n", token );
      if (strncmp(str2, token, strlen(str2)) == 0) {
        parse_flag = for_all; 
      }
      else if (strncmp(str3, token, strlen(str3)) == 0) {
        parse_flag = index_html; 
      }
      else if (strncmp(str4, token, strlen(str4)) == 0) {
        parse_flag = sort_cq; 
      }
      else if (strncmp(str5, token, strlen(str5)) == 0) {
        parse_flag = sort_dif; 
      } else if(strncmp(str6, token, strlen(str6)) == 0) {
        parse_flag = filtered; 
        int k = 0;
        keyword_index = strlen(str6);
        while (token[keyword_index] != ' ') {
          keyword[k] = token[keyword_index];
          keyword_index += 1;
          k++;
        }
        keyword[k] = '\0';
      } else if (strncmp(str7, token, strlen(str7)) == 0) {
        parse_flag = average_cd; 
      } else if (strncmp(str8, token, strlen(str8)) == 0) {
        parse_flag = average_iq;
      }

      token = strtok(NULL, s);
    }

    memset(&cp2[0], '\0', 41000);
     // send(fd, cp2, strlen(cp2), 0); 

    if (parse_flag == for_all) {
      initialize(rows, cp2, 0, count_line);
      send(fd, cp2, strlen(cp2), 0);
    } 

    else if (parse_flag == index_html) {
      initialize(rows, cp2, 1, count_line);
      send(fd, cp2, strlen(cp2), 0);
    }

    else if (parse_flag == sort_cq) {
      initialize(rows, cp2, 2, count_line);
      send(fd, cp2, strlen(cp2), 0);
    }

    else if (parse_flag == sort_dif) {
      initialize(rows, cp2, 3, count_line);
      send(fd, cp2, strlen(cp2), 0);
    }

    else if (parse_flag == filtered) {
      initialize(rows, cp2, 4, count_line);
      send(fd, cp2, strlen(cp2), 0);
    }

    else if (parse_flag == average_cd) {
      initialize(rows, cp2, 5, count_line);
      send(fd, cp2, strlen(cp2), 0);
    }

    else if (parse_flag == average_iq) {
      initialize(rows, cp2, 6, count_line);
      send(fd, cp2, strlen(cp2), 0);
    }

	// 6. send: send the outgoing message (response) over the socket
	// note that the second argument is a char*, and the third is the number of chars	


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

