#ifndef CONDITIONS_H
#define CONDITIONS_H

#include <sstream>
#include <vector>
#include <regex>
#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>
using namespace std;

vector<string> getConditionTokens(string eq){ //String + "One + One"
    string temp = "";
    vector<string> tokens;

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
       else if (eq[i] == '<'){
           tokens.push_back(temp);
           temp = "";
            if (eq[i + 1] == '='){
            tokens.push_back("<=");
            i++;
            continue;
           }
            else{
                tokens.push_back("<");
                continue;
            }
       }
        else if (eq[i] == '>'){
            tokens.push_back(temp);
            temp = "";
            if (eq[i + 1] == '='){
            tokens.push_back(">=");
            i++;
            continue;
           }
            else{
                tokens.push_back(">");
                continue;
            }
       }
        else if (eq[i] == '='){
            tokens.push_back(temp);
            temp = "";
            if (eq[i + 1] == '='){
                i++;
                tokens.push_back("==");
                continue;
           }
       }
        else if (eq[i] == '!'){
            tokens.push_back(temp);
            temp = "";
            if (eq[i + 1] == '='){
                i++;
                tokens.push_back("!=");
                continue;
           }
       }
       temp += eq[i];
   }
   if (temp != "")
    tokens.push_back(temp);
    return tokens;
}

bool intCondition(int tok1, string op, int tok2){
    if (op == "=="){
        if (tok1 == tok2) return true;
        else return false;
    }
    else if (op == "!="){
        if (tok1 != tok2) return true;
        else return false;
    }
    else if(op == ">="){
        if (tok1 >= tok2) return true;
        else return false;
    }
    else if(op == "<="){
        if (tok1 <= tok2) return true;
        else return false;
    }
    else if (op[0] == '<'){
        if (tok1 < tok2) return true;
        else return false;
    }
    else if (op[0] == '>'){
        if (tok1 > tok2) return true;
        else return false;
    }
    return false;
}

bool stringCondition(string tok1, string op, string tok2){
    if (op == "=="){
        if (tok1 == tok2) return true;
        else return false;
    }
    else if (op == "!="){
        if (tok1 != tok2) return true;
        else return false;
    }
    else if (op[0] == '<'){
        if (tok1 < tok2) return true;
        else return false;
    }
    else if(op == "<="){
        if (tok1 <= tok2) return true;
        else return false;
    }
    else if (op[0] == '>'){
        if (tok1 > tok2) return true;
        else return false;
    }
    else if(op == ">="){
        if (tok1 >= tok2) return true;
        else return false;
    }
    return false;
}

string getBlock(string input){
    stringstream ss(input);
    string line = "";
    string block = "";

    getline(ss, line, '\n');
    int i = 0;
    string whitespace = "";
    while(line[i] == ' ' || line[i] == '\t'){
        whitespace += line[i];
        i++;
    }
    block += line + '\n';

    while(getline(ss, line, '\n')){
        if(whitespace == line.substr(0, whitespace.size())){
            block += line + '\n';
        }
        else break;
    }
    return block;
}

int findExpression(vector<string> tokens){
    for (int i = 0; i < tokens.size(); i++){
        if (tokens[i][0] == '<' || tokens[i][0] == '>' || tokens[i] == "<=" || tokens[i] == "==" || tokens[i] == "!=")
            return i;
    }
    return -1;
}


#endif