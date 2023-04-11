#include "MipsWriter.h"

/*
* Amit Zohar    313307720
* Ofek Ben Atar 322208430
*/

extern size_t errorCount;        /**< Stores the total number of errors throughout the compilation process */
extern FILE* mips;               /**< Reference to mips .s file */

static size_t RegTrackerFloat    = 0U; /**< Floating register tracker, keeps track of the registers in use */
static size_t RegTrackerInteger  = 0U; /**< Int register tracker, keeps track of the registers in use */
static size_t IfIndex            = 0U; /**< If indexer      (used to avoid label duplicates) */
static size_t WhileIndex         = 0U; /**< While indexer   (used to avoid label duplicates) */
static size_t ForEachIndex       = 0U; /**< ForEach indexer (used to avoid label duplicates) */
static size_t SwitchIndex        = 0U; /**< Switch indexer  (used to avoid label duplicates) */
static size_t CaseIndex          = 0U; /**< Case indexer    (used to avoid label duplicates) */
static size_t BufferIndex        = 0U; /**< Buffer indexer  (used to avoid label duplicates) */
static size_t StringCount        = 0U; /**< String indexer  (used to avoid label duplicates) */

static const Node* SwitchNode = NULL;  /**< Switch Node, once switch is opened stores the node pointer */

bool IsAssignValid(Type type1, Type type2)
{
    MY_ASSERT((type1 < TYPE_COUNT) && (type2 < TYPE_COUNT), "Invalid IsAssignValid", false)

    return (type1 == type2) || (type1 == FLOATING); 
}

void MipsData()
{
    fprintf(mips, "\t.data\n");
}  
    
void MipsDecl(Type t, const char* id, const char* sval)
{
    MY_ASSERT((t < TYPE_COUNT) && (id != NULL) && (sval != NULL), "Invalid MipsDecl", VOID_VAL)

    const char* tStr = (t == FLOATING) ? "float" : "word";
    fprintf(mips, "%s:\t.%s %s\n", id, tStr, sval);
}

void MipsIn(const Node* node)
{
    MY_ASSERT((node != NULL) && (node->_type < TYPE_COUNT), "Invalid MipsIn", VOID_VAL)

    if (node->_isConst)
    {
        yyerror("Can't assign input to const variable \"$s\"", node->_name);
        return;
    }

    fprintf(mips, "\n\t# read input\n");
    if (node->_type == STR)
    {
        fprintf(mips, "\n\t.data\n"\
                          "buffer%zu: .space %d # allocate buffer to read input\n"\
	                      "\t.text\n", BufferIndex, BUFFER_SIZE);
        
        fprintf(mips, "\n\t# input string  \n"\
                      "\tli $v0, 8         \n"\
                      "\tla $a0, buffer%zu \n"\
                      "\tli $a1, %d        \n"\
                      "\tsw $a0, %s        \n"\
                      "\tsyscall           \n", BufferIndex, BUFFER_SIZE, node->_name);
        ++BufferIndex;
    }
    else if (node->_type == INTEGER)
    {
        fprintf(mips, "\n\t# input int \n"\
                      "\tli $v0, 5     \n"\
                      "\tsyscall       \n"\
                      "\tsw $v0, %s    \n", node->_name);
    }
    else // FLOATING
    {
        fprintf(mips, "\n\t# input float \n"\
                      "\tli $v0, 6     \n"\
                      "\tsyscall       \n"\
                      "\ts.s $f0, %s   \n", node->_name);
    }
}

void MipsOut(const Reg* reg)
{
    MY_ASSERT((reg != NULL) &&
              (reg->_type < TYPE_COUNT) && 
              (reg->_name != NULL), "Invalid MipsOut", VOID_VAL)

    static const int OutTable[] = { 1, 2, 4 }; // INTEGER=0, FLOATING=1, STR=2
    static const char* CmdTable[] = { "move $a0", "mov.s $f12", "move $a0" };

    fprintf(mips, "\n\t# printing \n"\
                  "\tli $v0, %d   \n"\
                  "\t%s, %s       \n"\
                  "\tsyscall      \n", OutTable[reg->_type], CmdTable[reg->_type], reg->_name);
}

Reg MipsMathOp(MathOp mathOp, Reg reg0, Reg reg1)
{
    Reg res;
    MY_ASSERT(mathOp < MATH_OP_COUNT, "Invalid MipsMathOp", res)

    if ((reg0._type == STR) || (reg1._type == STR))
    {
        yyerror("can't perform arithmetic operations on string");
        return res;
    }

    res._type = ((reg0._type == FLOATING) || (reg1._type == FLOATING)) ? FLOATING : INTEGER;
    static const char* MathOpTable[] = { "add", "sub", "mul", "div" };
    const char* op = MathOpTable[mathOp];

    if (res._type == FLOATING)
    {        
        if (reg0._type != FLOATING)
        {
            reg0 = MipsCast(reg0, FLOATING);
            res._name = reg1._name;
        }
        else if (reg1._type != FLOATING)
        {
            reg1 = MipsCast(reg1, FLOATING);
            res._name = reg0._name;
        }
        else
        {
            res._name = reg0._name;
        }
        
        fprintf(mips, "\n\t# mathop two floats\n"\
                        "\t%s.s %s, %s, %s\n", op, res._name, reg0._name, reg1._name);
        FreeReg(FLOATING);
    }
    else // INTEGER
    {
        res._name = reg0._name;
        fprintf(mips, "\n\t# mathop two ints\n"\
                      "\t%s %s, %s, %s\n", op, res._name, reg0._name, reg1._name);
        FreeReg(INTEGER);
    }
    return res;
}

Reg MipsRelOp(RelOp relOp, Reg reg0, Reg reg1)
{
    Reg res;
    MY_ASSERT(relOp < REL_OP_COUNT, "Invalid MipsRelOp", res)

    if ((reg0._type == STR) || (reg1._type == STR))
    {
        yyerror("can't perform arithmetic operations on string");
        return res;
    }

    res._type = ((reg0._type == FLOATING) || (reg1._type == FLOATING)) ? FLOATING : INTEGER;
    const char* op;
    
    if (res._type == FLOATING)
    {        
        if (reg0._type != reg1._type)
        {
            reg0 = MipsCast(reg0, FLOATING);
            reg1 = MipsCast(reg1, FLOATING);
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
                          "\tmovf %s, %s\n", &op[1], reg0._name, reg1._name, res._name, res._name, trueReg);
        }
        else
        {
            // Normal operation is available
            fprintf(mips, "\n\t# compare two floats\n"\
                          "\tc.%s.s %s, %s\n"\
                          "\tmovt %s, %s\n"\
                          "\tmovf %s, $zero\n", op, reg0._name, reg1._name, res._name, trueReg, res._name);
        }
        FreeReg(INTEGER); // Free true reg
        FreeReg(FLOATING);
        FreeReg(FLOATING);
    }
    else // INTEGER
    {
        res._name = reg0._name;
        static const char* IntRelOpTable[] = { "seq", "sne", "slt", "sgt", "sle", "sge" };
        const char* op = IntRelOpTable[relOp];
        fprintf(mips, "\n\t# compare two ints\n"\
                      "\t%s %s, %s, %s\n", op, res._name, reg0._name, reg1._name);
    }
    return res;
}

void MipsAssign(const Node* node, Reg reg)
{
    MY_ASSERT((node != NULL) &&
              (node->_type < TYPE_COUNT) &&
              (reg._type < TYPE_COUNT), "Invalid MipsAssign", VOID_VAL)

    // Validating assignment is valid
    if (node->_isConst)
    {
        yyerror("Invalid assignment to const variable \"%s\"", node->_name);
        return;
    }

    if (!IsAssignValid(node->_type, reg._type))
    {
        yyerror("Invalid type assignment \"%s\"", node->_name);
        return;
    }

    // Casting if needed
    if (node->_type != reg._type)
    {
        reg = MipsCast(reg, node->_type);
    }

    // Assigning
    const char* assignInstruction = node->_type == FLOATING ? "s.s" : "sw";
    fprintf(mips, "\n\t# assigning value\n"
                    "\t%s %s, %s\n", assignInstruction, reg._name, node->_name);

}

Reg MipsLogOp(LogOp logOp, Reg reg0, Reg reg1)
{
    Reg res;
    MY_ASSERT(logOp < LOG_OP_COUNT, "Invalid MipsLogOp", res)

    if ((reg0._type == STR) || (reg1._type == STR))
    {
        yyerror("can't perform logical operations on string");
        return res;
    }

    // Casting if needed
    if (reg0._type != reg1._type)
    {
        reg0 = MipsCast(reg0, FLOATING);
        reg1 = MipsCast(reg1, FLOATING);
    }

    // Performing logical operation
    res._type = ((reg0._type == FLOATING) || (reg1._type == FLOATING)) ? FLOATING : INTEGER;
    static const char* LogOpTable[] = { "and", "or", "nor" };
    const char* op = LogOpTable[logOp];
    fprintf(mips, "\n\t# logical operation\n" \
                  "\t%s %s, %s, %s", op, res._name, reg0._name, reg1._name);
    
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
    fprintf(mips, "\t.text"\
                   "\n\t.globl main"\
                   "\n\nmain:\n");
}

Reg MipsLoadVar(const Node* node)
{
    Reg reg;
    MY_ASSERT((node != NULL) && 
              (node->_type < TYPE_COUNT) &&
              (node->_name != NULL), "Invalid MipsLoadVar", reg)
    
    // Allocating register
    Type t = node->_type;
    reg._type = node->_type;
    reg._name = GetReg(node->_type);
    
    // Loading variable to register
    const char* loadInstruction = t == FLOATING ? "l.s" : "lw";
    fprintf(mips, "\n\t%s %s, %s\n", loadInstruction, reg._name, node->_name);

    return reg;
}

Reg MipsLoadImmediate(const Val* val)
{
    Reg reg;
    MY_ASSERT((val != NULL) &&
              (val->_type < TYPE_COUNT) &&
              (val->_sval != NULL), "Invalid MipsLoadImmediate", reg)
    
    // Allocating register
    const char* str = "str";
    reg._type = val->_type;
    reg._name = GetReg(val->_type);

    if (val->_type == STR)
    {
        fprintf(mips, "\n\t.data\n"\
                        "%s%zu: .asciiz %s\n"\
                        "\t.text\n", str, StringCount, val->_sval);

        fprintf(mips, "\n\t# store pointer to string\n"\
                        "\tla %s, %s%zu\n", reg._name, str, StringCount);

        ++StringCount;
    } 
    else if (val->_type == INTEGER)
    {
        fprintf(mips, "\n\t# loading immediate int\n" \
                      "\tli %s, %s\n", reg._name, val->_sval);
    }
    else // FLOATING
    {
        float f = strtof(val->_sval, NULL);   // Converting string to float
        int* x = (int*)&(f);                  // reinterpret_cast to int
        const char* tmpReg = GetReg(INTEGER); // Getting tmp reg
        
        fprintf(mips, "\n\t# loading immediate float\n" \
                      "\tli %s, 0x%x\n", tmpReg, (*x));
        fprintf(mips, "\tmtc1 %s, %s\n", tmpReg, reg._name);
        FreeReg(INTEGER);
    }
    return reg;
}

Reg MipsCast(Reg reg, Type t)
{
    MY_ASSERT((reg._type < TYPE_COUNT) && (t < TYPE_COUNT), "Invalid MipsCast", reg)

    // Returning reg if no need for casting
    if (reg._type == t)
        return reg;

    Reg returnReg;
    if (t == FLOATING)
    {
        returnReg._type = FLOATING;
        returnReg._name = GetReg(FLOATING);
        fprintf(mips, "\n\t# Move integer value to floating-point register\n"\
                      "\tmtc1 %s, %s\n"\
                      "\tcvt.s.w %s, %s\n", reg._name, returnReg._name, returnReg._name, returnReg._name);
        FreeReg(INTEGER);
    }
    return returnReg;
}

void MipsIf(const Reg* reg, uint32_t part)
{
    MY_ASSERT((part < 3U) && (reg != NULL), "Invalid MipsIf", VOID_VAL)

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
    MY_ASSERT(part < 3U, "Invalid MipsForEach", VOID_VAL)

    if(part == 0U)
    {
        fprintf(mips,"\n\tj for_each_stmt_%zu\n"\
                     "\nfor_each_increment_%zu:\n", ForEachIndex, ForEachIndex);
    }
    else if(part == 1U)
    {
        fprintf(mips,"\n\tj while_%zu\n"\
                     "\nfor_each_stmt_%zu:\n", WhileIndex, ForEachIndex);
    }
    else
    {
        fprintf(mips,"\n\tj for_each_increment_%zu\n", ForEachIndex);
        ++ForEachIndex;
    }
}

void MipsWhile(const Reg* reg, uint32_t part)
{
    MY_ASSERT(part < 4U, "Invalid MipsWhile", VOID_VAL)

    if (part == 0U)
    {
        fprintf(mips,"\nwhile_%zu:\n", WhileIndex);
    }
    else if (part == 1U)
    {
        MY_ASSERT(reg != NULL, "Invalid MipsWhile", VOID_VAL)
        fprintf(mips,"\n\tbeq %s, $zero, endwhile_%zu\n", reg->_name, WhileIndex);
    }
    else if (part == 2U)
    {
        fprintf(mips,"\n\tj while_%zu\n", WhileIndex);
    }
    else
    {
        fprintf(mips,"\nendwhile_%zu:\n", WhileIndex);
        ++WhileIndex;
    }
}

void MipsSwitch(const Node* node, bool start)
{
    if (start)
    {
        MY_ASSERT(node != NULL, "Invalid MipsSwitch", VOID_VAL)
        
        fprintf(mips, "\n\t# start switch %zu\n", SwitchIndex);
        SwitchNode = node;
    }
    else
    {
        fprintf(mips, "\nend_switch_%zu:\n", SwitchIndex);
        ++SwitchIndex;
    }
}

void MipsCase(Val* val, bool start)
{
    if (start)
    {
        MY_ASSERT((val != NULL) && (SwitchNode != NULL), "Invalid MipsCase", VOID_VAL)
        
        fprintf(mips, "\ncase_%zu_%zu:\n", SwitchIndex, CaseIndex);
        Reg reg0 = MipsLoadVar(SwitchNode);
        Reg reg1 = MipsLoadImmediate(val);
        Reg result = MipsRelOp(EQ, reg0, reg1);
        fprintf(mips, "\n\tbeq %s, $zero, end_case_%zu_%zu\n", result._name, SwitchIndex, CaseIndex);
    }
    else
    {
        fprintf(mips, "\n\tj end_switch_%zu\n", SwitchIndex);
        fprintf(mips, "end_case_%zu_%zu:\n", SwitchIndex, CaseIndex);
        ++CaseIndex;
        SwitchNode = NULL; 
    }
}

const char* GetReg(Type t)
{
    MY_ASSERT(t < TYPE_COUNT, "Invalid GetReg", "")

    if (t == INTEGER)
    {
        MY_ASSERT(RegTrackerInteger < TmpRegCount, "Invalid GetReg INTEGER", "")
        return IntTmpRegs[RegTrackerInteger++];
    }
    if (t == FLOATING)
    {
        MY_ASSERT(RegTrackerFloat < TmpRegCount, "Invalid GetReg FLOATING", "")
        return FloatTmpRegs[RegTrackerFloat++];
    }
    else // STRING
    {
        MY_ASSERT(RegTrackerInteger < TmpRegCount, "Invalid GetReg STRING", "")
        return IntTmpRegs[RegTrackerInteger++];
    }
    
}

void FreeReg(Type t)
{
    MY_ASSERT(t < TYPE_COUNT, "Invalid FreeReg", VOID_VAL)

    if (t == INTEGER)
    {
        MY_ASSERT(RegTrackerInteger != 0U, "Invalid FreeReg Integer", VOID_VAL)
        --RegTrackerInteger;
    }
    if (t == FLOATING)
    {
        MY_ASSERT(RegTrackerFloat != 0U, "Invalid FreeReg Floating", VOID_VAL)
        --RegTrackerFloat;
    }
}

void FreeAllRegs()
{
    RegTrackerFloat = 0U;
    RegTrackerInteger = 0U;
}
