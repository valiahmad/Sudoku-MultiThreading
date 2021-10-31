// SudokuMultiThreading.cpp
/*
*
* solving sudoku by multithread
* =============PROJECT============
* Copyright (c) vali ahmad rahmani
* Date May,3,2021
* name SUDOKU MULTITHREAD
*
*/
#include <iostream>
#include <Windows.h>
using namespace std;
/// 
/// //////////////////////////////////////////////////////////////////////////
/// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
///  using struct                                                    +
/// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
typedef struct {
	int row;
	int col;
	int(*board)[9];
}Parameters;
//
#define parent_row 0
#define parent_column 1
#define num_children 9
int Result_Of_Child[num_children];
int Result_Of_row;
int Result_Of_column;
int sudoku[9][9] = {
			{1, 2, 4, 5, 3, 9, 1, 8, 7},
			{5, 1, 9, 7, 2, 8, 6, 3, 4},
			{8, 3, 7, 6, 1, 4, 2, 9, 5},
			{1, 4, 3, 8, 6, 5, 7, 2, 9},
			{9, 5, 8, 2, 4, 7, 3, 6, 1},
			{7, 6, 2, 3, 9, 1, 4, 5, 8},
			{3, 7, 1, 9, 5, 6, 8, 4, 2},
			{4, 9, 6, 1, 8, 2, 5, 7, 3},
			{2, 8, 5, 4, 7, 3, 9, 1, 6}
};
/*
*
*checking every row has valu between 1-9
*/
DWORD WINAPI check_rows(LPVOID params) {
	Parameters* data = (Parameters*)params;
	int startRow = data->row;
	int startCol = data->col;
	for (int i = startRow; i < 9; ++i) {
		int row[10] = { 0 };
		for (int j = startCol; j < 9; ++j) {
			int val = data->board[i][j];
			if (row[val] != 0) {
				Result_Of_row = 0;
				return 0;
			}
			else {
				row[val] = 1;
			}
		}
	}
	Result_Of_row = 1;
	return 1;
}
/*
*
*checking every column has valu between 1-9
*/
DWORD WINAPI check_columns(LPVOID params) {
	Parameters* data = (Parameters*)params;
	int startRow = data->row;
	int startCol = data->col;
	for (int i = startCol; i < 9; ++i) {
		int col[10] = { 0 };
		for (int j = startRow; j < 9; ++j) {
			int val = data->board[j][i];
			if (col[val] != 0) {
				Result_Of_column = 0;
				return 0;
			}
			else {
				col[val] = 1;
			}
		}
	}
	Result_Of_column = 1;
	return 1;
}
/*
*
* checking nine grid 3x3
*/
DWORD WINAPI check_child(LPVOID params) {
	Parameters* data = (Parameters*)params;
	int startRow = data->row;
	int startCol = data->col;
	int index = startRow / 3 + startCol;
	int saved[10] = { 0 };
	for (int i = startRow; i < startRow + 3; ++i) {
		for (int j = startCol; j < startCol + 3; ++j) {
			int val = data->board[i][j];
			if (saved[val] != 0) {
				Result_Of_Child[index] = 0;
				return 0;
			}
			else {
				saved[val] = 1;
			}
		}
	}
	Result_Of_Child[index] = 1;
	return 1;
}
//////////////////////////////////////////////////////
void create_child(Parameters child[]) {
	for (int i = 0; i < num_children / 3; i++) {
		for (int j = 0; j < num_children / 3; j++) {
			child[3 * i + j].row = i * 3;
			child[3 * i + j].col = j * 3;
			child[3 * i + j].board = sudoku;
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//MAIN
int main()
{
	Parameters* Child;
	Parameters* Parent;
	/////////////////////////////create nine child
	Child = new Parameters[num_children];
	create_child(Child);
	////////////////////////create parent
	Parent = new Parameters;
	Parent->row = 0;
	Parent->col = 0;
	Parent->board = sudoku;
	///////////////////////

	////////////////////////////////////////////////////////
	//Data_Th and Date_Ch save data of thread when exit
	UINT Data_Ch[num_children];
	UINT Data_Pa[2];
	//ThreadID save ID of nine threads
	DWORD ThreadID[num_children];
	//threads
	HANDLE ThreadChild[num_children];
	HANDLE ThreadParent[2];

	//create parents threads
	ThreadParent[parent_row] = CreateThread(NULL, 0, check_rows, &Parent, CREATE_SUSPENDED, 0);
	if (Parent == NULL)ExitProcess(Data_Pa[0]);
	else cout << "\nParent \"row\" Has Create Seccessfuly..";
	ThreadParent[parent_column] = CreateThread(NULL, 0, check_columns, &Parent, CREATE_SUSPENDED, 0);
	if (Parent == NULL)ExitProcess(Data_Pa[1]);
	else cout << "\nParent \"column\" Has Create Seccessfuly..";
	//
	//create children threads
	for (int i = 0; i < num_children; i++) {
		ThreadChild[i] = CreateThread(NULL, 0, check_child, &Child[i], 0, &ThreadID[i]);
		if (ThreadChild[i] == NULL)ExitProcess(Data_Ch[i]);
		else cout << "\nChild " << i + 1 << "th Has Createed Successfuly With ID : " << ThreadID[i];
	}
	cout << "\nWaiting For Children...";
	WaitForMultipleObjects(num_children, ThreadChild, TRUE, INFINITE);
	for (int i = 0; i < num_children; i++)
		CloseHandle(ThreadChild[i]);
	//
	//
	if (//check the result of children
		Result_Of_Child[0] == 1
		&& Result_Of_Child[1] == 1
		&& Result_Of_Child[2] == 1
		&& Result_Of_Child[3] == 1
		&& Result_Of_Child[4] == 1
		&& Result_Of_Child[5] == 1
		&& Result_Of_Child[6] == 1
		&& Result_Of_Child[7] == 1
		&& Result_Of_Child[8] == 1
		) {
		cout << "\nChildren Has Done Successfuly.";
		ResumeThread(ThreadParent[parent_row]);
		ResumeThread(ThreadParent[parent_column]);
		cout << "\nWaiting For Parents...";
		WaitForMultipleObjects(2, ThreadParent, TRUE, INFINITE);
		CloseHandle(ThreadParent[parent_row]);
		CloseHandle(ThreadParent[parent_column]);
		if (Result_Of_row && Result_Of_column) {//check parents
			cout << "\nParents Has Done Successfuly.";
			cout << "\n\n\t\t\t\t***Sudoku Solved***\n";
			return 0;
		}
		else {
			cout << "\nParent Failed";
			return 1;
		}
	}
	else {
		cout << "Children Failed";
		return 1;
	}
	/////////////////////////////////////////////////////////////////
	return 0;
}