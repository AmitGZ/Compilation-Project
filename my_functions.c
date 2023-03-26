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

void MipsAdd(char* result, const char* reg1, const char* reg2)
{

}

void MipsMul(char* result, const char* reg1, const char* reg2)
{

}

void MipsOr(char* result, const char* reg1, const char* reg2)
{

}

void MipsAnd(char* result, const char* reg1, const char* reg2)
{

}

void MipsRelop(char* result, const char* reg1, const char* reg2, RelOp relOp)
{

}

void MipsNot(char* result, const char* reg1)
{

}
