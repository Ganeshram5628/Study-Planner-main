#include <stdio.h>
#include <dirent.h>
#include <string.h>

struct fn{
    char name[15];
};

struct date{
    char dy[3];
    char mn[3];
    char yr[5];
};

void main(){
    control();
}
void control(){
    struct date crt_date;
    char crt_datename[15];

    getDate(&crt_date);
    taskToday(&crt_date);

    int choice;

    printf("\n---------------------------------------------------------\n");
    printf("Welcome to study planner!\n");
    printf("1.Add task\n");
    printf("2.view a task\n");
    printf("3.View all tasks\n");
    printf("4.exit\n");
    printf("---------------------------------------------------------\n");
    printf("Enter a choice: \n");
    scanf("%d",&choice);
    switch(choice) {
        case 1:
            createTask();
            system("cls");
            exit(1);
            break;
        case 2:
            viewTask();
            system("cls");
            exit(1);
            break;
        case 3:
            viewFiles(&crt_date);
            system("cls");
            exit(1);
            break;
        case 4:
            exit(1);
            break;
        default:
            printf("Invalid choice");
            control();
            break;
    }
}

void getDate(struct date *crt_date){

    time_t current_time = time(NULL);
    char date_string[20];

    strftime(date_string, 20, "%d-%m-%Y", localtime(&current_time));
    snprintf(crt_date->dy,sizeof(crt_date->dy),"%c%c",date_string[0], date_string[1]);
    snprintf(crt_date->mn,sizeof(crt_date->mn),"%c%c",date_string[3], date_string[4]);
    snprintf(crt_date->yr,sizeof(crt_date->yr),"%c%c%c%c",date_string[6], date_string[7], date_string[8], date_string[9]);

}

void createTask(){
    struct date newDate;

    printf("Enter a date to set as Deadline\n");
    printf("Day in DD format : ");
    scanf("%s",&newDate.dy);
    printf("Month in MM format: ");
    scanf("%s",&newDate.mn);
    printf("Year in YYYY format: ");
    scanf("%s",&newDate.yr);

    FILE *fp;
    char filename[15];

    snprintf(filename,sizeof(filename),"%s-%s-%s",&newDate.dy,&newDate.mn,&newDate.yr);
    snprintf(filename,sizeof(filename),"%s%s",filename,".txt");

    fp = fopen(filename,"w");

    if(fp == NULL){
        printf("ERROR FILE CAN NOT BE CREATED");
        exit(1);
    }
    else{
        printf("The file %s has been created\n",filename);

        int nl;
        int i;
        char str[100];

        fprintf(fp ,"%s",filename);
        fprintf(fp ,"\r\n");

        printf("Enter the number of lines in file\n");
        scanf("%d",&nl);

        printf("Enter the contents of file\n");
        for(i = 0; i <= nl; i++){
            gets(str);
            fprintf(fp ,"%s",str);
            fprintf(fp ,"\r\n");
        }
    }
    printf("Press enter to continue");
    getch();
}

void viewFiles(struct date *crt_date){

    struct dirent *de;
    struct date rem_date[100];
    struct fn fn[100];

    int i = 0,n;
    DIR *dr = opendir(".");

    if (dr == NULL)
    {
        printf("Could not open the directory" );
        return 0;
    }

    while ((de = readdir(dr)) != NULL){
        if(strcmp(de->d_name + de->d_namlen - 4,".txt")==0){

            strcpy(fn[i].name,de->d_name);

            snprintf(rem_date[i].dy,sizeof(rem_date[i].dy),"%c%c",de->d_name[0], de->d_name[1]);
            snprintf(rem_date[i].mn,sizeof(rem_date[i].mn),"%c%c",de->d_name[3], de->d_name[4]);
            snprintf(rem_date[i].yr,sizeof(rem_date[i].yr),"%c%c%c%c",de->d_name[6], de->d_name[7], de->d_name[8], de->d_name[9]);
            i++;
        }
    }

    int frt = 0;
    for(n=0;n<i;n++){
        if((cnvt_toint(rem_date[n]) < cnvt_toint(crt_date)) != 0){
            if(frt == 0){
                printf("Deadline reached for task marked on date\n");
                frt++;
            }
            printf("\t%.10s\n",fn[n].name);
        }
    }



    frt = 0;
    for(n=0;n<i;n++){
        if((cnvt_toint(rem_date[n]) == cnvt_toint(crt_date)) != 0){
            if(frt == 0){
                printf("Today is the Deadline for tasks\n");
                frt++;
            }
            printf("\t%.10s\n",fn[n].name);
        }
    }



    frt = 0;
    for(n=0;n<i;n++){
        if((cnvt_toint(rem_date[n]) > cnvt_toint(crt_date)) != 0){
            if(frt == 0){
               printf("Task that are yet to be completed\n");
               frt++;
            }
            printf("\t%.10s\n",fn[n].name);
        }
    }


    closedir(dr);
    printf("Press enter to continue");
    getch();
}

void viewTask(){
    char view_dt[11];
    printf("Enter the date of the task in ""DD-MM-YYYY"" format\n");
    scanf("%s",&view_dt);

    char filename[15];
    snprintf(filename,sizeof(filename),"%s%s",view_dt,".txt");

    FILE *f = fopen(filename,"r");
    if(f == NULL){
        printf("Could not open the file\n" );
    }
    else{
        char str1;
        str1 = fgetc(f);
        printf("The contents of the task marked on %s is \n",view_dt);
        printf("\n---------------------------------------------------------\n");
        while (str1 != EOF)
            {
                printf ("%c", str1);
                str1 = fgetc(f);
            }
        printf("\n---------------------------------------------------------\n");
    }
    printf("Press enter to continue");
    getch();
}

void taskToday(struct date *crt_date){

    char filename[15];
    snprintf(filename,sizeof(filename),"%s-%s-%s",crt_date->dy,crt_date->mn,crt_date->yr);
    snprintf(filename,sizeof(filename),"%s%s",filename,".txt");

    FILE *f = fopen(filename,"r");
    if(f == NULL){
            printf("There no tasks Deadlined today");
    }
    else{
        char str1;
        str1 = fgetc(f);
        printf("The contents of the task marked today is \n");
        printf("\n---------------------------------------------------------\n");
        while (str1 != EOF)
            {
                printf ("%c", str1);
                str1 = fgetc(f);
            }
    }
}

int cnvt_toint(struct date *dt){
    int a;
    a = (atoi(dt->yr)*10*10*10*10);
    a +=(atoi(dt->mn)*10*10);
    a += atoi(dt->dy);
    return a;
}
