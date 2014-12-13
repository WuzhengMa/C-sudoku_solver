//Author: Wuzheng Ma
//Login name: WM813
//Date: 26/3/2014
//Email: wuzheng.ma13@imperial.ac.uk

#include<iostream>
#include<fstream>
#include<cstdlib>
#include<time.h>



using namespace std;


bool enablePut(int Sudo[][9], int x,int y,int value);
void uniNum(int Sudo[][9]);

void backTrack(int Sudo[][9],int count);
bool placeOrNot(int Sudo[][9], int count);

int main(){
	int Sudo[9][9];
	int Sudo1[9][9];
	ifstream infile;
	ifstream infile1;
	infile.open("sudoku.txt");

	if(!infile.is_open()){
		cout<<"can not open the file"<<endl;
		exit(EXIT_FAILURE);
	}
	for(int i=0;i<9;i++){
		for(int j=0;j<9;j++){
			infile>>Sudo[i][j];	//Sudo is the sudoku which will be solved by the first algorithm
		}
	}

	//Set up clock to obtain the execution time
	clock_t start_time = clock();
	uniNum(Sudo);
	clock_t end_time = clock();
	cout<< "Running time is: "<<static_cast<double>(end_time-start_time)/CLOCKS_PER_SEC*1000<<"ms"<<endl;
	infile.close();

	infile1.open("sudoku.txt");
	for(int i=0;i<9;i++){
		for(int j=0;j<9;j++){
			infile1>>Sudo1[i][j];	//Sudo1 is the sudoku which will be solved by the second algorithm
		}
	}

	//Set up clock to obtain the execution time
	clock_t start1_time = clock();
	backTrack(Sudo1,0);
	clock_t end1_time = clock();
	cout<< "Running time is: "<<static_cast<double>(end1_time-start1_time)/CLOCKS_PER_SEC*1000<<"ms"<<endl;
	infile1.close();	
	
	return 0;
}


//First algorithm solves the sodoku without using recursions
//It works by finding a unique number for each spot, which the number can be put into that spot
void uniNum(int Sudo[][9]){
	int countZero=0;
	for(int j=0;j<9;j++){
		for(int i=0;i<9;i++){
			if(Sudo[i][j]==0){
				countZero++;	//Counting the number of unknown spots
			}
		}
	}

	while(countZero!=0){	
		for(int i=0;i<9;i++){
			for(int j=0;j<9;j++){
				if(Sudo[i][j]!=0) continue;
				int numPut=0;
				for(int k=0;k<9;k++){
					if(enablePut(Sudo,i,j,k+1)){	//Check whether "K+1" can be put into the spot
						if(numPut==0){
							numPut=k+1;
						}else{
							numPut=0;	//If the number of numbers which can be put into a spot is temporarilly more than one, 
										//Then the unique number for this spot cannot be found temporarilly.
										//So we skip the rest of the loop and start with next spot
							break;
						}
					}
				}
				if(numPut!=0){			//When the unique number for a spot is determined, 
										//We put that number into the spot and decrease the countZero			
					Sudo[i][j]=numPut;	
					countZero--;
				}
			}
		}
	}
	if(countZero==0){		//Finally when the countZero falls to 0, the sudoku is then solved.
							//Display the solved sudoku
		cout<<"The solved sudoku by using the first algorithm is:"<<endl;
		for(int i=0;i<9;i++){	
			for(int j=0;j<9;j++){
				cout<<Sudo[i][j]<<" ";
			}
			cout<<endl;
		}
	}

}

//Function will check whether a number can be placed into a spot.
//A number can be placed when there is no same number appeared 
//In the row, column, and 3*3 block of where the number is going 
//To be placed
bool enablePut(int Sudo[][9],int row,int column,int value){
	int blockrow = (row/3)*3;
	int blockcolumn = (column/3)*3;	//Declare a 3*3 block
	for(int i=0;i<9;i++){
		if(Sudo[i][column] == value && i != row) return false;	//Same number in the row of where the number will be placed
		if(Sudo[row][i] == value && i != column) return false;	//In the column
		if(Sudo[blockrow+(i%3)][blockcolumn+(i/3)] == value && i != row && i != column) return false;	//In the 3*3 block
	}
	return true;
}

//Second algorithm
//Solve sudoku with backtracking
void backTrack(int Sudo[][9],int count){
	int i, j;

	if(count == 81){
		cout<<"The solved sudoku by using the second algorithm is:"<<endl;
		for(i=0;i<9;i++){			//Prints out the sudoku when function ends
			for(j=0;j<9;j++){
				cout<<Sudo[i][j]<<" ";
			}
			cout<<endl;
		}
		return;
	}
	
	int row = count/9;
	int column = count%9;

	if(Sudo[row][column] == 0){
		for(i=0;i<9;i++){
			Sudo[row][column] = (i+1);		//Starts with putting 1 into the specific place
			if(placeOrNot(Sudo, count)){			
				backTrack(Sudo, count+1);			//If the number can be placed there, go to next recursion
			}
		}
		Sudo[row][column] = 0;		//If the number cannot be placed there, set it back to zero
	}
	else{
		backTrack(Sudo, count+1);		//In the case which the number is not unknown
	}
	return;
}

//Function again will check whether a number can be placed into a spot.
//Funtion performs the same function as the "enablePut" does
bool placeOrNot(int Sudo[][9], int count){			
													
	int row = count/9;		//Set 9 numbers to be a row
	int column = count%9;	//Set 9 numbers to be a column
	int i, j;

	for(i=0;i<9;i++){		//Same number in the row of where the number will be placed
		if(Sudo[i][column] == Sudo[row][column] && i != row){
			return false;
		}
	}
	
	for(j=0;j<9;j++){		//In the column
		if(Sudo[row][j] == Sudo[row][column] && j != column){
			return false;
		}
	}

	int blockrow = (row/3)*3;			
	int blockcolumn = (column/3)*3;

	for(i=blockrow;i<blockrow+3;i++){		//In the 3*3 block
		for(j=blockcolumn;j<blockcolumn+3;j++){
			if(Sudo[i][j] == Sudo[row][column] && i != row && j != column){
				return false;
			}
		}
	}
	return true;
}
