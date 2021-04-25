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
	(*h) = (headNode*)malloc(sizeof(headNode));
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

	while(temp->rlink != NULL){//끝으로 이동
		temp = temp->rlink;//다음 노드의 주소 임시저장
	}
	node->llink = temp;
	temp->rlink = node;//temp는 rlink인 상태
	return 0;
}



/**
 * list의 마지막 노드 삭제
 */
int deleteLast(headNode* h) {
	listNode *del_node = h->first;
	listNode *temp;
	
	if(h->first == NULL){//노드 삭제 전처리
		printf("There is no node to delete.\n\n");//삭제할 노드가 없습니다.
		return 0;
	}

	while(del_node->rlink != NULL){//끝으로 이동
		del_node = del_node->rlink;//다음 노드의 주소 임시저장
	}

	//노드가 하나일때
	if(del_node == h->first){
		deleteFirst(h);
		return 0;
	}

	temp = del_node->llink;//마지막의 이전 노드를 저장
	temp->rlink = NULL;//del_node를 가르키던 링크에 NULL저장
	free(del_node);//삭제

	return 0;
}



/**
 * list 처음에 key에 대한 노드하나를 추가
 */
/*1. 노드 동적할당 2.노드에 키 저장 3. 헤드노드가 이전에 가르키던 노드 rlink에 저장
4.헤드노드를 llink에 저장 5.헤드노드가 가르키도록 함*/
//노드가 하나도 없을때, 원래 노드가 있을 때 구분
int insertFirst(headNode* h, int key) {

	listNode* node = (listNode*)malloc(sizeof(listNode));
	listNode* will_sec_node;

	node->key = key;//값 저장
	will_sec_node = h->first;//첫번째 노드

	if(will_sec_node == NULL){//아무 노드도 저장되어있지 않을 때
		node->llink = h->first;//헤드 노드의 주소 
		h->first = node;
		node->rlink = NULL;//처음이면 끝 링크에 NULL값 저장.
		return 0;
	}
	//원래 노드가 있었을 때
	node->rlink = will_sec_node;//두번째 노드의 주소
	node->llink = NULL;//헤드노드 가르킬 필요 없음
	h->first = node;
	will_sec_node->llink = node;
	return 0;
}

/**
 * list의 첫번째 노드 삭제
 */
int deleteFirst(headNode* h) {
	listNode *del_node;//지울 노드
	listNode *second_node;//지울 노드의 다음 노드

	if(h->first == NULL){//노드 삭제 전처리
		printf("There is no node to delete.\n\n");
		return 0;
	}
	del_node = h->first;
	second_node = del_node->rlink;

	if(second_node == NULL){//노드가 하나일 때
		h->first = NULL;
		free(del_node);
		return 0;
	}
	//노드 두개 이상일 때
	h->first = second_node;
	second_node->llink = h->first;//헤드 노드와 연결
	free(del_node);

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
	if(h->first == NULL){//하나도 저장되어있지 않으면 그냥 저장
		insertFirst(h, key);
		return 0;
	}
	
	listNode* node = (listNode*)malloc(sizeof(listNode));//노드 생성
	listNode* temp = h->first;
	listNode* last_node;

	node->key = key;//값 저장

	while(temp != NULL){//노드가 헤드노드랑 첫번째 노드 사이에 온다면?
		if(temp->key >= key){//탐색중인 노드의 값이 입력 값보다 크면
			if(temp == h->first){//첫번째에 삽입해야 하는 경우
				node->llink = NULL;//헤드 노드는 가르킬 필요가 없음
				node->rlink = temp;
				h->first = node;
				temp->llink = node;//양쪽에서 삽입한 노드 가르키도록 설정
				return 0;
			}
			//이 외에 삽입할때
			node->llink = temp->llink;
			node->rlink = temp;//노드의 오른쪽 링크는 큰 값을 검사한 노드
			temp->llink->rlink = node;//이전 노드의 오른쪽 링크를 노드로 바꾸고
			temp->llink = node;
			return 0;
		}
		last_node = temp;//마지막 노드 저장
		temp = temp->rlink;//다음 노드로 이동
	}
	//끝까지 왔으면
	node->rlink = NULL;
	node->llink = last_node;
	last_node->rlink = node;

	return 0;
}


/**
 * list에서 key에 대한 노드 삭제
 */
int deleteNode(headNode* h, int key) {
	if(h->first == NULL){//노드 삭제 전처리
		printf("There is no node to delete.\n\n");
		return 0;
	}

	return 1;
}