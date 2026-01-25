/*Critical concept: Intrusive Linked lists
  This is a core systems concept:
   Intrusive linked list show up everywhere in kernels,
   loaders, schedulers, and low-level runtimes (Windows,
   Linux, BSD, etc.).
 */

#include <stddef.h>
#include <stdio.h>

#define container_of(ptr, type, member) \
    ((type *)((char *)(ptr) - offsetof(type, member)))

#define LIST_FOR_EACH(pos, head) \
    for(pos = (head)->next; pos != (head); pos = pos->next)

//Intrusive:

//List Node
typedef struct _LIST_ENTRY{
    struct _LIST_ENTRY *prev;
    struct _LIST_ENTRY *next;
} LIST_ENTRY;

static inline void ListInit(LIST_ENTRY *head){
    head->next = head;
    head->prev = head;
}

static inline void ListInsertTail(LIST_ENTRY *head, LIST_ENTRY *entry){
    entry->prev = head->prev;
    entry->next = head;

    head->prev->next = entry;
    head->prev = entry;
}

static inline void ListRemove(LIST_ENTRY *entry){
    entry->prev->next = entry->next;
    entry->next->prev = entry->prev;

    entry->next = entry;
    entry->prev = entry;  
}

static inline int ListIsEmpty(const LIST_ENTRY *head){
    return head->next ==  head;
}

//Object which contain List Node
typedef struct _PROCESS{
    int pid;
    char name[64];
    LIST_ENTRY memolink;//list intrudes
    LIST_ENTRY loadlink;
} PROCESS;

typedef struct _CORE_STRUCT{
    LIST_ENTRY memoList;
    LIST_ENTRY loadLiset;
} CORESTRUCT;

int main(){

    CORESTRUCT crSt;
    
    LIST_ENTRY *tauko = &crSt.memoList;
    ListInit(tauko);
    // LIST_ENTRY loadList;
    //ListInit(&loadList);
    

    PROCESS p1 = {
        .pid = 111,
        .name = "FirstProcess"
    }; 
    PROCESS p2 = {
        .pid = 112,
        .name = "SecondProcess"
    };
    PROCESS p3 = {
        .pid = 113,
        .name = "ThirdProcess"
    };

    ListInsertTail(tauko, &p1.memolink);
    ListInsertTail(tauko, &p2.memolink);
    ListInsertTail(tauko, &p3.memolink);

    /* ListInsertTail(&loadList, &p1.loadlink); */
    /* ListInsertTail(&loadList, &p2.loadlink); */
    /* ListInsertTail(&loadList, &p3.loadlink); */

    printf("Process List:\n");

    LIST_ENTRY *pos;
    puts("Memo List of Process Object:");
    LIST_FOR_EACH(pos, tauko){
        PROCESS *p = container_of(pos, PROCESS, memolink);
        printf("PID = %d Name = %s\n", p->pid, p->name);
    }

    /* puts("Load List of Process Object:"); */
    /* LIST_FOR_EACH(pos, &loadList){ */
    /*     PROCESS *p = container_of(pos, PROCESS, loadlink); */
    /*     printf("PID = %d Name = %s\n", p->pid, p->name); */
    /* } */

    ListRemove(&p2.memolink);
    puts("Remove a process2 form memoList of Process Object:");
    puts("After remove operation\nMemo List of Process Object:");
    LIST_FOR_EACH(pos, tauko){
        PROCESS *p = container_of(pos, PROCESS, memolink);
        printf("PID = %d Name = %s\n", p->pid, p->name);
    }

    /* puts("Load List of Process Object:"); */
    /* LIST_FOR_EACH(pos, &loadList){ */
    /*     PROCESS *p = container_of(pos, PROCESS, loadlink); */
    /*     printf("PID = %d Name = %s\n", p->pid, p->name); */
    /* } */

    
    return 0;
}

