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

//플레이어 구조체 형식 정의 
typedef struct player{
	char name[MAX_CHARNAME];//이름
    int position;//현재위치 
	int energy;//보유에너지 
	int accumCredit;//누적학점 
	int flag_graduate;//졸업여부  
}player_t;

static player_t *cur_player;//구조체가리키는 포인터변수선언 
//static player_t cur_player[MAX_PLAYER];

//function prototypes
#if 0
int isGraduated(void); //check if any player is graduated 
void generatePlayers(int n, int initEnergy); //generate a new player*
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

//저장된 플레이어 정보 출력 함수  
void printPlayerStatus(void)
{
	int i;
	for(i=0; i<player_nr;i++)
	{
		printf("%s : position %i, energy %i, accumCredit %i\n",
		 cur_player[i].name, 
		 cur_player[i].position,
		 cur_player[i].accumCredit,
		 cur_player[i].energy);
		 //cur_player[i].flag_graduate
	}
}
//플레이어 생성및 초기화 함수 
void generatePlayers(int n, int initEnergy)//generate a new player
{
	int i;
     //n time loop
     for (i=0;i<n;i++)
     {
         //*input name*
         printf("Input player %i's name: ", i);  
         scanf("%s", cur_player[i].name);
         fflush(stdin);
         
         //set position 
         cur_player[i].position = 0;
         //set energy
         cur_player[i].energy = initEnergy;
         cur_player[i].accumCredit = 0;
         cur_player[i].flag_graduate = 0;
     }
}

//주사위굴리기(자기턴마다 성적열람가능)
int rolldie(int player)
{
    char c;
    printf(" Press any key to roll a die (press g to see grade): ");
    c = getchar();
    fflush(stdin);
    //성적을 본다면(press g) 
#if 0
    if (c == 'g')
        printGrades(player);
#endif
    //주사위굴리기 
    return (rand()%MAX_DIE + 1); //주사위 1~6 
}

//action code when a player stays at a node
void actionNode(int player) //바꿔야함 
{
	void* boardPtr = smmdb_getData(LISTNO_NODE, cur_player[player].position);
    int type = smmObj_getObjType(boardPtr);
    char *name = smmObj_getObjName(boardPtr);
    void *gradePtr;
    //랜덤성적부여 
    int smmObjGrade_e = (rand()%MAX_GRADE);
    
	switch(type)//각타입에 대한액션 
    {
        //case lecture:
        case SMMNODE_TYPE_LECTURE:
         cur_player[player].accumCredit += smmObj_getObjCredit(boardPtr);
         cur_player[player].energy -= smmObj_getObjCredit(boardPtr);
        //addtail.. 
        
        //grade generation
		gradePtr = smmObj_genObject(name, smmObjType_grade, 0, smmObj_getObjCredit(boardPtr), 0, smmObjGrade_e);
        smmdb_addTail(LISTNO_OFFSET_GRADE + player, gradePtr);
        
         break;
         
		default:
            break;
    }
}

//전진.위치변경 
void goForward(int player, int step)
{
	void *boardPtr; 
	cur_player[player].position += step;//전진
	boardPtr = smmdb_getData(LISTNO_NODE, cur_player[player].position);//addtail??? 
	
	printf("%s go to node %i (name:%s)\n",
	 cur_player[player].name, cur_player[player].position, smmObj_getObjName(boardPtr));//여기약간?? 
}


int main(int argc, const char * argv[]) {
    
    FILE* fp;
    char name[MAX_CHARNAME];
    int type;
    int credit;
    int energy;
    int i;
    int initEnergy; //*집노드의 보충에너지 
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
    }//파일 안열림
    
    //보드칸 만들기 
    printf("Reading board component......\n");//파일 열기 
    while (fscanf(fp, "%s %i %i %i", name, &type, &credit, &energy) == 4) //read a node parameter set
    {
        //store the parameter set
        void *boardObj = smmObj_genObject(name, smmObjType_board, type, credit, energy, 0); 
        smmdb_addTail(LISTNO_NODE, boardObj); //링크드리스트 객체 포인터. 메모리의 주소 //리스트 여러개
		 
        board_nr ++;//보드저장개수 
    }
    fclose(fp);//파일닫기 
    printf("Total number of board nodes : %i\n", board_nr);//보드칸수확인 
    
    for (i = 0;i<board_nr;i++){
    
        void *boardObj = smmdb_getData(LISTNO_NODE, i);
        printf("node %i : %s, %i(%s), credit %i, energy %i\n", 
		i, smmObj_getObjName(boardObj), smmObj_getObjType(boardObj), smmObj_getTypeName(smmObj_getObjType(boardObj)),smmObj_getObjCredit(boardObj), smmObj_getObjEnergy(boardObj));
    }//각 보드칸 정보 확인 
    
    
   
    //2. food card config 
    if ((fp = fopen(FOODFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", FOODFILEPATH);
        return -1;
    }//파일안열림 
    
    printf("\n\nReading food card component......\n");
    while (fscanf(fp, "%s %i", name, &energy) == 2) //read a food parameter set
    {
        //store the parameter set
        void *foodObj = smmObj_genObject(name, smmObjType_food, 0, 0, energy, 0); 
        smmdb_addTail(LISTNO_FOODCARD, foodObj);//필요없지않나 보드판처럼이어진게아니니까 
        
        food_nr ++;
    }
    fclose(fp);
    printf("Total number of food cards : %i\n", food_nr);
    
    for (i = 0;i<food_nr;i++)
    {	
        void *foodObj = smmdb_getData(LISTNO_FOODCARD, i);
        printf("node %i : %s, energy %i\n", 
		i, smmObj_getObjName(foodObj), smmObj_getObjEnergy(foodObj));
    }//각 음식카드  정보 확인 
    
    
    //3. festival card config 
    if ((fp = fopen(FESTFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", FESTFILEPATH);
        return -1;
    }//파일안열림 
    
    printf("\n\nReading festival card component......\n");
    while (fscanf(fp, "%s", name) == 1) //read a festival card string
    {
        //store the parameter set
        void *festivalObj = smmObj_genObject(name, smmObjType_festival, 0, 0, 0, 0); 
        smmdb_addTail(LISTNO_FESTCARD, festivalObj);
        
        festival_nr ++;
    }
    fclose(fp);
    printf("Total number of festival cards : %i\n", festival_nr);
    
    for (i = 0;i<festival_nr;i++)
    {	
        void *festivalObj = smmdb_getData(LISTNO_FESTCARD, i);
        printf("node %i : %s\n", 
		i, smmObj_getObjName(festivalObj));
    }//축제카드  정보 확인 
 
    
    //2. Player configuration ---------------------------------------------------------------------------------
    do
    {
        //*input player number to player_nr*
        printf("input player number:");
        scanf("%d", &player_nr);//플레이어수 저장 
        fflush(stdin); // 버퍼비우기 
    }
    while (player_nr < 0 || player_nr > MAX_PLAYER);
    
    //플레이어구조체 메모리할당 
    cur_player = (player_t*)malloc(player_nr*sizeof(player_t));
    if(cur_player == NULL){
    	printf("동적메모리 할당 오류\n");
		exit(1); 
	}//에러핸들링 
    
    //플레이어 상태 초기화
    generatePlayers(player_nr, initEnergy);//initEnergy 정해야됨!!! 
    
    
   
    //3. SM Marble game starts ---------------------------------------------------------------------------------
    while (1) //is anybody graduated?
    {
        int die_result;
        
        //4-1. initial printing
        //플레이어 상태 
		printPlayerStatus();
        
        
        //4-2. die rolling (if not in experiment)
        die_result = rolldie(turn);
        
        //4-3. go forward
        goForward(turn, die_result); 

		//4-4. take action at the destination node of the board
        actionNode(turn); 
        
        //4-5. next turn
        turn = (turn + 1)%player_nr;
        
    }
    free(cur_player);//플레이어구조체 메모리 반납 
    system("PAUSE");
    return 0;
}
