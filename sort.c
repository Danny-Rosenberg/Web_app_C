rows* c_qual_arr[count_line];

//filling an array with all the c_quality values
for(int i = 0; i < count_line; i++){
   c_qual_arr[i] = rows[i];
	}

//but we want to keep the row with the value. 

//sort the array	
	
	i = 0;
	//ascending order insertion sort
	while(c_qual_arr[i] != null){ 
		if (atof(c_qual_arr[i].c_quality) > atof(c_qual_arr[i+1].c_quality)){
		//swap
		char temp[10];
		strcpy(temp, c_qual_arr[i].c_quality);
		strcpy(c_qual_arr[i].c_quality, c_qual_arr[i+1].c_quality);
		strcpy(c_qual_arr[i+1].c_quality, temp);
		i++;
		}
	}

	//display c_qual_arr
