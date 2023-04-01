#include "my_functions.h"

int Hash(const char* key)
{
    assert(key != NULL);

    int sum = 0;
    for (int i = 0; i < strlen(key); i++) 
    {
        sum += key[i];
    }
    return sum % TABLE_SIZE;
}

Node* GetFromTable(const Bucket* table, const char* name)
{
    assert((table != NULL) && (name != NULL));

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

void InsertToTable(Bucket* table, const char* id, Type t, bool isConst)
{
    assert((table != NULL) && (id != NULL) && (t < TYPE_COUNT));

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
    assert(bucket != NULL);

    Node *current = bucket->_head;
    while (current != NULL)
    {
        Node* temp = current;
        current = current->_next;
        free(temp);
    }
    bucket->_head = NULL;
}

void FreeTable(Bucket* table) 
{
    assert(table != NULL);

    for (int i = 0; i < TABLE_SIZE; i++) 
    {
        FreeBucket(&table[i]);
    }
}

bool IsAssignValid(Type type1, Type type2) 
{
    assert((type1 < TYPE_COUNT) && (type2 < TYPE_COUNT));

    return (type1 == type2) || (type1 == FLOATING); 
}

void yyerror(const char* s)
{
	fprintf(stderr, "Parse error: %s\n", s);
	//exit(1);
}

void MipsDecl(FILE* file, Type t, const char* id, const char* val)
{
    assert((file != NULL) && (t < TYPE_COUNT) && (id != NULL) && (val != NULL));

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
    fprintf(file, "%s:\t.%s %s\n", id, tStr, val);
}

void MipsIn(FILE* file, const Node* node)
{
    assert((file != NULL) && (node != NULL));

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
        fprintf(file, "\tli $v0, %d   # set $v0 to indicate we want to read input\n", readCmd);
        fprintf(file, "\tsyscall      # execute the syscall instruction to read the input\n");
        fprintf(file, "\tsw $v0, %s   # store the input\n\n", node->_name);
    }
}

const char* MipsAddOp(FILE* file, AddOp addOp, const char* reg1, const char* reg2)
{
    assert((file != NULL) && (addOp < ADD_OP_COUNT) && (reg1 != NULL) && (reg2 != NULL));

    return "";
}

const char* MipsMulOp(FILE* file, MulOp mulOp, const char* reg1, const char* reg2)
{
    assert((file != NULL) && (mulOp < MUL_OP_COUNT) && (reg1 != NULL) && (reg2 != NULL));

    return "";
}

const char* MipsRelOp(FILE* file, RelOp relOp, const char* reg1, const char* reg2)
{
    assert((file != NULL) && (relOp < REL_OP_COUNT) && (reg1 != NULL) && (reg2 != NULL));

    return "";
}

void MipsAssign(FILE* file, const Node* node, const void* p)
{
    assert((file != NULL) && (node != NULL) && (p != NULL));

}

const char* MipsLogOp(FILE* file, LogOp logOp, const char* reg1, const char* reg2)
{
    assert((file != NULL) && (logOp < LOG_OP_COUNT) && (reg1 != NULL) && (reg2 != NULL));
    
    static const char* LogOpTable[] = { "and", "or", "nor" };
    const char* op = LogOpTable[logOp];

    const char* result = Temporaries[0];

    fprintf(file, "%s %s, %s, %s", op, result, reg1, reg2);

    return result;
}

void MipsExit(FILE* file)
{   
    assert(file != NULL);
    fprintf(file, "\n\t# exit the program\
                   \n\tli $v0, 10\
                   \n\tsyscall");
}