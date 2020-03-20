//
//  main.c
//  Music Player
//
//  Created by Pawel Gates on 26/12/2019.
//  Copyright © 2019 Pawel Gates. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct Node
{
    int id;
    char artistName[31];
    char albomName[31];
    int trackNum;
    char trackName[31];
    struct Node* next;
    struct Node* prev;
};



void importData(char *path , struct Node* *headOf){
    int id, trackNo;
    char artistName[31], albumName[31], trackName[31];    //note +1 for '\0'
    char h1[6], h2[31], h3[31], h4[10], h5[10];
    
    FILE *fp = fopen(path, "r");
    if (!fp)
    {
        printf("File not found!\n");
    }
    else
    { //else 1
        
        
        struct Node* temp = NULL;
        struct Node* nn = NULL;
        
        fscanf(fp, "%5s%30s%30s%10s%30s", h1, h2, h3, h4, h5);
        printf("%-5s %-30s %-30s %-10s %-30s \n", h1, h2, h3, h4, h5);
        while (fscanf(fp, "%5d%30[^\n]%30[^\n]%5d%30[^\n]", &id, artistName, albumName, &trackNo, trackName)>0)
        {
            printf("%5d %30s %30s %10d %30s \n", id, artistName, albumName, trackNo, trackName);
            if(*headOf == NULL){
                temp = (struct Node*)malloc(sizeof(struct Node));
                nn = (struct Node*)malloc(sizeof(struct Node));
                temp->id = id;
                strcpy(temp->artistName, artistName);
                strcpy(temp->albomName, albumName);
                temp->trackNum = trackNo;
                strcpy(temp->trackName, trackName);
                temp->next = NULL;
                temp->prev = NULL;
                *headOf = temp;
            }
            else{ //else 2
                nn = (struct Node*)malloc(sizeof(struct Node));
                nn = *headOf;
                
                while(nn->next != NULL){
                nn = nn->next;
                }
                temp = (struct Node*)malloc(sizeof(struct Node));
            
                temp->id = id;
                strcpy(temp->artistName, artistName);
                strcpy(temp->albomName, albumName);
                temp->trackNum = trackNo;
                strcpy(temp->trackName, trackName);
                temp->next = NULL;
                temp->prev = nn;
                nn->next = temp;
                while(nn->prev != NULL){
                    nn = nn->prev;
                }
                *headOf = nn;
            }   //end of else 2
        }// end of while
    }//end of else 1
    fclose(fp);
}

void printList(struct Node* n)      //PRINTING THE LINKED LIST
{
    while (n != NULL) {
        printf(" %d ", n->id);
        n = n->next;
    }
    printf("\n");
}

void printPlaylist(char* f){
    int c;
    FILE *file;
    file = fopen(f, "r");
    if (file) {
        while ((c = getc(file)) != EOF)
            putchar(c);
        fclose(file);
    }
    printf("\n");
}

int playingSong(struct Node* hd){
    int id = 0;
    printf("Enter the song's ID:\n");
    scanf("%d", &id);
    struct Node* h = NULL;
    h = (struct Node*)malloc(sizeof(struct Node));
    h = hd;
    int check = 0;
    while(h != NULL){
        if(id == h->id){
            printf("Playing the song: %s - %s \n", h->artistName, h->trackName);
            check = 1;
            break;
        }
        else{
            h=h->next;
        }
    }
    if(check == 0){
        printf("The ID is INCORRECT\n");
    }
    free(h);
    return id;
}

int playingNextSong(struct Node* hd, int id){
    struct Node* h = NULL;
    h = (struct Node*)malloc(sizeof(struct Node));
    h = hd;
    while( h != NULL ){
        if(h->id != id){
            h = h->next;
        }
        else{
            
            if(h->next != NULL){
                printf("Playing the song: %s - %s \n", h->next->artistName, h->next->trackName);
                id = h->next->id;
                break;
            }
            else{
                printf("There is no next song! \n");
                printf("Playing the song: %s - %s \n", h->artistName, h->trackName);
                break;
            }
        }
    }
    return id;
}

int playingPrevSong(struct Node* hd, int id){
    struct Node* h = NULL;
    h = (struct Node*)malloc(sizeof(struct Node));
    h = hd;
    while( h != NULL ){
        if(h->id != id){
            h = h->next;
        }
        else{
            
            if(h->prev != NULL){
                printf("Playing the song: %s - %s \n", h->prev->artistName, h->prev->trackName);
                id = h->prev->id;
                break;
            }
            else{
                printf("There is no previous song! \n");
                printf("Playing the song: %s - %s \n", h->artistName, h->trackName);
                break;
            }
        }
    }
    return id;
}

int main()
{
    //printPlaylist("playlist.txt");
    
    struct Node* head = NULL;
    importData("playlist.txt", &head);
    
    printf("\n");
    printf("ACTION: \n");
    printf("(1) - Play song | (2) - Next song | (3) - Prev song | (4) - Search | (5) - Delete song | (6) - Add song | (7) - Sort\n");
    int i=1000;
    int songId = 0;
    while(i != 0){
        
        
        scanf("%d", &i);
        switch (i){
            case 1:
                songId = playingSong(head);
                break;
            case 2:
                songId = playingNextSong(head, songId);
                break;
            case 3:
                songId = playingPrevSong(head, songId);
                break;
            case 4:
                printf("Searching the song\n");
                break;
            case 5:
                printf("Deleting the song\n");
                break;
            case 6:
                printf("Adding the song\n");
                break;
            case 7:
                printf("Sorting the song\n");
                break;
        }
        
        
        
    }
    
    
    return 0;
}
