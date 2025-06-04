/*** INCLUDES ***/

#include<ctype.h>
#include<stdio.h>
#include<stdlib.h>
#include <termios.h>
#include <unistd.h>
#include<iostream>
#include<error.h>
using namespace std;

/*** DATA ***/
struct termios orig_termios;

/*** TERMINAL ***/

//a die function that prints the error message and exits. Most C library functions that fail will set the global errno variable to indicate what the error was. perror() looks at the global errno variable and prints a descriptive error message for it. Exit with status 1 means failure, with 0 means success.

void die( const char *s) {
    perror(s);
    exit(1);
}

void disableRawMode(){
    if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1){
        die("tcsetattr");
    }
}

//traditionally, our terminal works in canonical mode. It interprets certain keystrokes in a particular way(like upward //arrow). We need to disable that.So we enable raw mode so that all keystrokes go straight to our program without preprocessing. //we want to clear the echo bit right, so thats what we are doing.

void enableRawMode() {
  if(tcgetattr(STDIN_FILENO, &orig_termios) == -1){
    die("tcgetattr");
  }

  atexit(disableRawMode); //no braces as atexit takes a fxn pointer (add braces itself)
  struct termios raw = orig_termios;
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  
  

  //this is basically minimum time before which read function shall return
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;

  if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1){
    die("tcsetattr");
  }
}

/*** INIT ***/

int main(){

    enableRawMode();

    //iscntrl() tests whether a character is a control character. Control characters are nonprintable characters that we don’t want to print to the screen

    while(1){
        char c = '\0';
        if(read(STDIN_FILENO, &c,1) == -1 && errno!= EAGAIN){
            die("read");
        }

        if(iscntrl(c)){
           // cout<<int(c)<<endl;
           printf("%d\r\n", c);
        }
        else{
             //cout << c << " (" << (int)c << ")" << endl;
             printf("%d ('%c')\r\n", c, c);
        }
        if(c=='q'){
            break;
        }
    }
    return 0;
}