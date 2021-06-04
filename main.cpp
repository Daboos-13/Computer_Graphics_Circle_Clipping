#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include<iostream>

using namespace std;

#include<math.h>

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain(HINSTANCE hThisInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpszArgument,
                   int nCmdShow) {
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

/* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof(WNDCLASSEX);

/* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
/* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

/* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx(&wincl))
        return 0;

/* The class is registered, let's create the program*/
    hwnd = CreateWindowEx(
            0,                   /* Extended possibilites for variation */
            szClassName,         /* Classname */
            _T("Code::Blocks Template Windows App"),       /* Title Text */
            WS_OVERLAPPEDWINDOW, /* default window */
            CW_USEDEFAULT,       /* Windows decides the position */
            CW_USEDEFAULT,       /* where the window ends up on the screen */
            544,                 /* The programs width */
            375,                 /* and height in pixels */
            HWND_DESKTOP,        /* The window is a child-window to desktop */
            NULL,                /* No menu */
            hThisInstance,       /* Program Instance handler */
            NULL                 /* No Window Creation data */
    );

/* Make the window visible on the screen */
    ShowWindow(hwnd, nCmdShow);

/* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage(&messages, NULL, 0, 0)) {
/* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
/* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

/* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}

//Clipping

int clipRadius; //the radius of the clipping circle
int clipXC, clipYC;  //center point of the clipping Circle
void isInsideClip(HDC hdc, int x, int y) {
    int dist = sqrt(pow((x - clipXC), 2) +
                    pow((y - clipYC), 2)); //calculating distance between point (x,y)and clipping circle center
    if (dist <= clipRadius) {
        SetPixel(hdc, x, y, RGB(1, 1, 255));
        //If distance is less than circle radius, then the points is within the circle clip
    } else {
        SetPixel(hdc, x, y, RGB(255, 1, 1));
    }
}

//Parametric line

void parametricline(HDC hdc,int x1,int y1,int x2,int y2){
    double dx=x2-x1;
    double dy=y2-y1;
    for(double t=0;t<1;t+=0.001){
        int x=x1+(dx*t);
        int y=y1+(dy*t);
        isInsideClip(hdc,x,y);
    }

}

//For circle clipping

void draweight(HDC hdc, int x, int y, int xc, int yc) {
    SetPixel(hdc, xc + x, yc + y, RGB(100, 0, 0));
    SetPixel(hdc, xc - x, yc + y, RGB(100, 0, 0));
    SetPixel(hdc, xc + x, yc - y, RGB(100, 0, 0));
    SetPixel(hdc, xc - x, yc - y, RGB(100, 0, 0));

    SetPixel(hdc, xc - y, yc + x, RGB(100, 0, 0));
    SetPixel(hdc, xc + y, yc - x, RGB(100, 0, 0));
    SetPixel(hdc, xc + y, yc + x, RGB(100, 0, 0));
    SetPixel(hdc, xc - y, yc - x, RGB(100, 0, 0));
}

//Circle Clipping

void midpoint(HDC hdc, int xc, int yc, int r) {
    int x = 0;
    int y = r;
    double d = 1 - r;
    while (x < y) {
        if (d <= 0) {
            d = d + 2 * x + 3;
            x++;
        } else {
            d = d + 2 * (x - y) + 5;
            x++;
            y--;
        }
        draweight(hdc, x, y, xc, yc);
    }
}

//For Half Circle

void drawTwo(HDC hdc,int x,int y,int xc,int yc)
{
    isInsideClip(hdc,xc+x,yc+y);
    isInsideClip(hdc,xc-x,yc+y);

    isInsideClip(hdc,xc-y,yc+x);
    isInsideClip(hdc,xc+y,yc+x);

}

//Half Circle

void midpointHalf(HDC hdc,int xc,int yc,int r)
{
    int x=0;
    int y=r;
    double d=1-r;
    while(x<y)
    {
        if(d<=0)
        {
            d=d+2*x+3;
            x++;
        }
        else
        {
            d=d+2*(x-y)+5;
            x++;
            y--;
        }
        drawTwo(hdc,x,y,xc,yc);
    }
}

//For small circle

void DrawSmall(HDC hdc, int x, int y, int xc, int yc) {
    {
        isInsideClip(hdc, xc + x, yc + y);
        isInsideClip(hdc, xc - x, yc + y);
        isInsideClip(hdc, xc + x, yc - y);
        isInsideClip(hdc, xc - x, yc - y);

        isInsideClip(hdc, xc - y, yc + x);
        isInsideClip(hdc, xc + y, yc - x);
        isInsideClip(hdc, xc + y, yc + x);
        isInsideClip(hdc, xc - y, yc - x);
    }
}

//Circle Small

void Draw(HDC hdc, int xc, int yc, int r) {
    int x = 0;
    int y = r;
    double d = 1 - r;
    while (x < y) {
        if (d <= 0) {
            d = d + 2 * x + 3;
            x++;
        } else {
            d = d + 2 * (x - y) + 5;
            x++;
            y--;
        }
        DrawSmall(hdc, x, y, xc, yc);
    }
}

/*  This function is called by the Windows function DispatchMessage()  */

int rr, X_c, Y_c, X, Y, X_1, Y_1, X_3, Y_3,x1_line, y1_line, x2_line, y2_line, x3_line, y3_line, x_1, y_1, x_2, y_2 ;
int counter = 1;

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    HDC hdc = GetDC(hwnd);
    switch (message)                  /* handle the messages */
    {
        case WM_LBUTTONDOWN:
            if (counter == 1) {
                X_c = LOWORD(lParam);
                Y_c = HIWORD(lParam);
                counter++;
            } else if (counter == 2) {
                X = LOWORD(lParam);
                Y = HIWORD(lParam);
                rr = sqrt(pow((X_c - X), 2) + pow((Y_c - Y), 2));
                clipRadius = rr;
                clipXC = X_c;
                clipYC = Y_c;
                midpoint(hdc, X_c, Y_c, rr);// point circle 0 and r (original)
                counter++;
            } else if (counter == 3) {
                hdc = GetDC(hwnd);
                X_1 = LOWORD(lParam);
                Y_1 = HIWORD(lParam);
                counter++;
            } else if (counter == 4) {
                hdc = GetDC(hwnd);
                X_3 = LOWORD(lParam);
                Y_3 = HIWORD(lParam);
                rr = sqrt(pow((X_3 - X_1), 2) + pow((Y_3 - Y_1), 2));
                Draw(hdc, X_1, Y_1, rr);// point circle 1 and r (small)
                counter++;
            } else if (counter == 5) {
                hdc = GetDC(hwnd);
                X_1 = LOWORD(lParam);
                Y_1 = HIWORD(lParam);
                counter++;
            } else if (counter == 6) {
                hdc = GetDC(hwnd);
                X_3 = LOWORD(lParam);
                Y_3 = HIWORD(lParam);
                rr = sqrt(pow((X_3 - X_1), 2) + pow((Y_3 - Y_1), 2));
                Draw(hdc, X_1, Y_1, rr); // point circle 2 and r (small)
                counter++;
            } else if (counter == 7) {
                x1_line=LOWORD(lParam);
                y1_line=HIWORD(lParam);
                counter++;
            } else if (counter == 8) {
                x2_line=LOWORD(lParam);
                y2_line=HIWORD(lParam);
                counter++;
            } else if (counter == 9) {
                x3_line=LOWORD(lParam);
                y3_line=HIWORD(lParam);
                parametricline(hdc,x1_line,y1_line,x2_line,y2_line);
                parametricline(hdc,x1_line,y1_line,x3_line,y3_line);
                parametricline(hdc,x2_line,y2_line,x3_line,y3_line);
                counter++;
            }
            else if(counter == 10)
            {
                hdc = GetDC(hwnd);
                x_1 = LOWORD(lParam);
                y_1 = HIWORD(lParam);
                counter++;
            }
            else if(counter == 11)
            {
                hdc = GetDC(hwnd);
                x_2 = LOWORD(lParam);
                y_2 = HIWORD(lParam);
                rr = sqrt(pow((x_1 - x_2), 2) + pow((y_1 - y_2), 2));
                midpointHalf(hdc, x_1, y_1, rr); // point circle 2 and r (small)
                counter++;
            }
            break;
        case WM_DESTROY:
            PostQuitMessage(0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc(hwnd, message, wParam, lParam);
    }

    return 0;
}