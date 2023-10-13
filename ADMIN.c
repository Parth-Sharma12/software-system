#include<stdlib.h>
#include<unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "Structures.c"

#include <fcntl.h>
int extractNumericId1(const char *id) {
        int numericId;
        sscanf(id, "MT@%d", &numericId);
        return numericId;
}
void generateNewId1(char *newId, int highestId) {
        sprintf(newId, "MT@%d", highestId + 1);
}

void performAdmin_Task(int choice, int client_socket,int id) {
    switch (choice) {
        case 1: //Add user
            int file3 = open("student_data.txt", O_RDWR);
            
            struct student student;
            send(client_socket, "Enter password:", 40, 0);
            recv(client_socket, student.password, sizeof(student.password), 0);

            send(client_socket, "Enter name:", 40, 0);
            recv(client_socket, student.name, sizeof(student.name), 0);
        
            send(client_socket, "Enter email: ", 14, 0);
            recv(client_socket, student.email, sizeof(student.email), 0);

            send(client_socket, "Enter address: ", 16, 0);
            recv(client_socket, student.address, sizeof(student.address), 0);

            int new_id = 0;
            struct student stucpy;
            char New[20];
            if(SEEK_END>0){
                lseek(file3,-(sizeof(struct student)),SEEK_END);
                read(file3,&stucpy,sizeof(struct student));
                generateNewId1(New,extractNumericId1(stucpy.id));
            }
            
            strcpy(student.id,New);
            lseek(file3,0,SEEK_END);
            write(file3,&student,sizeof(struct student));
            close(file3);            
            break;
        case 2:
            char ID[20];
            int found = 0;
            send(client_socket,"Enter Id to search",30,0);
            recv(client_socket,ID,sizeof(ID),0);
            void searchStudentByID(char *ID) {
                struct student student;
                int fd1 = open("student_data.txt", O_RDONLY);

                int numid = extractNumericId1(ID);
                int offset = (numid-1)*sizeof(struct student);
                lseek(fd1,offset,SEEK_SET);
                read(fd1,&student,sizeof(struct student));
                if(strcmp(student.id,ID)==0){
                    char re1[5];
                    send(client_socket,&student,sizeof(struct student),0);
                    recv(client_socket,re1,sizeof(re1),0);
                    found = 1;
                }
            if (!found) {
                printf("Student with ID '%s' not found.\n", ID);
            }
                close(fd1);
            }
            searchStudentByID(ID);
            break;
        case 3:
            struct Faculty faculty;
            int file1 = open("faculty.txt", O_RDWR);
            send(client_socket,"Send_data of faculty (PLEASE USE UNDERSCORE INSTEAD OF WHITESPACE)\n",100,0);
            recv(client_socket, &faculty, sizeof(faculty), 0);
            printf("Data received\n");

            //Generating an ID for Faculty

            int extractNumericId(const char *id) {
                int numericId;
                sscanf(id, "iiitb@%d", &numericId);
                return numericId;
            }
            void generateNewId(char *newId, int highestId) {
                sprintf(newId, "iiitb@%d", highestId + 1);
            }
            struct Faculty facultycpy;
            
            char New1[20];
            if(SEEK_END>0){
                lseek(file1,-(sizeof(struct Faculty)),SEEK_END);
                read(file1,&facultycpy,sizeof(struct Faculty));
                generateNewId(New1,extractNumericId(facultycpy.id));
            }
            
            strcpy(faculty.id,New1);
            lseek(file1,0,SEEK_END);
            write(file1,&faculty,sizeof(struct Faculty));
            close(file1);
           
            break;
        case 4:
            char ID1[20];
            send(client_socket,"Enter Id to search",30,0);
            recv(client_socket,ID1,sizeof(ID1),0);
            void searchFacultyByID(char *ID) {
                struct Faculty faculty;
                int file = open("faculty.txt", O_RDONLY);

                int numid1 = extractNumericId(ID);
                int offset1 = (numid1-1)*sizeof(struct Faculty);
                lseek(file,offset1,SEEK_SET);
                read(file,&faculty,sizeof(struct Faculty));
                if (strcmp(faculty.id,ID)==0) {
                    char re[5];
                    send(client_socket,&faculty,sizeof(struct Faculty),0);
                    recv(client_socket,re,sizeof(re),0);
                
                    found = 1;
                }
                if (!found) {
                printf("Faculty with ID '%s' not found.\n", ID);
            }      
                close(file);
            }
            searchFacultyByID(ID1);
            break;
        case 9:
            close(client_socket);
            exit(0);
            break;
        default:
            // Invalid choice
            send(client_socket, "Invalid choice.\n", 16, 0);
            break;
    }
    return;
}

//CLIENT SIDE 
void Admin(int choice,int client_socket){
        struct student student;
        switch (choice) {
        case 1:
            char pass[50];
            recv(client_socket,pass,sizeof(pass),0);
            printf(" %s\n",pass);
            scanf("%s",student.password);
            student.password[strcspn(student.password, "\n")] = '\0'; // Remove the newline character
            send(client_socket,student.password, 13, 0);

            char name[100]; 
            recv(client_socket,name,sizeof(name),0);
            printf(" %s\n",name);
            scanf("%s",student.name);
            student.name[strcspn(student.name, "\n")] = '\0'; // Remove the newline character
            send(client_socket,student.name, 13, 0);

            char email[100];
            recv(client_socket,email,sizeof(email),0);
            printf(" %s\n",email);
            scanf("%s",student.email);
            student.email[strcspn(student.email, "\n")] = '\0';
            send(client_socket,student.email,sizeof(student.email), 0);

            char address[100];
            recv(client_socket,address,sizeof(address),0);
            printf(" %s\n",address);
            scanf("%s",student.address);
            student.address[strcspn(student.address, "\n")] = '\0';     
            send(client_socket, student.address, sizeof(student.address), 0);
            break;
        case 2:
            char buff[200];
            char key[20];
            recv(client_socket,buff,sizeof(buff),0);
            printf("%s",buff);
            buff[0] = '\0';

            scanf("%s",key);
            send(client_socket,key,sizeof(key),0);
            key[0]='\0';
            struct student stuinfo;
            recv(client_socket,&stuinfo,sizeof(struct student),0);
            send(client_socket,"ok",2,0);
            printf("ID:%s\n",stuinfo.id);
            printf("name:%s\n",stuinfo.name);
            printf("password:%s\n",stuinfo.password);
            printf("Email:%s\n",stuinfo.email);
            printf("Address:%s\n",stuinfo.address);
            break;
        case 3:
            char mem[100];
            recv(client_socket,mem,sizeof(mem),0);
            printf("%s\n",mem);
            mem[0] = '\0';
            struct Faculty faculty;
            strcpy(faculty.id,"0");
            char input[50];
            input[0] = '\0';
            fflush(stdin);
            fflush(stdout);

            printf("Enter Password for faculty\n:");
            memset(input, 0, sizeof(input));
            scanf("%s",input);
            input[strcspn(input, "\n")] = '\0';
            strcpy(faculty.password, input);
            input[0] = '\0';

            printf("Enter name of the faculty\n:");
            //fgets(input,sizeof(input),stdin);
            fflush(stdin);
            memset(input, 0, sizeof(input));
            scanf("%s",input);
            input[strcspn(input, "\n")] = '\0';
            strcpy(faculty.name, input);
            input[0] = '\0';

            printf("Enter Department of the faculty\n:");
            memset(input, 0, sizeof(input));
            scanf("%s",input);
            input[strcspn(input, "\n")] = '\0';
            strcpy(faculty.department, input);
            input[0] = '\0';

            printf("Enter email of the faculty\n:");
            memset(input, 0, sizeof(input));
            scanf("%s",input);
            input[strcspn(input, "\n")] = '\0';
            strcpy(faculty.email, input);
            input[0] = '\0';

            printf("Enter Address of the faculty\n:");
            memset(input, 0, sizeof(input));
            scanf("%s",input);
            input[strcspn(input, "\n")] = '\0';
            strcpy(faculty.address, input);
            input[0] = '\0';

            printf("Enter designation of the faculty\n:");
            memset(input, 0, sizeof(input));
            scanf("%s",input);
            input[strcspn(input, "\n")] = '\0';
            strcpy(faculty.designation, input);
            input[0] = '\0';
            
            send(client_socket, &faculty, sizeof(faculty), 0);
            break;
        case 4:
            //char buff[200];
            char key1[20];
            memset(buff, 0, sizeof(buff));
            recv(client_socket,buff,sizeof(buff),0);
            printf("%s",buff);
            scanf("%s",key1);
            key1[strcspn(key1, "\n")] = '\0';
            send(client_socket,key1,sizeof(key1),0);

            struct Faculty facinfo;
            recv(client_socket,&facinfo,sizeof(struct Faculty),0);
            send(client_socket,"ok",2,0);
            printf("ID:%s\n",facinfo.id);
            printf("password:%s\n",facinfo.password);
            printf("name:%s\n",facinfo.name);
            printf("Email:%s\n",facinfo.email);
            printf("Address:%s\n",facinfo.address);
          
            break;

        default:
            // Invalid choice
            send(client_socket, "Invalid choice.\n", 16, 0);
            break;
    }

    return;

}