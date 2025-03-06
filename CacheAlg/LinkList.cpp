#include <iostream>
#include <cstdlib>
using namespace std;

struct Node
{
    int data;
    Node* next;
};

Node* head = new Node();

void Insert(int value, int n)
{
    Node* temp1 = new Node();
    temp1->data = value;
    temp1->next = NULL;
    if(n == 1)
    {
        temp1 -> next = head;
        head = temp1 ;
        return;
    }
    Node* temp2 = head;
    for (int i = 0; i < n-2; i++)
    {
        temp2 = temp2 -> next;
    }
    temp1->next = temp2->next;
    temp2->next = temp1;
    
}

void Print() // Better for normal printing
{
    Node* temp = head;
    while( temp != NULL)
    {
        cout << temp->data << endl;
        temp = temp->next;
    }
}


void PrintOn(Node* head) // Better for inverse printing
{
    if (head == NULL)
    {
        return;
    }
    cout << head->data;
    PrintOn(head -> next);
}

void Delete(int n)
{
    Node* temp1 = head;
    if(n == 1)
    {
        head = head->next;
        free(temp1);
    }
    for (int i = 0; i < n-2; i++)
    {
        temp1 = temp1 -> next;
    }
    Node* temp2 = temp1 ->next;
    temp1->next = temp2 -> next;
    free(temp2);
}


Node* Reverse()
{
    Node* current = head;
    Node* prev = NULL;
    Node* next = new Node();
    while(current != NULL)
    {
        next = current -> next;
        current->next = prev;
        prev = current;
        current = next;
    }
    head = prev;
    return head;
}

int main()
{
    head->next = NULL;
    Insert(2,1);  
    Insert(3,2);
    Insert(4,1);
    Insert(5,3);
    Insert(6,4);
    Delete(1);
    PrintOn(head);
    cout << endl;   
    head = Reverse();
    PrintOn(head);
    return 0;   
}

