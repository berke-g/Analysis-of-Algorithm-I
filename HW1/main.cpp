/* 
BLG335 Project1
Name: Berke Gülçiçek
Student number: 150160054
Date: 10.12.2020
*/

#include<iostream>
#include<fstream>
#include<string>
#include<ctime>
#include<stdlib.h>

using namespace std;

/*
 *I have used struct Sale to keep contents of each line
 *Everything is kept as string to keep output file consistent with input file
*/
struct Sale {
	string country;
	string item_type;
	string order_id;
	string units_sold;
	string profit;
};

//I have used Lomuto partition scheme
//pivot is chosen as the last element
int part(Sale a[], int low, int high) {
	Sale pivot;
	pivot = a[high];
	Sale temp;	

	int i = low - 1; 
	for (int j = low; j <= high-1; j++){
		if ( (a[j].country).compare(pivot.country) < 0) { //if alphabetically lower than pivot
			i++; 
			
			//swap a[i] and a[j] 
			temp = a[i];
			a[i] = a[j];
			a[j] = temp;
		} else if ( (a[j].country).compare(pivot.country) == 0) { //if same country name, compare profits
			if ( atof((a[j].profit).c_str()) > atof((pivot.profit).c_str()) ) { //if profit is less than pivot
				i++;  
				
				//swap a[i] and a[j]
				temp = a[i]; 
				a[i] = a[j];
				a[j] = temp;
			}	
		}	
		
	}
	
	temp = a[i+1];
	a[i+1] = a[high];
	a[high] = temp;

	
	return (i+1);
}

void quickSort(Sale a[], int low, int high) {	
	if (low < high) {

		int p = part(a, low, high);
		
		quickSort(a, low, p-1);
		quickSort(a, p+1, high);
	}
	
}

int main(int argc, char* argv[]){

	//open file
	ifstream file;
	file.open("sales.txt");
	
	//check if opening failed
	if (!file){
		cerr << "File cannot be opened!";
		exit(1);
		}
		
	
	int N; //size of array
	
	//try to read N
	N = atoi(argv[1]); 

	Sale *sales = new Sale[N]; //file's contents are held at this array	
		
	string line;	
	getline(file, line); //this is the header line	


	for(int i = 0; i<N; i++){
		getline(file, line, '\t'); //country (string)
		sales[i].country = line; 
		getline(file, line, '\t'); //item type (string)
		sales[i].item_type = line;
		getline(file, line, '\t'); //order id (string)
		sales[i].order_id = line;
		file >> line; //units sold (integer)
		sales[i].units_sold = line;		
		file >> line; //total profit (float)
		sales[i].profit = line;
	    getline(file, line, '\n'); //this is for reading the \n character into dummy variable.
	}	
	
	clock_t t;
	t = clock();	

	quickSort(sales, 0, N-1);

	t = clock() - t;
	cout << "elapsed time of execution(seconds): " << float(t)/CLOCKS_PER_SEC << endl;
	
	//sorted file
	ofstream output_file;
	output_file.open("sorted.txt");
	
	//write header line
	output_file << "Country	Item Type	Order ID	Units Sold	Total Profit\n";
	
	//write each line
	for (int i = 0; i < N; i++) {
		output_file << sales[i].country << "\t";
		output_file << sales[i].item_type << "\t";
		output_file << sales[i].order_id << "\t";
		output_file << sales[i].units_sold << "\t";
		output_file << sales[i].profit << "\t";
		output_file << "\n";
	}	
		
	delete [] sales;
	
	return 0;
}
