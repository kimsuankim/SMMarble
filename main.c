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

//�÷��̾� ����ü ���� ���� 
typedef struct player{
	char name[MAX_CHARNAME];//�̸�
    int position;//������ġ 
	int energy;//���������� 
	int accumCredit;//�������� 
	int flag_graduate;//��������  
}player_t;

static player_t *cur_player;//����ü����Ű�� �����ͺ������� 
//static player_t cur_player[MAX_PLAYER];

//function prototypes
#if 0
int isGraduated(void); //check if any player is graduated 
void generatePlayers(int n, int initEnergy); //generate a new player*
void printGrades(int player); //print grade history of the player*
void goForward(int player, int step); //make player go "step" steps on the board (check if player is graduated)
void printPlayerStatus(void); //print all player status at the beginning of each turn*
float calcAverageGrade(int player); //calculate average grade of the player
smmGrade_e takeLecture(int player, char *lectureName, int credit); //take the lecture (insert a grade of the player)
void* findGrade(int player, char *lectureName); //find the grade from the player's grade history
void printGrades(int player); //print all the grade history of the player
#endif
#if 0
//�����ο� 
smmGrade_e takeLecture(int player, char *lectureName, int credit)
{
	//�������� 
	void *gradePtr = smmObj_genObject(name, smmObjType_grade, 0, smmObj_getObjCredit(boardPtr), 0, smmObjGrade_e);
        smmdb_addTail(LISTNO_OFFSET_GRADE + player, gradePtr); //����ü���� 
	
	smmObjGrade_e = (rand()%MAX_GRADE);
}
//���� �̷� �˻� �Լ�
void* findGrade(int player, char *lectureName)
{
	
}
#endif
//��������ü ��� �Լ� 
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

//����� �÷��̾� ���� ��� �Լ�  
void printPlayerStatus(void)
{
	int i;
	for(i=0; i<player_nr;i++)
	{
		printf("%s : position %i, energy %i, accumCredit %i\n",
		 cur_player[i].name, 
		 cur_player[i].position,
		 cur_player[i].energy,
		 cur_player[i].accumCredit);
		 //cur_player[i].flag_graduate
	}
}
//�÷��̾� ������ �ʱ�ȭ �Լ� 
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

//�ֻ���������(�ڱ��ϸ��� ������������)
int rolldie(int player)
{
    char c;
    printf(" Press any key to roll a die (press g to see grade): ");
    c = getchar();
    fflush(stdin);
    //������ ���ٸ�(press g) 
    if (c == 'g')
        printGrades(player);//��������Լ�ȣ�� 
    //�ֻ��������� 
    return (rand()%MAX_DIE + 1); //�ֻ��� 1~6 
}

//action code when a player stays at a node
void actionNode(int player, int food_nr, int festival_nr) //�ٲ���� 
{
	void* boardPtr = smmdb_getData(LISTNO_NODE, cur_player[player].position);
    int type = smmObj_getObjType(boardPtr);
    char *name = smmObj_getObjName(boardPtr);
    void *gradePtr;
    //���������ο� 
    int smmObjGrade_e = (rand()%MAX_GRADE);
    //����ī��̱� 
    int foodcard_r = (rand()%food_nr);
    int festcard_r = (rand()%festival_nr);
    void *foodObj = smmdb_getData(LISTNO_FOODCARD, foodcard_r);
    void *festObj = smmdb_getData(LISTNO_FESTCARD, festcard_r);
    
	switch(type)//��Ÿ�Կ� ���Ѿ׼� 
    {
        //case lecture:
        case SMMNODE_TYPE_LECTURE:
        	//�ҿ信�����̻�, ������������, ����������� 
        	if(cur_player[player].energy >= smmObj_getObjEnergy(boardPtr) /*&& findGrade(player,name) =*/)
        	{
        	 char c;
        	 printf("You can take a %s\n (press a to take a lecture)", name);
        	 c = getchar();
        	 fflush(stdin);
        	  //���� ���� ���� 
        	  if(c == 'a')
        	  {
        	  	cur_player[player].accumCredit += smmObj_getObjCredit(boardPtr);
                cur_player[player].energy -= smmObj_getObjEnergy(boardPtr);
                //addtail.. 
        
                //grade generation ��������ü���� 
		        gradePtr = smmObj_genObject(name, smmObjType_grade, 0, smmObj_getObjCredit(boardPtr), 0, smmObjGrade_e);
                smmdb_addTail(LISTNO_OFFSET_GRADE + player, gradePtr);
			  }
			  //���ǵ������ 
			  else
			  printf("You drop a %s\n",name);
			  
			  break;
			}
			//�ҿ信�����������ϰų� �����������
			printf("You can't take a %s\n(�ҿ信���� ����,����� ����)\n",name);
         
         break;
        //�Ĵ� : ���������� 
        case SMMNODE_TYPE_RESTAURANT:
		 cur_player[player].energy += smmObj_getObjEnergy(boardPtr);
		 printf("You got %i energy!\n", smmObj_getObjEnergy(boardPtr));
		 break; 
		/*����� : ���ε��� ��� 
		case SMMNODE_TYPE_LABORATORY:*/
		//��: ���������� :���ް��� 
		case SMMNODE_TYPE_HOME: //�������¼���! 
		 cur_player[player].energy += smmObj_getObjEnergy(boardPtr);
		 printf("WELCOME HOME\n");
		 printf("You got %i energy!\n", smmObj_getObjEnergy(boardPtr));
		 break; 
			
		/*�����ΰ� : ����Ƿ� �̵�, ������ ���� 
		case SMMNODE_TYPE_GOTOLAB:
		 cur_player[player].position =  //����Ƿΰ�����!*/
		//��������:��������ī��, ���������� 
		case SMMNODE_TYPE_FOODCHANCE:
         printf("foodcard %i : %s, energy %i\n", 
		 foodcard_r, smmObj_getObjName(foodObj), smmObj_getObjEnergy(foodObj));
		 cur_player[player].energy += smmObj_getObjEnergy(foodObj);
		 break;
		//����: ��������ī��, �̼Ǽ��� 
		case SMMNODE_TYPE_FESTIVAL:
         printf("festivalcard %i : %s\n", 
		 festcard_r, smmObj_getObjName(festObj));
		 printf("You have to complete'%s'(press any key to continue)\n", smmObj_getObjName(festObj)); 
		 break;
		default:
            break;
    }
}

//����.��ġ���� 
void goForward(int player, int step)
{
	void *boardPtr; 
	cur_player[player].position += step;//����
	boardPtr = smmdb_getData(LISTNO_NODE, cur_player[player].position);//�ٲ���ġ���� 
	
	printf("%s go to node %i (name:%s)\n",
	 cur_player[player].name, cur_player[player].position, smmObj_getObjName(boardPtr));//����ణ?? 
}


int main(int argc, const char * argv[]) {
    
    FILE* fp;
    char name[MAX_CHARNAME];
    int type;
    int credit;
    int energy;
    int i;
    int initEnergy; //*������� ���濡���� 
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
    }//���� �ȿ���
    
    //����ĭ ����� 
    printf("Reading board component......\n");//���� ���� 
    while (fscanf(fp, "%s %i %i %i", name, &type, &credit, &energy) == 4) //read a node parameter set
    {
        //store the parameter set
        void *boardObj = smmObj_genObject(name, smmObjType_board, type, credit, energy, 0); 
        smmdb_addTail(LISTNO_NODE, boardObj); //��ũ�帮��Ʈ ��ü ������. �޸��� �ּ� //����Ʈ ������
		 
        board_nr ++;//�������尳�� 
    }
    fclose(fp);//���ϴݱ� 
    printf("Total number of board nodes : %i\n", board_nr);//����ĭ��Ȯ�� 
    
    for (i = 0;i<board_nr;i++){
    
        void *boardObj = smmdb_getData(LISTNO_NODE, i);
        printf("node %i : %s, %i(%s), credit %i, energy %i\n", 
		i, smmObj_getObjName(boardObj), smmObj_getObjType(boardObj), smmObj_getTypeName(smmObj_getObjType(boardObj)),smmObj_getObjCredit(boardObj), smmObj_getObjEnergy(boardObj));
    }//�� ����ĭ ���� Ȯ�� 
    //�ʱ⿡���� = ��������  
   //����ĭ���� 
    
   
    //1-2. food card config 
    if ((fp = fopen(FOODFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", FOODFILEPATH);
        return -1;
    }//���Ͼȿ��� 
    
    printf("\n\nReading food card component......\n");
    while (fscanf(fp, "%s %i", name, &energy) == 2) //read a food parameter set
    {
        //store the parameter set
        void *foodObj = smmObj_genObject(name, smmObjType_food, 0, 0, energy, 0); 
        smmdb_addTail(LISTNO_FOODCARD, foodObj);//�ʿ�����ʳ� ������ó���̾����Ծƴϴϱ� 
        
        food_nr ++;
    }
    fclose(fp);
    printf("Total number of food cards : %i\n", food_nr);
    
    for (i = 0;i<food_nr;i++)
    {	
        void *foodObj = smmdb_getData(LISTNO_FOODCARD, i);
        printf("node %i : %s, energy %i\n", 
		i, smmObj_getObjName(foodObj), smmObj_getObjEnergy(foodObj));
    }//�� ����ī��  ���� Ȯ�� 
    
    
    //1-3. festival card config 
    if ((fp = fopen(FESTFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", FESTFILEPATH);
        return -1;
    }//���Ͼȿ��� 
    
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
    }//����ī��  ���� Ȯ�� 
 
    
    //2. Player configuration ---------------------------------------------------------------------------------
    do
    {
        //*input player number to player_nr*
        printf("input player number:");
        scanf("%d", &player_nr);//�÷��̾�� ���� 
        fflush(stdin); // ���ۺ��� 
    }
    while (player_nr < 0 || player_nr > MAX_PLAYER);
    
    //�÷��̾��ü �޸��Ҵ� 
    cur_player = (player_t*)malloc(player_nr*sizeof(player_t));
    if(cur_player == NULL){
    	printf("�����޸� �Ҵ� ����\n");
		exit(1); 
	}//�����ڵ鸵 
    
    //�÷��̾� ���� �ʱ�ȭ
    initEnergy = 18;
    generatePlayers(player_nr, initEnergy);//initEnergy ���ؾߵ�!!! 
    
    
   
    //3. SM Marble game starts ---------------------------------------------------------------------------------
    while (1) //is anybody graduated?
    {
        int die_result;
        
        //4-1. initial printing
        //�÷��̾� ���� 
		printPlayerStatus();
        
        
        //4-2. die rolling (if not in experiment)
        die_result = rolldie(turn);
        
        //4-3. go forward
        goForward(turn, die_result); 

		//4-4. take action at the destination node of the board
        actionNode(turn, food_nr, festival_nr); 
        
        //4-5. next turn
        turn = (turn + 1)%player_nr;
        
    }
    
    //4. SM Marble game end------------------------------------------------------------------------------------------
    //���� �������
	
    free(cur_player);//�÷��̾��ü �޸� �ݳ� 
    system("PAUSE");
    return 0;
}
