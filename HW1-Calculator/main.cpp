#include <iostream>
#include <string>

using namespace std;

const char number = '8';
const char quit = 'q';
const char print = ';';

const string prompt = ">";
const string result = "=";


struct Token{
    char kind;
    double value;
};

class Token_stream{
public:
    Token get();
    void putback(Token);
private:
    bool full{false};
    Token buffer;
};

Token Token_stream::get(){
    if(full){
        full = false;
        return buffer;
    }
    char ch;
    cin>>ch;
    switch(ch){
        case '(':
        case ')':
        case ';':
        case 'q':
        case '+':
        case '-':
        case '*':
        case '/':
            return Token{ch};
        case '.':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            cin.putback(ch);
            double val;
            cin>>val;
            return Token{number,val};
        default:
            perror("Bad Token");
    }
}

void Token_stream::putback(Token t){
    if(full) perror("putback() into a full buffer");
    buffer = t;
    full = true;
}

Token_stream ts;
double expression();
double term();
double primary();
double expression(){
    double left = term();
    while (true){
        Token t = ts.get();
        switch(t.kind){
            case '+': left += term(); break;
            case '-': left -= term(); break;
            default : 
                ts.putback(t);
                return left;
        }
    }
}
double term(){
    double left = primary();
    double d;
    while (true){
        Token t = ts.get();
        switch(t.kind){
            case '*':
                left *=primary();
                break;
            case '/':
                d=primary();
                if(d==0) perror("divided by zero");
                left /=d;
                break;
            default:
                ts.putback(t);
                return left;
        }
    }
}
double primary(){
    Token t = ts.get();
    double d;
    switch (t.kind){
        case '(':
            d = expression();
            t = ts.get();
            if(t.kind != ')') perror("')' expected!");
            return d;
        case '8':
            return t.value;
        default:
            perror("primary expected!");
    }
}


void calculate(){
    while(cin){
        cout<<prompt;
        Token t = ts.get();
        while(t.kind == print) t=ts.get();
        if(t.kind == quit) return;
        ts.putback(t);
        cout<<result<<expression()<<endl;
    }
}


int main(){
    calculate();
    return 0;
}