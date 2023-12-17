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
//구조체 형식 정의 
typedef struct player{
	int energy;
	int position;
	char name[MAX_CHARNAME];
	int accumCredit;//누적학점 
	int flag_graduate;//졸업여부  
}player_t;//플레이어의 구조체화 

static player_t *cur_player;//구조체가리키는 포인터변수선언 
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
//성적 접근 후 프린트 
void printGrades(int player)
{
	int i;
	void *gradePtr;  
	for(i=0;i<smmdb_len(LISTNO_OFFSET_GRADE + player);i++)
	{
		gradePtr = smmdb_getData(LISTNO_OFFSET_GRADE + player, i);
		printf("%s : %i\n", smmObj_getObjName(gradePtr), smmObj_getObjGrade(gradePtr));
	}
}
void printPlayerStatus(void)
{
	int i;
	for(i=0; i<player_nr;i++)
	{
		printf("%s : credit %i, energy %i, position %i\n",
		 cur_player[i].name, 
		 cur_player[i].accumCredit,
		 cur_player[i].energy,
		 cur_player[i].position);
	}
}
void generatePlayers(int n, int initEnergy)//generate a new player
{
	int i;
     //n time loop
     for (i=0;i<n;i++)
     {
         //input name
         printf("Input player %i's name: ", i); //안내문 
         scanf("%s", cur_player[i].name);
         fflush(stdin);
         
         //set position 초기화 
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
void actionNode(int player) //바꿔야함 
{
	int type = smmObj_getNodeType(cur_player[player].position);
    switch(type)//각타입에 대한액션 
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


void goForward(int player, int step)
{
	cur_player[player].position += step;
	printf("%s go to node %i (name:%s)\n", cur_player[player].position, smmObj_getObjName(cur_player[player].position));
}
#endif

int main(int argc, const char * argv[]) {
    
    FILE* fp;
    char name[MAX_CHARNAME];
    int type;
    int credit;
    int energy;
    int i;
    int initEnergy;
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
    
    //보드칸 만들기 
    printf("Reading board component......\n");
    while (fscanf(fp, "%s %i %i %i", name, &type, &credit, &energy) == 4) //read a node parameter set
    {
        //store the parameter set
        void *boardObj = smmObj_genObject(name, smmObjType_board, type, credit, energy, 0); //보드칸 만드는..적절한 이넘! 구조체 포인터 
        smmdb_addTail(LISTNO_NODE, boardObj); //링크드리스트 객체 포인터. 메모리의 주소 //리스트 여러개
		 
        board_nr ++;
    }
    fclose(fp);
    printf("Total number of board nodes : %i\n", board_nr);
    
    for (i = 0;i<board_nr;i++){
    
        void *boardObj = smmdb_getData(LISTNO_NODE, i); 
        printf("node %i : %s, %i(%s), credit %i, energy %i\n", 
		i, smmObj_getObjName(boardObj), smmObj_getObjType(boardObj), smmObj_getTypeName(smmObj_getObjType(boardObj)),smmObj_getObjCredit(boardObj), smmObj_getObjEnergy(boardObj));
    }//각 노드칸  
    
    
    #if 0 
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
    #if 0
    do
    {
        //input player number to player_nr
        printf("input player no :");
        scanf("%d", &player_nr);
        fflush(stdin); // 버퍼비우기 
    }
    while (player_nr < 0 || player > MAX_PLAYER);
    
    cur_player = (player_t*)malloc(player_nr*sizeof(player_t));
    
    generatePlayers(player_nr, initEnergy);
    
    
   
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
