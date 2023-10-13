#include <stdlib.h>
#include<unistd.h>

struct student{
	char id[50];
	char name[50];
	char password[50];
	char email[50];
	char address[100];
};

struct Faculty{
	char id[20];
	char password[50];
	char name[50];
	char department[50];
	char email[50];
	char address[100];
	char designation[50];
	
};
struct course{
	char faculty_id[20];
	int id;
	char name[50];
	char faculty_name[50];
	int seats;
	char code[20];
};

struct course_enrolled{
	char student_id[30];
	int course_id;
	char name[50];
	char faculty_name[30];
	char code[20];
};