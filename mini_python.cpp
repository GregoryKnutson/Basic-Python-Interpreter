#include <fstream>
#include <cstdlib>
#include <regex>
#include <string>
#include <sstream>
#include <iostream>
#include "linkedLists.h"
#include "conditions.h"
#include "addition.h"
#include <unordered_map>
using namespace std;

unordered_map<string, string> stringVariables;
unordered_map<string, int> integerVariables;
unordered_map<string, stringLinkedList> stringListVariables;
unordered_map<string, intLinkedList> intListVariables;    

//a     =       "This is a sentence"
string squeezeBack(string var){
    string newVar = "";
    bool inQ = false;
    
    for(int i = 0; i < var.size(); i++){
        if(var[i] == '"')
            inQ = !inQ;
        if(!inQ && (var[i] == ' ' || var[i] == '\t' || var[i] == '\n'))
            continue;
        newVar += var[i]; 
    }
   return newVar;
}


//skip comment
void handle_comment(int& num, string input){
    while(num < input.size() && input[num] != '\n')
        num++;
    
    return;
}

//print
void handle_print(string p){
    if (stringVariables.find(p) != stringVariables.end())
        cout << stringVariables[p] << endl;
    else if (integerVariables.find(p) != integerVariables.end())
        cout << integerVariables[p] << endl;
    else if (intListVariables.find(p) != intListVariables.end())
        cout << intListVariables[p].print() << endl;
    else if (stringListVariables.find(p) != stringListVariables.end())
        cout << stringListVariables[p].print() << endl;

    else if (p[0] == '"' && p[p.size() - 1] == '"')
        {
            string temp = p.substr(1, p.size() - 2);
            cout << p << endl;
        }
    else if(isNumber(p))
        cout << p << endl;
    
    else cout << "error" << endl;
}


void handle_assignment(string subInput){
    string varName = "";
    smatch matches;
    regex assignmentReg("^([a-zA-Z0-9]+)\\s*={1}");
    subInput = squeezeBack(subInput);
    regex_search(subInput, matches, assignmentReg);
    varName = matches[0].str();
    varName.pop_back();

    string varContent = subInput.substr(matches[0].str().size());

    if (varContent[0] == '"' && varContent[varContent.size() - 1] == '"') //string assignment
        {
            varContent = varContent.substr(1, varContent.size() - 2);
            stringVariables[varName] = varContent;
            return;
        }

    else if(isdigit(varContent[0])){ //num assignment
        int temp = stoi(varContent);
        integerVariables[varName] = temp;
        return;
    } //int var copy
    else if(integerVariables.find(varContent) != integerVariables.end()){
        integerVariables[varName] = integerVariables[varContent];
        return;
    } //string var copy
    else if(stringVariables.find(varContent) != stringVariables.end()){
        stringVariables[varName] = stringVariables[varContent];
        return;
    } //list assignment
    else if (varContent[0] == '[' && varContent[varContent.size() - 1] == ']'){
        vector<string> v = getListContent(varContent);
        string test = v[0];

        if (test[0] == '"' && test[test.size() - 1] == '"'){
            stringLinkedList head;
            for (int i = 0; i < v.size(); i++){
                head.addStringNode(v[i]);
            }
            stringListVariables[varName] = head;
            return;
        }

        else if (isdigit(test[0])){
            intLinkedList head;
            for (int i = 0; i < v.size(); i++){
                head.addIntNode(stoi(v[i]));
            }
            intListVariables[varName] = head;
            return;
        }

    }
    regex listAssignment("([a-zA-Z0-9]+)\\[\\d+\\]");
    smatch match2;
    regex_search(subInput, match2, listAssignment);
    if(match2.size() > 0){
        string num = match2[0].str();
        size_t pos = num.find('[');
        string list = num.substr(0, pos);
        num.erase(0, pos + 1);
        num.pop_back();
        
        //check if its a list
        if(stringListVariables.find(list) != stringListVariables.end()){
            int size = stringListVariables[list].getSize() - 1;
            if (size < stoi(num)) return;
            varContent = stringListVariables[list].getVal(stoi(num));
            stringVariables[varName] = varContent;
        }
        else if(intListVariables.find(list) != intListVariables.end()){
            int size = intListVariables[list].getSize() - 1;
            if (size < stoi(num)) return;
            int v = intListVariables[list].getVal(stoi(num));
            integerVariables[varName] = v;
        }

    }
    else if(checkTokensForListSplit(varContent)){ //check for split list[1:]
        string var = "";
        string slice = "";
        int i = 0;
        while (varContent[i] != '['){
            var+= varContent[i];
            i++;
            }
        slice = varContent.substr(var.size());
        if(intListVariables.find(var)!=intListVariables.end()){
            intLinkedList l = intListVariables[var].getValSlice(slice);
            intListVariables[varName] = l;
        }
        else if(stringListVariables.find(var)!=stringListVariables.end()){
            stringLinkedList l = stringListVariables[var].getValSlice(slice);
            stringListVariables[varName] = l;
        }
    }
    else{ //error
        cout << "error";
        exit(0);
    }
    return;
}

int main(int argc, char** argv){ 
    
    string filename = argv[1];
    ifstream inputFile(filename);
    string input;

    while(!inputFile.eof())
    {
        string temp = "";
        getline(inputFile, temp);
        input = input + temp + "\n";
    }
    regex assignmentReg("([a-zA-Z0-9]+)\\s*={1}");

    //variables["a"] = "2";

    bool newLine = true;
    for(int charNum = 0; charNum < input.size(); charNum++)
    {
        string sub;
        int pos;
        if (charNum > pos && newLine == false)
            newLine = true;

        if (input[charNum] == '\n'){ //handle new line
            continue;
        }

        if (newLine == true)
        {
            pos = input.find_first_of('\n', charNum);
            sub = input.substr(charNum, pos - charNum);
            newLine = false;
        }

        if (input[charNum] == '#') //comments
        {
            handle_comment(charNum, input);
            continue;
        }
                //check for conditions
        smatch match;
        regex ifConditional("if|IF");
        regex elseConditional("else|ELSE");
        regex_search(sub, match, ifConditional);
        if(match.size() > 0){
            charNum += sub.size();
            sub = squeezeBack(sub);
            sub = sub.substr(2, sub.size() - 3);
            vector<string> toks = getConditionTokens(sub);
            string block = getBlock(input.substr(charNum + 1, input.size()));
            int opPos = findExpression(toks);
            if (opPos != -1){
                if(integerVariables.find(toks[opPos - 1]) != integerVariables.end() || isNumber(toks[opPos - 1])){
                    int var1;
                    int var2;
                    if(integerVariables.find(toks[opPos - 1]) != integerVariables.end())
                        var1 = integerVariables[toks[opPos - 1]];
                    else 
                        var1 = stoi(toks[opPos - 1]);
                    if(integerVariables.find(toks[opPos + 1]) != integerVariables.end())
                        var2 = integerVariables[toks[opPos + 1]];
                    else 
                        var2 = stoi(toks[opPos + 1]);
                    if(intCondition(var1, toks[opPos], var2))
                    {
                        continue;
                    }
                    else{
                        charNum += block.size() + 1; //skip block

                    
                        while(input[charNum] == ' ' || input[charNum] == '\n')
                            charNum++;
                    

                        size_t nextLine = input.find_first_of('\n', charNum);
                        sub = input.substr(charNum, nextLine - charNum);
                        smatch match2;
                        regex_search(sub, match2, elseConditional);
                        if(match2.size() > 0){
                            charNum += sub.size();
                            continue;
                        }
                    }
                }

                else if(stringVariables.find(toks[opPos - 1]) != stringVariables.end() || (toks[opPos][0] == '"' && toks[opPos + 1][toks[opPos].size() - 1] == '"')){
                    string var1;
                    string var2;
                    if(stringVariables.find(toks[opPos - 1]) != stringVariables.end())
                        var1 = stringVariables[toks[opPos - 1]];
                    else 
                        var1 = toks[opPos - 1];
                    if(stringVariables.find(toks[opPos + 1]) != stringVariables.end())
                        var2 = stringVariables[toks[opPos + 1]];
                    else 
                        var2 = toks[opPos + 1];
                    if(stringCondition(var1, toks[opPos], var2))
                    {
                        continue;
                    }
                    else{
                        charNum += block.size() + 1; //skip block
                    
                        while(input[charNum] == ' ' || input[charNum] == '\n')
                            charNum++;
                    
                        size_t nextLine = input.find_first_of('\n', charNum);
                        sub = input.substr(charNum, nextLine - charNum);
                        smatch match2;
                        regex_search(sub, match2, elseConditional);
                        if(match2.size() > 0){
                            charNum += sub.size();
                            continue;
                        }
                    }
                }
            }
        }
        //skip else
        regex_search(sub, match, elseConditional);
        if(match.size() > 0){
            charNum += sub.size();
            string block = getBlock(input.substr(charNum + 1, input.size()));
            charNum += block.size();
        }
            regex_search(sub, match, assignmentReg);
            string leftSide = match[0].str();
            //\\[(\\s*\\S*,?\\s*)+
            if (match.size() > 0)
            {
                charNum += sub.size();
                //check for addition of anything
                regex additionAssignment("(([a-zA-Z0-9]+\\[[0-9]+\\]|[a-zA-Z0-9]+\\[:[0-9]+\\]|[a-zA-Z0-9]+\\[[0-9]+:\\]|[a-zA-Z0-9]+\\[[0-9]+:[0-9]+\\])|\\(*[a-zA-Z0-9]+\\)*|\x22[a-zA-Z0-9\\s\\W]+\x22|\\(*\\d+\\)*|\\[(\\s*(\x22[a-zA-Z0-9\\s\\W]+\x22|\\d+)+\\s*,?)+\\])\\s*\\+\\s*(([a-zA-Z0-9]+\\[[0-9]+\\]|[a-zA-Z0-9]+\\[:[0-9]+\\]|[a-zA-Z0-9]+\\[[0-9]+:\\]|[a-zA-Z0-9]+\\[[0-9]+:[0-9]+\\])|\\(*[a-zA-Z0-9]+\\)*|\x22[a-zA-Z0-9\\s\\W]+\\(*\x22\\)*|\\d+|\\[(\\s*\\S+\\s*,?)+\\])(\\s*\\+\\s*(([a-zA-Z0-9]+\\[[0-9]+\\]|[a-zA-Z0-9]+\\[:[0-9]+\\]|[a-zA-Z0-9]+\\[[0-9]+:\\]|[a-zA-Z0-9]+\\[[0-9]+:[0-9]+\\])|\\(*[a-zA-Z0-9]+\\)*|\x22[a-zA-Z0-9\\s\\W]+\x22|\\(*\\d+\\)*|\\[(\\s*\\S+\\s*,?)+\\]))*");
                smatch match2;
                regex_search(sub, match2, additionAssignment);
                string temp;
                if(match2.size() > 0){
                    temp = add(squeezeBack(match2[0].str()), stringVariables, integerVariables, stringListVariables, intListVariables);
                    if (temp != "Invalid Data Types")
                        handle_assignment(leftSide+temp);
                    continue;
                }
                //string test2 = getValue(sub, stringVariables, integerVariables);
                handle_assignment(sub);
                continue;
            }
            //check for print
        regex printAssingment("(print\\s*\\()");
        regex_search(sub, match, printAssingment);
        if(match.size() > 0){
            size_t pos = sub.find('(');
            charNum += pos + 1;
            string temp = "";
            while (input[charNum] != ')'){
                temp += input[charNum];
                charNum++;
            }
            charNum++;
            handle_print(temp);
        }
    }

    return 0;
}
