#include<iostream>
#include<string>
#include<vector>
#include"Graph.h"
#include <cmath>
#include<unordered_map>
using namespace std;

#pragma once
class Student
{

public:
	int ID;
	string name;
	string pass; //vector of IDS of finishedCourses
	int year;
	int maxCH;
	Student();
	vector <int> CoursesinProg;
	vector<int> finCourses;
	void fill(); //For test
	void Login();
	void StudentPage(int id);
	int availableCourses(int id);
	void showSpecCourse(); //specific course
	void RegCourse(int id);
	void showAllhisCourses(int id);
	void editData(int id);
};

class Admin
{
public:
	string name;
	string password;
	
	Admin();
	void AdminPage();
	void AddCourse();
	void Fill();
	void Store();
	void EditCourseName(int,int);
	void EditCourseMaxStud(int,int);
	void EditCourseCH(int,int);
	void EditCourseParent(int,int);
	void EditCourseData();
	Student fillStudentData();
	void AddStudent();
	void FillStud();
	void StoreStud();
	void ViewStudentsInCourse(int);
	void FinProgCofStud(int studID);
	
	~Admin();
};
