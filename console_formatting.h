// ******************** PLEASE READ THE DOCUMENTATION BEFORE USING THESE FUNCTIONS *********************

// ABSTRACTION BY: MOHAMMED MAAZ BIN KHAWAR
// if you find any bug or just want to contribute in this open source project
// just contact: maazproductions25@gmail.com or 03162627726

#include <iostream>
#include <windows.h>
#include <ctime>
#include <conio.h>
#include <vector>
using namespace std;

class dateAndTime{

public :
    int year()
    {
        time_t t = time(0);   // get time now
        struct tm * now = localtime( & t );
        return (now->tm_year + 1900);
    }
    string month()
    {
        time_t t = time(0);   // get time now
        struct tm * now = localtime( & t );
        string months[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
        return months[ (now->tm_mon) ];
    }
    string weekDay()
    {
        time_t t = time(0);   // get time now
        struct tm * now = localtime( & t );
        string months[7] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
        return months[ (now->tm_wday) ];
    }
    int day()
    {
        time_t t = time(0);   // get time now
        struct tm * now = localtime( & t );
        return (now->tm_mday);
    }
    int hour()
    {
        time_t t = time(0);   // get time now
        struct tm * now = localtime( & t );
        return now->tm_hour;
    }
    int minute()
    {
        time_t t = time(0);   // get time now
        struct tm * now = localtime( & t );
        return now->tm_min;
    }
    int second()
    {
        time_t t = time(0);   // get time now
        struct tm * now = localtime( & t );
        return now->tm_sec;
    }
    string time12()
    {
    string mode, mint;
    short hr = hour();
    if(hr > 11)
    {
        if(hr!=12)
            hr = hr - 12;
        mode = "PM";
    }
    else
    {
        if(hr == 0)
            hr = 12;
        mode = "AM";
    }
    mint = to_string(minute());

    if(minute() < 10)
        mint = "0" + mint;


    return ( to_string(hr)+":"+mint+" "+mode );
    }

};

string center(string text)
{
    string spacing ="";
    if(text.size()>80)
        text.resize(80);
    spacing.resize( (( 80-text.size() )/2),' ' );
    return spacing + text;
}

string whiteSp(short num)
{
    string wspc = "";
    wspc.resize(num,' ');
    return wspc;
}

int validateInt(bool flag=1)
{
    int inp;
    string str;

    inp = getch();

    do
    {
    while(inp!=13)
    {
        if( (inp == 8)&&(!str.empty()) )
        {
            cout << "\b \b";
            str.pop_back();
        }
        else if (inp == 27)
            return 2147483647;
        else if(str.size()==10)
        {
            if(flag==1)
                cout << '\a';
        }
        else if( (inp>47 && inp<58) || (inp == 45 && str.empty()) )
        {
            str = str + (char)inp;
            cout << (char)inp;
        }
        else
        {
            if(flag==1)
                cout << '\a';
        }
        inp = getch();
    }

    if( !str.empty() && str!="-" && stod(str)<2147483647)
            return stoi(str);

    inp = getch();

    }//end do
    while(true);

}

int clrStrToCode(string clrName)
{
    int clrCode;
    string clrArr[] = {"Black","Blue","Green","Cyan","Red","Purple","Yellow","White","Gray","L Blue","L Green",
                        "L Cyan", "L Red", "L Purple", "L Yellow", "Bright White"};

    for(clrCode = 0; clrCode<16; ++clrCode)
    {
        if(clrName == clrArr[clrCode])
            return clrCode;
    }

    return -1;
}

string clrCodeToStr(int clrCode)
{
    string clrArr[] = {"Black","Blue","Green","Cyan","Red","Purple","Yellow","White","Gray","L Blue","L Green",
                        "L Cyan", "L Red", "L Purple", "L Yellow", "Bright White"};

    return clrArr[clrCode];
}

void setConsoleDimensions(short width, short height)
{
    SMALL_RECT r;
    HANDLE hOut = GetStdHandle( STD_OUTPUT_HANDLE );
    r.Left   = r.Top    = 0;
    r.Right  = width -1;
    r.Bottom = height -1;
    SetConsoleWindowInfo( hOut, TRUE, &r );
    SetConsoleScreenBufferSize(hOut, {width, 300});
}

int getConsoleWidth()
{
    CONSOLE_SCREEN_BUFFER_INFO cns;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cns);
    return (cns.srWindow.Right - cns.srWindow.Left);
}

int getConsoleHeight()
{
    CONSOLE_SCREEN_BUFFER_INFO cns;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cns);
    return (cns.srWindow.Bottom - cns.srWindow.Top);
}

void setConsoleScrollBarVisibility(bool vis)
{
    HANDLE hOut;
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if(vis)
        SetConsoleScreenBufferSize(hOut, {getConsoleWidth()+1, 300});
    else
        SetConsoleScreenBufferSize(hOut, {getConsoleWidth()+1, getConsoleHeight()+1});
}


int getCurrentClrCode(){
        CONSOLE_SCREEN_BUFFER_INFO info;
        GetConsoleScreenBufferInfo( GetStdHandle(STD_OUTPUT_HANDLE), &info );
        return info.wAttributes;
}

int getPixelColor(short x, short y)
{
    HANDLE stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD rCords = {x,y};
    DWORD written = 0;
    WORD rAttr;

    ReadConsoleOutputAttribute(stdout_handle, &rAttr, 1, rCords, &written);
    return rAttr/16;
}

bool setBgClr(string BgClrName, bool whole=false)
{
    //Declarations :
    HANDLE stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD rCords = {0,0}, wCords;
    DWORD written = 0;
    short x,y, futureClr;
    int len;
    if(whole)
        len = (getConsoleWidth()+1)*300; //for coloring whole console
    else
        len = (getConsoleHeight()+1)*(getConsoleWidth()+1); //for coloring only visible screen
    vector<WORD> rAttr(len*sizeof(WORD));
    WORD wBgClr, wFgClr;

    //validating color name and assigning it's code to wBgClr
    if( clrStrToCode(BgClrName)>-1 )
        wBgClr = clrStrToCode(BgClrName);
    else
        return false; //Invalid background color name! So retaining previous background color.

    //saving attributes of entire console screen
    ReadConsoleOutputAttribute(stdout_handle, &rAttr[0], len, rCords, &written);

    //checking cell by cell to change the background color of each cell
    for(int i=0; i<len; ++i)
    {
        x = i%80;
        y = i/80;
        wCords = {x,y};

        if( rAttr[i] % 17 )
        {
            wFgClr = (wBgClr*16) + (rAttr[i]%16);
            WriteConsoleOutputAttribute(stdout_handle, &wFgClr, 1, wCords, &written);
        }
        else
        {
            WriteConsoleOutputAttribute(stdout_handle, &wBgClr, 1, wCords, &written);
        }
    }

    //setting future colors
    futureClr = (wBgClr*16) + (getCurrentClrCode()%16);
    SetConsoleTextAttribute(stdout_handle,futureClr);

    return true; //confirm that the background color has changed
}

bool setFgClr(string FgClrName, string BgClrName = " ")
{
    short newFgClr;

    if( (clrStrToCode(FgClrName)== -1)&&(clrStrToCode(BgClrName)== -1) )
        return false; // if color name is invalid then retain previous foreground color, new color can't be set!
    else if( clrStrToCode(BgClrName)== -1 )
        newFgClr = (getCurrentClrCode()/16)*16 + clrStrToCode(FgClrName);
    else if( clrStrToCode(FgClrName)== -1 )
        newFgClr = (clrStrToCode(BgClrName)*16) + (getCurrentClrCode()%16);
    else
        newFgClr = (clrStrToCode(BgClrName)*16) + (clrStrToCode(FgClrName));

    //setting new FG color.
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), newFgClr);
    return true; //confirming the color has been set.
}

void colorPixel(short x, short y, string bgClrName, string fgClrName="White")
{
    short /*fgCode, bgCode,*/ clrCode;

    clrCode = (clrStrToCode(bgClrName)*16)+clrStrToCode(fgClrName);

    HANDLE stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coords = {x,y};
    DWORD written = 0;
    WORD Attr = clrCode;

    WriteConsoleOutputAttribute(stdout_handle, &Attr, 1, coords, &written);
}

void colorArea(string fgClrName, short x, short y, int units, string bgClrName = " ")
{
    short fgCode, bgCode, clrCode;

    if( (clrStrToCode(fgClrName) == -1)&&(clrStrToCode(bgClrName) == -1) || units<=0 )
        return;
    else if( clrStrToCode(bgClrName) == -1 )
        clrCode = (getCurrentClrCode()/16)*16 + clrStrToCode(fgClrName);
    else if( clrStrToCode(fgClrName) == -1 )
        clrCode = (clrStrToCode(bgClrName)*16) + (getCurrentClrCode()%16);
    else
        clrCode = (clrStrToCode(bgClrName)*16) + (clrStrToCode(fgClrName));

    HANDLE stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coords = {x,y};
    DWORD written = 0;
    vector<WORD> Attr(units*sizeof(WORD),clrCode);

    WriteConsoleOutputAttribute(stdout_handle, &Attr[0], units, coords, &written);
}


void Locate(short x, short y) {
    COORD pos = {x, y};
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(output, pos);
}

bool getCursorVisibility(void)
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;

    GetConsoleCursorInfo(h, &info);
    return info.bVisible;
}

short getCursorSize(void)
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;

    GetConsoleCursorInfo(h, &info);
    return info.dwSize;
}

void cursorVisAndSize(bool visibility, short len = -1)
{
    if(len<1 || len>100)
    {
        len = getCursorSize(); //don't change length
    }
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = len;
    info.bVisible = visibility;
    SetConsoleCursorInfo(consoleHandle, &info);
}

void WriteTextAtLoc(string text, short x, short y)
{
    HANDLE stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD written = 0;
    int len = text.size();
    COORD cords = {x,y};

    WriteConsoleOutputCharacter(stdout_handle,&text[0],len,cords,&written);
}

void clrdLine(string clr, short x1, short y1, short x2, short y2)
{
    short leastX, leastY;
    if(x1<x2)
        leastX = x1;
    else
        leastX = x2;
    if(y1<y2)
        leastY = y1;
    else
        leastY = y2;

    for(int i=0; i<abs(y2-y1); ++i)
        colorArea( clr,leastX,leastY+i,abs(x2-x1),clr );
}

struct _location
    {
        short x;
        short y;
    };

class Menu
{
private:
    vector<string> menu;
    string heading="";
    string menuClr="L Cyan", headingClr="L Red", selectedClr="L Yellow";
    struct _location menuPos = {34, 5};
    struct _location headingPos = {-1,1};
    short y_spacing = 2;
    bool ptr_visible = true, c_scr = false;

public:
    Menu(vector<string> userMenu)
    {
        menu = userMenu;
    }

    void setMenuColor(string m_clr)
    {
        menuClr = m_clr;
    }

    void setMenuPosition(short x, short y)
    {
        if( x<2 || x>getConsoleWidth() || y<0 || y>getConsoleHeight() )
            return;
        menuPos = {x,y};
    }

    void setHeading(string _heading)
    {
        if(_heading.size()>(getConsoleWidth()-headingPos.x+1))
            heading.resize(getConsoleWidth()-headingPos.x+1);
        heading = _heading;
    }

    void setHeadingColor(string h_clr)
    {
        headingClr = h_clr;
    }

    void setHeadingPosition(short x, short y)
    {
        if( x<0 || x>getConsoleWidth() || y<0 || y>getConsoleHeight() )
            return;
        headingPos = {x,y};
    }

    void setSelectionColor(string s_clr)
    {
        selectedClr = s_clr;
    }

    void setVerticalSpacing(short sp)
    {
        if(sp<1 || sp>5)
            return;
        y_spacing = sp;
    }

    void showPointer(bool p)
    {
        ptr_visible = p;
    }

    void continousScrolling(bool scr)
    {
        c_scr = scr;
    }

    int drawMenu(void)
    {
        //get cursor's current state before hiding
        bool prevCursorVis = getCursorVisibility();
        short prevCursorSize = getCursorSize();
        cursorVisAndSize(0); //hide cursor

        //displaying heading
        if(headingPos.x == -1)
        {
            Locate((getConsoleWidth()-heading.size())/2, headingPos.y);
            cout << heading;
            colorArea(headingClr, (getConsoleWidth()-heading.size())/2, headingPos.y, heading.size());
        }
        else
        {
            Locate(headingPos.x, headingPos.y);
            cout << heading;
            colorArea(headingClr, headingPos.x, headingPos.y, heading.size());
        }

        //displaying menus
        for(int i=0; i<menu.size(); ++i)
        {
            Locate(menuPos.x, menuPos.y+y_spacing*i);
            cout << menu[i];
            colorArea(menuClr, menuPos.x, menuPos.y+y_spacing*i, menu[i].size());
        }

        struct _location prevCursor, newCursor{menuPos.x-2,menuPos.y};
        int i=0;
        string ch;
        if(ptr_visible)
            ch = (char)175;
        else
            ch=" ";
        //showing first arrow indicator
        WriteTextAtLoc(ch,newCursor.x, newCursor.y);
        colorArea(selectedClr, newCursor.x, newCursor.y, 2+menu[i].size());

    keyPressWaiting:
        switch(getch())
        {
        case 72: //Up
            if(i==0)
            {
                if(c_scr)
                {
                    i=menu.size()-1;
                    prevCursor = newCursor;
                    newCursor.y = newCursor.y+i*y_spacing;
                    colorArea(menuClr, prevCursor.x, prevCursor.y, 2+menu[0].size()); // de-color previous menu
                }
                else
                    goto keyPressWaiting;
            }
            else
            {
                prevCursor = newCursor;
                newCursor.y = newCursor.y-y_spacing;
                --i;
                colorArea(menuClr, prevCursor.x, prevCursor.y, 2+menu[i+1].size()); // de-color previous menu
            }
            break;

        case 80: //Down
            if(i==menu.size()-1)
            {
                if(c_scr)
                {
                    prevCursor = newCursor;
                    newCursor.y = newCursor.y-i*y_spacing;
                    colorArea(menuClr, prevCursor.x, prevCursor.y, 2+menu[i].size()); // de-color previous menu
                    i=0;
                }
                else
                    goto keyPressWaiting;
            }
            else
            {
                prevCursor = newCursor;
                newCursor.y = newCursor.y+y_spacing;
                ++i;
                colorArea(menuClr, prevCursor.x, prevCursor.y, 2+menu[i-1].size()); // de-color previous menu
            }
            break;

        case '\r':
            cursorVisAndSize(prevCursorVis, prevCursorSize); //get cursor to it's previous state before returning
            return (i+1);
        }
        //cout << "x: " << prevCursor.x << "  y: " << prevCursor.y << endl;
        WriteTextAtLoc(ch,newCursor.x, newCursor.y); //new cursor position
        WriteTextAtLoc(" ",prevCursor.x, prevCursor.y); //remove previous cursor
        colorArea(selectedClr, newCursor.x, newCursor.y, 2+menu[i].size()); //color selected area
        goto keyPressWaiting;
    }
};




