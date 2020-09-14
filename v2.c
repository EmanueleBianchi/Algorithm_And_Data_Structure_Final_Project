#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>




//FILE* fp;




#define ListFirstIndex 1

typedef char** lines;

static char* endLines = "\0";

int linesSize(lines lines){
     if(lines == NULL){
          return 0;
     }
     int size = 0;
     while( lines[size]!= endLines){
          size++;
     }
     return size;
}


//must check if start< lines Size before calling
void printLines(lines lines, int start, int end){
     for (size_t i = start-1; i < end && lines[i] != endLines; i++) {
          printf("%s\n",lines[i]);
     }
}

void destructor_Lines(lines lines){
     if(lines == NULL){
          return;
     }


     int i = 0;
     while( lines[i]!= endLines){
          free(lines[i]);
          i++;
     }
     free(lines);
}








struct Instruction{
     void (*print)(void* _self,int start, int end);
     int (*size)(void* _self);
     void* (*getLast)(void* _self);
     void* (*getNext)(void* _self);

     void (*setLast)(void* _self, void* last);
     void (*setNext)(void* _self, void* next);
     void (*destructor)(void* _self);
};

void print_Instruction(void* _self,int start, int end){
     struct Instruction** self = _self;
     (*((*self)->print))(_self,start, end);
}

int getSize_Instruction(void* _self){
     struct Instruction** self = _self;
     return (*((*self)->size))(_self);
}

void* getLast(void* _self){
     struct Instruction** self = _self;
     return (*((*self)->getLast))(_self);
}

void* getNext(void* _self){
     struct Instruction** self = _self;
     return (*((*self)->getNext))(_self);
}


void setLast(void* _self, void* last){
     struct Instruction** self = _self;
     (*((*self)->setLast))(_self, last);
}
void setNext(void* _self, void* next){
     struct Instruction** self = _self;
     (*((*self)->setNext))(_self, next);
}



void destroy_Instruction(void* _self){
     struct Instruction** self = _self;
     (*((*self)->destructor))(_self);
}








//NULL
//*****************************************************************************************************************************************************
//*****************************************************************************************************************************************************
//*****************************************************************************************************************************************************



struct NullInst{
     struct Instruction* fct;
     void* next;
     void* last;
};

void print_Null(void* _self,int start, int end){
     return;
}

int getSize_Null(void* _self){
     return 0;
}


void* getLast_Null(void* _self){
     return _self;
}

void* getLast_NullLast(void* _self){
     struct NullInst* self = _self;

     return self->last;
}


void* getNext_Null(void* _self){
     return _self;
}
void* getNext_NullFirst(void* _self){
     struct NullInst* self = _self;
     return self->next;
}



void setLast_Null(void* _self, void*last){
     return;
}
void setLast_NullLast(void* _self, void*last){
     struct NullInst* self = _self;
     self->last = last;
     return;
}



void setNext_Null(void* _self, void*next){
     return;
}
void setNext_NullFirst(void* _self, void*next){
     struct NullInst* self = _self;
     self->next = next;
     return;
}



void destroy_Null(void* _self){
     return;
}


static struct Instruction nullInstruction  = {&print_Null, &getSize_Null, &getLast_Null, &getNext_Null, &setLast_Null, &setNext_Null,&destroy_Null};
static struct NullInst nullStruct = {&nullInstruction,NULL, NULL};
static struct Instruction nullInstructionFirst  = {&print_Null, &getSize_Null, &getLast_Null, &getNext_NullFirst, &setLast_Null, &setNext_NullFirst, &destroy_Null};
static struct Instruction nullInstructionLast  = {&print_Null, &getSize_Null, &getLast_NullLast, &getNext_Null, &setLast_NullLast, &setNext_Null,&destroy_Null};
static struct Instruction nullInstructionMiddle  = {&print_Null, &getSize_Null, &getLast_NullLast, &getNext_NullFirst, &setLast_NullLast, &setNext_NullFirst,&destroy_Null};



void* new_Null() {
     struct NullInst* null = malloc(sizeof(struct NullInst));
     null->fct = &nullInstruction;
     null->last = &nullStruct;
     null->next = &nullStruct;
     return null;
}


void* new_NullFirst() {
     struct NullInst* null = malloc(sizeof(struct NullInst));
     null->fct = &nullInstructionFirst;
     null->last = &nullStruct;
     null->next = &nullStruct;
     return null;
}

void* new_NullLast() {
     struct NullInst* null = malloc(sizeof(struct NullInst));
     null->fct = &nullInstructionLast;
     null->last = &nullStruct;
     null->next = &nullStruct;
     return null;
}

void* new_NullMiddle() {
     struct NullInst* null = malloc(sizeof(struct NullInst));
     null->fct = &nullInstructionMiddle;
     null->last = &nullStruct;
     null->next = &nullStruct;
     return null;
}







// CHANGE/ADD InstructionNode
//*****************************************************************************************************************************************************
//*****************************************************************************************************************************************************
//*****************************************************************************************************************************************************

struct Change{
     struct Instruction* fct;
     void* next;
     void* last;

     lines lines;
     int linesNumber;

     int size;

     int start;

};



void print_Change(void* _self,int start, int end){
     struct Change* self = _self;
     int linesNumber =  self->start+self->linesNumber-1;



     if( end < self->start){
          print_Instruction(getLast(_self), start, end);

     } else {

          if(start < self->start){
               print_Instruction(getLast(_self), start, self->start-1);

               if(end < linesNumber+1){
                    printLines(self->lines, ListFirstIndex, end-self->start+1);
               } else {
                    printLines(self->lines, ListFirstIndex, linesNumber);
                    print_Instruction(getLast(_self), linesNumber+1 , end);
               }


          } else {

               if(start < linesNumber+1){//check lines
                    if(end < linesNumber+1 ){
                         printLines(self->lines, start-self->start+1, end-self->start+1 );
                    } else {
                         printLines(self->lines, start-self->start+1, linesNumber);
                         print_Instruction(getLast(_self), linesNumber+1, end);
                    }

               } else {
                    print_Instruction(getLast(_self), start, end);
               }

          }
     }








}

int getSize_Change(void* _self){
     struct Change* self = _self;
     return self->size;
}


void* getLast_Change(void* _self){
     struct Change* self = _self;
     return self->last;
}
void* getNext_Change(void* _self){
     struct Change* self = _self;
     return self->next;
}

void setLast_Change(void* _self, void* last){
     struct Change* self = _self;
     self->last = last;

     int newSize =  self->start-1+self->linesNumber;
     if(newSize>getSize_Instruction(last)){
          self->size = newSize;
     } else {
          self->size = getSize_Instruction(last);
     }
}
void setNext_Change(void* _self, void* next){
     struct Change* self = _self;
     self->next = next;
}

void destroy_Change(void* _self){
     struct Change* self = _self;
     destructor_Lines(self->lines);
     destroy_Instruction(getNext(_self));
     free(_self);
}



static struct Instruction ChangeInstruction = {&print_Change, &getSize_Change, &getLast_Change, &getNext_Change, &setLast_Change, &setNext_Change,&destroy_Change};

void* new_Change(lines lines,int start) {
     struct Change* inst= malloc(sizeof(struct Change));

     inst->fct = &ChangeInstruction;


     inst->lines = lines;
     inst->linesNumber = linesSize(lines);
     inst->size = inst->linesNumber;

     inst->start = start;

     inst->next = &nullStruct;
     inst->last = &nullStruct;

     return inst;
}






//DELETE INSTRUCTION;
//*****************************************************************************************************************************************************
//*****************************************************************************************************************************************************
//*****************************************************************************************************************************************************

struct Delete{
     struct Instruction* fct;
     void* next;
     void* last;

     int size;

     int start;
     int end;
};

void print_Delete(void* _self,int start, int end){

     struct Delete* self = _self;
     int deletedLinesNumber = self->end-self->start+1;
     if(start < self->start){
          if( end < self->start){
               print_Instruction(getLast(_self), start, end);
               return;
          } else {
               print_Instruction(getLast(_self), start, self->start-1);
               print_Instruction(getLast(_self), self->end+1, end + deletedLinesNumber );
          }

     } else {
          print_Instruction(getLast(_self), start+deletedLinesNumber, end + deletedLinesNumber );
     }
}

int getSize_Delete(void* _self){
     struct Delete* self = _self;
     return self->size;
}




void* getLast_Delete(void* _self){
     struct Delete* self = _self;
     return self->last;
}
void* getNext_Delete(void* _self){
     struct Delete* self = _self;
     return self->next;
}


void setLast_Delete(void* _self, void* last){
     struct Delete* self = _self;
     self->last = last;
     int newSize = getSize_Instruction(last)-(self->end-self->start+1);
     if( newSize > 0 ){
          self->size = newSize;
     } else {
          self->size = 0;
     }
}


void setNext_Delete(void* _self, void*next){
     struct Delete* self = _self;
     self->next = next;
}

void destroy_Delete(void* _self){
     struct Delete* self = _self;
     destroy_Instruction(getNext(_self));
     free(_self);
}


static struct Instruction DeleteInstruction = {&print_Delete, &getSize_Delete, &getLast_Delete, &getNext_Delete, &setLast_Delete, &setNext_Delete,&destroy_Delete};

void* new_Delete(int start, int end) {
     struct Delete* inst= malloc(sizeof(struct Delete));
     inst->fct = &DeleteInstruction;

     inst->start = start;
     inst->end = end;
     inst->last = &nullStruct;
     inst->next = &nullStruct;
     return inst;
}







//*****************************************************************************************************************************************************
//*****************************************************************************************************************************************************
//*****************************************************************************************************************************************************

struct Text{
     void* current;
     void* first;
     void* last;
};


void addInstruction(struct Text* _self, void* newInstruction){
     destroy_Instruction(getNext(_self->current));
     setNext(_self->current, newInstruction);
     setLast(newInstruction, _self->current);
     setLast(_self->last, newInstruction);
     setNext(newInstruction, _self->last);
     _self->current = newInstruction;
}


int getCurrentSize(struct Text* _self){
     return getSize_Instruction(_self->current);
}

void print_Text(struct Text* _self, int start, int end) {
     while (start<ListFirstIndex) {
          printf(".\n");
          start++;
     }
     print_Instruction(_self->current, start, end);
     while ( end> getSize_Instruction(_self->current) ) {
          printf(".\n");
          end--;
     }
}


void change_Text(struct Text* _self, char** lines, int start){
     struct Change* newInstruction = new_Change(lines,start);
     addInstruction(_self,newInstruction);
}

void delete_Text(struct Text* _self,int start, int end){
     int currentSize = getSize_Instruction(_self->current);
     if( start > currentSize || end< ListFirstIndex ){
          addInstruction(_self,new_NullMiddle());
          return;
     }

     if(start< ListFirstIndex){
          if(end <= currentSize){
               struct Delete* newInstruction = new_Delete(ListFirstIndex, end);
               addInstruction(_self,newInstruction);
          } else {
               struct Delete* newInstruction = new_Delete(ListFirstIndex, currentSize);
               addInstruction(_self,newInstruction);
          }
     } else {
          if(end <= currentSize){
               struct Delete* newInstruction = new_Delete(start, end);
               addInstruction(_self,newInstruction);
          } else {
               struct Delete* newInstruction = new_Delete(start, currentSize);
               addInstruction(_self,newInstruction);
          }
     }


}

void undo_Text(struct Text* _self, int times){
     for (size_t i = 0; i < times && _self->current!= _self->first; i++) {
          _self->current = getLast(_self->current);
     }
}

void redo_Text(struct Text* _self,int times){
     for (size_t i = 0; i < times && getNext(_self->current) != _self->last; i++) {
          _self->current = getNext(_self->current);
     }
}

void destructor_Text(struct Text* _self){
     destroy_Instruction(getNext(_self->first));
     free(_self);
}



struct Text* new_Text(){
     struct Text* text = malloc(sizeof(struct Text));
     text->first = new_NullFirst();
     text->last = new_NullLast();
     setNext(text->first,text->last);
     setLast(text->last,text->first);

     text->current = text->first;
     return text;
}










//COMMAND
// ***********************************************************************************
// ***********************************************************************************

struct Command{
     void (*command_fct)(void* _self, struct Text* text);
     void (*destructor_fct)(void* _self);
};

void applyCommand(void* _self, struct Text* text) {
     struct Command** self = _self;
     (*((*self)->command_fct))(_self, text);
}

void destructor_Command(void* _self) {
     struct Command** self = _self;
     (*((*self)->destructor_fct))(_self);
}

//INSERT_COMMAND
// ***********************************************************************************
// ***********************************************************************************

struct insertCommand{
     struct Command* fct;
     lines lines;
     int start;
};

void applyCommand_Insert(void* _self, struct Text* text){
     struct insertCommand* self = _self;
     change_Text(text, self->lines,self->start);
}

void destructor_InsertCommand(void* _self){
     free(_self);
}

static struct Command insertCommand_fct = {&applyCommand_Insert,&destructor_InsertCommand};


void* new_insertCommand(char** lines,int start){
     struct insertCommand* command = malloc(sizeof(struct insertCommand));
     command->fct = &insertCommand_fct;
     command->lines = lines;
     command->start = start;
     return command;
}






//DELETE_COMMAND
// ***********************************************************************************
// ***********************************************************************************

struct deleteCommand{
     struct Command* fct;
     int start;
     int end;
};

void applyCommand_Delete(void* _self,struct Text* text){
     struct deleteCommand* self = _self;
     delete_Text(text, self->start, self->end);
}

void destructor_deleteCommand(void* _self){
     free(_self);
}

static struct Command deleteCommand_fct = {&applyCommand_Delete,&destructor_deleteCommand};


void* new_deleteCommand(int start, int end){
     struct deleteCommand* command = malloc(sizeof(struct deleteCommand));
     command->fct = &deleteCommand_fct;
     command->start = start;
     command->end = end;
     return command;
}






//PRINT_COMMAND
// ***********************************************************************************
// ***********************************************************************************

struct printCommand{
     struct Command* fct;
     int start;
     int end;
};

void applyCommand_Print(void* _self, struct Text* text){
     struct deleteCommand* self = _self;
     print_Text(text,self->start,self->end);
}

void destructor_printCommand(void* _self){
     free(_self);
}

static struct Command printCommand_fct = {&applyCommand_Print,&destructor_printCommand};


void* new_printCommand(int start, int end){
     struct printCommand* command = malloc(sizeof(struct printCommand));
     command->fct = &printCommand_fct;
     command->start = start;
     command->end = end;
     return command;
}







//UNDO_COMMAND
// ***********************************************************************************
// ***********************************************************************************

struct undoCommand{
     struct Command* fct;
     int times;
};

void applyCommand_Undo(void* _self, struct Text* text){
     struct undoCommand* self = _self;
     undo_Text(text,self->times);
}

void destructor_undoCommand(void* _self){
     free(_self);
}

static struct Command undoCommand_fct = {&applyCommand_Undo,&destructor_undoCommand};


void* new_undoCommand(int times){
     struct undoCommand* command = malloc(sizeof(struct undoCommand));
     command->fct = &undoCommand_fct;
     command->times = times;
     return command;
}







//REDO_COMMAND
// ***********************************************************************************
// ***********************************************************************************

struct redoCommand{
     struct Command* fct;
     int times;
};

void applyCommand_Redo(void* _self,struct Text* text){
     struct redoCommand* self = _self;
     redo_Text(text,self->times);
}

void destructor_redoCommand(void* _self){
     free(_self);
}

static struct Command redoCommand_fct = {&applyCommand_Redo,&destructor_redoCommand};


void* new_redoCommand(int times){
     struct redoCommand* command = malloc(sizeof(struct redoCommand));
     command->fct = &redoCommand_fct;
     command->times = times;
     return command;
}

//QUIT_COMMAND
// ***********************************************************************************
// ***********************************************************************************

struct quitCommand{
     struct Command* fct;
};

void applyCommand_Quit(void* _self, struct Text* text){
     destructor_Text(text);
     exit(0);
}

void destructor_quitCommand(void* _self){
     free(_self);
}

static struct Command quitCommand_fct = {&applyCommand_Quit,&destructor_quitCommand};
static struct quitCommand quitCommandStruct = {&quitCommand_fct};


//INPUT
// ***********************************************************************************
// ***********************************************************************************




#define LINE_BUFFER_SIZE 1000

char* readLine(){
     int ch;
     int size = LINE_BUFFER_SIZE;
     size_t index = 0;
     char* string =  malloc( size * sizeof(char));;

     if(string == NULL){
          return NULL;
     }

     for(;;){
          ch = getchar();
          //ch = fgetc(fp);


          if(ch == '\n' || ch == EOF){
               break;
          }
          string[index] = ch;
          index++;
          if(index==size){
               size *= 2;
               char* newString = realloc(string, size*sizeof(char));
               if(newString == NULL){
                    return NULL;
               }
               string = newString;
          }
     }

     string[index]='\0';

     return realloc(string, strlen(string)+1);

}

lines readText(int times){

     lines lines = malloc((times* sizeof(char*))+1);
     for (size_t i = 0; i < times; i++) {
          lines[i] = readLine();
     }
     char* line = readLine();

     if(line[0] == '.'){
          lines[times] = endLines;
          free(line);
          return lines;
     } else {
          printf("invalid input\n");
          exit(-1);
     }

}

int extractNumber(char* string, char** endptr, int base){
     int digit = strtol( string , endptr, 10 );
     if( (*endptr)[0] == ',' ){
          *endptr = &((*endptr)[1]);
     }
     return digit;
}


void* insertCommandBuilder(char* string){
     int start = extractNumber(string, &string, 10);
     int end = extractNumber(string, &string, 10);
     char** lines = readText(end-start+1);
     return new_insertCommand(lines,start);
}

void* deleteCommandBuilder(char* string){
     int start = extractNumber(string, &string, 10);
     int end = extractNumber(string, &string, 10);
     return new_deleteCommand(start, end);
}

void* printCommandBuilder(char* string){
     int start = extractNumber(string, &string, 10);
     int end = extractNumber(string, &string, 10);
     return new_printCommand(start, end);
}

void* undoCommandBuilder(char* string){
     int times = extractNumber(string, &string, 10);
     return new_undoCommand(times);
}

void* redoCommandBuilder(char* string){
     int times = extractNumber(string, &string, 10);
     return new_redoCommand(times);
}


struct Command* lineParser(char* line){
     void* command = NULL;

     switch (line[strlen(line)-1]) {
          case 'c':
               command = insertCommandBuilder(line);
               free(line);
               return command;
          case 'd':
               command = deleteCommandBuilder(line);
               free(line);
               return command;
          case 'p':
               command = printCommandBuilder(line);
               free(line);
               return command;
          case 'u':
               command = undoCommandBuilder(line);
               free(line);
               return command;
          case 'r':
               command = redoCommandBuilder(line);
               free(line);
               return command;
          case 'q':
               command = &quitCommandStruct;
               free(line);
               return command;
          default:
               return NULL;
               /*
               printf("unknown istruction\n");
               free(line);
               exit(-1);*/
     }

}









//TEST
// ***********************************************************************************
// ***********************************************************************************

char** testList(char* a,int size){
     char** test = malloc(size*sizeof(char*));
     int len = strlen(a);
     for (size_t i = 0; i < len; i++) {
          char* string = malloc(2*sizeof(char));
          string[0] = a[i];
          string[1] = '\0';
          test[i] = string;
     }
     return test;
}



void testChange(){
     struct Change* change = new_Change(testList("abcde12345",10), 1);
     printf("%d\n",getSize_Instruction(change) );
     print_Instruction(change, 1,10);
     struct Change* change2 = new_Change(testList("ABCD",4), 1);
}



















int main (){

     //fp  = fopen("test.txt", "r");

     struct Text* text = new_Text();

     for(;;){
          char* line = readLine();
          int size = strlen(line);
          for (size_t i = 0; size-1-i>0 && !isalpha(line[size-1-i]); i++) {
               line[strlen(line)-1] = '\0';
          }
          if(strlen(line)>0){
               void* command = lineParser(line);
               if( command != NULL){
                    applyCommand(command, text);
               }
          }
     }

     //fclose(fp);
     return 0;
}
