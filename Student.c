#include<stdlib.h>
#include<unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
struct student student;

void process_course_id(int ID,char* login){
    FILE *file = fopen("course.txt", "r+");
    struct course course;
    if (file == NULL) {
        perror("fopen");
        exit(1);
    }
    FILE *file5 = fopen("course_enrolled.txt", "a+");
    //struct course course;
    if (file5 == NULL) {
        perror("fopen");
        exit(1);
    }
    int fd2 = fileno(file);
    char ack[2];
    int val[1];
    long pos=-1;
    int curr_seats = 0;
    while (fscanf(file, "Faculty_id: %49s\n", course.faculty_id) == 1) {  
        pos = ftell(file);
        fscanf(file, "ID: %10d\n", &course.id);
        fscanf(file, "Name: %49s\n", course.name);
        fscanf(file, "Faculty_name: %49s\n", course.faculty_name);
        fscanf(file, "Maximum Seats: %10d\n", &course.seats);
        fscanf(file, "Course Code: %99s\n", course.code);
       
    if(course.id == ID){  //Writing in course_enrolled file
        fprintf(file5, "Student_id: %s\n", login);
        fprintf(file5, "ID: %d\n", course.id);
        fprintf(file5, "Faculty_id: %s\n", course.faculty_id);
        fprintf(file5, "Name: %s\n", course.name);
        fprintf(file5, "Faculty_name: %s\n", course.faculty_name);
        fprintf(file5, "Course Code: %s\n", course.code);
        curr_seats = course.seats;
        fclose(file5);
        break;
    }    
    }
    course.seats--;
    lseek(fd2,pos,SEEK_SET);
    fprintf(file, "ID: %d\n", course.id);
    fprintf(file, "Name: %s\n", course.name);
    fprintf(file, "Faculty_name: %s\n", course.faculty_name);
    fprintf(file, "Maximum Seats: %d\n", course.seats);
    fprintf(file,"Course Code: %s\n", course.code); 
    fclose(file);
    close(fd2);
}


int searchStudentByID(char *ID,int client_socket,char *password,int br) {
            FILE *file = fopen("student_data.txt", "r");
            if (file == NULL) {
                perror("File opening failed");
                return -1;
            }

            int found = 0; // Flag to check if the student was found
            while (fscanf(file, "ID: %s\n", student.id) == 1) { // Read ID and check if successful
            fscanf(file, "Password: %49s\n", student.password);
            fscanf(file, "Name: %49s\n", student.name);
            fscanf(file, "Email: %49s\n", student.email);
            fscanf(file, "Address: %99s\n", student.address);
            int ver[2];
            if (strcmp(student.id,ID)==0) {
                printf("Student_password:%s\n",student.password);
            // Student found, print their information
                if(strcmp(student.password,password)==0){
                    ver[0] = 0;
                    char buff_[5];
                    send(client_socket,ver,sizeof(ver),0);
                    recv(client_socket,buff_,sizeof(buff_),0);
                    break;
                }
                else{
                    ver[0] = 1;         
                    send(client_socket,ver,sizeof(ver),0);
                    char buff_[5];
                    recv(client_socket,buff_,sizeof(buff_),0);
                    br = 1;
                    break;
                }
            }
        }
        return br;
    }
void performStudent_Task(int choice, int client_socket,char* login) {
    switch (choice) {
        case 1: //View Offering courses
            FILE *file = fopen("course.txt", "r");
            struct course course;
            if (file == NULL) {
                perror("fopen");
                exit(1);
            }
            char ack[2];
            int val[1];

            while (fscanf(file, "Faculty_id: %49s\n", course.faculty_id) == 1) { // Read ID and check if successful
                fscanf(file, "ID: %10d\n", &course.id);
                fscanf(file, "Name: %49s\n", course.name);
                fscanf(file, "Faculty_name: %49s\n", course.faculty_name);
                fscanf(file, "Maximum Seats: %10d\n", &course.seats);
                fscanf(file, "Course Code: %99s\n", course.code);
                send(client_socket,"Start",6,0);
                char repl1[5];
                memset(repl1, 0, sizeof(repl1));
                recv(client_socket,repl1,sizeof(repl1),0);

                val[0] = course.id;
                send(client_socket,val,sizeof(val),0);
                recv(client_socket,ack,sizeof(ack),0);
                ack[0] = '\0'; 

                send(client_socket,course.name,strlen(course.name),0);
                recv(client_socket,ack,sizeof(ack),0);
                ack[0] = '\0';

                send(client_socket,course.faculty_name,strlen(course.faculty_name),0);
                recv(client_socket,ack,sizeof(ack),0);
                ack[0] = '\0';

                val[0] = course.seats;
                send(client_socket,val,sizeof(val),0);
                recv(client_socket,ack,sizeof(ack),0);
                ack[0] = '\0';

                send(client_socket,course.code,strlen(course.code),0);
                recv(client_socket,ack,sizeof(ack),0); 
            }                   
            send(client_socket,"Done",5,0);
            char repl[5];
            recv(client_socket,repl,sizeof(repl),0);
            fclose(file);
            break;
        case 2://Enrolling in a course
            int opted[2];
            send(client_socket,"Enter Course ID you want to Enroll in\n",50,0);
            recv(client_socket,opted,sizeof(opted),0);
            process_course_id(opted[0],login);
            break;
        case 3:
            
        case 4:
           
        case 9:
          
        default:
            // Invalid choice
            send(client_socket, "Invalid choice.\n", 16, 0);
            break;
    }
    return;
}

//CLIENT SIDE 
void Student(int choice, int client_socket,char* login){
        switch (choice) {
        case 1:
            while(1){
            printf("********************************************************************************\n");
            char rec[6];
            memset(rec, 0, sizeof(rec));
            recv(client_socket,rec,sizeof(rec),0);
            send(client_socket,"ok",2,0);
            if(strcmp("Done",rec)==0){
                break;
            }
           
            int info[10];
            memset(info, 0, sizeof(info));
            recv(client_socket,info,sizeof(info),0);
            printf("CourseID:%d\n",info[0]);
            send(client_socket,"Ok",2,0);

            char info1[20];
            memset(info1, 0, sizeof(info1));
            recv(client_socket,info1,sizeof(info1),0);
            info1[strcspn(info1, "\n")] = '\0';
            printf("Course Name:%s\n",info1);
            send(client_socket,"Ok",2,0);

            char info2[20];
            memset(info2, 0, sizeof(info2));
            recv(client_socket,info2,sizeof(info2),0);
            info2[strcspn(info2, "\n")] = '\0';
            printf("Course Faculty_name:%s\n",info2);
            send(client_socket,"Ok",2,0);

            int info3[20];
            recv(client_socket,info3,sizeof(info3),0);
            printf("Course Seats:%d\n",info3[0]);
            send(client_socket,"Ok",2,0);
            
            char info4[20];
            memset(info4, 0, sizeof(info4));
            recv(client_socket,info4,sizeof(info4),0);
            info4[strcspn(info4, "\n")] = '\0';
            printf("Course Code:%s\n",info4);
            send(client_socket,"Ok",2,0);   
            printf("********************************************************************\n");        
        }
            break;
        case 2:
            char buff5[50];
            recv(client_socket,buff5,sizeof(buff5),0);
            printf("%s\n",buff5);
            int opt1[2];
            scanf("%d",&opt1[0]); //Entering course id
            send(client_socket,opt1,sizeof(opt1),0);
        case 3:
            
        case 4:
           

        default:
            // Invalid choice
            send(client_socket, "Invalid choice.\n", 16, 0);
            break;
    }

    return;

}