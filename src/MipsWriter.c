#include "MipsWriter.h"

extern size_t errorCount;        /**< Description */
extern FILE* mips;               /**< Description */

static size_t RegTrackerF  = 0U; /**< Description */
static size_t RegTrackerT  = 0U; /**< Description */
static size_t IfIndex      = 0U; /**< Description */
static size_t WhileIndex   = 0U; /**< Description */
static size_t ForEachIndex = 0U; /**< Description */
static size_t SwitchIndex  = 0U; /**< Description */
static size_t CaseIndex    = 0U; /**< Description */
static Node* SwitchNode = NULL;  /**< Description */


bool IsAssignValid(Type type1, Type type2) 
{
    assert((type1 < TYPE_COUNT) && (type2 < TYPE_COUNT));

    return (type1 == type2) || (type1 == FLOATING); 
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

void MipsOut(const Reg* reg)
{
    assert((reg != NULL) && (reg->_type < TYPE_COUNT));

    static const int OutTable[] = { 1, 2, 4 }; // INTEGER=0, FLOATING=1, STR=2
    static const char* CmdTable[] = { "move $a0", "mov.s $f12", "move $a0" };

    fprintf(mips, "\n\t# printing \n"\
                  "\tli $v0, %d   \n"\
                  "\t%s, %s       \n"\
                  "\tsyscall      \n", OutTable[reg->_type], CmdTable[reg->_type], reg->_name);

}

Reg MipsMathOp(MathOp mathOp, Reg* reg0, Reg* reg1)
{
    assert((mathOp < MATH_OP_COUNT) && (reg0 != NULL) && (reg1 != NULL));
    Reg res;

    if ((reg0->_type == STR) || (reg1->_type == STR))
    {
        yyerror("can't perform arithmetic operations on string");
        return res;
    }

    res._type = ((reg0->_type == FLOATING) || (reg1->_type == FLOATING)) ? FLOATING : INTEGER;
    static const char* MathOpTable[] = { "add", "sub", "mul", "div" };
    const char* op = MathOpTable[mathOp];

    if (res._type == FLOATING)
    {        
        if (reg0->_type != FLOATING)
        {
            MipsCast(reg0, FLOATING);
            res._name = reg1->_name;
        }
        else if (reg1->_type != FLOATING)
        {
            MipsCast(reg1, FLOATING);
            res._name = reg0->_name;
        }
        else
        {
            res._name = reg0->_name;
        }
        
        fprintf(mips, "\n\t# mathop two floats\n"\
                        "\t%s.s %s, %s, %s\n", op, res._name, reg0->_name, reg1->_name);
        FreeReg(FLOATING);
    }
    else // INTEGER
    {
        res._name = reg0->_name;
        fprintf(mips, "\n\t# mathop two ints\n"\
                      "\t%s %s, %s, %s\n", op, res._name, reg0->_name, reg1->_name);
        FreeReg(INTEGER);
    }
    return res;
}

Reg MipsRelOp(RelOp relOp, Reg* reg0, Reg* reg1)
{
    assert((relOp < REL_OP_COUNT) && (reg0 != NULL) && (reg1 != NULL));
    Reg res;

    if ((reg0->_type == STR) || (reg1->_type == STR))
    {
        yyerror("can't perform arithmetic operations on string");
        return res;
    }

    res._type = ((reg0->_type == FLOATING) || (reg1->_type == FLOATING)) ? FLOATING : INTEGER;
    const char* op;
    
    if (res._type == FLOATING)
    {        
        if (reg0->_type != reg1->_type)
        {
            MipsCast(reg0, FLOATING);
            MipsCast(reg1, FLOATING);
        }
        res._name = GetReg(INTEGER);

        /*                                              sne          sgt          sge  */
        static const char* FloatRelOpTable[] = { "eq", "!eq", "lt", "!le", "le", "!lt" };
        op = FloatRelOpTable[relOp];
        const char* trueReg = GetReg(INTEGER);
        fprintf(mips, "\n\t# true register"\
                      "\n\tli %s, 1\n", trueReg);

        if (op[0] == '!')
        {
            // Negate operation 
            fprintf(mips, "\n\t# compare two floats and negate\n"\
                          "\tc.%s.s %s, %s\n"\
                          "\tmovt %s, $zero\n"\
                          "\tmovf %s, %s\n", &op[1], reg0->_name, reg1->_name, res._name, res._name, trueReg);
        }
        else
        {
            // Normal operation is available
            fprintf(mips, "\n\t# compare two floats\n"\
                          "\tc.%s.s %s, %s\n"\
                          "\tmovt %s, %s\n"\
                          "\tmovf %s, $zero\n", op, reg0->_name, reg1->_name, res._name, trueReg, res._name);
        }
        FreeReg(INTEGER); // Free true reg
        FreeReg(FLOATING);
        FreeReg(FLOATING);
    }
    else // INTEGER
    {
        res._name = reg0->_name;
        static const char* IntRelOpTable[] = { "seq", "sne", "slt", "sgt", "sle", "sge" };
        const char* op = IntRelOpTable[relOp];
        fprintf(mips, "\n\t# compare two ints\n"\
                      "\t%s %s, %s, %s\n", op, res._name, reg0->_name, reg1->_name);
    }
    return res;
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
                          "\tsw %s, %s\n", reg->_name, node->_name);
            break;
        
        case INTEGER:
            fprintf(mips, "\n\t# assigning integer value\n"
                          "\tsw %s, %s\n", reg->_name, node->_name);
            break;

        default: // FLOATING
            fprintf(mips, "\n\t# assigning float value\n"
                          "\ts.s %s, %s\n", reg->_name, node->_name);
            break;
    }
}

Reg MipsLogOp(LogOp logOp, Reg* reg0, Reg* reg1)
{
    assert((logOp < LOG_OP_COUNT) && (reg0 != NULL) && (reg1 != NULL));
    Reg res;

    if ((reg0->_type == STR) || (reg1->_type == STR))
    {
        yyerror("can't perform arithmetic operations on string");
        return res;
    }

    if (reg0->_type != reg1->_type)
    {
        MipsCast(reg0, FLOATING);
        MipsCast(reg1, FLOATING);
    }

    res._type = ((reg0->_type == FLOATING) || (reg1->_type == FLOATING)) ? FLOATING : INTEGER;
    static const char* LogOpTable[] = { "and", "or", "nor" };
    const char* op = LogOpTable[logOp];

    fprintf(mips, "\t%s %s, %s, %s", op, res._name, reg0->_name, reg1->_name);
    
    return res;
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

Reg MipsLoadV(const Node* node)
{
    assert((node != NULL) && (node->_type < TYPE_COUNT) && (node->_name != NULL));
    Reg reg;

    switch(node->_type)
    {
        case STR:
            reg._type = STR;
            reg._name = GetReg(STR);
            fprintf(mips, "\n\tla %s, %s\n", reg._name, node->_name);
            break;
        
        case INTEGER:
            reg._type = INTEGER;
            reg._name = GetReg(INTEGER);
            fprintf(mips, "\n\tlw %s, %s\n", reg._name, node->_name);
            break;
            
        default: // FLOATING
            reg._type = FLOATING;
            reg._name = GetReg(FLOATING);
            fprintf(mips, "\n\tl.s %s, %s\n", reg._name, node->_name);
            break;
    }
    return reg;
}

Reg MipsLoadI(const Val* val)
{
    assert((val != NULL) && (val->_type < TYPE_COUNT) && (val->_sval != NULL));
    static size_t stringCount = 0U;
    const char* str = "str";
    Reg reg;

    switch(val->_type)
    {
        case STR:
            reg._type = STR;
            reg._name = GetReg(STR);
            fprintf(mips, "\n\t.data\n"\
                          "%s%zu: .asciiz %s\n"\
	                      "\t.text\n", str, stringCount, val->_sval);

            fprintf(mips, "\n\t# store pointer to string\n"\
                          "\tla %s, %s%zu\n", reg._name, str, stringCount);

            ++stringCount;
            break;
        
        case INTEGER:
            reg._type = INTEGER;
            reg._name = GetReg(INTEGER);
            fprintf(mips, "\n\tli %s, %s\n", reg._name, val->_sval);
            break;

        default: // FLOATING
            reg._type = FLOATING;
            reg._name = GetReg(FLOATING);
            float f = strtof(val->_sval, NULL);   // Converting string to float
            int* x = (int*)&(f);                  // reinterpret_cast to int
            const char* tmpReg = GetReg(INTEGER); // Getting tmp reg
            
            fprintf(mips, "\n\tli %s, 0x%x\n", tmpReg, (*x));
            fprintf(mips, "\tmtc1 %s, %s\n", tmpReg, reg._name);
            FreeReg(INTEGER);
            break;
    }
    return reg;
}

void MipsCast(Reg* reg, Type t)
{
    assert((reg->_type < TYPE_COUNT) && (t < TYPE_COUNT));

    if (reg->_type == t)
        return;

    const char* val;
    if (t == FLOATING)
    {
        val = reg->_name;
        reg->_name = GetReg(FLOATING);
        fprintf(mips, "\n\t# Move integer value to floating-point register\n"\
                      "\tmtc1 %s, %s\n"\
                      "\tcvt.s.w %s, %s\n", val, reg->_name, reg->_name, reg->_name);
        FreeReg(INTEGER);
    }
}

void MipsIf(Reg* reg, uint32_t part)
{
    assert((part < 3U) && (reg != NULL));

    if(part == 0U)
    {
        fprintf(mips,"\n\tbeq %s, $zero, else%zu\n", reg->_name, IfIndex);
    }
    else if(part == 1U)
    {
        fprintf(mips,"\n\tj continue%zu\n", IfIndex);
        fprintf(mips,"\nelse%zu:\n", IfIndex);
    }
    else
    {
        fprintf(mips,"\ncontinue%zu:\n", IfIndex);
        ++IfIndex;
    }
}

void MipsForEach(uint32_t part)
{
    assert(part < 3U);

    if(part == 0U)
    {
        fprintf(mips,"\n\tj for_each_stmt%zu\n"\
                     "\nfor_each_increment%zu:\n", ForEachIndex, ForEachIndex);
    }
    else if(part == 1U)
    {
        fprintf(mips,"\n\tj while%zu\n"\
                     "\nfor_each_stmt%zu:\n", WhileIndex, ForEachIndex);
    }
    else
    {
        fprintf(mips,"\n\tj for_each_increment%zu\n", ForEachIndex);
        ++ForEachIndex;
    }
}

void MipsWhile(Reg* reg, uint32_t part)
{
    assert(part < 4U);

    if (part == 0U)
    {
        fprintf(mips,"\nwhile%zu:\n", WhileIndex);
    }
    else if (part == 1U)
    {
        assert(reg != NULL);
        fprintf(mips,"\n\tbeq %s, $zero, endwhile%zu\n", reg->_name, WhileIndex);
    }
    else if (part == 2U)
    {
        fprintf(mips,"\n\tj while%zu\n", WhileIndex);
    }
    else
    {
        fprintf(mips,"\nendwhile%zu:\n", WhileIndex);
        ++WhileIndex;
    }
}

void MipsSwitch(Node* node)
{
    if (node == NULL)
    {
        yyerror("id not found");
    }
    SwitchNode = node;
}

const char* GetReg(Type t)
{
    assert(t < TYPE_COUNT);

    if (t == INTEGER)
    {
        assert(RegTrackerT < 8U);
        return TmpRegs[RegTrackerT++];
    }
    if (t == FLOATING)
    {
        assert(RegTrackerF < 8U);
        return FloatRegs[RegTrackerF++];
    }
    else // STRING
    {
        assert(RegTrackerT < 8U);
        return TmpRegs[RegTrackerT++];
    }
    
}

void FreeReg(Type t)
{
    assert(t < TYPE_COUNT);

    if (t == INTEGER)
    {
        assert(RegTrackerT != 0U);
        --RegTrackerT;
    }
    if (t == FLOATING)
    {
        assert(RegTrackerF != 0U);
        --RegTrackerF;
    }
}

void FreeAllRegs()
{
    RegTrackerF = 0U;
    RegTrackerT = 0U;
}
