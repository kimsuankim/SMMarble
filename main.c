//
//  main.c
//  SMMarble
//
//  Created by Juyeop Kim on 2023/11/05.
//

#include <time.h>
#include <string.h>
#include "smm_object.h"
#include "smm_database.h"
#include "smm_common.h"

#define BOARDFILEPATH "marbleBoardConfig.txt"
#define FOODFILEPATH "marbleFoodConfig.txt"
#define FESTFILEPATH "marbleFestivalConfig.txt"

//board configuration parameters
static int board_nr;
static int food_nr;
static int festival_nr;

static int player_nr;
#if 0
static int player_energy[MAX_PLAYER];
static int player_position[MAX_PLAYER];
static char player_name[MAX_PLAYER][MAX_CHARNAME];
#endif
//����ü ���� ���� 
typedef struct player{
	int energy;
	int position;
	char name[MAX_CHARNAME];
	int accumCredit;//�������� 
	int flag_graduate;//��������  
}player_t;//�÷��̾��� ����üȭ 

static player_t *cur_player;//�����ͺ����� 
//static player_t cur_player[MAX_PLAYER];

//function prototypes
#if 0
int isGraduated(void); //check if any player is graduated
void generatePlayers(int n, int initEnergy); //generate a new player
void printGrades(int player); //print grade history of the player
void goForward(int player, int step); //make player go "step" steps on the board (check if player is graduated)
void printPlayerStatus(void); //print all player status at the beginning of each turn
float calcAverageGrade(int player); //calculate average grade of the player
smmGrade_e takeLecture(int player, char *lectureName, int credit); //take the lecture (insert a grade of the player)
void* findGrade(int player, char *lectureName); //find the grade from the player's grade history
void printGrades(int player); //print all the grade history of the player
#endif
//���� ���� �� ����Ʈ 
void printGrades(int player)
{
	int i;
	void *gradePtr;
	for(i=0;i<;i++)
	{
		gradePtr = smmdb_getData(LISTNO_OFFSET_GRADE + player, i);
		printf("%s : %i\n", smmObj_getNodeName(grade_Ptr), smmObj_getNodeGrade(grade_Ptr));
	}
}
void printPlayerStatus(void)
{
	int i;
	for(i=0; i<player_nr;i++)
	{
		printf("%s : credit %i, energy %i, position %i\n",
		 cur_player[player].accumCredit)
	}
}
void generatePlayers(int n, int initEnergy)//generate a new player
{
	int i;
     //n time loop
     for (i=0;i<n;i++)
     {
         //input name
         printf(Input player %i's name: , i); //�ȳ��� 
         scanf("%s", cur_player[i].name);
         fflush(stdin);
         
         //set position �ʱ�ȭ 
         //player_position[i] = 0;
         cur_player[i].position = 0;
         //set energy
         //player_energy[i] = initEnergy;
         cur_player[i].energy = initEnergy;
         cur_player[i].accumCredit = 0;
         cur_player[i].flag_graduate = 0;
		  
         
     }
}



int rolldie(int player)
{
    char c;
    printf(" Press any key to roll a die (press g to see grade): ");
    c = getchar();
    fflush(stdin);
    
#if 0
    if (c == 'g')
        printGrades(player);
#endif
    
    return (rand()%MAX_DIE + 1);
}

#if 0
//action code when a player stays at a node
void actionNode(int player) //�ٲ���� 
{
	int type = smmObj_getNodeType(cur_player[player].position);
    switch(type)//��Ÿ�Կ� ���Ѿ׼� 
    {
        //case lecture:
        case SMMNODE_TYPE_LECTURE;
         cur_player[player].accumCredit += smmObj_getNodeCredit(cur_player[player].position);
         cur_player[player].energy -= smmObj_getNodeCredit(cur_player[player].position);
        //addtail.. 
		default:
            break;
    }
}
#endif

void goForward(int player, int step)
{
	cur_player[player].position += step;
	printf("%s go to node %i (name:%s)\n", cur_player[player].position, smm_Obj_getNodeName(cur_player[player].position));
}


int main(int argc, const char * argv[]) {
    
    FILE* fp;
    char name[MAX_CHARNAME];
    int type;
    int credit;
    int energy;
    int i;
    int turn = 0;
    
    board_nr = 0;
    food_nr = 0;
    festival_nr = 0;
    
    srand(time(NULL));
    
    
    //1. import parameters ---------------------------------------------------------------------------------
    //1-1. boardConfig 
    if ((fp = fopen(BOARDFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", BOARDFILEPATH);
        getchar();
        return -1;
    }
    
    printf("Reading board component......\n");
    while (fscanf(fp, "%s %i %i %i", name, &type, &credit, &energy) == 4) //read a node parameter set
    {
        //store the parameter set
        void *boardObj = smmObj_genObject(name, smmObjType_board, type, credit, energy, 0); //����ĭ �����..������ �̳�! 
        smmdb_addTail(LISTNO_NODE, boardObj); //��ũ�帮��Ʈ ��ü ������. �޸��� �ּ� //����Ʈ ������
		 
        board_nr ++;
    }
    fclose(fp);
    printf("Total number of board nodes : %i\n", board_nr);
    
    for (i = 0;i<board_nr;i++){
    
        void *boardObj = ammdb_getData(LISTNO_NODE, i); 
        printf("node %i : %s, %i(%s), credit %i, energy %i\n", 
		i, smmObj_getNodeName(i), smmObj_getNodeType(i), smmObj_getTypeName(smmObj_getNodeType(i)),smmObj_getCredit(i), smmObj_getEnergy(i));
    }
    printf("(%s)", smmObj_getTypeName(SMMNODE_TYPE_LECTURE));
    
    
    //2. food card config 
    if ((fp = fopen(FOODFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", FOODFILEPATH);
        return -1;
    }
    
    printf("\n\nReading food card component......\n");
    while () //read a food parameter set
    {
        //store the parameter set
    }
    fclose(fp);
    printf("Total number of food cards : %i\n", food_nr);
    
    #if 0 
    
    //3. festival card config 
    if ((fp = fopen(FESTFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", FESTFILEPATH);
        return -1;
    }
    
    printf("\n\nReading festival card component......\n");
    while () //read a festival card string
    {
        //store the parameter set
    }
    fclose(fp);
    printf("Total number of festival cards : %i\n", festival_nr);
    #endif
    
    //2. Player configuration ---------------------------------------------------------------------------------
    
    do
    {
        //input player number to player_nr
        printf("input player no :");
        scanf("%d", &player_nr);
        fflush(stdin) ���ۺ��� 
    }
    while (player_nr < 0 || player > MAX_PLAYER);
    
    cur_player = (player_t*)malloc(player_nr*sizeof(player_t));
    
    generatePlayers(player_nr, initEnergy);
    
    
    #if 0
    //3. SM Marble game starts ---------------------------------------------------------------------------------
    while (1) //is anybody graduated?
    {
        int die_result;
        
        
        //4-1. initial printing
        printPlayerStatus();
        
        //4-2. die rolling (if not in experiment)
        die_result = rolldie();
        
        //4-3. go forward
        goForward(turn, die_result); 

		//4-4. take action at the destination node of the board
        actionNode(); 
        
        //4-5. next turn
        turn = (turn + 1)%player_nr;
        
    }
    #endif
    free(cur_player);
    system("PAUSE");
    return 0;
}
