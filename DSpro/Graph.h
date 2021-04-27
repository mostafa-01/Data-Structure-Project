#pragma once
#include<iostream>
#include<string>
#include<vector>

using namespace std;


class Course
{

public:
	string name;
	int ID;
	int maxStudents;
	int currentSudents;
	int creditHours;
	int parentID;
	Course* next;
	Course();
	
	Course(string, int,int,int,int,int);
};

class collectdata
{

public:
	Course* headElement;
	int dependencies;
	collectdata();
	void Append(string, int,int max, int current, int ch, int p);
	void DeleteAt(int);
	void displayList();
	int sizeOf();
	void clear();
};

class Graph
{
	int counter;
	int size;
	


public:
	vector<collectdata> vertix;
	int gID;
	Graph();
	void pushc(Course, Course);
	void deleteC(int parentID, int delID);
	void edit();
	void traverse();
	
	Course Find(int);
	
	~Graph();
};


