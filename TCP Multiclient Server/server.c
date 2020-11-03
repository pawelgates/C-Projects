#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 4449
#define LOG_PATH "log.txt"
#define USERS_PATH "users.txt"

struct systemUser
{
    char username[20];
    char id[10];
    int checkInStatus;
    int hourIn;
    int hourOut;
    int minuteIn;
    int minuteOut;
    struct systemUser* next;
    struct systemUser* prev;
};

struct systemUser* read_users(char *path, struct systemUser* head_of_users, int pri)
{
    int day, month, year, hours, mins;
    int checkInStatus;
    char id[10];
    char username[20];
    
    struct systemUser* tmp_head = NULL;
    
    FILE *fp = fopen(path, "r");
    while (!fp)
    {
        printf("File not found!\n");
    }
    
    while (fscanf(fp, "%3s %[^\n]%*c", id, username) != EOF)
    {
        struct systemUser* tmp = (struct systemUser*)malloc(sizeof(struct systemUser));
        struct systemUser* tmp_head2 = head_of_users;
        
        if(pri == 1){
            
            printf("\t %-3s %-19s \n", id, username);
        }
        
        strcpy(tmp->id, id);
        strcpy(tmp->username, username);
        tmp->checkInStatus = 0;
        tmp->next = NULL;
        
        if(tmp_head == NULL){
            tmp_head = tmp;
            tmp_head->prev = NULL;
        }
        
        else{
            while(tmp_head->next != NULL){
                tmp_head = tmp_head->next;
            }
            tmp_head2 = tmp_head;
            tmp_head->next = tmp;
            tmp_head->next->prev = tmp_head2;
            while(tmp_head->prev != NULL){
                tmp_head = tmp_head->prev;
            }
        }
    }
    fclose(fp);
    return tmp_head;
}

void getDateTime(int *day, int *month, int *year, int *hours, int *mins)
{
    time_t rawtime;
    struct tm* timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    *day = timeinfo->tm_mday;
    *month = timeinfo->tm_mon + 1;
    *year = timeinfo->tm_year + 1900;
    *hours = timeinfo->tm_hour;
    *mins = timeinfo->tm_min;
}

void file_name(struct systemUser* user, int totalHours, int totalMins){
    
    struct systemUser* tmp = user;
    char startName[100];
    char txtFile[100];
    int day, month, year, hours, mins;
    strcpy(startName, "Time Of ");
    strcpy(txtFile, ".txt");
    strcat(startName, tmp->username);
    strcat(startName, txtFile);
    
    FILE *fp = fopen(startName, "a");
    while (!fp)
    {
        printf("File not found!\n");
    }
    getDateTime(&day, &month, &year, &hours, &mins);
    fprintf(fp, "%02d/%02d/%d    |    IN: %02d:%02d    OUT: %02d:%02d    |    TOTAL: %02d:%02d\n", day, month, year, tmp->hourIn, tmp->minuteIn, tmp->hourOut, tmp->minuteOut, totalHours, totalMins);
    fclose(fp);
}

void free_system_user(struct systemUser* head)
{
   struct systemUser* tmp;
   while (head != NULL)
    {
       tmp = head;
       head = head->next;
       free(tmp);
    }
}

int main(int argc,const char* argv[]){

	int day, month, year, hours, mins;
	int sockfd, ret, i;
    int approve;
    int totalHours;
    int totalMins;
    struct systemUser* head_of_users = NULL;
	struct sockaddr_in serverAddr;
	int newSocket;
	struct sockaddr_in newAddr;

	socklen_t addr_size;
	char buffer[100];
    char username[20];
    char userID[10];
    char userMessege[100];
    char userMess2[100];
    char userFilePath[100];
    char thours[20], tmins[20], ttime[40], ddot[1];
	pid_t childpid;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
	if(sockfd < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Server Socket is created.\n");
    
    //LOG FILE
    FILE *log = fopen(LOG_PATH, "a");
    getDateTime(&day, &month, &year, &hours, &mins);
    fprintf(log,"\n%02d/%02d/%d, %02d:%02d : Server SOCKET is created.\n", day, month, year, hours, mins);
    fclose(log);

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in binding.\n");
		exit(1);
	}
	printf("[+]Bind to port %d\n", PORT);

    //READING USERS.TXT
    printf("[+]Reading the USERS list: \n");
    printf("\n");
    head_of_users = read_users(USERS_PATH, head_of_users, 1);
    printf("\n");
    
    
    struct systemUser* test_user = head_of_users;
    
	if(listen(sockfd, 10) == 0){
		printf("[+]Listening....\n");
        
        
        //LOG FILE
        FILE *log = fopen(LOG_PATH, "a");
        getDateTime(&day, &month, &year, &hours, &mins);
        fprintf(log,"%02d/%02d/%d, %02d:%02d : Server is listening.\n", day, month, year, hours, mins);
        fclose(log);
	}else{
		printf("[-]Error in binding.\n");
	}

    bzero(userMessege, sizeof(userMessege));
    bzero(buffer, sizeof(buffer));
    bzero(username, sizeof(username));
    bzero(userID, sizeof(userID));
    
	while(1){
		newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0){
			exit(1);
		}
		printf("[+]Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
        printf("\n");
        
        //LOG FILE
        FILE *log = fopen(LOG_PATH, "a");
        getDateTime(&day, &month, &year, &hours, &mins);
        fprintf(log,"%02d/%02d/%d, %02d:%02d : Connection accepted from %s:%d.\n", day, month, year, hours, mins, inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
        fclose(log);

		if((childpid = fork()) == 0){
			close(sockfd);

			while(1){
				recv(newSocket, buffer, 100, 0);
                recv(newSocket, userID, 10, 0);
                
				if(strcmp(userID, "exit") == 0){
					printf("[-]Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
					break;
				}
                else{
                    
                    //CHECKING ID OF CLIENT
                    approve = 0;
                    test_user = head_of_users;
                    
                    while(test_user != NULL){
                        if(strcmp(userID, test_user->id) == 0){
                            approve = 1;
                            strcpy(username, test_user->username);
                            break;
                        }
                        test_user = test_user->next;
                    }
                    
                    if(approve == 1){
                        if(strcmp(buffer, "IN") == 0 || strcmp(buffer, "in") == 0){
                            
                            if(test_user->checkInStatus == 0){
                                test_user->checkInStatus = 1;
                                
                                getDateTime(&day, &month, &year, &hours, &mins);
                                printf("%02d/%02d/%d, %02d:%02d : User %s CHECKED IN. \n", day, month, year, hours, mins, username);
                                //TIME
                                test_user->hourIn = hours;
                                test_user->minuteIn = mins;
                                
                                strcpy(userMessege, "Thank you, ");
                                strcat(userMessege, username);
                                strcpy(userMess2, ". You have CHECKED IN to the system!");
                                strcat(userMessege, userMess2);
                                send(newSocket, userMessege, strlen(userMessege), 0);
                                
                                //LOG FILE
                                FILE *log = fopen(LOG_PATH, "a");
                                fprintf(log,"%02d/%02d/%d, %02d:%02d : User %s CHECKED IN to the system.\n", day, month, year, hours, mins, username);
                                fclose(log);
                            }
                            else{
                                strcpy(userMessege, "NO NO NO, ");
                                strcat(userMessege, username);
                                strcpy(userMess2, ". You already CHECKED IN.");
                                strcat(userMessege, userMess2);
                                send(newSocket, userMessege, strlen(userMessege), 0);
                                
                            }
                            
                            //LOG FILE
                            FILE *log = fopen(LOG_PATH, "a");
                            fprintf(log,"%02d/%02d/%d, %02d:%02d : User %s CHECKED IN to the system.\n", day, month, year, hours, mins, username);
                            fclose(log);
                        }
                        else if(strcmp(buffer, "OUT") == 0 || strcmp(buffer, "out") == 0){
                            
                            if(test_user->checkInStatus == 1){
                                test_user->checkInStatus = 0;
                                
                                getDateTime(&day, &month, &year, &hours, &mins);
                                
                                //TIME
                                test_user->hourOut = hours;
                                test_user->minuteOut = mins;
                                //COUNTING THE TIME
                                totalHours = test_user->hourOut - test_user->hourIn;
                                totalMins = test_user->minuteOut - test_user->minuteIn;
                                if(test_user->minuteOut < test_user->minuteIn){
                                    totalHours = totalHours - 1;
                                    totalMins = totalMins + 60;
                                }
                                if(test_user->hourOut < test_user->hourIn){
                                    totalHours = totalHours + 24;
                                }
                                
                                
                                printf("%02d/%02d/%d, %02d:%02d : User %s CHECKED OUT. Total time: %02d:%02d.\n", day, month, year, hours, mins, username, totalHours,totalMins);
                                //CREATING TIME FILE
                                if(argc > 1){
                                    if(strcmp(argv[1], "time") == 0){
                                        file_name(test_user, totalHours, totalMins);
                                    }
                                }
                                
                                
                                
                                strcpy(userMessege, "Thank you, ");
                                strcat(userMessege, username);
                                strcpy(userMess2, ". You have CHECKED OUT from the system!");
                                strcat(userMessege, userMess2);
                                send(newSocket, userMessege, strlen(userMessege), 0);
                                
                                //LOG FILE
                                FILE *log = fopen(LOG_PATH, "a");
                                fprintf(log,"%02d/%02d/%d, %02d:%02d : User %s CHECKED OUT from the system.\n", day, month, year, hours, mins, username);
                                fclose(log);
                            }
                            else{
                                strcpy(userMessege, "NO NO NO, ");
                                strcat(userMessege, username);
                                strcpy(userMess2, ". You already CHECKED OUT.");
                                strcat(userMessege, userMess2);
                                send(newSocket, userMessege, strlen(userMessege), 0);
                            }
                        }
                        else{
                            printf("%s: %s\n", username, buffer);
                            strcpy(userMessege, "INCORRECT LOG IN! Enter 'IN' or 'OUT'!");
                            send(newSocket, userMessege, strlen(userMessege), 0);
                            
                            //LOG FILE
                            FILE *log = fopen(LOG_PATH, "a");
                            fprintf(log,"%02d/%02d/%d, %02d:%02d : User %s made INCORRECT login.\n", day, month, year, hours, mins, username);
                            fclose(log);
                        }
                    }
                    else{
                        strcpy(userMessege, "INCORRECT USER ID!");
                        send(newSocket, userMessege, strlen(userMessege), 0);
                    }
                    
                    bzero(userMessege, sizeof(userMessege));
                    bzero(buffer, sizeof(buffer));
                    bzero(username, sizeof(username));
                    bzero(userID, sizeof(userID));
				}
			}
		}
	}
    
    free_system_user(head_of_users);
	close(newSocket);

	return 0;
}
