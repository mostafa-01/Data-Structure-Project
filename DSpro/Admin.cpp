#include "Admin.h"
#include"pugixml.cpp"
using namespace pugi;
unordered_map<int, Graph>courseGraphs;
unordered_map<int, Student>studsInfo;
int number;

Admin::Admin() {
	name = "ADMIN";
	password = "adminadmin";
	Fill();
	FillStud();
	//courseGraphs[1].traverse();
}
void Admin::AdminPage() {
	char cont;
	int choice;
	int id, gid;
	do {
		cout << "[1]Add student\n[2]Add new course\n" <<
			"[3]View list of all students in a specific course\n[4]View list of all courses"
			<< " of a specific student\n[5]Edit course's data\n\n";
		cin >> choice;
		switch (choice)
		{
		case 1: AddStudent();
			break;
		case 2: AddCourse();
			break;
		case 3:
			cout << "Please enter the course's id: ";
			cin >> id;
			ViewStudentsInCourse(id);
			break;
		case 4:
			cout << "Please enter the student's id: ";
			cin >> id;
			FinProgCofStud(id);
			break;
		case 5:
			EditCourseData();
			break;
		default:
			AdminPage();
			break;
		}
		cout << "Do you want to continue? ";
		cin >> cont;

	} while (cont == 'y');
}
void Admin::FillStud() {
	xml_document doc;
	xml_parse_result result = doc.load_file("UserData.xml");
	int  i = 1;
	for (xml_node list = doc.child("student"); list; list = list.next_sibling("student")) {
		studsInfo[i].name = list.attribute("Name").value();
		studsInfo[i].pass = list.attribute("Password").value();
		studsInfo[i].ID = list.attribute("ID").as_int();
		studsInfo[i].year = list.attribute("Year").as_int();
		{
			string x = list.attribute("coursesInProgress").value();
			int l = 0;
			string temp = "";
			for (int j = 0; j < x.size(); j++)
			{
				if (x[j] != 32)
				{
					temp += x[j];
					l++;
				}
				else {
					char* t = new char[temp.size() + 1];
					strcpy(t, temp.c_str());
					studsInfo[i].CoursesinProg.push_back(stoi(t));
					temp = "";
					l = 0;
				}
			}
		}
		{
			string x = list.attribute("FinishedCourses").value();
			int l = 0;
			string temp = "";
			for (int j = 0; j < x.size(); j++)
			{
				if (x[j] != 32)
				{
					temp += x[j];
					l++;
				}
				else {
					char* t = new char[temp.size() + 1];
					strcpy(t, temp.c_str());
					studsInfo[i].finCourses.push_back(stoi(t));
					temp = "";
					l = 0;
				}
			}
		}
		i++;
	}
}
void Admin::Fill() {
	xml_document doc2;
	xml_parse_result result2 = doc2.load_file("NumberOfGraphs.xml");
	number = doc2.child("Graphs").attribute("number").as_int();

	for (int k = 1; k <= number; k++)
	{
		name = "Graph" + to_string(k) + "Courses.xml";
		char* namechar = new char[name.size() + 1];
		strcpy(namechar, name.c_str());
		Course co[1000] = {};
		xml_document doc;
		xml_parse_result result = doc.load_file(namechar);
		int  i = 0;
		for (xml_node list = doc.child("course"); list; list = list.next_sibling("course")) {

			for (xml_node insideList = doc.child(list.attribute("name").value()); insideList; insideList = insideList.next_sibling(list.attribute("name").value())) {
				co[i].name = insideList.attribute("name").value();
				co[i].ID = stoi((insideList.attribute("ID").value()));
				co[i].maxStudents = stoi(insideList.attribute("max").value());
				co[i].currentSudents = stoi(insideList.attribute("current").value());
				co[i].creditHours = stoi(insideList.attribute("creditHours").value());
				co[i].parentID = stoi(insideList.attribute("parent").value());
				i++;
			}
		}
		Graph gtemp;
		for (int k = 0; k < i; k++)
		{
			for (int j = 0; j < i; j++) {
				if (co[j].ID == co[k].parentID) {
					gtemp.gID = 1;
					gtemp.pushc(co[k], co[j]);
					break;
				}
			}
		}
		courseGraphs[k] = gtemp;
	}
}
void Admin::Store() {
	xml_document doc2;
	xml_parse_result result2 = doc2.load_file("NumberOfGraphs.xml");
	xml_attribute attr = doc2.child("Graphs").attribute("number");
	attr.set_value(courseGraphs.size());
	doc2.save_file("NumberOfGraphs.xml");

	string name;
	for (int k = 1; k <= courseGraphs.size(); k++) {
		name = "Graph" + to_string(k) + "Courses.xml";
		char* namechar = new char[name.size() + 1];
		strcpy(namechar, name.c_str());
		xml_document doc;
		xml_parse_result result = doc.load_file(namechar);
		doc.reset();
		for (int i = 0; i < courseGraphs[k].vertix.size(); i++) {
			xml_node node = doc.append_child("course");
			string vertixname = courseGraphs[k].vertix[i].headElement->name;
			char* c = new char[vertixname.size() + 1];
			strcpy(c, vertixname.c_str());

			Course* tmp = courseGraphs[k].vertix[i].headElement;
			node.append_attribute("name") = c;

			for (int j = 0; j < courseGraphs[k].vertix[i].dependencies; j++) {

				if (i != 0) {
					tmp = tmp->next;
					j++;
				}

				xml_node child = doc.append_child(c);
				string x;
				char* n = new char[tmp->name.size() + 1];
				strcpy(n, tmp->name.c_str());
				x = to_string(tmp->ID);
				char* id = new char[x.size() + 1];
				strcpy(id, x.c_str());
				x = to_string(tmp->maxStudents);
				char* max = new char[x.size() + 1];
				strcpy(max, x.c_str());
				x = to_string(tmp->currentSudents);
				char* current = new char[x.size() + 1];
				strcpy(current, x.c_str());
				x = to_string(tmp->creditHours);
				char* ch = new char[x.size() + 1];
				strcpy(ch, x.c_str());
				x = to_string(tmp->parentID);
				char* parent = new char[x.size() + 1];
				strcpy(parent, x.c_str());

				child.append_attribute("name") = n;
				child.append_attribute("ID") = id;
				child.append_attribute("max") = max;
				child.append_attribute("current") = current;
				child.append_attribute("creditHours") = ch;
				child.append_attribute("parent") = parent;
				if (i == 0 && tmp->next != NULL)
					tmp = tmp->next;
			}
		}
		doc.save_file(namechar);
	}

}
void Admin::AddCourse() {
	Course newCourse;
	bool courseExist = 0;
	cout << "Graph ID?\n";
	int gID;
	cin >> gID;
	while (true) {
		cout << "\nID: ";
		cin >> newCourse.ID;
		if (courseGraphs[gID].Find(newCourse.ID).ID == 0)
			break;
		else
			cout << "course already exists\n";
	}
	cout << "\nName: ";
	cin >> newCourse.name;
	cout << "\nMax number: ";
	cin >> newCourse.maxStudents;
	cout << "\nCredit hours: ";
	cin >> newCourse.creditHours;
	cout << "\nParent course ID: ";
	cin >> newCourse.parentID;



	Graph temp;
	Course parent;
	Graph temp2;
	temp = courseGraphs[gID];
	temp2 = courseGraphs[gID];
	if (temp2.Find(newCourse.parentID).ID == 0) {
		if (newCourse.parentID == newCourse.ID) {
			temp.pushc(newCourse, newCourse);
			courseGraphs[gID] = temp;
			//courseGraphs[gID].traverse();
		}
		else
			cout << "Wrong input!!!!!!!!!\n";
	}
	else {
		parent = temp2.Find(newCourse.parentID);
		temp.gID = gID;
		temp.pushc(newCourse, parent);
		courseGraphs[gID] = temp;
	}
}
Student Admin::fillStudentData() {
	Student newStudent;
	cout << "Username:\n";
	cin >> newStudent.name;
	cout << "Password:\n";
	cin >> newStudent.pass;
	cout << "Year:\n";
	cin >> newStudent.year;
	return newStudent;
}
void Admin::StoreStud() {
	xml_document doc;
	xml_parse_result result = doc.load_file("UserData.xml");
	doc.reset();
	for (int i = 1; i <= studsInfo.size(); i++)
	{
		xml_node node = doc.append_child("student");
		string x;
		char* n = new char[studsInfo[i].name.size() + 1];
		strcpy(n, studsInfo[i].name.c_str());

		char* pass = new char[studsInfo[i].pass.size() + 1];
		strcpy(pass, studsInfo[i].pass.c_str());

		x = to_string(studsInfo[i].ID);
		char* id = new char[x.size() + 1];
		strcpy(id, x.c_str());

		x = to_string(studsInfo[i].year);
		char* year = new char[x.size() + 1];
		strcpy(year, x.c_str());

		string progress = "";
		for (int j = 0; j < studsInfo[i].CoursesinProg.size(); j++)
		{
			progress += to_string(studsInfo[i].CoursesinProg[j]) + " ";
		}
		char* p = new char[progress.size() + 1];
		strcpy(p, progress.c_str());
		string finished = "";
		for (int j = 0; j < studsInfo[i].finCourses.size(); j++)
		{
			finished += to_string(studsInfo[i].finCourses[j]) + " ";
		}
		char* f = new char[finished.size() + 1];
		strcpy(f, finished.c_str());
		node.append_attribute("Name") = n;
		node.append_attribute("Password") = pass;
		node.append_attribute("ID") = id;
		node.append_attribute("Year") = year;
		node.append_attribute("coursesInProgress") = p;
		node.append_attribute("FinishedCourses") = f;
	}
	doc.save_file("UserData.xml");
	
}
void Admin::AddStudent() {
	Student newStudent = fillStudentData();
	newStudent.ID = studsInfo.size() + 1;
	studsInfo[newStudent.ID] = newStudent;
}
void Admin::EditCourseName(int id, int gID) {
	string n;
	Course* tmp = courseGraphs[gID].vertix[0].headElement;
	cout << "Edit name: "; cin >> n;
	for (int i = 0; i < courseGraphs[gID].vertix.size(); i++)
	{
		tmp = courseGraphs[gID].vertix[i].headElement;
		for (int j = 0; j < courseGraphs[gID].vertix[i].dependencies; j++)
		{

			if (tmp->ID == id)
			{
				tmp->name = n;
			}
			else
				if (tmp->next != NULL)
					tmp = tmp->next;
		}
	}
}
void Admin::EditCourseMaxStud(int id, int gID) {
	int max;
	Course* tmp = courseGraphs[gID].vertix[0].headElement;
	cout << "Edit maximum number of students: "; cin >> max;
	for (int i = 0; i < courseGraphs[gID].vertix.size(); i++)
	{
		tmp = courseGraphs[gID].vertix[i].headElement;
		for (int j = 0; j < courseGraphs[gID].vertix[i].dependencies; j++)
		{

			if (tmp->ID == id)
			{
				tmp->maxStudents = max;

			}
			else
				if (tmp->next != NULL)
					tmp = tmp->next;
		}
	}
}
void Admin::EditCourseCH(int id, int gID) {
	Course* tmp = courseGraphs[gID].vertix[0].headElement;
	int creditH;
	cout << "Edit CreditHours: "; cin >> creditH;
	for (int i = 0; i < courseGraphs[gID].vertix.size(); i++)
	{
		tmp = courseGraphs[gID].vertix[i].headElement;
		for (int j = 0; j < courseGraphs[gID].vertix[i].dependencies; j++)
		{

			if (tmp->ID == id)
			{
				tmp->creditHours = creditH;

			}
			else
				if (tmp->next != NULL)
					tmp = tmp->next;
		}
	}
}
void Admin::EditCourseParent(int id, int gID) {
	Course* tmp = courseGraphs[gID].vertix[0].headElement;
	int oldid; cout << "Enter PreCourse ID: "; cin >> oldid;
	Course* tmp2 = tmp;
	int deletedindex;
	bool brz = 0;
	for (int i = 0; i < courseGraphs[gID].vertix.size(); i++)
	{
		if (brz == 1)
			break;
		tmp = courseGraphs[gID].vertix[i].headElement;
		for (int j = 0; j < courseGraphs[gID].vertix[i].dependencies; j++)
		{

			if (tmp->ID == id)
			{
				deletedindex = i;
				brz = 1;
				break;
			}
			else
				if (tmp->next != NULL)
					tmp = tmp->next;
		}
	}
	int index;
	bool b = 0;
	for (int i = 0; i < courseGraphs[gID].vertix.size(); i++)
	{
		if (b == 1)
			break;
		tmp2 = courseGraphs[gID].vertix[i].headElement;
		for (int j = 0; j < courseGraphs[gID].vertix[i].dependencies; j++)
		{

			if (tmp2->ID == oldid)
			{
				index = i;
				b = 1;
				break;
			}
			else
				if (tmp2->next != NULL)
					tmp2 = tmp2->next;
		}
	}
	courseGraphs[gID].vertix[index].Append(tmp->name, tmp->ID, tmp->maxStudents, tmp->currentSudents, tmp->creditHours, tmp->parentID);
	courseGraphs[gID].vertix[deletedindex].DeleteAt(id);
}
void Admin::EditCourseData()
{
	char cont;
	int choice;
	int id, gid;
	do {
		cout << "[1]Edit Course's Name\n[2]Edit Course's Max Number of Students\n[3]Edit Course Credit Hours\n" <<
			"[4]Edit Course's Parent\n\n";
		cin >> choice;
		switch (choice)
		{
		case 1:
			cin >> id;
			cout << "Please enter the course's id" << endl;
			cin >> gid;
			EditCourseName(id, gid);
			break;
		case 2:
			cin >> id;
			cout << "Please enter the course's id" << endl;
			cin >> gid;
			EditCourseMaxStud(id, gid);
			break;
		case 3: cout << "Please enter the course's id: ";
			cin >> id;
			cout << "Please enter the graph id of this course: ";
			cin >> gid;
			EditCourseCH(id, gid);
			break;
		case 4:
			cout << "Please enter the course's id: ";
			cin >> id;
			cout << "Please enter the graph's id of this course: ";
			cin >> gid;
			EditCourseParent(id, gid);
			break;
		default:
			EditCourseData();
			break;
		}
		cout << "Do you want to continue? ";
		cin >> cont;
	} while (cont == 'y');
}

void Admin::FinProgCofStud(int studID) {
	cout << "Finshed courses are:\n";
	for (int i = 0; i < studsInfo[studID].finCourses.size(); i++)
		cout << studsInfo[studID].finCourses[i] << endl;

	cout << "Inprogress courses are:\n";
	for (int i = 0; i < studsInfo[studID].CoursesinProg.size(); i++)
		cout << studsInfo[studID].CoursesinProg[i] << endl;

}
void Admin::ViewStudentsInCourse(int courseID) {
	bool noStudents = true;
	cout << "Students:\n";
	for (int i = 1; i <= studsInfo.size(); i++) {
		for (int j = 0; j < studsInfo[i].CoursesinProg.size(); j++) {
			if (studsInfo[i].CoursesinProg[j] == courseID) {
				cout << studsInfo[i].name << endl;
				noStudents = false;
				break;
			}
		}
	}
	if (noStudents)
		cout << "No students are inrolled yet in this course...\n";
}

Admin::~Admin() {
	Store();
	StoreStud();
}

//////////////////////////////////////////////////

Student::Student()
{

	ID = 1;
	year = 1;
	maxCH = 14;
	name = "tefa";
	pass = "amrfunky";
}
void Student::StudentPage(int id)
{
	char cont;
	int choice;
	do {
		cout << "[1]Show Available Courses\n[2]Show Details of a specific course\n" <<
			"[3]View all your courses\n[4]Register for a course"
			<< "\n[5]Edit Your Data\n\n";
		cin >> choice;
		switch (choice)
		{
		case 1:
			availableCourses(id);
			break;
		case 2:
			showSpecCourse();
			break;
		case 3:
			showAllhisCourses(id);
			break;
		case 4:
			RegCourse(id);
			break;
		case 5:
			editData(id);
			break;
		default:
			StudentPage(id);
			break;
		}
		cout << "Do you want to continue? ";
		cin >> cont;

	} while (cont == 'y');
}
void Student::Login() {
	string username;
	string password;
	cout << "To login\n";
	cout << "Please enter your username: ";
	cin >> username;
	cout << "Please enter your password: ";
	cin >> password;
	if (username == "ADMIN" && password == "adminadmin") {
		cout << "Welcome our admin\n";
		Admin ad;
		ad.AdminPage();
		return;
	}
	else {
		for (int i = 1; i <= studsInfo.size(); i++)
		{
			if (studsInfo[i].name == username && studsInfo[i].pass == password) {
				cout << "Logged in successfully\n";
				StudentPage(studsInfo[i].ID);
				return;
			}
		}
	}
	cout << "Username or password is incorrect\n";
	Login();
}

void Student::editData(int id)
{
	string newName;
	string newPass;
	cout << "1.Edit Name\n2.Edit Password\n3.Completed Course\n4.Edit Year\n";
	int choice; cin >> choice;
	if (choice == 1)
	{
		cout << "Please enter new name: ";
		cin >> newName;
		studsInfo[id].name = newName;
	}
	else if (choice == 2)
	{
		cout << "Please enter new password: ";
		cin >> newPass;
		studsInfo[id].pass = newPass;
	}
	else if (choice == 3)
	{
		cout << "Please enter completed course ID: ";
		int CourseID; cin >> CourseID;
		vector<int>::iterator it1 = studsInfo[id].CoursesinProg.begin();
		while (it1 != studsInfo[id].CoursesinProg.end())
		{
			if (CourseID == *it1)
			{
				studsInfo[id].CoursesinProg.erase(it1);
				studsInfo[id].finCourses.push_back(CourseID);
				break;
			}
			it1++;
		}
	}
	else if (choice == 4)
	{
		int newYear;
		cout << "Enter your year honestly: ";
		cin >> newYear;
		studsInfo[id].year = newYear;
	}
}

int Student::availableCourses(int id) {
	for (int k = 1; k <= number; k++) {
		vector<collectdata>::iterator it = courseGraphs[k].vertix.begin();
		cout << "Available Courses:\n";
		if (studsInfo[id].finCourses.size() == 0) {//Special if he didn't take intro course

			{
				cout << "Name: " << courseGraphs[k].vertix[0].headElement->name << endl;
				cout << "CreditHours: " << courseGraphs[k].vertix[0].headElement->creditHours << endl;
				return 0;
			}
		}
		{
			while (it != courseGraphs[k].vertix.end())
			{
				Course* tmp = it->headElement->next;
				bool found = false;
				bool must = false;
				for (int i = 0; i < studsInfo[id].finCourses.size(); i++)
				{
					if (it->headElement->ID == studsInfo[id].finCourses[i])
					{
						must = true;
					}
				}
				while (tmp != NULL)
				{
					for (int i = 0; i < studsInfo[id].finCourses.size(); i++)
					{
						if (tmp->ID == studsInfo[id].finCourses[i])
						{
							found = true;
							break;
						}
					}
					if (found == false && must == true)
					{
						cout << "ID: " << tmp->ID << endl;
						cout << "Name: " << tmp->name << endl;
						cout << "CreditHours: " << tmp->creditHours << endl;
					}
					tmp = tmp->next;
				}
				it++;
			}
		}
	}
	return 0;
}

void Student::showSpecCourse()
{
	cout << "Enter Course ID: "; int courseID; cin >> courseID;
	Course specific;
	for (int i = 1; i <= number; i++) {
		if (courseGraphs[i].Find(courseID).ID != 0) {
			specific = courseGraphs[i].Find(courseID);
			break;
		}
	}
	if (specific.ID != 0) {

		cout << "Name : " << specific.name << endl;
		cout << "CreditHours: " << specific.creditHours << endl;
		cout << "Maximum Number of Students: " << specific.maxStudents << endl;
		cout << "PreCourse ID: " << specific.parentID << endl;
	}
	else {
		cout << "not availabe\n";
	}
}

void Student::showAllhisCourses(int id) {
	bool found = 0;
	cout << "In Progress Courses: ";
	Course specific;
	for (int i = 0; i < studsInfo[id].CoursesinProg.size(); i++)
	{
		for (int j = 1; j <= number; j++)

		{
			if (courseGraphs[j].Find(studsInfo[id].CoursesinProg[i]).ID != 0) {
				specific = courseGraphs[j].Find(studsInfo[id].CoursesinProg[i]);
				found = 1;
				break;
			}
		}
		cout << "Name : " << specific.name << endl;
		cout << "CreditHours: " << specific.creditHours << endl;
		cout << "Maximum Number of Students: " << specific.maxStudents << endl;
		cout << "PreCourse ID: " << specific.parentID << endl;
	}
	if (found == 0)
		cout << "No courses are inprogressed...\n";
	found = 0;
	cout << "Finished Courses: ";

	for (int i = 0; i < studsInfo[id].finCourses.size(); i++)
	{
		for (int j = 1; j <= number; j++)
		{

			if (courseGraphs[j].Find(studsInfo[id].finCourses[i]).ID != 0) {
				specific = courseGraphs[j].Find(studsInfo[id].finCourses[i]);
				found = 1;
				break;
			}
		}
		cout << "Name : " << specific.name << endl;
		cout << "CreditHours: " << specific.creditHours << endl;
		cout << "Maximum Number of Students: " << specific.maxStudents << endl;
		cout << "PreCourse ID: " << specific.parentID << endl;
		cout << endl;
	}
	if (found == 0)
		cout << "No courses are finished...\n";
}

void Student::RegCourse(int id)
{
	cout << "Enter the Course ID: ";
	int tmpid;
	cin >> tmpid;
	Course specific;
	for (int i = 0; i < studsInfo[id].finCourses.size(); i++)
	{
		if (tmpid == studsInfo[id].finCourses[i])
		{
			cout << "You have already taken this course\n";
			return;
		}
	}

	for (int i = 0; i < studsInfo[id].CoursesinProg.size(); i++)
	{
		if (tmpid == studsInfo[id].CoursesinProg[i])
		{
			cout << "You are already inrolled this course\n";
			return;
		}
	}
	for (int i = 1; i <= number; i++) {
		if (courseGraphs[i].Find(tmpid).ID != 0) {
			specific = courseGraphs[i].Find(tmpid);
			break;
		}
	}

	if (specific.ID == 0) {
		cout << "There's no course with this ID\n";
		return;
	}

	bool Foundparent = false;
	if (specific.parentID == specific.ID) {
		Foundparent = true;
	}
	else {
		for (int i = 0; i < studsInfo[id].finCourses.size(); i++) {
			if ((specific.parentID == studsInfo[id].finCourses[i])) {
				Foundparent = true;
				break;
			}
		}
	}
	/*while (it != courseGraphs[1].vertix.end())
	{
		if (tmpid == it->headElement->ID)
		{
			FoundInparent = true;
		}
		it++;
	}
	*/
	if (Foundparent == false)
	{
		cout << "You must finish its PreCourse\n";
		return;
	}

	studsInfo[id].CoursesinProg.push_back(specific.ID);

	return;
}
