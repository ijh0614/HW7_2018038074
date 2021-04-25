/*
 *  doubly-linked-list.c
 *
 *  Doubly Linked List
 *
 *  Data Structures
 *  Department of Computer Science 
 *  at Chungbuk National University
 *
 */



#include<stdio.h>
#include<stdlib.h>
/* 필요한 헤더파일 추가 if necessary */


typedef struct Node {//노드 구조
	int key;//저장 값
	struct Node* llink;//struct를 붙혀서 지연시켜준다.
	struct Node* rlink;//링크가 두개 임. llink는 앞, rlink는 뒤의 노드 주소를 저장함.
} listNode;



typedef struct Head {//헤드노드. 시작 주소를 저장한다. 데이터가 비어있는지 구분해 줄 때 사용
	struct Node* first;//Node의 포인터형 변수 중 맨 처음 노드의 주소를 저장
}headNode;


/* 함수 리스트 */
/* note: initialize는 이중포인터를 매개변수로 받음
         singly-linked-list의 initialize와 차이점을 이해 할것 */

/*반환값이 int, 매개변수가 이중포인터라는 점에서 singly-linked-list와 다르다.
이중 포인터를 써서 포인터 변수가 가르키는 값을 변경. 즉 initialize함수 자체에서 초기화*/
int initialize(headNode** h);

/* note: freeList는 싱글포인터를 매개변수로 받음
        - initialize와 왜 다른지 이해 할것
        - 이중포인터를 매개변수로 받아도 해제할 수 있을 것 */
/*initialize함수에서 호출할때는 매개변수로 이중포인터가 오고 */
int freeList(headNode* h);

int insertNode(headNode* h, int key);
int insertLast(headNode* h, int key);
int insertFirst(headNode* h, int key);
int deleteNode(headNode* h, int key);
int deleteLast(headNode* h);
int deleteFirst(headNode* h);
int invertList(headNode* h);

void printList(headNode* h);


int main()
{
	char command;
	int key;
	headNode* headnode=NULL;

	do{
		printf("-------[2018038074]-----------------------[임종훈]--------------\n");
		printf("                     Doubly Linked  List                        \n");
		printf("----------------------------------------------------------------\n");
		printf(" Initialize    = z           Print         = p \n");
		printf(" Insert Node   = i           Delete Node   = d \n");
		printf(" Insert Last   = n           Delete Last   = e\n");
		printf(" Insert First  = f           Delete First  = t\n");
		printf(" Invert List   = r           Quit          = q\n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {
		case 'z': case 'Z':
		//headnode의 주소를 받음. 따라서 이중포인터로 받아줘야 함.
			initialize(&headnode);
			break;
		case 'p': case 'P':
			printList(headnode);
			break;
		case 'i': case 'I':
			printf("Your Key = ");
			scanf("%d", &key);
			insertNode(headnode, key);
			break;
		case 'd': case 'D':
			printf("Your Key = ");
			scanf("%d", &key);
			deleteNode(headnode, key);
			break;
		case 'n': case 'N':
			printf("Your Key = ");
			scanf("%d", &key);
			insertLast(headnode, key);
			break;
		case 'e': case 'E':
			deleteLast(headnode);
			break;
		case 'f': case 'F':
			printf("Your Key = ");
			scanf("%d", &key);
			insertFirst(headnode, key);
			break;
		case 't': case 'T':
			deleteFirst(headnode);
			break;
		case 'r': case 'R':
			invertList(headnode);
			break;
		case 'q': case 'Q':
			freeList(headnode);
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	freeList(headnode);//프로그램 종료시 동적할당 해제
	return 1;
}


int initialize(headNode** h) {//headnode의 주소를 받음

	/* headNode가 NULL이 아니면, freeNode를 호출하여 할당된 메모리 모두 해제 */
	//headNode가 NULL이라는 것은 만들어진 연결리스트가 없다는 것이고, NULL이 아니면 아직 연결리스트가 남았는데 초기화해주는 것.
	if(*h != NULL){
		freeList(*h);//여기서는 이중포인터이므로 한번 역참조 해줘야 함.
	}

	//값을 저장하는 노드를 선언하는게 아니라, 첫번째 노드의 주소를 저장하는 헤드노드 선언
	headNode** h = (headNode*)malloc(sizeof(headNode));
	(*h)->first = NULL;//아직 주소 없음

	return 1;
}

int freeList(headNode* h){//headnode의 주소
	listNode* p = h->first;//첫번째 노드의 주소
	listNode* temp = NULL;//다음 노드 임시저장

	while(temp != NULL){
		temp = p->rlink;//다음 노드의 주소 임시저장
		free(p);
	}
	//마지막에 헤드노드 해제
	free(h);
	return 0;
}


void printList(headNode* h) {
	int i = 0;
	listNode* p;

	printf("\n---PRINT\n");

	if(h == NULL) {
		printf("Nothing to print....\n");
		return;
	}

	p = h->first;

	while(p != NULL) {
		printf("[ [%d]=%d ] ", i, p->key);
		p = p->rlink;
		i++;
	}

	printf("  items = %d\n", i);
}




/**
 * list에 key에 대한 노드하나를 추가
 */
/*1. 노드 동적할당 2.노드에 키 저장 3.마지막 노드까지 이동
4. 마지막 노드 rlink에 새로운 노드 주소 추가 5. 새로운 노드 llink에 
마지막 노드 주소 추가 6.새로운 노드 rlink에 NULL추가*/
int insertLast(headNode* h, int key) {
	listNode* node = (listNode*)malloc(sizeof(listNode));
	listNode* temp = h->first;//다음 노드 임시저장할 노드 선언

	if(temp == NULL){//전처리. 저장되어있는 노드가 없을 경우
		insertFirst(h, key);
		return 0;
	}

	node->key = key;
	node->rlink = NULL;

	while(temp != NULL){//끝으로 이동
		node->llink = temp;//
		temp = temp->rlink;//다음 노드의 주소 임시저장
	}
	temp = node;//temp는 rlink인 상태
	return 0;
}



/**
 * list의 마지막 노드 삭제
 */
int deleteLast(headNode* h) {


	return 0;
}



/**
 * list 처음에 key에 대한 노드하나를 추가
 */
/*1. 노드 동적할당 2.노드에 키 저장 3. 헤드노드가 이전에 가르키던 노드 rlink에 저장
4.헤드노드를 llink에 저장 5.헤드노드가 가르키도록 함*/
int insertFirst(headNode* h, int key) {

	listNode* node = (listNode*)malloc(sizeof(listNode));
	node->key = key;//값 저장
	node->rlink = h->first;
	node->llink = h;//link 저장
	h->first = node;
	return 0;
}

/**
 * list의 첫번째 노드 삭제
 */
int deleteFirst(headNode* h) {
	
	return 0;
}



/**
 * 리스트의 링크를 역순으로 재 배치
 */
int invertList(headNode* h) {

	return 0;
}



/* 리스트를 검색하여, 입력받은 key보다 큰값이 나오는 노드 바로 앞에 삽입 */
int insertNode(headNode* h, int key) {

	return 0;
}


/**
 * list에서 key에 대한 노드 삭제
 */
int deleteNode(headNode* h, int key) {

	return 1;
}