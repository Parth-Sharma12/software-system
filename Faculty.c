#include<stdlib.h>
#include<unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>

int Next_ID(){
    
    struct course course;
    FILE *file3 = fopen("course.txt", "a+");
    if (file3 == NULL) {
        perror("File opening failed");
        exit(1);
    }
    int highestNumericId1 = 0;

    while (fscanf(file3, "Faculty_id: %s\n", course.faculty_id) == 1) {
        // Read and discard the other information
        fscanf(file3, "ID: %d\n", &course.id);
        //printf("%d\n",course.id);
        if (course.id > highestNumericId1) {
            highestNumericId1 = course.id;
        }

        fscanf(file3, "Name: %s\n", course.name);
        fscanf(file3, "Faculty_name: %s\n", course.faculty_name);
        fscanf(file3, "Maximum Seats: %d\n", &course.seats);
        fscanf(file3, "Course Code: %s\n", course.code);
    }
    return highestNumericId1+1;
}

struct Faculty faculty;
int searchFacultyByID(char *ID,int client_socket,char *password,int br) {
        int file = open("faculty.txt", O_RDONLY);
        
        struct Faculty facultyinfo;
        int found = 0; // Flag to check if the student was found
        while (read(file,&facultyinfo,sizeof(struct Faculty))) { // Read ID and check if successfull
        int ver[2];
        if (strcmp(facultyinfo.id,ID)==0) {
        // Student found, print their information
            if(strcmp(facultyinfo.password,password)==0){
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
    close(file);
    return br;
}
void performFaculty_Task(int choice, int client_socket,char* login) {
    switch (choice) {
        case 1:
            int file6 = open("course.txt", O_RDWR);
            
            struct course course;
            send(client_socket, "Enter Course Name:", 40, 0);
            recv(client_socket, course.name, sizeof(course.name), 0);

            send(client_socket, "Enter faculty name:", 40, 0);
            recv(client_socket, course.faculty_name, sizeof(course.faculty_name), 0);
            
            int SEAT[2];
            send(client_socket, "Enter Seats: ", 14, 0);
            recv(client_socket, SEAT, sizeof(SEAT), 0);

            course.seats = SEAT[0];

            send(client_socket, "Enter Code ", 16, 0);
            recv(client_socket, course.code, sizeof(course.code), 0);

            int new_id = 1;
            struct course coursecpy;
            struct stat file_info;
            fstat(file6, &file_info);
            char New[20];
            if(file_info.st_size != 0){
                lseek(file6,-(sizeof(struct course)),SEEK_END);
                read(file6,&coursecpy,sizeof(struct course));
                new_id = coursecpy.id+1;
            }
            strcpy(course.faculty_id,login);
            course.id= new_id;
            lseek(file6,0,SEEK_END);
            write(file6,&course,sizeof(struct course));
            close(file6);            
            break;
            
        case 2:
            //struct course course;
            int file_show = open("course.txt",O_RDONLY);
            char ack[2];
            int val[1];
            struct course course_show;
            while (read(file_show,&course_show,sizeof(struct course))) { // Read ID and check if successful
                printf("Course name:%s\n",course_show.name);
                printf("fac name:%s\n",course_show.faculty_name);
                if(strcmp(course_show.faculty_id,login)==0){

                    send(client_socket,"Start",6,0);
                    char repl1[5];
                    memset(repl1, 0, sizeof(repl1));
                    recv(client_socket,repl1,sizeof(repl1),0);

                    val[0] = course_show.id;
                    send(client_socket,val,sizeof(val),0);
                    recv(client_socket,ack,sizeof(ack),0);
                    ack[0] = '\0'; 

                    send(client_socket,course_show.name,strlen(course_show.name),0);
                    recv(client_socket,ack,sizeof(ack),0);
                    ack[0] = '\0';

                    send(client_socket,course_show.faculty_name,strlen(course_show.faculty_name),0);
                    recv(client_socket,ack,sizeof(ack),0);
                    ack[0] = '\0';

                    val[0] = course_show.seats;
                    send(client_socket,val,sizeof(val),0);
                    recv(client_socket,ack,sizeof(ack),0);
                    ack[0] = '\0';

                    send(client_socket,course_show.code,strlen(course_show.code),0);
                    recv(client_socket,ack,sizeof(ack),0); 
                }               
            }    
            send(client_socket,"Done",5,0);
            char repl[5];
            recv(client_socket,repl,sizeof(repl),0);
            close(file_show);
            break;
        case 3:
            int upd = open("course.txt",O_RDWR);
            int a1[2];
            send(client_socket,"Enter course id to update",strlen("Enter course id to update"),0);
            recv(client_socket,a1,sizeof(a1),0);
            int targetcourseID = a1[0];
            struct course course_update;
            long position = -1; // Position of the record to update

            // Find the record with the matching Faculty_id
            while (read(upd,&course_update,sizeof(struct course))) {
                if(course_update.id == targetcourseID){
                    lseek(upd,-(sizeof(struct course)),SEEK_CUR);
                    struct course updatecpy;
                    send(client_socket,"Choose from the following which information you want to update\n 1) Maximum_Seats\n 2)Course_Code\n 3) Course Name\n",1000,0);
                    int choice2[1];
                    recv(client_socket,choice2,sizeof(choice2),0);
                    if(choice2[0] == 1 ){
                        int k[1];
                        send(client_socket,"Enter the number of seats you want",100,0);
                        recv(client_socket,k,sizeof(k),0);
                        updatecpy.id=course_update.id;
                        strcpy(updatecpy.faculty_id,course_update.faculty_id);
                        strcpy(updatecpy.faculty_name,course_update.faculty_name);
                        strcpy(updatecpy.name,course_update.name);
                        strcpy(updatecpy.code,course_update.code);
                        updatecpy.seats = k[0];
                        write(upd,&updatecpy,sizeof(struct course));
                    }
                    if(choice2[0] == 2 ){
                        char new_code[20];
                        send(client_socket,"Enter the New Course Code",100,0);
                        recv(client_socket,new_code,sizeof(new_code),0);
                        updatecpy.id = course_update.id;
                        strcpy(updatecpy.faculty_id,course_update.faculty_id);
                        strcpy(updatecpy.faculty_name,course_update.faculty_name);
                        strcpy(updatecpy.name,course_update.name);
                        updatecpy.seats= course_update.seats;
                        strcpy(updatecpy.code,new_code);
                        write(upd,&updatecpy,sizeof(struct course));
                    }
                    if(choice2[0] == 3){
                        char new_name[50];
                        send(client_socket,"Enter New Name of the course",100,0);
                        recv(client_socket,new_name,sizeof(new_name),0);
                        updatecpy.id = course_update.id;
                        strcpy(updatecpy.faculty_id,course_update.faculty_id);
                        strcpy(updatecpy.faculty_name,course_update.faculty_name);
                        strcpy(updatecpy.code ,course_update.code);
                        updatecpy.seats= course_update.seats;
                        strcpy(updatecpy.name,new_name);
                        write(upd,&updatecpy,sizeof(struct course));
                    }
                }
            }
            break;  
        case 4:
            
        case 9:
            
        default:
            
    }
    return;
}

//CLIENT SIDE 
void Faculty(int choice,int client_socket,char *login){
        struct course course1;
        switch (choice) {
        case 1:
            char c_Name[50];
            recv(client_socket,c_Name,sizeof(c_Name),0);
            printf(" %s\n",c_Name);
            scanf("%s",course1.name);
            course1.name[strcspn(course1.name, "\n")] = '\0'; // Remove the newline character
            send(client_socket,course1.name, sizeof(course1.name), 0);

            char Fac_name[100]; 
            recv(client_socket,Fac_name,sizeof(Fac_name),0);
            printf("%s\n",Fac_name);
            scanf("%s",course1.faculty_name);
            course1.faculty_name[strcspn(course1.faculty_name, "\n")] = '\0'; // Remove the newline character
            send(client_socket,course1.faculty_name, sizeof(course1.faculty_name), 0);

            char seats[100];
            int seat[2];
            recv(client_socket,seats,sizeof(seats),0);
            printf(" %s\n",seats);
            scanf("%d",&seat[0]);
            //course.seats[strcspn(course.seats, "\n")] = '\0';
            send(client_socket,seat,sizeof(seat), 0);

            char code[100];
            recv(client_socket,code,sizeof(code),0);
            printf(" %s\n",code);
            scanf("%s",course1.code);
            course1.code[strcspn(course1.code, "\n")] = '\0';     
            send(client_socket, course1.code, sizeof(course1.code), 0);
            break;
            
            /*char mem[100];
            recv(client_socket,mem,sizeof(mem),0);
            printf("%s\n",mem);
            struct course course;
            course.id = 1;
            char input[50];
            input[0] = '\0';
            

            printf("Enter name of the Course\n:");
            scanf("%s",input);
            input[strcspn(input, "\n")] = '\0';
            strcpy(course.name, input);
            input[0] = '\0';

            printf("Enter Name of the faculty\n:");
            scanf("%s",input);
            input[strcspn(input, "\n")] = '\0';
            strcpy(course.faculty_name, input);
            input[0] = '\0';

            int seats[2];
            printf("Enter Seats of the course\n:");
            scanf("%d",seats);
            //seats[strcspn(seats, "\n")] = '\0';
            course.seats = seats[0];

            printf("Enter Code of the course\n:");
            scanf("%s",input);
            input[strcspn(input, "\n")] = '\0';
            strcpy(course.code, input);
            input[0] = '\0';
            
             printf("course:%s\n",course.name);
            printf("faculty:%s\n",course.faculty_name);
            printf("seats:%d\n",course.seats);
            printf("Code:%s\n",course.code);
            send(client_socket, &course, sizeof(course), 0);
            break;*/
        case 2:
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
        case 3:
            char toupdate[50];
            recv(client_socket,toupdate,sizeof(toupdate),0);
            int inp[1];
            printf("%s\n",toupdate);
            scanf("%d",&inp[0]);
            send(client_socket,inp,sizeof(inp),0);

            char BUFF[1000];
            recv(client_socket,BUFF,sizeof(BUFF),0);
            printf("%s\n",BUFF);
            int choice1[1];
            scanf("%d",&choice1[0]);
            send(client_socket,choice1,sizeof(choice1),0);

            char BUFF1[100];
            recv(client_socket,BUFF1,sizeof(BUFF1),0);
            printf("%s\n",BUFF1);
            if(choice1[0]==1){
                int k2[1];
                scanf("%d",&k2[0]);
                send(client_socket,k2,sizeof(k2),0);
            }
            else{
                char BUFF2[100];
                scanf("%s",BUFF2);
                send(client_socket,BUFF2,sizeof(BUFF2),0);
            }
            break;
        case 4:
            
        default:
            // Invalid choice
            send(client_socket, "Invalid choice.\n", 16, 0);
            break;
    }

    return;

}