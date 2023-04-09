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

void InsertToTable(Bucket* table, const char* id, Type t, bool isConst)
{
    MY_ASSERT((table != NULL) && (id != NULL) && (t < TYPE_COUNT), "invalid InsertToTable",)

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
    MY_ASSERT(bucket != NULL, "Invalid FreeBucket",)

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
    MY_ASSERT(table != NULL, "Invalid FreeTable",)

    for (int i = 0; i < TABLE_SIZE; i++) 
    {
        FreeBucket(&table[i]);
    }
}

void yyerror(const char* str)
{
    errorCount++;
	fprintf(stderr, "Error | Line: %d,\t%s\n", yylineno, str);
}