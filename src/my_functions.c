#include "my_functions.h"

int Hash(const char* key)
{
    int sum = 0;
    for (int i = 0; i < strlen(key); i++) 
    {
        sum += key[i];
    }
    return sum % TABLE_SIZE;
}

Node* GetFromTable(const Bucket* table, const char* name)
{
    int index = Hash(name);
    Node* current = table[index]._head;
    while (current != NULL) 
    {
        if (strcmp(current->_name, name) == 0U) 
        {
            return current;
        }
        current = current->_next;
    }
    return NULL; // Key not found
}

void InsertToTable(Bucket* table, char* id, Type t, bool isConst)
{
    Node* tmp = GetFromTable(table, id);
    if (GetFromTable(table, id) != NULL)
    {
        yyerror(strcat("Redeclaration of variable: ", id)); // Variable name already exists
        return;
    }
    int index = Hash(id);
    Node *new_node = (Node*) malloc(sizeof(Node));
    new_node->_name = id;
    new_node->_isConst = isConst;
    new_node->_type = t;
    new_node->_next = table[index]._head;
    table[index]._head = new_node;
}

void FreeBucket(Bucket* bucket) 
{
    Node *current = bucket->_head;
    while (current != NULL)
    {
        Node* temp = current;
        current = current->_next;
        free(temp->_name);
        free(temp);
    }
    bucket->_head = NULL;
}

void FreeTable(Bucket* table) 
{
    for (int i = 0; i < TABLE_SIZE; i++) 
    {
        FreeBucket(&table[i]);
    }
}

bool IsAssignValid(Type type1, Type type2) 
{
    return (type1 == type2) || (type1 == FLOATING); 
}

void yyerror(const char* s)
{
	fprintf(stderr, "Parse error: %s\n", s);
	//exit(1);
}

void MipsDecl(FILE* file, Type t, const char* id, const char* val)
{
    char* tStr;
    if (t == STR)
    {
        tStr = "asciiz";
    }
    else if (t == INTEGER)
    {
        tStr = "word";
    }
    else
    {
        tStr = "float";
    }
    fprintf(file, "\t%s:\t.%s %s\n", id, tStr, val);
}

// li $v0 8 
// la $a0, id
// li $a1, buffersize
// syscall
void MipsIn(FILE* file, const Node* node)
{
    printf("%s %d\n", node->_name, node->_type);
    if (node->_type == STR)
    {
        fprintf(file, "\tli $v0, 8    # set $v0 to indicate we want to read input\n");
        fprintf(file, "\tla $a0, %s   # Buffer adress\n", node->_name);
        fprintf(file, "\tli $a1, 256, # Buffer size of 256 bytes\n");
        fprintf(file, "\tsyscall      # execute the syscall instruction to read the input\n");

    }
    else
    {
        int readCmd;
        if (node->_type == INTEGER)
        {
            readCmd = 5;
        }
        else
        {
            readCmd = 6;
        }
        fprintf(file, "\tli $v0, %d    # set $v0 to indicate we want to read input\n", readCmd);
        fprintf(file, "\tsyscall      # execute the syscall instruction to read the input\n");
        fprintf(file, "\tsw $v0, %s   # store the input\n\n", node->_name);
    }
}

void MipsAdd(FILE* file, char* result, const char* reg1, const char* reg2)
{

}

void MipsMul(FILE* file, char* result, const char* reg1, const char* reg2)
{

}

void MipsOr(FILE* file, char* result, const char* reg1, const char* reg2)
{

}

void MipsAnd(FILE* file, char* result, const char* reg1, const char* reg2)
{

}

void MipsRelop(FILE* file, char* result, const char* reg1, const char* reg2, RelOp relOp)
{

}

void MipsNot(FILE* file, char* result, const char* reg1)
{

}
