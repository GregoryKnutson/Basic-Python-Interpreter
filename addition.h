#ifndef ADDITION_H
#define ADDITION_H

#include <sstream>
#include <vector>
#include <regex>
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>
#include "linkedLists.h"
using namespace std;

bool isNumber(string s)
{
    for (int i = 0; i < s.length(); i++)
        if (isdigit(s[i]) == false)
            return false;
 
    return true;
}

bool isValid(string s) {
    stack<char> stack;
    
    for(int i =0 ; i < s.length(); i++){
        char c = s[i];
        if(c == ')'){
            if(stack.size() == 0 ) return false;
            if(c == ')' && stack.top() != '(') return false;
            stack.pop();
            
         } else if (c == '('){
            stack.push(c);
        }
        else continue;
    }
    if(stack.size() == 0) return true;
    return false;
}

void getTokens(string eq, vector<string>& tokens){ //String + "One + One"
    string temp = "";

   for (int i = 0; i < eq.size(); i++){
       if (eq[i] == ' ' && temp != "")
       {
           tokens.push_back(temp);
           temp = "";
           continue;
       }
       else if (eq[i] == '"'){
           temp += eq[i];
           i++;
           while(eq[i] != '"'){
                if (eq[i] != '"'){
                temp += eq[i];
                i++;
                }
           }
           temp += '"';
           tokens.push_back(temp);
           temp = "";
           continue;
       }
        else if (eq[i] == '['){
           temp += eq[i];
           i++;
           while(eq[i] != ']'){
                temp += eq[i];
                i++;
           }
           temp += ']';
           tokens.push_back(temp);
           temp = "";
           continue;
       }
       else if (eq[i] == '+' && temp != ""){
           tokens.push_back(temp);
           temp = "";
           continue;
       }
       else if (eq[i] == '+' && temp == "") continue;
       else if (eq[i] == ' ' && temp == "") continue;
       else if (eq[i] == '(' && temp == "") continue;
       else if (eq[i] == ')') continue;

       temp += eq[i];
   }
   if (temp != "")
    tokens.push_back(temp);
    return;
}

bool checkTokensForStrings(string token){
    string temp = token;
    if (temp[0] != '"') return false;
    return true;
}

bool checkTokensForInt(string token){
        string temp = token;
        if (!isdigit(temp[0])) return false;
    return true;
}

bool checkTokensForIntList(string token){
    if (token[0] == '[' && token[token.size() - 1] == ']'){
        vector<string> list = getListContent(token);

        for (int i = 0; i < list.size(); i++){
            string test = list[i];
            if (!isdigit(test[0])) return false;
        }
        return true;
    }
    return false;
}

bool checkTokensForListSplit(string token){
    regex split("([a-zA-Z0-9]+\\[:[0-9]+\\]|[a-zA-Z0-9]+\\[[0-9]+:\\]|[a-zA-Z0-9]+\\[[0-9]+:[0-9]+\\])");
    smatch match;

    regex_search(token, match, split);
    if (match.size() > 0)
        return true;

    return false;
}

bool checkTokensForSingleElement(string token){
    regex single("([a-zA-Z0-9]+\\[[0-9]+\\])");
    smatch match;

    regex_search(token, match, single);
    if (match.size() > 0)
        return true;

    return false;
}

bool checkTokensForIntListVariable(string token, unordered_map<string, intLinkedList> intListVariables){
    regex var("([a-zA-Z0-9]+\\[)");
    smatch match;

    regex_search(token, match, var);
    if (match.size() > 0){
        string s = match[0].str();
        s.pop_back();
        if(intListVariables.find(s) != intListVariables.end()){
            return true;
        }
        else return false;
    }
    return false;
}

bool checkTokensForStringListVariable(string token, unordered_map<string, stringLinkedList> stringListVariables){
    regex var("([a-zA-Z0-9]+\\[)");
    smatch match;

    regex_search(token, match, var);
    if (match.size() > 0){
        string s = match[0].str();
        s.pop_back();
        if(stringListVariables.find(s) != stringListVariables.end()){
            return true;
        }
        else return false;
    }
    return false;
}

bool checkTokensForStringList(string token){
    if (token[0] == '[' && token[token.size() - 1] == ']'){
        vector<string> list = getListContent(token);

        for (int i = 0; i < list.size(); i++){
            string test = list[i];
            if (test[0] != '"' && test[test.size() - 1] != '"') return false;
        }
        return true;
    }
    return false;
}

//same as before but recursively adding
string additionHelper(vector<string>& tokens, string curr, int n, unordered_map<string, string> stringMap, unordered_map<string, int> intMap, unordered_map<string, stringLinkedList> stringListVariables, unordered_map<string, intLinkedList> intListVariables){
    string ans;
    if (n >= tokens.size()) return curr;
    int temp;
    if(intMap.find(tokens[n]) != intMap.end() || checkTokensForInt(tokens[n])){
        if(intMap.find(tokens[n]) != intMap.end())
            temp = intMap[tokens[n]];
        else
            temp = stoi(tokens[n]);

        curr = to_string(stoi(curr) + temp);
    }
    
    else if(stringMap.find(tokens[n]) != stringMap.end() || checkTokensForStrings(tokens[n])){
        string temp;
        if(stringMap.find(tokens[n]) != stringMap.end())
            temp = stringMap[tokens[n]];
        else{
            temp = tokens[n];
            temp = temp.substr(1, temp.size() - 2);

        }

        curr = curr + temp;
    }
    else if(intListVariables.find(tokens[n]) != intListVariables.end() || checkTokensForIntList(tokens[n]) || (checkTokensForListSplit(tokens[n]) && checkTokensForIntListVariable(tokens[n], intListVariables)) || (checkTokensForSingleElement(tokens[n]) && checkTokensForIntListVariable(tokens[n], intListVariables))){
        intLinkedList var1Head;
        int var1;
        if(intListVariables.find(tokens[n]) != intListVariables.end())
            var1Head = intListVariables[tokens[n]];
        else if (checkTokensForIntList(tokens[n])){
            vector<string> list = getListContent(tokens[n]);
            for (int i = 0; i < list.size(); i++){
                var1Head.addIntNode(stoi(list[i]));
            }            
        }
        else if (checkTokensForListSplit(tokens[n])){
            string var = "";
            string slice = "";
            int i = 0;
            while (tokens[n][i] != '['){
                var+= tokens[n][i];
                i++;
            }
            slice = tokens[0].substr(var.size());
            var1Head = intListVariables[var].getValSlice(slice);
        }
        else{
            string var = "";
            string slice ="";
            int i = 0;
            while (tokens[n][i] != '['){
                var+= tokens[n][i];
                i++;
            }
            slice = tokens[n].substr(var.size() + 1);
            slice.pop_back();
            var1 = intListVariables[var].getVal(stoi(slice));
            if (isNumber(curr)){
                int v = stoi(curr);
                v = v + var1;
                string varStr = to_string(v);
                string ans = additionHelper(tokens, varStr, n+1, stringMap, intMap, stringListVariables, intListVariables);
                return ans;
            }
        }
    }
    else if(stringListVariables.find(tokens[n]) != stringListVariables.end() || checkTokensForStringList(tokens[n]) || (checkTokensForListSplit(tokens[n]) && checkTokensForStringListVariable(tokens[n], stringListVariables)) || (checkTokensForSingleElement(tokens[n]) && checkTokensForStringListVariable(tokens[n], stringListVariables))){
        stringLinkedList var1Head;
        string var1;
        if(stringListVariables.find(tokens[n]) != stringListVariables.end())
            var1Head = stringListVariables[tokens[n]];
        else if (checkTokensForStringList(tokens[n])){
            vector<string> list = getListContent(tokens[n]);
            for (int i = 0; i < list.size(); i++){
                var1Head.addStringNode(list[i]);
            }            
        }
        else if (checkTokensForListSplit(tokens[n])){
            string var = "";
            string slice = "";
            int i = 0;
            while (tokens[n][i] != '['){
                var+= tokens[n][i];
                i++;
            }
            slice = tokens[n].substr(var.size());
            var1Head = stringListVariables[var].getValSlice(slice);
            stringNode* currNode = var1Head.getHead();
            while(currNode != nullptr){
                curr += currNode->val;
                curr += "\", \"";
                currNode = currNode->next;
            }
        }
        else{
            string var = "";
            string slice ="";
            int i = 0;
            while (tokens[n][i] != '['){
                var+= tokens[n][i];
                i++;
            }
            slice = tokens[n].substr(var.size() + 1);
            slice.pop_back();
            var1 = stringListVariables[var].getVal(stoi(slice));
            string v = curr + var1;
            ans = additionHelper(tokens, v, n+1, stringMap, intMap, stringListVariables, intListVariables);
            return ans;
        }
    }
    return additionHelper(tokens, curr, n+1, stringMap, intMap, stringListVariables, intListVariables);
}


string add(string eq, unordered_map<string, string> stringMap, unordered_map<string, int> intMap, unordered_map<string, stringLinkedList> stringListVariables, unordered_map<string, intLinkedList> intListVariables){
    if (!isValid(eq))
        return "Invalid Data Types";
    
    vector<string> tokens;
    getTokens(eq, tokens); 

    //int test = integerVariables["Hello"];


    if(intMap.find(tokens[0]) != intMap.end() || checkTokensForInt(tokens[0])) //int add
    {
        int var1;
        int var2;
        if(intMap.find(tokens[0]) != intMap.end())
            var1 = intMap[tokens[0]];
        else
            var1 = stoi(tokens[0]);
        if(intMap.find(tokens[1]) != intMap.end() || checkTokensForInt(tokens[1]))
        {
            if(intMap.find(tokens[1]) != intMap.end())
                var2 = intMap[tokens[1]];
            else
                var2 = stoi(tokens[1]);
        }
        else if(checkTokensForSingleElement(tokens[1]) && checkTokensForIntListVariable(tokens[1], intListVariables)){ //check if its a list token
            string var = "";
            string slice ="";
            int i = 0;
            while (tokens[1][i] != '['){
                var+= tokens[1][i];
                i++;
            }
            slice = tokens[1].substr(var.size() + 1);
            slice.pop_back();
            var2 = intListVariables[var].getVal(stoi(slice));
        }
        else return "Invalid Data Types";

        int temp = var1 + var2;
        string ans = additionHelper(tokens, to_string(temp), 2, stringMap, intMap, stringListVariables, intListVariables);
        return ans;
    }

    else if(stringMap.find(tokens[0]) != stringMap.end() || checkTokensForStrings(tokens[0])) //string add
    {
        string var1;
        string var2;
        if(stringMap.find(tokens[0]) != stringMap.end())
            var1 = stringMap[tokens[0]];
        else{
            var1 = tokens[0];
            var1 = var1.substr(1, var1.size() - 2);

        }
        if(stringMap.find(tokens[1]) != stringMap.end() || checkTokensForStrings(tokens[1]))
        {
            if(stringMap.find(tokens[1]) != stringMap.end())
                var2 = stringMap[tokens[1]];
            else
            {
                var2 = tokens[1];
                var2 = var2.substr(1, var2.size() - 2);
            }
        }
         else if(checkTokensForSingleElement(tokens[1]) && checkTokensForStringListVariable(tokens[1], stringListVariables)){ //check if its a list token
            string var = "";
            string slice ="";
            int i = 0;
            while (tokens[1][i] != '['){
                var+= tokens[1][i];
                i++;
            }
            slice = tokens[1].substr(var.size() + 1);
            slice.pop_back();
            var2 = stringListVariables[var].getVal(stoi(slice));
         }
        
        else return "Invalid Data Types";


        string temp = var1 + var2;

        string ans = additionHelper(tokens, temp, 2, stringMap, intMap, stringListVariables, intListVariables);
        ans.insert(0, "\"");
        ans.push_back('\"');
        return ans;
    }
    //addition of intLists
    else if(intListVariables.find(tokens[0]) != intListVariables.end() || checkTokensForIntList(tokens[0]) || (checkTokensForListSplit(tokens[0]) && checkTokensForIntListVariable(tokens[0], intListVariables)) || (checkTokensForSingleElement(tokens[0]) && checkTokensForIntListVariable(tokens[0], intListVariables))){
        intLinkedList var1Head;
        intLinkedList var2Head;
        int var1, var2;
        if(intListVariables.find(tokens[0]) != intListVariables.end()) //if its a variable
            var1Head = intListVariables[tokens[0]];
        else if (checkTokensForIntList(tokens[0])){ //adding a new list
            vector<string> list = getListContent(tokens[0]);
            for (int i = 0; i < list.size(); i++){
                var1Head.addIntNode(stoi(list[i]));
            }            
        }
        else if (checkTokensForListSplit(tokens[0])){ //checking for a split
            string var = "";
            string slice = "";
            int i = 0;
            while (tokens[0][i] != '['){
                var+= tokens[0][i];
                i++;
            }
            slice = tokens[0].substr(var.size());
            var1Head = intListVariables[var].getValSlice(slice);
        }
        else{ //checking for an index
            string var = "";
            string slice ="";
            int i = 0;
            while (tokens[0][i] != '['){
                var+= tokens[0][i];
                i++;
            }
            slice = tokens[0].substr(var.size() + 1);
            slice.pop_back();
            var1 = intListVariables[var].getVal(stoi(slice));
            if ((checkTokensForSingleElement(tokens[1])) && (checkTokensForIntListVariable(tokens[1], intListVariables))){
                var = "";
                slice = "";
                int i = 0;
                while (tokens[1][i] != '['){
                    var+= tokens[1][i];
                    i++;
                }  
                slice = tokens[1].substr(var.size() + 1);
                slice.pop_back();
                var2 = intListVariables[var].getVal(stoi(slice));
                int var3 = var1 + var2;
                string var3String = to_string(var3);
                string ans = additionHelper(tokens, var3String, 2, stringMap, intMap, stringListVariables, intListVariables);
                return ans;
            }
        }
        if (isNumber(tokens[1]) || intMap.find(tokens[1]) != intMap.end()){ //checking if its just a number to add to a index
            if (isNumber(tokens[1])){
                var2 = stoi(tokens[1]);
            }
            else{
                var2 = intMap[tokens[1]];
            }
            int var3 = var1 + var2;
            string var3String = to_string(var3);
            string ans = additionHelper(tokens, var3String, 2, stringMap, intMap, stringListVariables, intListVariables);
            return ans;
        }
        //repeat but for next index
        else if(intListVariables.find(tokens[1]) != intListVariables.end() || checkTokensForIntList(tokens[1]) || (checkTokensForListSplit(tokens[1]) && checkTokensForIntListVariable(tokens[1], intListVariables)) || (checkTokensForSingleElement(tokens[1]) && checkTokensForIntListVariable(tokens[1], intListVariables))){
                if(intListVariables.find(tokens[1]) != intListVariables.end())
                    var2Head = intListVariables[tokens[1]];
                else if (checkTokensForIntList(tokens[1])){
                    vector<string> list = getListContent(tokens[1]);
                    for (int i = 0; i < list.size(); i++){
                        var2Head.addIntNode(stoi(list[i]));
                    }
                }
            else if (checkTokensForListSplit(tokens[1])){
                string var = "";
                string slice = "";
                int i = 0;
                while (tokens[1][i] != '['){
                    var+= tokens[1][i];
                    i++;
                }
                slice = tokens[1].substr(var.size());
                var1Head = intListVariables[var].getValSlice(slice);
            }
            else{
                string var = "";
                string slice ="";
                int i = 0;
                while (tokens[1][i] != '['){
                    var+= tokens[1][i];
                    i++;
                }
                slice = tokens[1].substr(var.size() + 1);
                slice.pop_back();
                var1 = intListVariables[var].getVal(stoi(slice));
            }
        }

        string temp = "[";
        intNode* curr = var1Head.getHead();
        while(curr != nullptr){
            temp += to_string(curr->val);
            temp += ',';
            curr = curr->next;
        }
        curr = var2Head.getHead();
        while(curr != nullptr){
            temp += to_string(curr->val);
            temp += ',';
            curr = curr->next;
        }
        string ans = additionHelper(tokens, temp, 2, stringMap, intMap, stringListVariables, intListVariables);
        ans.pop_back();
        ans += ']';
        return ans;
    }

    else if(stringListVariables.find(tokens[0]) != stringListVariables.end() || checkTokensForStringList(tokens[0]) || (checkTokensForListSplit(tokens[0]) && checkTokensForStringListVariable(tokens[0], stringListVariables)) || (checkTokensForSingleElement(tokens[0]) && checkTokensForStringListVariable(tokens[0], stringListVariables))){
        stringLinkedList var1StringHead;
        stringLinkedList var2StringHead;
        string var1, var2;
        if(stringListVariables.find(tokens[0]) != stringListVariables.end())
            var1StringHead = stringListVariables[tokens[0]];
        else if (checkTokensForStringList(tokens[0])){
            vector<string> list = getListContent(tokens[0]);
            for (int i = 0; i < list.size(); i++){
                string word = list[i];
                word = word.substr(1, word.size() - 2);
                var1StringHead.addStringNode(word);
            }
        }
        else if (checkTokensForListSplit(tokens[0])){
                string var = "";
                string slice = "";
                int i = 0;
                while (tokens[0][i] != '['){
                    var+= tokens[0][i];
                    i++;
                }
                slice = tokens[0].substr(var.size());
                var1StringHead = stringListVariables[var].getValSlice(slice);
            }
        else{
            string var = "";
            string slice ="";
            int i = 0;
            while (tokens[0][i] != '['){
                var+= tokens[0][i];
                i++;
            }
            slice = tokens[0].substr(var.size() + 1);
            slice.pop_back();
            var1 = stringListVariables[var].getVal(stoi(slice));
            if ((checkTokensForSingleElement(tokens[1])) && (checkTokensForStringListVariable(tokens[1], stringListVariables))){
                var = "";
                slice = "";
                int i = 0;
                while (tokens[1][i] != '['){
                    var+= tokens[1][i];
                    i++;
                }  
                slice = tokens[1].substr(var.size() + 1);
                slice.pop_back();
                var2 = stringListVariables[var].getVal(stoi(slice));
                string var3 = var1 + var2;
                string ans =  "\"";
                ans += additionHelper(tokens, var3, 2, stringMap, intMap, stringListVariables, intListVariables) + "\"";

                return ans;
            }
        }
        if(stringListVariables.find(tokens[1]) != stringListVariables.end() || checkTokensForStringList(tokens[1]) || checkTokensForListSplit(tokens[1])){
            if(stringListVariables.find(tokens[1]) != stringListVariables.end())
                var2StringHead = stringListVariables[tokens[1]];
            else if (checkTokensForStringList(tokens[1])){
                vector<string> list = getListContent(tokens[1]);
                string word = list[0];
                //word = word.substr(1, word.size() - 2);
                for (int i = 0; i < list.size(); i++){
                    string word = list[i];
                    word = word.substr(1, word.size() - 2);
                    var2StringHead.addStringNode(word);
                }
            }
            else{
                string var = "";
                string slice = "";
                int i = 0;
                while (tokens[1][i] != '['){
                    var+= tokens[1][i];
                    i++;
                }
                slice = tokens[1].substr(var.size());
                var2StringHead = stringListVariables[var].getValSlice(slice);
            }
        }
        string temp = "[\"";
        stringNode* curr = var1StringHead.getHead();
        while(curr != nullptr){
            temp += curr->val;
            temp += "\", \"";
            curr = curr->next;
        }
        curr = var2StringHead.getHead();
        while(curr != nullptr){
            temp += curr->val;
            temp += "\", \"";
            curr = curr->next;
        }
        string ans = additionHelper(tokens, temp, 2, stringMap, intMap, stringListVariables, intListVariables);
        ans = ans.substr(0, ans.size() - 3);
        ans += ']';
        return ans;
    }
    
    return "Invalid Data Types";
}

#endif