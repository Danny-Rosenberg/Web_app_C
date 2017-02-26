#include <httpserver.c>

//here we want to go from an array of structs to a properly formatted html file 
 

char* initialize(row[] rows, int x){
		
   char* html = malloc(sizeof(char) * 40000);

   addHeader(html);  

   if (x == 1) filter_1();
   if (x == 2) filter_2();
   if (x == 3) sort_1(rows); //shouldn't make a copy
   if (x == 4) sort_2();	

   addBreaks(rows, html);
   addFooter(html);
   return html;

}

void sort1(row[] c_qual_arr){
	int i = 1;
	while(c_qual_arr[i] != null){ 
		if (atof(c_qual_arr[i].c_quality) < atof(c_qual_arr[i-1].c_quality)){
		//swap
		char temp[10];
		strcpy(temp, c_qual_arr[i].c_quality);
		strcpy(c_qual_arr[i].c_quality, c_qual_arr[i-1].c_quality);
		strcpy(c_qual_arr[i-1].c_quality, temp);
		i++;
		}
	}
}


void addBreaks(row[] rows, char* file){
 //  int size = sizeof(rows);
 //  int start_point = size/sizeof(rows);
   int i = 0;

	//probably also need to add ','
   while(rows[i] != null){
	line = malloc(sizeof(char) * 1000);
	strcat(line, rows[i].class);
	strcat(line, rows[i].instructor);
	strcat(line, rows[i].size);
	strcat(line, rows[i].c_quality);
	strcat(line, rows[i].i_quality);
	strcat(line, rows[i].difficulty);
	strcat(line, "<br>");
	strcat(file, line); 
   	i++;
    }


}


void addHeader(char* file){
	
     char http_before[] = "<!DOCTYPE HTML><html><head><title>project1</title></head><body>";
     int i = 0;
     while (http_before[i] != NULL) {
     file[i] = http_before[i];
     i++;
     }
}


void addFooter(char* file){
	
  char forms[] = "<form action=\"httpserver.c\" method=\"get\"><input type=\"radio\" name=\"category\" value=\"sort\" checked>sort<br><input type=\"radio\" name=\"category\" value=\"filter\"> filter<br><input type=\"radio\" name=\"category\" value=\"calculate\"> calculate<br><input type=\"submit\" value=\"Submit\"></form>";
  
   strcat(file, forms);
	
  char after[] = "</body></html>";
  strcat(file, after);

}


