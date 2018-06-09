#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MF 0
#define FM 1
#define MM 2
#define FF 3 
typedef struct UCupID {
	char SID[20];
	char First_Name[20];
	char Sex;
	char Sex_Orientation;
	int Age;
	int Political_Leaning;
	char Major_Hobby[20];
	char Minor_Hobby[20];
	int Height;
	int Weight;
	struct UCupID *next;
}UCupID;
int Sex_Judege(UCupID* c1, UCupID* c2);
int Hobby_Judge(UCupID* c1, UCupID* c2);
int Height_Weight_Judge(UCupID* c1, UCupID* c2);
int Age_Political_Judge(UCupID* c1, UCupID* c2);
void free_link(UCupID *id);
int min(int a,int b);
int main(int argc,char* argv[])
{
	if (argc < 3)
	{
		printf("Usage: ./UCupiD db_file SID\n");
		exit(1);
	}
	FILE* file = fopen(argv[1], "r");
	if (file == NULL)
	{
		printf("Error: cannot open %s\n", argv[1]);
		exit(1);
	}
	char Temp[256];
	UCupID *UCupID_Data=NULL;
	UCupID *UCupID_Data_Pre=NULL;
	UCupID *head;
	int i = 0;
	while (!feof(file))
	{
		UCupID_Data = (UCupID*)malloc(sizeof(UCupID));
		fgets(Temp, 256, file);
		char buf1[20], buf2[20], buf3[20], buf4[20], buf5[20], buf6[20], buf7[20], buf8[20], buf9[20], buf10[20];
		sscanf(Temp, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,]", buf1, buf2 , buf3, buf4, buf5, buf6, buf7, buf8, buf9, buf10);
		strcpy(UCupID_Data->SID,buf1);
		strcpy(UCupID_Data->First_Name, buf2);
		UCupID_Data->Sex = buf3[0];
		UCupID_Data->Sex_Orientation = buf4[0];
		UCupID_Data->Age = atoi(buf5);
		UCupID_Data->Political_Leaning = atoi(buf6);
		strcpy(UCupID_Data->Major_Hobby, buf7);
		strcpy(UCupID_Data->Minor_Hobby, buf8);
		UCupID_Data->Height = atoi(buf9);
		UCupID_Data->Weight = atoi(buf10);
		UCupID_Data->next = NULL;
		if (i != 0 && !strcmp(UCupID_Data->SID, UCupID_Data_Pre->SID))
			break;
		if (i == 0)
			head = UCupID_Data;
		if (i != 0)
			UCupID_Data_Pre->next = UCupID_Data;
		UCupID_Data_Pre = UCupID_Data;
		UCupID_Data = UCupID_Data->next;
		++i;
	}
	fclose(file);
	UCupID *ptr = head;
	while (ptr != NULL)
	{
		if (!strcmp(ptr->SID, argv[2]))
			break;
		ptr = ptr->next;
	}
	if (ptr == NULL)
	{
		printf("User with SID %s not found\n", argv[2]);
		exit(1);
	}
	UCupID *ptr_find[20];
	UCupID *ptr_move = head;
	int j = 0;
	while (ptr_move != NULL)
	{
		if (!strcmp(ptr_move->SID, ptr->SID))
		{
			ptr_move = ptr_move->next;
			continue;
		}
		if (Sex_Judege(ptr_move, ptr))
		{
			if (Age_Political_Judge(ptr, ptr_move) || Hobby_Judge(ptr, ptr_move) || Height_Weight_Judge(ptr, ptr_move))
			{
				ptr_find[j] = ptr_move;
				++j;
				ptr_move = ptr_move->next;
			}
			else
				ptr_move = ptr_move->next;
		}
		else
		{
			ptr_move = ptr_move->next;
		}
	}
	char s1[100] = "match_";
	char s2[100] = ".txt";
	strcat(s1, argv[2]);
	strcat(s1, s2);
	FILE* wfile = fopen(s1, "wt");
	if (wfile == NULL)
	{
		printf("Error: cannot open %s\n",s1);
		exit(1);
	}
	int k = 0;
	fprintf(wfile,"Matches for user %s (%s): \n", ptr->SID, ptr->First_Name);
	while (k < j)
	{
		if(!strcmp(ptr->First_Name,"John"))
		fprintf(wfile,"- %s (%s)\n", ptr_find[k]->SID, ptr_find[k]->First_Name);
		++k;
	}
	fclose(wfile);
	free_link(head);
}

void free_link(UCupID *id)
{
	UCupID *p = id,*q;
	while (p != NULL)
	{
		q = p;                //q���浱ǰ��p
		p = p->next;
		free(q);
	}
}

int Sex_Judege(UCupID* c1, UCupID* c2)
{
	int sex_or1, sex_or2;
	if (c1->Sex == 'M'&&c1->Sex_Orientation == 'F')
		sex_or1 = MF;
	else if (c1->Sex == 'F'&&c1->Sex_Orientation == 'M')
		sex_or1 = FM;
	else if (c1->Sex == 'M'&&c1->Sex_Orientation == 'M')
		sex_or1 = MM;
	else
		sex_or1 = FF;
	if (c2->Sex == 'M'&&c2->Sex_Orientation == 'F')
		sex_or2 = MF;
	else if (c2->Sex == 'F'&&c2->Sex_Orientation == 'M')
		sex_or2 = FM;
	else if (c2->Sex == 'M'&&c2->Sex_Orientation == 'M')
		sex_or2 = MM;
	else
		sex_or2 = FF;
	if (sex_or1 == MF && sex_or2 == FM)
		return 1;
	else if (sex_or1 == FM && sex_or2 == MF)
		return 1;
	else if (sex_or1 == MM && sex_or2 == MM)
		return 1;
	else if (sex_or1 == FF && sex_or2 == FF)
		return 1;
	else
		return 0;
}

int Age_Political_Judge(UCupID* c1, UCupID* c2)
{
	if (abs(c1->Age - c2->Age) <= 10 || abs(c1->Political_Leaning - c2->Political_Leaning) <= 2)
		return 1;
	else
		return 0;
}

int Hobby_Judge(UCupID* c1, UCupID* c2)
{
	if (!strcmp(c1->Major_Hobby, c2->Major_Hobby) && !strcmp(c1->Minor_Hobby, c2->Minor_Hobby))
		return 1;
	else
		return 0;
}

int Height_Weight_Judge(UCupID* c1, UCupID* c2)
{
	if (abs(c1->Height - c2->Height) <= 0.1*min(c1->Height,c2->Height) && abs(c1->Weight - c2->Weight) <= 0.1*min(c1->Weight, c2->Weight))
		return 1;
	else
		return 0;
}

int min(int a,int b)
{
	if(a<b)
	return a;
	else
	return b;
}