#include "Graph.h"
#include<string>
#include"Admin.h"
using namespace std;

//course 
Course::Course()
{
	ID = 0;
	next = 0;
	maxStudents = 0;
	currentSudents = 0;
	creditHours = 0;
	parentID = 0;
}
Course::Course(string n, int id, int max, int current, int ch, int p)
{
	name = n;
	ID = id;
	maxStudents=max ;
	currentSudents = current;
	creditHours = ch;
	parentID = p;
	next = 0;
}


//collecdata
collectdata::collectdata()
{
	dependencies = 0;
	headElement = 0;
}

void collectdata::Append(string n, int id, int max, int current, int ch, int p)
{
	Course* newCourse = new Course(n, id, max, current, ch, p);
	if (dependencies == 0)
		headElement = newCourse;
	else
	{
		newCourse->next = headElement->next;
		headElement->next = newCourse;
	}
	dependencies++;
}

void collectdata::DeleteAt(int id)
{
	Course* tmp = headElement;
	Course* del = headElement;
	int index = 0;
	for (int i = 0; i < dependencies; i++)
	{
		if (del->ID == id)
		{
			index = i;
			break;
		}
		else
		{
			del = del->next;
		}
	}
	for (int i = 0; i < index - 2; i++)
	{
		tmp = tmp->next;
	}
	tmp->next = del->next;
	delete del;
	dependencies--;
}

int collectdata::sizeOf()
{
	return dependencies;
}

void collectdata::displayList() {
	Course* tmp = headElement;
	for (int i = 0; i < dependencies; i++) {
		cout << tmp->name <<" parent "<<tmp->parentID<< "\n";
		tmp = tmp->next;
	}
}

void collectdata::clear()
{
	Course* tmp = headElement;
	for (int i = 0; i < dependencies; i++) {
		headElement = headElement->next;
		delete tmp;
		tmp = headElement;
	}
}

//graph

Graph::Graph()
{
	counter = 0;
	size = 10;

}
void Graph::pushc(Course N,Course O)
{
	collectdata old;
	old.Append(O.name, O.ID,O.maxStudents,O.currentSudents,O.creditHours,O.parentID);
	collectdata add;
	add.Append(N.name, N.ID, N.maxStudents, N.currentSudents, N.creditHours, N.parentID);
	bool found = 0;
	if (counter == 0) {
		vertix.push_back(add);
		found = 1;
		counter++;
	}
	else {
		for (int i = 0; i < counter; i++)
		{
			if (vertix[i].headElement->ID ==O.ID)
			{

				//cout << "in no prob\n";
				vertix[i].Append(N.name, N.ID, N.maxStudents, N.currentSudents, N.creditHours, N.parentID);
				found = 1;
			}
		}
	}
	if (!found) {

		vertix.push_back(old);
		vertix[counter].Append(N.name, N.ID, N.maxStudents, N.currentSudents, N.creditHours, N.parentID);
		counter++;
	}


}
Course Graph::Find(int id) {
	Course *temp;
	Course notFOUND;
	notFOUND.ID = 0;
	for (int i = 0; i < vertix.size(); i++) {
		if (vertix[i].headElement->ID == id) {
		
			return *vertix[i].headElement;
		}
		temp = vertix[i].headElement;
		for (int j = 0; j < vertix[i].dependencies; j++) {
			if (temp->ID == id) {
					
				return *temp;
			}
			else
				temp = temp->next;
		}
	}
	
	return notFOUND;
}
void Graph::deleteC(int parentID, int delID) {

	//searches for parent of delC
	//del delC from parent list
	vector<collectdata>::iterator it;
	it = vertix.begin();
	for (int i = 0; i < vertix.size(); i++) {
		if (vertix[i].headElement->ID == parentID) {
			vertix[i].DeleteAt(delID);
			if (vertix[i].dependencies == 1) {
				vertix.erase(it + i);
			}
			break;
		}
	}

	for (it = vertix.begin(); it != vertix.end(); it++) {
		if (it->headElement->ID == delID) {
			it->clear();
			vertix.erase(it);
			break;
		}
	}
}
void Graph::traverse() {
	vector<collectdata>::iterator it;
	for (it = vertix.begin(); it != vertix.end(); it++) {
		//cout << it->headElement->name << "\n";
		
		it->displayList();

		cout << "\\\\\\\\\\\\\\\\\\\\\\\\\\\ \n";
	}
}
Graph::~Graph()
{
	
}


//void Graph::edit()
//{
//	int id;
//	cout << "Enter ID: "; cin >> id;
//	Course* tmp = vertix[0].headElement;
//	Course* tmp2 = tmp;
//	int deletedindex;
//	bool brz = 0;
//	for (int i = 0; i < vertix.size(); i++)
//	{
//		if (brz == 1)
//			break;
//		tmp = vertix[i].headElement;
//		for (int j = 0; j < vertix[i].dependencies; j++)
//		{
//
//			if (tmp->ID == id)
//			{
//				deletedindex = i;
//				brz = 1;
//				break;
//			}
//			else
//				if (tmp->next != NULL)
//					tmp = tmp->next;
//		}
//	}
//	cout << "1.Edit name\n2.Edit maximum number of students\n3.Edit number of credit hours\n4.Edit PreCourses\n";
//	int choice;
//	cin >> choice;
//	if (choice == 1)
//	{
//		string n;
//		cout << "Edit name: "; cin >> n;
//		for (int i = 0; i < vertix.size(); i++)
//		{
//			tmp = vertix[i].headElement;
//			for (int j = 0; j < vertix[i].dependencies; j++)
//			{
//
//				if (tmp->ID == id)
//				{
//					tmp->name = n;
//				}
//				else
//					if (tmp->next != NULL)
//						tmp = tmp->next;
//			}
//		}
//	}
//	else if (choice == 2)
//	{
//		int max;
//		cout << "Edit maximum number of students: "; cin >> max;
//		for (int i = 0; i < vertix.size(); i++)
//		{
//			tmp = vertix[i].headElement;
//			for (int j = 0; j < vertix[i].dependencies; j++)
//			{
//
//				if (tmp->ID == id)
//				{
//					tmp->maximumNoStud = max;
//
//				}
//				else
//					if (tmp->next != NULL)
//						tmp = tmp->next;
//			}
//		}
//	}
//	else if (choice == 3)
//	{
//		int creditH;
//		cout << "Edit CreditHours: "; cin >> creditH;
//		for (int i = 0; i < vertix.size(); i++)
//		{
//			tmp = vertix[i].headElement;
//			for (int j = 0; j < vertix[i].dependencies; j++)
//			{
//
//				if (tmp->ID == id)
//				{
//					tmp->CreditHours = creditH;
//
//				}
//				else
//					if (tmp->next != NULL)
//						tmp = tmp->next;
//			}
//		}
//	}
//	else if (choice == 4)
//	{
//		int oldid; cout << "Enter PreCourse ID: "; cin >> oldid;
//		int index;
//		bool b = 0;
//		for (int i = 0; i < vertix.size(); i++)
//		{
//			if (b == 1)
//				break;
//			tmp2 = vertix[i].headElement;
//			for (int j = 0; j < vertix[i].dependencies; j++)
//			{
//
//				if (tmp2->ID == oldid)
//				{
//					index = i;
//					b = 1;
//					break;
//				}
//				else
//					if (tmp2->next != NULL)
//						tmp2 = tmp2->next;
//			}
//		}
//		vertix[index].Append(tmp->name, tmp->ID);
//		vertix[deletedindex].DeleteAt(id);
//	}
//}