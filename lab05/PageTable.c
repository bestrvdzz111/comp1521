// Table.c ... implementation of  Table operations
// COMP1521 17s2 Assignment 2
// Written by John Shepherd, September 2017

#include <stdlib.h>
#include <stdio.h>
#include "Memory.h"
#include "Stats.h"
#include "PageTable.h"

// Symbolic constants

#define NOT_USED 0
#define IN_MEMORY 1
#define ON_DISK 2

// PTE =  Table Entry

typedef struct {
   char status;      // NOT_USED, IN_MEMORY, ON_DISK
   char modified;    // boolean: changed since loaded
   int  frame;       // memory frame holding this
   int  accessTime;  // clock tick for last access
   int  loadTime;    // clock tick for last time loaded
   int  nPeeks;      // total number times this  read
   int  nPokes;      // total number times this  modified
   // TODO: add more fields here, if needed ...
} PTE;

struct DLListRep{
  int nitems;
  int first;
  int tail;
  unsigned size;
  int *page;
};

struct DLListNode{
  int item;
  struct DLListNode *next;
  struct DLListNode *prev;
};

//double linklist to implement the LRU

// The virtual address space of the process is managed
//  by an array of  Table Entries (PTEs)
// The  Table is not directly accessible outside
//  this file (hence the static declaration)

static PTE *PageTable;      // array of page table entries
static int  nPages;         // # entries in page table
static int  replacePolicy;  // how to do page replacement
static int  fifoList;       // index of first PTE in FIFO list
static int  fifoLast;       // index of last PTE in FIFO list
static struct DLListRep* fifo; //fifo
static struct DLListNode *head;    //lru
static struct DLListNode *tail;    //lru

// Forward refs for private functions

static int findVictim(int);
struct DLListRep *NewList(unsigned);
int isFull(struct DLListRep*);
int isEmpty(struct DLListRep*);
int dropDLList(struct DLListRep*);
void enterNode(struct DLListRep*,int);
void InsertNode();
void deleteTail();
void deleteItem(int);

// initPageTable: create/initialise Page Table data structures

void initPageTable(int policy, int np)
{
   PageTable = malloc(np * sizeof(PTE));
   if (PageTable == NULL) {
      fprintf(stderr, "Can't initialise Memory\n");
      exit(EXIT_FAILURE);
   }
   replacePolicy = policy;
   nPages = np;
   fifoList = 0;
   fifoLast = nPages-1;
   for (int i = 0; i < nPages; i++) {
      PTE *p = &PageTable[i];
      p->status = NOT_USED;
      p->modified = 0;
      p->frame = NONE;
      p->accessTime = NONE;
      p->loadTime = NONE;
      p->nPeeks = p->nPokes = 0;
   }

   fifo = NewList(nPages);
}

void updatPageTable(int pno,int fno,int time)
{
   PTE *p = &PageTable[pno];

   p->status =  IN_MEMORY;
   p->modified = 0;
   p->frame = fno;
   p->loadTime= time;
   p->accessTime = time;
   enterNode(fifo, pno);
}

void updateVictm(int vno, int time)
{
   PTE *p = &PageTable[vno];
   p->status =  ON_DISK;
   p->modified = 0;
   p->frame = NONE;
   p->loadTime = NONE;
   p->accessTime = NONE;
   deleteItem(vno);
}
// requestPage: request access to page pno in mode
// returns memory frame holding this page
//  may have to be loaded
// PTE(status,modified,frame,accessTime,next,nPeeks,nWrites)

int requestPage(int pno, char mode, int time)
{
   if (pno < 0 || pno >= nPages) {
      fprintf(stderr,"Invalid page reference\n");
      exit(EXIT_FAILURE);
   }
   PTE *p = &PageTable[pno];
   int fno;
   switch (p->status) {
   case NOT_USED:
   case ON_DISK:
      // TODO: add stats collection
   countPageFault();
      fno = findFreeFrame();
      if (fno == NONE) {
         int vno = findVictim(time);
#ifdef DBUG
         printf("Evict page %d\n",vno);
#endif

        PTE *v = &PageTable[vno];
        if(p -> modified!=0)
        {
            saveFrame(fno);
        }
        fno = v->frame;
        updateVictm(vno,time);
         // TODO:
         // if victim page modified, save its frame
         // collect frame# (fno) for victim page
         // update PTE for victim page
         // - new status
         // - no longer modified
         // - no frame mapping
         // - not accessed, not loaded
      }
      printf("Page %d given frame %d\n",pno,fno);
      // TODO:
      // load page pno into frame fno
      // update PTE for page
      // - new status
      // - not yet modified
      // - associated with frame fno
      // - just loaded
      loadFrame(fno,pno,time);
      updatPageTable(pno,fno,time);
      break;
   case IN_MEMORY:
      // TODO: add stats collection
      countPageHit();
      break;
   default:
      fprintf(stderr,"Invalid page status\n");
      exit(EXIT_FAILURE);
   }
   if (mode == 'r')
      p->nPeeks++;
   else if (mode == 'w') {
      p->nPokes++;
      p->modified = 1;
   }
   p->accessTime = time;
   deleteItem(pno);
   InsertNode(pno);

   return p->frame;
}

// findVictim: find a  to be replaced
// uses the configured replacement policy

static int findVictim(int time)
{
   int victim = 0;
   switch (replacePolicy) {
   case REPL_LRU:
      // TODO: implement LRU strategy


      //************************************
      //implement Lru using DLList
      victim = tail->item;
      deleteTail();
      break;
   case REPL_FIFO:
      // TODO: implement FIFO strategy


      //***********************************
      //fifolist
      //dropDLList gives the early insert node
      victim = dropDLList(fifo);
      break;
   case REPL_CLOCK:
      return 0;
   }
   return victim;
}

// showTableStatus: dump  table
// PTE(status,modified,frame,accessTime,next,nPeeks,nWrites)

void showPageTableStatus(void)
{
   char *s;
   printf("%4s %6s %4s %6s %7s %7s %7s %7s\n",
          "Page","Status","Mod?","Frame","Acc(t)","Load(t)","#Peeks","#Pokes");
   for (int i = 0; i < nPages; i++) {
      PTE *p = &PageTable[i];
      printf("[%02d]", i);
      switch (p->status) {
      case NOT_USED:  s = "-"; break;
      case IN_MEMORY: s = "mem"; break;
      case ON_DISK:   s = "disk"; break;
      }
      printf(" %6s", s);
      printf(" %4s", p->modified ? "yes" : "no");
      if (p->frame == NONE)
         printf(" %6s", "-");
      else
         printf(" %6d", p->frame);
      if (p->accessTime == NONE)
         printf(" %7s", "-");
      else
         printf(" %7d", p->accessTime);
      if (p->loadTime == NONE)
         printf(" %7s", "-");
      else
         printf(" %7d", p->loadTime);
      printf(" %7d", p->nPeeks);
      printf(" %7d", p->nPokes);
      printf("\n");
   }
}

//********** DLList functions *************

struct DLListRep *NewList(unsigned size)
{
  struct DLListRep *d = (struct DLListRep *)malloc(sizeof(struct DLListRep));
  d->size = size;
  d->first = d->nitems = 0;
  d->tail = size - 1;
  d->page = (int*)malloc(d->size * sizeof(int));
  return d;
}

int isFull(struct DLListRep *d)
{
  return(d->nitems == d->size);
}

int isEmpty(struct DLListRep *d)
{
  return(d->size == 0);
}

//add node to DLList
void enterDLList(struct DLListRep *d,int item)
{
  if(isFull(d))return;
  d->tail = (d->tail + 1)%(d->size);
  d->page[d->tail] = item;
  d->nitems++;
}

int dropDLList(struct DLListRep *d)
{
  if(isEmpty(d))return 0;
  int item = d->page[d->first];
  d->first = (d->first + 1) % d->size;
  d->nitems--;
  return item;
}


//********Node list functions ********
struct DLListNode *newNode(int item)
{
  struct DLListNode *new = (struct DLListNode*)malloc(sizeof(struct DLListNode));
  new->item = item;
  new->prev = NULL;
  new->next = NULL;
  return new;
}

//intsert to head
void InsertNode(int item)
{
  struct DLListNode *new = newNode(item);
  if(head == NULL)
  {
    head = new;
    tail = new;
  }
  else
  {
    head->prev = new;
    new->next = head;
    head = new;
  }
}

void deleteTail()
{
  struct DLListNode *todoList;
  if(tail == NULL)
  {
    return;
  }
  else
  {
    todoList = tail;
    tail = tail->prev; //let tail point to new last
    tail->next = NULL; //delete the item
    free(todoList);
  }
}

void deleteItem(int item)
{
  struct DLListNode *curr;
  struct DLListNode *temp;
  for(curr = head;curr!=NULL;curr = curr->next)
  {
    if(curr->item == item)
    {
      if(curr->prev == NULL)
      {
        head = curr->next;
      }
      else if(curr->next == NULL)
      {
        deleteTail();
      }
      else
      {
        temp = curr->prev;
        temp->next = curr->next;
        temp = curr->next;
        temp->prev = curr->prev;
      }
      free(curr);
    }
  }
}
