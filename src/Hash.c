#include "Hash.h"

int Hash(const char* key)
{
    MY_ASSERT(key != NULL, "Invalid Hash key", 0)

    int sum = 0;
    for (int i = 0; i < strlen(key); i++) 
    {
        sum += key[i];
    }
    return sum % TABLE_SIZE;
}

Node* GetFromTable(const Bucket* table, const char* name)
{
    MY_ASSERT((table != NULL) && (name != NULL), "Invalid GetFromTable", NULL)

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

bool InsertToTable(Bucket* table, const char* id, Type t, bool isConst)
{
    MY_ASSERT((table != NULL) && (id != NULL) && (t < TYPE_COUNT), "invalid InsertToTable", false)

    Node* tmp = GetFromTable(table, id);
    if (GetFromTable(table, id) != NULL)
    {
        yyerror("Redeclaration of variable: %s", id); // Variable name already exists
        return false;
    }
    int index = Hash(id);
    Node *new_node = (Node*) malloc(sizeof(Node));
    new_node->_name = id;
    new_node->_isConst = isConst;
    new_node->_type = t;
    new_node->_next = table[index]._head;
    table[index]._head = new_node;
    return true;
}

void FreeBucket(Bucket* bucket) 
{
    MY_ASSERT(bucket != NULL, "Invalid FreeBucket", VOID_VAL)

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
    MY_ASSERT(table != NULL, "Invalid FreeTable", VOID_VAL)

    for (int i = 0; i < TABLE_SIZE; i++) 
    {
        FreeBucket(&table[i]);
    }
}

void yyerror(const char* format, ...)
{
    // Incrementing error count
    errorCount++;
    
    // Printing to stderr
	fprintf(stderr, "Error    | Line: %d,\t", yylineno);
    
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);

	fprintf(stderr, "\n");
}