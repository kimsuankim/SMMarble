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

/*board configuration parameters*/
static int board_nr;//보드칸수 
static int food_nr;//음식카드개수 
static int festival_nr;//축제카드개수 
static int player_nr;//플레이어명수 

/*구조체: 플레이어 구조체 형식 정의 */
typedef struct player{
	char name[MAX_CHARNAME];//이름
    int position;//현재위치 
	int energy;//보유에너지 
	int accumCredit;//누적학점 
	int flag_graduate;//졸업여부  
	int flag_lab;//실험중  
}player_t;

/*플레이어 구조체 가리키는 포인터 변수*/
static player_t *cur_player;//포인터변수선언 


//function prototypes
#if 0
int isGraduated(void); //check if any player is graduated*
void generatePlayers(int n, int initEnergy); //generate a new player*
void printGrades(int player); //print grade history of the player*
void goForward(int player, int step); //make player go "step" steps on the board (check if player is graduated)
void printPlayerStatus(void); //print all player status at the beginning of each turn*
float calcAverageGrade(int player); //calculate average grade of the player
smmGrade_e takeLecture(int player, char *lectureName, int credit); //take the lecture (insert a grade of the player)
void* findGrade(int player, char *lectureName); //find the grade from the player's grade history
int getGraduatedPlayer(void);//졸업한 플레이어 반환함수
int findLab(void);//실험실노드 반환함수 
#endif
void printGrades(int player); //print grade history of the player

/*공백만들기 함수*/
void blankSpace(void)
{
	printf("\n");
    printf("\n");
    printf("\n");
} 

/*게임시작알림함수*/
void opening(void)
{
	printf("===========================================================================\n");
	printf("-------------------------------WELCOME-------------------------------------\n");
	printf("--------------------------SOOKMYUNG MARBLE !-------------------------------\n");
	printf("------------------Let's Graduate(total credit: %i)-------------------------\n",GRADUATE_CREDIT);
	printf("===========================================================================\n");
} 

/*플레이어 생성및 초기화 함수
 입력: player_nr, initEnergy
 이름입력받아서 저장,  
 플레이어 구조체 정보(위치, 에너지, 학점, 졸업여부, 실헙중상태) 초기화 */
void generatePlayers(int n, int initEnergy)
{
	int i;
     //플레이어 수 만큼 반복 
     for (i=0;i<n;i++)
     {
         //input name
         printf("Input player %i's name: ", i); 
         scanf("%s", cur_player[i].name);//이름 저장 
         fflush(stdin);//버퍼비우기 
         
         //정보초기화 
         cur_player[i].position = 0;//위치 = 집노드(첫번째노드)에서 시작 
         cur_player[i].energy = initEnergy;//에너지 = 초기에너지 
         cur_player[i].accumCredit = 0;//학점 = 0 
         cur_player[i].flag_graduate = 0;//졸업여부 = 0 (아님)
         cur_player[i].flag_lab = 0;//실험중 = 0 (아님)
     }
}

/* 플레이어구조체 정보 출력 함수 
 입력: X 
 모든 플레이어의 상태(위치, 현재에너지, 누적학점, 실험중 상태)출력  */ 
void printPlayerStatus(void)
{
	int i;
	printf("------------------------------------------------------------------\n");
	for(i=0; i<player_nr;i++)//플레이어 수만큼 반복. 모든플레이어 상태  출력 
	{
		printf("%s : position %i, energy %i, accumCredit %i\n",
		 cur_player[i].name,//이름 
		 cur_player[i].position,//위치 
		 cur_player[i].energy,//에너지 
		 cur_player[i].accumCredit//누적학점
		 //cur_player[i].flag_lab//실험중여부 
		 );
	}
	printf("------------------------------------------------------------------\n");
}

/*주사위굴리기(자기턴마다 성적열람가능)
 입력: player(turn)
 반환: 랜덤 주사위 수*/
int rolldie(int player)
{
    char c;
    printf(" Press any key to roll a die (press g to see grade): \n");
    c = getchar();
    fflush(stdin);
    //성적을 본다면(press g) 
    if (c == 'g')
     printGrades(player);//성적출력함수호출 
    //주사위굴리기 
    return (rand()%MAX_DIE + 1); //주사위 1~6 반환 
}

/*전진.순환보드판구현 
 입력: player(turn), step(die_result), homeEnergy(집노드에너지)
 한칸씩 이동, 집노드 지나칠때 액션*/
void goForward(int player, int step, int homeEnergy)
{
	void *boardPtr;
	int i;
	int home = 1;
	//한칸씩 이동, 이동노드 출력 
	for(i = 0; i<step; i++)//step만큼 반복(최종 노드까지) 
	{
		//한칸이동, 순환보드판 구현(플레이어위치 : 0 ~ board_nr-1) 
		cur_player[player].position = (cur_player[player].position += 1)%board_nr;//다음위치 = (기존위치+1)%보드개수
		//한칸이동한 노드 데이터 받기   
	    boardPtr = smmdb_getData(LISTNO_NODE, cur_player[player].position);
		//한칸이동한 노드 출력 
		printf("=>Jump to %s\n", smmObj_getObjName(boardPtr));
		//집노드 지나치면
		if(cur_player[player].position == 0)
		{
	     //에너지충전 
    	 cur_player[player].energy += homeEnergy; //기존에너지+ 초기에너지(집노드의 에너지) 
		 printf("-->You Pass HOME\n");
		 printf("-->You got %i energy!(energy:%i)\n", homeEnergy, cur_player[player].energy);
	    } 
	}  
}

/*졸업여부 확인
 반환: 게임끝 여부
 졸업조건 만족한 플레이어 조사 -> 있다면 게임끝 반환(end = 0)*/ 
int isGraduated(void)
{
	int i;
	int end = 1;//게임 끝나지 않음 
	//누적학점>=졸업학점 : 졸업가능 
	for(i = 0; i<player_nr; i++)
	{
		if(cur_player[i].accumCredit >= GRADUATE_CREDIT)
		 cur_player[i].flag_graduate = 1;
	}
	//플레이어 구조체 멤버 졸업가능 & 집도착 
	for(i = 0; i<player_nr; i++)
	{
		if(cur_player[i].flag_graduate == 1 && cur_player[i].position == 0)//졸업조건 만족 
		end = 0;//게임끝 
		break;//반복문탈출 
	} 
	return end;
}
/*졸업한 플레이어 반환함수
 반환:졸업한 플레이어*/ 
int getGraduatedPlayer(void)
{
	int i;
	int gra_player=0;//졸업한 플레이어 선언 
	for(i=0;i<player_nr;i++)//플레이어 수 만큼 반복 
	{
		if(cur_player[i].flag_graduate == 1 && cur_player[i].position == 0)//졸업조건 만족 
		 gra_player = i;//졸업한 플레이어 찾음 
		break;//찾으면 반복문 나감 
	}
	return gra_player;//졸업한 플레이어 반환 
}


//랜덤성적부여함수 
int takeLecture(void)
{
	//랜덤성적변수 
	int grade = rand()%MAX_GRADE;
	printf("%i\n", grade);

	return grade;
}


//성적 이력 검색 함수
int findGrade(int player, char *lectureName)
{
	int i;
	void *gradePtr;
	int already = 0;//강의안들음 
	for(i=0;i<smmdb_len(LISTNO_OFFSET_GRADE + player);i++)
	{
		gradePtr = smmdb_getData(LISTNO_OFFSET_GRADE + player, i);
		//강의이력있으면 
		if(smmObj_getObjName(gradePtr) == lectureName)
		already = 1;//강의들음 
	}
	
	return already;//강의수강여부 반환 
}

//성적구조체 출력 함수 
void printGrades(int player)
{
	int i;
	void *gradePtr;  
	for(i=0;i<smmdb_len(LISTNO_OFFSET_GRADE + player);i++)
	{
		gradePtr = smmdb_getData(LISTNO_OFFSET_GRADE + player, i);
		printf("%s grade:%s credit:%i \n", smmObj_getObjName(gradePtr), smmObj_getGradeName(smmObj_getObjGrade(gradePtr)), smmObj_getObjCredit(gradePtr));
	}
}

/*실험실노드 반환 함수
 반환: 실험실노드 */
int findLab(void)
{
	int i;
	void *labPtr;  
	for(i=0;i<board_nr;i++)//모든노드확인 
	{
		labPtr = smmdb_getData(LISTNO_NODE, i);//노드데이터받기 
		//노드타입 == 실험실이면 
		if(smmObj_getObjType(labPtr) == SMMNODE_TYPE_LABORATORY) 
		break;//반복문나감 
	}
	return i;//실험실노드반환 
} 
//action code when a player stays at a node
void actionNode(int player) //바꿔야함 
{
	//변수지정 
	void* boardPtr = smmdb_getData(LISTNO_NODE, cur_player[player].position);//플레이어 위치따른 노드구조체주소  
    int type = smmObj_getObjType(boardPtr);//노드구조체 유형 
    char *name = smmObj_getObjName(boardPtr);//노드구조체 이름 
    int credit = smmObj_getObjCredit(boardPtr);//노드 구조체 학점
	int energy = smmObj_getObjEnergy(boardPtr);//노드 구조체 에너지 
    //void *gradePtr =  smmdb_getData(LISTNO_OFFSET_GRADE + player, cur_player[player].lecture+1);//강의들으면 그 성적 출력 
    //smmObjGrade_e grade_r = smmObj_getObjGrade(gradePtr)//성적받기 
    
    //랜덤카드뽑기 
    int foodcard_r = (rand()%food_nr);
    int festcard_r = (rand()%festival_nr);
    void *foodPtr = smmdb_getData(LISTNO_FOODCARD, foodcard_r);
    void *festPtr = smmdb_getData(LISTNO_FESTCARD, festcard_r);
    
	switch(type)//각타입에 대한액션 
    {
        /*1.강의노드(0)*/
        case SMMNODE_TYPE_LECTURE:
        	//1-1.강의수강가능 :소요에너지이상, 듣지않은강의
        	if(cur_player[player].energy >= smmObj_getObjEnergy(boardPtr) && findGrade(player, name) == 0 )
        	{
        	 char c;
        	 printf("-->Lecture %s(credit:%i, energy:%i) starts!\n(press a to take a lecture):"
			  ,name, credit, energy);
        	 c = getchar();
        	 fflush(stdin);
        	  //1-1.1.강의 수강 선택 
        	  if(c == 'a')
        	  {
        	  	//학점추가, 에너지소모 
        	  	cur_player[player].accumCredit += smmObj_getObjCredit(boardPtr);
                cur_player[player].energy -= smmObj_getObjEnergy(boardPtr);
                //grade generation 성적구조체생성,저장 
                int grade = takeLecture();  
                //성적구조체 생성,저장  
	            void* gradeObj = smmObj_genObject(name, smmObjType_grade, 0, credit, 0, grade);//강의이름,학점,성적 
                 smmdb_addTail(LISTNO_OFFSET_GRADE + player, gradeObj);
                //강의후 변화 출력 
                printf("-->%s successfully takes the lecture %s with grade %s, remained energy : %i", //(average:%i)
				 cur_player[player].name, smmObj_getObjName(boardPtr), smmObj_getGradeName(smmObj_getObjGrade(gradeObj)), cur_player[player].energy);
			  }
			  //1-1.2.강의드랍선택 
			  else
			  printf("You drop %s\n",name);
			  
			break;
			}
			//소요에너지가부족하거나 들었던강의임
			printf("You can't take a %s\n(소요에너지 부족, 들었던 강의)\n", name);
         
         break;
         
        /*식당(1) : 에너지충전 */
        case SMMNODE_TYPE_RESTAURANT:
         //에너지 충전
		 cur_player[player].energy += smmObj_getObjEnergy(boardPtr); 
		 printf("You got %i energy!\n", smmObj_getObjEnergy(boardPtr));
		 break; 
		 
		/*실험실(2) : 무인도랑 비슷*/ 
		case SMMNODE_TYPE_LABORATORY:
		 //2-1.실험중
		 if (cur_player[player].flag_lab == 1)
		  {
		  	//실험성공 기준값  
		  	int threshold = (rand()%MAX_DIE + 1);
		  	printf("->Experiment time!! Let's see if you can satisfy professor(threshold:%i)\n", threshold);
		  	//실험하기 = 주사위 굴리기, 에너지 소모 
		  	int ex_result = rolldie(player);
		  	printf("->Experiment result : %i\n", ex_result); 
		  	cur_player[player].energy -= smmObj_getObjEnergy(boardPtr);//에너지소모 
		  	//실험성공여부 
		  	//--성공 
		  	if(ex_result >= threshold)
			  {
			  	printf("success!! %s can exit this lab!", cur_player[player].name); 
			  	//실험중 상태 아님
			  	cur_player[player].flag_lab = 0;
				//실험실 탈출   
		      }
		    //--실패 
		    else
			printf("fail.. %s cannot exit this lab.. you can try it again next turn", cur_player[player].name);
		  } 
		  //2-2.실험중아님 
		  else
		  printf("->This is not experiment time. You can go through this lab.");
		break;
		
		/*집(3): 에너지충전 :월급개념
		case SMMNODE_TYPE_HOME: 
	    goForward 함수에 구현함;전진 함수 
		*/	
		
		/*실험실로가(4) : 실험실로 이동, 실험중 상태 */
		case SMMNODE_TYPE_GOTOLAB:
			//1)실험중 상태로 변경
			cur_player[player].flag_lab = 1;
			//2)실험실로 이동 :실험실노드반환함수 
			cur_player[player].position = findLab();//실험실로 이동함 
			actionNode(player);//실험실에서액션 
			break;
			
		/*보충찬스(5):랜덤음식카드, 에너지보충*/
		case SMMNODE_TYPE_FOODCHANCE:
		 //랜덤음식카드 뽑기 
         printf("foodcard %i : %s, energy %i\n", 
		 foodcard_r, smmObj_getObjName(foodPtr), smmObj_getObjEnergy(foodPtr));
		 //에너지보충 
		 cur_player[player].energy += smmObj_getObjEnergy(foodPtr);
		 break;
		 
		/*축제(6): 랜덤축제카드, 미션수행 */
		case SMMNODE_TYPE_FESTIVAL:
		 //랜덤축제카드 뽑기 
         printf("festivalcard %i : %s\n", 
		 festcard_r, smmObj_getObjName(festPtr));
		 //미션하기 
		 char c;
         printf("You have to complete'%s'(press any key to continue):\n", smmObj_getObjName(festPtr));
         c= getchar();
         fflush(stdin);
		  
		 break;
		 
		default:
            break;
    }
}

int main(int argc, const char * argv[]) {
    
    FILE* fp;//파일포인터 
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
    /*1-1. boardConfig : 보드노드구조체 생성, 정보저장, 노드수저장,노드구조체 출력*/
    //0)파일안열림 
    if ((fp = fopen(BOARDFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", BOARDFILEPATH);
        getchar();
        return -1;
    }
    
    //1)보드노드 구조체 생성 및 저장 
    printf("Reading board component......\n");//파일 열기 
    while (fscanf(fp, "%s %i %i %i", name, &type, &credit, &energy) == 4) //read a node parameter set
    {
        //*store the parameter set
        void *boardObj = smmObj_genObject(name, smmObjType_board, type, credit, energy, 0);//노드 구조체 생성함수, 구조체 정보저장 
        smmdb_addTail(LISTNO_NODE, boardObj); //노드 구조체 주소 저장 
		 
        board_nr ++;//보드저장개수 
    }
    fclose(fp);//파일닫기 
    //노드개수확인:board_nr 
    printf("Total number of board nodes : %i\n", board_nr); 
    
    //2)저장된 노드 정보 출력 
    for (i = 0;i<board_nr;i++)//노드칸수만큼 
	{
		//노드구조체 데이터 받기
        void *boardObj = smmdb_getData(LISTNO_NODE, i);//노드구조체 주소 출력함수 호출 
        //각노드구조체 데이터 출력(노드 구조체 멤버 접근 함수 호출) 
        printf("node %i : %s, %i(%s), credit %i, energy %i\n", 
		i, smmObj_getObjName(boardObj), smmObj_getObjType(boardObj), smmObj_getTypeName(smmObj_getObjType(boardObj)),smmObj_getObjCredit(boardObj), smmObj_getObjEnergy(boardObj));
    } 
    
   
    //1-2. food card config 
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
    
    
    //1-3. festival card config 
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
 
 
    //파일구현끝 
	//opening -------------------------------------------------------------------------------------------------
    blankSpace();//공백함수 
    opening();//게임오프닝 
    
    //2. Player configuration ---------------------------------------------------------------------------------
    
    /*2.1.input player number to player_nr*/
	do
    {
        printf("input player number(1~%i): ",MAX_PLAYER); 
        scanf("%d", &player_nr);//플레이어수 저장 
        fflush(stdin); // 버퍼비우기 
    }
    while (player_nr < 0 || player_nr > MAX_PLAYER);//0 < 플레이어수 < 최대플레이어수 
    
    /*2.2.플레이어구조체 메모리할당*/ 
    cur_player = (player_t*)malloc(player_nr*sizeof(player_t));//플레이어 구조체 동적 메모리 할당  
    if(cur_player == NULL){
    	printf("동적메모리 할당 오류\n");
		exit(1); 
	}//에러핸들링 
    
    /*2.3.플레이어 상태 초기화*/
    //1)초기에너지 설정 
	void *boardPtr_f = smmdb_getData(LISTNO_NODE, 0); //집노드(첫번째노드) 데이터 받기 
    initEnergy = smmObj_getObjEnergy(boardPtr_f);//초기에너지 = 집노드의 에너지 
    
    //2)플레이어 초기화 
    generatePlayers(player_nr, initEnergy);//플레이어 생성및 초기화 함수 호출 
    
    
   
    //3. SM Marble game starts ---------------------------------------------------------------------------------
    while (isGraduated() == 1) //is anybody graduated?
    {
        int die_result;
        int passHome = 1;
        
        //3-1. initial printing
        //1)플레이어 상태출력
		blankSpace(); 
		printPlayerStatus();
		//2)누구차례인지 
		printf("This is %s's turn:::", cur_player[turn].name);
        //3-2. die rolling (if not in experiment)
        if(cur_player[turn].flag_lab != 1) //실험중이 아니면 
        {
        	die_result = rolldie(turn);
        	printf("-->result : %i\n", die_result);
        
         //3-3. go forward
		 goForward(turn, die_result, initEnergy); 
         //집지나치는지 여부(passHome = 0) 
         
        }
		//3-4. take action at the destination node of the board
        actionNode(turn); 
        
        //3-5. next turn
        turn = (turn + 1)%player_nr;
        
    }
    
    //4. SM Marble game end------------------------------------------------------------------------------------------
    //졸업한 플레이어 확인, 출력 
    blankSpace(); 
    int player_gr = getGraduatedPlayer();//졸업한플레이어 
    printf("GAME END!!\n");
    printf("GraduatedPlayer is %s\n", cur_player[player_gr].name);
    //졸업한 플레이어의 수강이력 출력 
    printGrades(player_gr);
	printf("%s graduate SMWU!!!!!Congratuation!!!!!\n", cur_player[player_gr].name);
	
	
    free(cur_player);//플레이어구조체 메모리 반납
	free(boardPtr_f);//첫번째노드구조체  메모리 반납 
    system("PAUSE");
    return 0;
}
