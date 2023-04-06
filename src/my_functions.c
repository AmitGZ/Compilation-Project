#include "my_functions.h"

extern size_t errorCount;
extern FILE* mips;
static size_t RegTrackerF = 0U;
static size_t RegTrackerT = 0U;

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

void MipsData()
{
    fprintf(mips, "\t.data\n" \
                  "buffer: .space %d   # allocate %d bytes for the input buffer\n", BUFFER_SIZE, BUFFER_SIZE);
}  

void MipsDecl(Type t, const char* id, const char* val)
{
    assert((t < TYPE_COUNT) && (id != NULL) && (val != NULL));

    const char* tStr = (t == FLOATING) ? "float" : "word";
    fprintf(mips, "%s:\t.%s %s\n", id, tStr, val);
}

void MipsIn(const Node* node)
{
    assert((node != NULL) && (node->_type < TYPE_COUNT));

    fprintf(mips, "\n\t# read input\n");
    if (node->_type == STR)
    {
        fprintf(mips, "\tli $v0, 8      \n"\
                      "\tla $a0, buffer \n"\
                      "\tli $a1, %d,    \n"\
                      "\tsyscall        \n", BUFFER_SIZE);
    }
    else if (node->_type == INTEGER)
    {
        fprintf(mips, "\tli $v0, 5  \n"\
                      "\tsyscall    \n"\
                      "\tsw $v0, %s \n", node->_name);
    }
    else // FLOATING
    {
        fprintf(mips, "\tli $v0, 6    \n"\
                      "\tsyscall      \n"\
                      "\ts.s $f0, %s  \n", node->_name);
    }
}

void MipsOut(const Reg* val)
{
    assert((val != NULL) && (val->_type < TYPE_COUNT));

    static const int OutTable[] = { 1, 2, 4 }; // INTEGER=0, FLOATING=1, STR=2
    static const char* CmdTable[] = { "move $a0", "mov.s $f12", "move $a0" };

    fprintf(mips, "\n\t# printing \n"\
                  "\tli $v0, %d   \n"\
                  "\t%s, %s       \n"\
                  "\tsyscall      \n", OutTable[val->_type], CmdTable[val->_type], val->_sval);

}

void MipsMathOp(MathOp mathOp, Reg* res, Reg* reg0, Reg* reg1)
{
    assert((mathOp < MATH_OP_COUNT) && (res != NULL) && (reg0 != NULL) && (reg1 != NULL));

    if ((reg0->_type == STR) || (reg1->_type == STR))
    {
        yyerror("can't perform arithmetic operations on string");
        return;
    }

    res->_type = ((reg0->_type == FLOATING) || (reg1->_type == FLOATING)) ? FLOATING : INTEGER;
    static const char* MathOpTable[] = { "add", "sub", "mul", "div" };
    const char* op = MathOpTable[mathOp];

    if (res->_type == FLOATING)
    {        
        if (reg0->_type != FLOATING)
        {
            MipsCast(reg0, FLOATING);
            res->_sval = reg1->_sval;
        }
        else if (reg1->_type != FLOATING)
        {
            MipsCast(reg1, FLOATING);
            res->_sval = reg0->_sval;
        }
        else
        {
            res->_sval = reg0->_sval;
        }
        
        fprintf(mips, "\n\t# mathop two floats\n"\
                        "\t%s.s %s, %s, %s\n", op, res->_sval, reg0->_sval, reg1->_sval);
        FreeRegF();
    }
    else // INTEGER
    {
        res->_sval = GetRegT();
        fprintf(mips, "\n\t# mathop two ints\n"\
                      "\t%s %s, %s, %s\n", op, res->_sval, reg0->_sval, reg1->_sval);
        FreeRegT();
    }
}

void MipsRelOp(RelOp relOp, Reg* res, Reg* reg0, Reg* reg1)
{
    assert((relOp < REL_OP_COUNT) && (res != NULL) && (reg0 != NULL) && (reg1 != NULL));

    if ((reg0->_type == STR) || (reg1->_type == STR))
    {
        yyerror("can't perform arithmetic operations on string");
        return;
    }

    res->_type = ((reg0->_type == FLOATING) || (reg1->_type == FLOATING)) ? FLOATING : INTEGER;
    const char* op;
    
    if (res->_type == FLOATING)
    {        
        if (reg0->_type != reg1->_type)
        {
            MipsCast(reg0, FLOATING);
            MipsCast(reg1, FLOATING);
        }
        res->_sval = GetRegT();

        /*                                              sne          sgt          sge  */
        static const char* FloatRelOpTable[] = { "eq", "!eq", "lt", "!le", "le", "!lt" };
        op = FloatRelOpTable[relOp];
        if (op[0] == '!')
        {
            // Negate operation 
            fprintf(mips, "\n\t# compare two floats and negate\n"\
                          "\tc.%s.s %s, %s\n"\
                          "\tmovt %s, $zero, 0\n"\
                          "\tmovf %s, $zero, 1\n", &op[0], reg0->_sval, reg1->_sval, res->_sval, res->_sval);
        }
        else
        {
            // Normal operation is available
            fprintf(mips, "\n\t# compare two floats and negate\n"\
                          "\tc.%s.s %s, %s\n"\
                          "\tmovt %s, $zero, 1\n"\
                          "\tmovf %s, $zero, 0\n", &op[0], reg0->_sval, reg1->_sval, res->_sval, res->_sval);
        }
        FreeRegF();
        FreeRegF();
    }
    else // INTEGER
    {
        res->_sval = reg0->_sval;
        static const char* IntRelOpTable[] = { "seq", "sne", "slt", "sgt", "sle", "sge" };
        const char* op = IntRelOpTable[relOp];
        fprintf(mips, "\n\t# compare two ints\n"\
                      "\t%s %s, %s, %s\n", op, res->_sval, reg0->_sval, reg1->_sval);
        FreeRegT();
    }
}

void MipsAssign(const Node* node, Reg* reg)
{
    assert((node != NULL) && (node->_type < TYPE_COUNT) && (reg != NULL) && (reg->_type < TYPE_COUNT));

    if (!IsAssignValid(node->_type, reg->_type))
    {
        yyerror("Assignment invalid");
        return;
    }

    if (node->_type != reg->_type)
    {
        MipsCast(reg, node->_type);
    }

    switch(node->_type)
    {
        case STR:
            fprintf(mips, "\n\t# assigning string pointer\n"
                          "\tsw %s, %s\n", reg->_sval, node->_name);
            break;
        
        case INTEGER:
            fprintf(mips, "\n\t# assigning integer value\n"
                          "\tsw %s, %s\n", reg->_sval, node->_name);
            break;

        default: // FLOATING
            fprintf(mips, "\n\t# assigning float value\n"
                          "\ts.s %s, %s\n", reg->_sval, node->_name);
            break;
    }
}

void MipsLogOp(LogOp logOp, Reg* res, Reg* reg0, Reg* reg1)
{
    assert((logOp < LOG_OP_COUNT) && (res != NULL) && (reg0 != NULL) && (reg1 != NULL));

    if ((reg0->_type == STR) || (reg1->_type == STR))
    {
        yyerror("can't perform arithmetic operations on string");
        return;
    }

    if (reg0->_type != reg1->_type)
    {
        MipsCast(reg0, FLOATING);
        MipsCast(reg1, FLOATING);
    }

    res->_type = ((reg0->_type == FLOATING) || (reg1->_type == FLOATING)) ? FLOATING : INTEGER;
    static const char* LogOpTable[] = { "and", "or", "nor" };
    const char* op = LogOpTable[logOp];

    fprintf(mips, "\t%s %s, %s, %s", op, res->_sval, reg0->_sval, reg1->_sval);
}

void MipsExit()
{   
    fprintf(mips, "\n\t# exit the program\n"\
                  "\tli $v0, 10\n"\
                  "\tsyscall\n");
}

void MipsMain()
{
    fprintf(mips, "\n\t.text\
                   \n\t.globl main\
                   \n\nmain:\n");
}

void MipsLoadV(Reg* reg)
{
    assert((reg != NULL) && (reg->_type < TYPE_COUNT) && (reg->_sval != NULL));
    const char* var = reg->_sval;

    switch(reg->_type)
    {
        case STR:
            reg->_sval = GetRegT();
            fprintf(mips, "\n\tla %s, %s\n", reg->_sval, var);
            break;
        
        case INTEGER:
            reg->_sval = GetRegT();
            fprintf(mips, "\n\tlw %s, %s\n", reg->_sval, var);
            break;
            
        default: // FLOATING
            reg->_sval = GetRegF();
            fprintf(mips, "\n\tl.s %s, %s\n", reg->_sval, var);
            break;
    }
}

void MipsLoadI(Reg* reg)
{
    assert((reg != NULL) && (reg->_type < TYPE_COUNT) && (reg->_sval != NULL));
    static size_t stringCount = 0U;
    const char* str = "str";
    const char* val = reg->_sval;
    
    switch(reg->_type)
    {
        case STR:
            reg->_sval = GetRegT();
            fprintf(mips, "\n\t.data\n"\
                          "%s%zu: .asciiz %s\n"\
	                      "\t.text\n", str, stringCount, val);

            fprintf(mips, "\n\t# store pointer to string\n"\
                          "\tla %s, %s%zu\n", reg->_sval, str, stringCount);

            ++stringCount;
            break;
        
        case INTEGER:
            reg->_sval = GetRegT();
            fprintf(mips, "\n\tli %s, %s\n", reg->_sval, val);
            break;

        default: // FLOATING
            reg->_sval = GetRegF();
            float f = strtof(val, NULL); // Converting string to float
            int* x = (int*)&(f);         // reinterpret_cast to int
            fprintf(mips, "\n\tli $t0, 0x%x\n", (*x));
            fprintf(mips, "\tmtc1 $t0, %s\n", reg->_sval);
            break;
    }
}

void MipsCast(Reg* reg, Type t)
{
    assert((reg->_type < TYPE_COUNT) && (t < TYPE_COUNT));

    if (reg->_type == t)
        return;

    const char* val;
    if (t == FLOATING)
    {
        val = reg->_sval;
        reg->_sval = GetRegF();
        fprintf(mips, "\n\t# Move integer value to floating-point register\n"\
                      "\tmtc1 %s, %s\n"\
                      "\tcvt.s.w %s, %s\n", val, reg->_sval, reg->_sval, reg->_sval);
        FreeRegT();
    }
}

const char* GetRegT()
{
    assert(RegTrackerT < 8U);
    return TmpRegs[RegTrackerT++];
}

const char* GetRegF()
{
    assert(RegTrackerF < 8U);
    return FloatRegs[RegTrackerF++];
}

void FreeRegT()
{
    assert(RegTrackerT != 0U);
    --RegTrackerT;
}

void FreeRegF()
{
    assert(RegTrackerF != 0U);
    --RegTrackerF;
}

void FreeAllRegs()
{
    RegTrackerF = 0U;
    RegTrackerT = 0U;
}
