#include<ctype.h>
#include<stdio.h>
#include<stdlib.h>
#include <termios.h>
#include <unistd.h>
#include<iostream>
using namespace std;

struct termios orig_termios;

void disableRawMode(){
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

//traditionally, our terminal works in canonical mode. It interprets certain keystrokes in a particular way(like upward //arrow). We need to disable that.So we enable raw mode so that all keystrokes go straight to our program without preprocessing. //we want to clear the echo bit right, so thats what we are doing.

void enableRawMode(){
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disableRawMode); //at_exit
    struct termios raw = orig_termios;
    tcgetattr(STDIN_FILENO,&raw);

    raw.c_lflag &= ~(ECHO | ICANON); 
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

}

int main(){

    enableRawMode();

    char c;
    while(read(STDIN_FILENO, &c,1)==1 && c !='q');{
        if(iscntrl(c)){
            cout<<c<<endl;
        }
        else{
            cout<<"("<<to_int(c)<<")";
        }
    }
    return 0;
}