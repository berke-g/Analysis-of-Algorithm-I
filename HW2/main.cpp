/*
 *BLG 335 Project 2
 *Name: Berke Gülçiçek
 *Student Number: 150160054
 *Date: 25.12.2020
 *
 *NOTES:
 *A taxi is called each 100 operations, starting from 100. (100, 200, 300...)
 *A taxi call is not considered an operation. (addition_count + update_count = m)
*/

#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib> 
#include <ctime> 
#include <fstream>
#include <string>

using namespace std;

const double HOTEL_LONGITUDE = 33.40819;
const double HOTEL_LATITUDE = 39.19001;


class PQ {
private:
	vector<double> taxis;
	int size;

	int get_parent_index(int i); //get index of parent of ith node
	int get_left_child_index(int i); //get index of left child of ith node
	int get_right_child_index(int i); //get index of right child of ith node
	double get_parent_distance(int i); //get parent distance
	double get_left_child_distance(int i); //get left child distance
	double get_right_child_distance(int i); //get right child distance
	void shift_up(int i); //used for keeping heap structure
	void shift_down(int i); //used for keeping heap structure
	void change_distance(int i, double d); //change ith node's distance with d
	void swap(int i, int j); //swap two nodes with index i and j
	void remove(int i); //remove node with ith index 
	double extract_root(); //remove root (min value) and return its distance
	double get_distance(int i); //get distance of ith node 

public:
	void insert(double t); //add taxi
	void update(int i); //decrease distance of ith node by 0,01
	double call_taxi(); //extract root
	double get_root(); //returns root element 
	int get_size();
};

int PQ::get_parent_index(int i)  {
	return (i-1) / 2;
}

int PQ::get_left_child_index(int i) {
	return ((2*i) + 1);
}

int PQ::get_right_child_index(int i) {
	return ((2*i) + 2);
}

double PQ::get_distance(int i) {
	return taxis[i];
}

double PQ::get_parent_distance(int i)  {
	return taxis[get_parent_index(i)];
}

double PQ::get_left_child_distance(int i) {
	return taxis[get_left_child_index(i)];
}

double PQ::get_right_child_distance(int i) {
	return taxis[get_right_child_index(i)];
}

void PQ::shift_up(int i) {
	while (i > 0 && get_distance(i) < get_parent_distance(i) ) { //switch node with parent if parent is greater than node, until parent is less than node
		swap(i, get_parent_index(i));
		i = get_parent_index(i);
	}
}

void PQ::shift_down(int i) {
	int min_index = i; //index of node with minimum distance among left child, right child and parent

	int left_index = get_left_child_index(i);

	if (left_index <= taxis.size() && get_distance(left_index) < get_distance(min_index) ) { //check if left child is less than parent
		min_index = left_index;
	}

	int right_index = get_right_child_index(i);

	if (right_index <= taxis.size() && get_distance(right_index) < get_distance(min_index) ) { //check if right child is less than min(left, parent)
		min_index = right_index;
	}

	if (i != min_index) { //if minimum distance is not parent's
		swap(i, min_index); //swap parent with minimum child
		shift_down(min_index); //do all this again for old parent 
	}
}

void PQ::change_distance(int i, double d) {
	double old_d = get_distance(i);

	taxis[i] = d;

	//to fix heap structure
	if (old_d < d) {
		shift_down(i);
	} else {
		shift_up(i);
	}

}

void PQ::swap(int i, int j) {
	double temp = taxis[i];
	taxis[i] = taxis[j];
	taxis[j] = temp;
}

double PQ::get_root() {
	if (taxis.size() == 0) {
		throw "Emtpy heap.";
	}
	return taxis[0];
}

void PQ::remove(int i) {
	taxis[i] = get_root() - 1; //make that node smaller than root, which makes it the minimum node
	shift_up(i); //put it at correct place (root)
	extract_root(); //remove that node (which is now root)
}

double PQ::extract_root() {
	double distance = get_root();

	taxis[0] = taxis[taxis.size() - 1]; //root becomes last element
	taxis.pop_back(); //remove last element

	shift_down(0); //put new root to its correct place
	return distance;
}

void PQ::insert(double t) {
	taxis.push_back(t); //add it to end of array

	shift_up(taxis.size() - 1); //put it into its correct place
}

void PQ::update(int i) { 
	double new_distance = get_distance(i) - 0.01;

	if (new_distance < 0.0) { //distance cant be less than 0
		new_distance = 0.0;
	}

	change_distance(i, new_distance);
}

double PQ::call_taxi() {
	return extract_root();
}

int PQ::get_size() {
	return taxis.size();
}

double calculate_distance(double longitude, double  latitude) { //used to calculate taxis euclidian distance
	double latitude_diff = latitude - HOTEL_LATITUDE;
	double longitude_diff = longitude - HOTEL_LONGITUDE;
	return sqrt((longitude_diff * longitude_diff) + (latitude_diff * latitude_diff));
}

int main(int argc, char* argv[]) {
	clock_t t;
	t = clock();

	//open file
	ifstream file;
	file.open("locations.txt");
	
	//check if opening failed
	if (!file){
		cerr << "File cannot be opened!";
		exit(1);
	}

	int m = atoi(argv[1]); 
	double p = atof(argv[2]);
	
	//output variables	
	int update_count = 0; 
	int failed_update_count = 0;
	int addition_count = 0;
	int failed_call_count = 0;

	srand(time(NULL));

	PQ taxis;

	string line;
	getline(file, line); //header line

	cout << "The distance of the called taxis: " << endl; //output

	for (int i = 1; i <= m; i++) {		
		if (i % 100 == 0) { //for taxi calls
			try {
				taxis.call_taxi();
				cout << taxis.get_root() << " "; //output				
			} catch (const char* msg) {
				failed_call_count++;
			}
		}

		double random_double = (double)rand() / (double)RAND_MAX; //used to choose update or addition

		if (random_double <= p) { //update

			if (taxis.get_size() != 0) { //if heap is empty, skip
				int random_int = rand() % taxis.get_size();

				taxis.update(random_int);
				update_count++;

			} else {
				failed_update_count++;
			}

		} else { //add new taxi
			double longitude;
			double latitude;
			double distance;

			file >> longitude >> latitude;

			distance = calculate_distance(longitude, latitude);

			taxis.insert(distance);

			addition_count++;
		}
	}

	cout << endl;
	cout << "The number of failed taxi calls: " << failed_call_count << " (due to empty heap)"<< endl;
	cout << "The number of taxi additions: " << addition_count << endl;
	cout << "The number of successful distance updates: " << update_count << endl;
	cout << "The number of failed distance updates: " << failed_update_count << " (due to empty heap)" <<  endl; 

	t = clock() - t;
	cout << "Total running time in milliseconds: " << (float(t)/CLOCKS_PER_SEC)*1000 << endl;
	
	return 0;
}