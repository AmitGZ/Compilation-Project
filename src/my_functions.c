#include "my_functions.h"

extern size_t errorCount;

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
    errorCount++;
	fprintf(stderr, "Parse error: %s\n", s);
}

void MipsData(FILE* file)
{
    assert(file != NULL);
    fprintf(file, "\t.data\n" \
                  "buffer: .space %d   # allocate %d bytes for the input buffer\n", BUFFER_SIZE, BUFFER_SIZE);
}  

void MipsDecl(FILE* file, Type t, const char* id, const char* val)
{
    assert((file != NULL) && (t < TYPE_COUNT) && (id != NULL) && (val != NULL));

    char* tStr;
    if ((t == STR) || (t == INTEGER))
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
    assert((file != NULL) && (node != NULL) && (node->_type < TYPE_COUNT));

    if (node->_type == STR)
    {
        fprintf(file, "\n\tli $v0, 8    # set $v0 to indicate we want to read input\n");
        fprintf(file, "\tla $a0, buffer   # Buffer adress\n");
        fprintf(file, "\tli $a1, %d, # Buffer size of %d bytes\n", BUFFER_SIZE, BUFFER_SIZE);
        fprintf(file, "\tsyscall      # execute the syscall instruction to read the input\n");
    }
    else if (node->_type == INTEGER)
    {
        fprintf(file, "\n\tli $v0, 5   # set $v0 to indicate we want to read input\n");
        fprintf(file, "\tsyscall       # execute the syscall instruction to read the input\n");
        fprintf(file, "\tsw $v0, %s   # store the input\n", node->_name);
    }
    else // FLOATING
    {
        fprintf(file, "\n\tli $v0, 6   # set $v0 to indicate we want to read input\n");
        fprintf(file, "\tsyscall       # execute the syscall instruction to read the input\n");
        fprintf(file, "\ts.s $f0, %s  # store the input\n", node->_name);
    }
}

void MipsOut(FILE* file, const Val* val)
{
    assert((file != NULL) && (val != NULL) && (val->_type < TYPE_COUNT));

    //                      INTEGER=0, FLOATING=1, STR=2
    static const int OutTable[] = { 1, 2, 4 };
    static const char* CmdTable[] = { "lw $a0", "l.s $f12", "lw $a0" };

    fprintf(file, "\n\tli $v0, %d    # system call for printing \n" \
                  "\t%s, %s      # set argument print \n"       \
                  "\tsyscall         # execute system call\n", OutTable[val->_type], CmdTable[val->_type], val->_sval);

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

void MipsAssign(FILE* file, const Node* node, const char* name)
{
    assert((file != NULL) && (node != NULL) && (node->_type < TYPE_COUNT) && (name != NULL));

    fprintf(file, "\n\tsw %s, %s", name, node->_name);

    if (node->_type == STR)
    {

    }
    else if (node->_type == INTEGER)
    {

    }
    else // FLOATING
    {

    }
}

const char* MipsLogOp(FILE* file, LogOp logOp, const char* reg1, const char* reg2)
{
    assert((file != NULL) && (logOp < LOG_OP_COUNT) && (reg1 != NULL) && (reg2 != NULL));
    
    static const char* LogOpTable[] = { "and", "or", "nor" };
    const char* op = LogOpTable[logOp];
    const char* result = TmpRegs[0];

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

void MipsLoad(FILE* file, const Val* val, char reg)
{
    assert((file != NULL) && (val != NULL) && (val->_type < TYPE_COUNT) && (val->_sval != NULL));

    if (val->_type == STR)
    {
        if (val->_isImmediate)
        {
            const char* str = "input_prompt";

            fprintf(file, "\t.data\n"                        \
                          "%s: .asciiz %s\n" \
	                      "\t.text\n", str, val->_sval);

            fprintf(file, "\n\t# store pointer to string in $s%c\n" \
                          "\tla $s%c, %s\n", reg, reg, str);
        }
        else
        {
            fprintf(file, "\n\tla $t%c, %s\n", reg, val->_sval);
        }
    }
    else if (val->_type == INTEGER)
    {
        if (val->_isImmediate)
        {
            fprintf(file, "\n\tli $t%c, %s\n", reg, val->_sval);
        }
        else
        {
            fprintf(file, "\n\tlw $t%c, %s\n", reg, val->_sval);
        }
    }
    else // FLOATING
    {
        if (val->_isImmediate)
        {
            float f = strtof(val->_sval, NULL); // Converting string to float
            int* x = (int*)&(f);                // reinterpret_cast to int
            fprintf(file, "\n\tli $t%c, 0x%x\n", reg, (*x));
            fprintf(file, "\tmtc1 %s, $f%c\n", val->_sval, reg);
        }
        else
        {
            fprintf(file, "\n\tl.s $f%c, %s\n", reg, val->_sval);
        }
    }
}
