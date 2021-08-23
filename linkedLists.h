#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <sstream>
#include <vector>
#include <regex>
#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>
using namespace std;

struct intNode{
    int val;
    intNode* next;
};

struct stringNode{
    string val;
    stringNode* next;
};

class stringLinkedList{
    private:
        stringNode *head;
        stringNode *tail;
        int size;
    public:
        stringLinkedList() {head = nullptr; tail = nullptr; size = 0;}
        void addStringNode(string data);
        string getVal(int index);
        stringLinkedList getValSlice(string s);
        stringNode* getHead() {return head;}
        int getSize() {return size;}
        string print();
};

string stringLinkedList::print(){
    string p = "['";
    stringNode* curr = head;

    while(curr->next != nullptr){
        p += curr->val + "','";
        curr = curr->next;
    }
    p += curr->val + "']";

    return p;
}

string stringLinkedList::getVal(int index){
    if (index == 0 && head != nullptr) return head->val;
    stringNode* curr = head;
    for (int i = 0; i < index && curr->next != nullptr; i++){
        curr = curr->next;
    }
    return curr->val;
}

stringLinkedList stringLinkedList::getValSlice(string s){
    string sStart, sEnd;
    int start, end;
    stringLinkedList newList;
    for (int i = 1; i <= s.size() - 1; i++){
        if (i == s.size() - 1){
            newList.addStringNode(getVal(stoi(sStart)));
            return newList;
        }
        if(s[i] == ':') break;
        sStart += s[i];
    }
    for (int i = sStart.size() + 2; i < s.size() - 1; i++){
        sEnd += s[i];
    }
    if (sStart != "")
        start = stoi(sStart);
    else 
        (start = 0);
    if (sEnd != "")
        end = stoi(sEnd);
    else
        end = size - 1;


    stringNode* curr = head;
    for(int i = 0; i < start; i++)
        curr = curr->next;

        for (int i = start; i <= end || curr == nullptr; i++){
            string value = "\"";
            value += getVal(i) + "\"";
            newList.addStringNode(getVal(i));
        }

    return newList;
}

void stringLinkedList::addStringNode(string data){
    stringNode *newNode = new stringNode;

    if (data[0] == '"' && data[data.size() - 1] == '"')
        data = data.substr(1, data.size() - 2);

    newNode->val = data;
    newNode->next = nullptr;

    if(head == nullptr){
        head = newNode;
        tail = newNode;
        size++;
    }
    else{
        tail->next = newNode;
        tail = tail->next;
        size++;
    }

}

class intLinkedList{
    private:
        intNode *head;
        intNode *tail;
        int size;
    public:
        intLinkedList() {head = nullptr; tail = nullptr; size = 0;}
        void addIntNode(int data);
        intNode* getHead() {return head;}
        int getVal(int index);
        intLinkedList getValSlice(string s);
        int getSize(){return size;}
        string print();
};

string intLinkedList::print(){
    string p = "[";
    intNode* curr = head;

    while(curr->next != nullptr){
        p += to_string(curr->val) + ",";
        curr = curr->next;
    }
    p += to_string(curr->val) + "]";

    return p;
}

int intLinkedList::getVal(int index){
    if (index == 0 && head != nullptr) return head->val;
    intNode* curr = head;
    for (int i = 0; i < index && curr->next != nullptr; i++){
        curr = curr->next;
    }
    return curr->val;
}

intLinkedList intLinkedList::getValSlice(string s){
    string sStart, sEnd;
    int start, end;
    intLinkedList newList;

    for (int i = 1; i <= s.size() - 1; i++){
        if (i == s.size() - 1){
            newList.addIntNode(getVal(stoi(sStart)));
            return newList;
        }
        if(s[i] == ':') break;
        sStart += s[i];
    }
    for (int i = sStart.size() + 2; i < s.size() - 1; i++){
        sEnd += s[i];
    }

    if (sStart != "")
        start = stoi(sStart);
    else 
        (start = 0);
    if (sEnd != "")
        end = stoi(sEnd);
    else
        end = size - 1;


    intNode* curr = head;
    for(int i = 0; i < start; i++)
        curr = curr->next;

        for (int i = start; i <= end || curr == nullptr; i++){
            newList.addIntNode(getVal(i));
        }

    return newList;
}

void intLinkedList::addIntNode(int data){
    intNode *newNode = new intNode;
    newNode->val = data;
    newNode->next = nullptr;

    if(head == nullptr){
        head = newNode;
        tail = newNode;
        size++;
    }
    else{
        tail->next = newNode;
        tail = tail->next;
        size++;
    }

}

vector<string> getListContent(string list){
        list = list.substr(1, list.size() - 2);
        stringstream ss(list);
        vector<string> v;
        while(ss.good()) {
            string substr;
            getline(ss, substr, ',');
            v.push_back(substr);
        }
        return v;
}
#endif