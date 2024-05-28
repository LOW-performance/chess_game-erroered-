//======================================================================
//  File:        pclaf.h
//  Authors:     Timothy P. Justice, Yves Peneveyre, Doohun Eum
//  Created:     September 2006
//  Description: This file contains the interface of the
//               classes for the Little Application Framework.
//----------------------------------------------------------------------
//  Copyright (c) 2006, 2015 by Timothy P. Justice
//						Oregon State U.,
//						Yves Peneveyre
//                      Ecole d'Ingenieurs du Canton de Vaud, Switzerland,
//						Doohun Eum
//                      Duksung Women's Univ., Seoul, Korea
//
//  This file is part of LAF, the Little Application Framework.
//
//  Permission is hereby granted to everyone to copy and distribute
//  without fee verbatim copies of this file.  This copyright notice
//  must be retained on all copies.  No warranty expressed or implied
//  is made concerning this software.
//
//  Authors e-mail addresses: justict@cs.orst.edu
//                            Yves.Peneveyre@eivd.ch (www.eivd.ch)
//                            dheum@duksung.ac.kr
//======================================================================

#ifndef LAF_H
#define LAF_H

// Header Files

#include <WINDOWS.H>			// Microsoft Windows interface
#include <stdio.h>			// C++ standard i/o library
#include <stdlib.h>			// C++ standard library
#include <string.h>
#include <TCHAR.H>

// Definitions

#define MAXCONTROLS 100
#define MAXITEMS     20
#define MAXMENUS     20

#define MAKEPOINT(l) (*((POINT FAR *) & (l)))
#define M_PI 3.14

// Class Hierarchy: Little Application Framework

class  Wostream;
class  Window;
class  Application;
class  ChildWindow;
class  Menu;
class  Control;
class  BasicMenuItem;
class  BasicButton;
class  EditText;
class  StaticText;
class  ListBox;

// Other Types

enum StreamBase { octBase, decBase, hexBase };

enum Color
{
    black, blue, green, cyan, red, magenta, brown, gray,
    white, brightBlue, brightGreen, brightCyan, brightRed,
    brightMagenta, brightYellow, brightGray
};

enum LineStyle { solidLine, dashedLine, dottedLine, nullLine };

enum TextAlign { leftText, centerText, rightText };

typedef HBITMAP Bitmap;
typedef POINT Point;

// --- Window Stream Class ---------------------------------------------
//
//  Wostream is used to simulate the behavior of C++ output
//  streams in a window.
//----------------------------------------------------------------------
class Wostream
{
public:
    Wostream();
    Wostream(Window*);
    StreamBase base() const;
    StreamBase base(StreamBase);
    int        precision() const;
    int        precision(int);
    int        width() const;
    int        width(int);
    Window* outputWindow();

    Wostream& operator << (TCHAR);
    Wostream& operator << (const TCHAR*);
    Wostream& operator << (double);
    Wostream& operator << (float);
    Wostream& operator << (int);
    Wostream& operator << (long);
private:
    int        fieldWidth;
    int        floatPrecision;
    StreamBase numberBase;
    Window* winptr;
};

// --- Window Class ----------------------------------------------------
//
//  Window is an abstract class that defines the behavior common
//  to all windows.
//----------------------------------------------------------------------
class Window
{
public:
    // constructor
    Window(const TCHAR* title, int width, int height, int x, int y);

    // destructor
    virtual ~Window();

    // add a control to the window
    void add(Control* aControl);

    // add a menu to the window
    void add(Menu* aMenu);

    // return the average character width
    int avgCharWidth();

    // return the y coordinate for the bottom of the window
    int bottom();

    void           cartesianOff();
    void           cartesianOn(Color, LineStyle, int);
    void           circle(const int, const int, const int);
    void           circle(const POINT&, const int);
    void           checkMenuItem(int);
    void           clearAndUpdate();
    virtual void   command(int);
    virtual void   create();
    EditText       createEditText(int, int, int, int, int,
        TextAlign = leftText, int = 1, const TCHAR* = 0);
    ListBox        createListBox(int, int, int, int, int, int = 1);
    StaticText     createStaticText(int, int, int, int,
        TextAlign = leftText, int = 0, const TCHAR* = 0);
    virtual void   doubleClick(int, int);
    void           gridOn(Color, LineStyle, int);
    void           gridOff();
    HWND           handle();
    int            height();
    HINSTANCE      instance();
    int            left();
    void           line(const int, const int, const int, const int);
    void           line(const POINT&, const POINT&);
    HMENU          menuHandle();
    virtual void   mouseDown(int, int);
    virtual void   mouseRDown(int, int);
    virtual void	 mouseMDown(int, int);
    virtual void   paint();
    void           point(const int, const int);
    void           polygon(POINT[], int);
    LRESULT WINAPI process(HWND, UINT, WPARAM, LPARAM);
    virtual void   quit();
    void           rectangle(const int, const int, const int, const int);
    int            right();
    virtual void   run();
    void           scrollToBottom();
    void           scrollToLeft();
    void           scrollToRight();
    void           scrollToTop();
    void           setBrush(Color);
    void           setMaxX(int);
    void           setMaxY(int);
    void           setPen(Color, LineStyle = solidLine, int = 1);
    void           setTextColor(Color);
    void           setTextPosition(int, int);
    int            show(int);
    virtual void   size(int, int);
    int            textHeight();
    int            textWidth(TCHAR*);
    void           textPosition(int&, int&);
    virtual void   timer();
    int            top();
    void           uncheckMenuItem(int);
    void           update();
    void           vector(const int, const int, const int, const int);
    int            width();

    void			 drawBitmap(Bitmap, int, int, int, int);
    Bitmap		 loadBitmap(int);
    Bitmap		 loadBitmap(const TCHAR*);


    Wostream       wout;

    // friends
    friend class Wostream;
    friend class Menu;

protected:
    void           createBrush();
    void           createPen();
    void           deleteBrush();
    void           deletePen();
    void           _initialize_();
    void           print(const TCHAR*);

    COLORREF     brushColor;
    Color        cartesianColor;
    LineStyle    cartesianStyle;
    int          cartesianWidth;
    int          charHeight;
    int          charWidth;
    int          clientX;
    int          clientY;
    Control* ctl[MAXCONTROLS];
    int          ctlCount;
    HBRUSH       currentBrush;
    HPEN         currentPen;
    HDC          device;
    Color        gridColor;
    LineStyle    gridStyle;
    int          gridWidth;
    HMENU        hmenu;
    int          hScrollPos;
    HWND         hwnd;
    int          initialHeight;
    int          initialWidth;
    int          initialX;
    int          initialY;
    POINT        lowerRight;
    int          maxHScroll;
    int          maxVScroll;
    int          maxX;
    int          maxY;
    TEXTMETRIC   metric;
    Menu* mnu[MAXMENUS];
    int          mnuCount;
    TCHAR* name;
    POINT        nextPos;
    COLORREF     penColor;
    int          penStyle;
    int          penWidth;
    POINT        upperLeft;
    int          vScrollPos;
    HINSTANCE    prevInstance;
    POINT        prevPos;
    int          tabStop[10];
    COLORREF     textColor;
    HINSTANCE    thisInstance;
    TCHAR* title;
};

// --- Application Class -----------------------------------------------
//
//  Application is the basic framework class.  It is a special kind
//  of window that is opened when the application is started.  By
//  itself, application is not very functional.  If you create an
//  instance of application and send it the message "run", a blank
//  window is displayed.  The only functionality available is to
//  resize, move, minimize, maximize, and close the window (i.e.,
//  terminate the application).  The main work done by an application
//  that is visible to the user is performed by the "paint" method.
//  Normally, you will create a subclass of Application and override
//  the "paint" method (and probably other methods) to perform the
//  processing required by your application.
//----------------------------------------------------------------------
class Application : public Window
{
public:
    // constructor
    Application(const TCHAR* title,
        int width = CW_USEDEFAULT, int height = CW_USEDEFAULT,
        int x = CW_USEDEFAULT, int y = CW_USEDEFAULT);

    // destructor
    virtual ~Application();

    // event handling
    virtual void create();
    virtual void doubleClick(int, int);
    virtual void mouseDown(int, int);
    virtual void mouseRDown(int, int);
    virtual void mouseMDown(int, int);
    virtual void paint();
    virtual void size(int, int);
    virtual void timer();

    // non-event behavior
    virtual int  okBox(const TCHAR*, const TCHAR* = 0);
    virtual int  debugBox(TCHAR*);
    virtual void run();
    virtual void quit();
    virtual void startTimer(unsigned int = 1000);
    virtual void stopTimer();
    virtual void sendTimerMessage();

private:
    int timerInterval;
};

// --- ChildWindow Class -----------------------------------------------
//
//  A ChildWindow is a separate window from the main application
//  window.
//----------------------------------------------------------------------
class ChildWindow : public Window
{
public:
    // constructor
    ChildWindow(const TCHAR* title,
        int width = CW_USEDEFAULT, int height = CW_USEDEFAULT,
        int x = CW_USEDEFAULT, int y = CW_USEDEFAULT);

    // destructor
    virtual ~ChildWindow();

    // event handling
    void    create();
    void    doubleClick(int, int);
    void    mouseDown(int, int);
    void    mouseRDown(int, int);
    void	  mouseMDown(int, int);
    void    paint();
    void    size(int, int);
    void    timer();

    // non-event behavior
    void    run();
};

// --- Menu Class ------------------------------------------------------
//
//  The Menu class implements menus.
//----------------------------------------------------------------------
class Menu
{
public:
    // constructors
    Menu(Window& aWindow, const TCHAR* text, const TCHAR choice = '\0');

    // destructor
    virtual ~Menu();

    // add the menu item to menu
    virtual void add(BasicMenuItem& anItem);

    // add the menu to a window
    virtual void add(Window& aWindow);

    // create the control
    virtual void create();

    // friends
    friend class Window;

private:
    Window& theWindow;
    TCHAR* textValue;
    TCHAR           choiceValue;
    BasicMenuItem* item[MAXITEMS];
    int            itemCount;
    HMENU          handleValue;
};

// --- Control Class ---------------------------------------------------
//
//  The Control class is an abstract superclass for all controls.
//----------------------------------------------------------------------
class Control
{
public:
    // constructors
    Control(Window& aWindow);

    // equality operator
    int operator == (const Control&);

    // return the identifier
    virtual int id() const;

    // select the control
    virtual void select() = 0;

    // create the control
    virtual void create() = 0;

protected:
    Window& theWindow;
    int    idValue;
    HWND   hwnd;
};

// --- Menu Item Class -------------------------------------------------
//
//  The MenuItem class implements individual menu items.
//----------------------------------------------------------------------
class BasicMenuItem : public Control
{
public:
    // constructors
    BasicMenuItem(Window& aWindow, const TCHAR* text, const TCHAR choice,
        Menu& aMenu);

    // destructor
    virtual ~BasicMenuItem();

    // select the control
    virtual void select();

    // create the control
    virtual void create();

    // friends
    friend class Menu;

protected:
    Menu& menuValue;
    TCHAR* textValue;
    TCHAR choiceValue;
};


template <class T> class MenuItem : public BasicMenuItem
{
public:
    MenuItem(T& anApp, const TCHAR* text, const TCHAR choice, Menu& aMenu)
        : BasicMenuItem(anApp, text, choice, aMenu), app(anApp)
    { /* do nothing */
    }

    // select the menu item
    void select()
    { /* do nothing */
    }

protected:
    T& app;
};



// --- Button Classes --------------------------------------------------
//
//  The BasicButton class implements buttons.
//----------------------------------------------------------------------
class BasicButton : public Control
{
public:
    // constructors
    BasicButton(Window& aWindow, const TCHAR* aTitle = TEXT("Button"),
        int aWidth = 50, int aHeight = 50, int x = 0, int y = 0);
    BasicButton(const BasicButton&);

    // destructor
    virtual ~BasicButton();

    // assignment operator
    BasicButton& operator = (const BasicButton&);

    // select the control
    virtual void select();

    // create the control
    virtual void create();

protected:
    const TCHAR* titleValue;
    POINT upperLeft;
    int   heightValue;
    int   widthValue;
};


template <class T> class Button : public BasicButton
{
public:
    // constructor
    Button(T& anApp, const TCHAR* aTitle,
        int aWidth, int aHeight, int x, int y)
        : BasicButton(anApp, aTitle, aWidth, aHeight, x, y), app(anApp)
    { /* do nothing */
    }

    // select the button
    void select()
    { /* do nothing */
    }

protected:
    T& app;
};


// --- EditText Class --------------------------------------------------
//
//  The EditText class implements fields of editable text.
//----------------------------------------------------------------------
class EditText
{
public:
    // constructors
    EditText();
    EditText(Window*, int, int, int, int, int,
        TextAlign = leftText, int = 1, const TCHAR* = 0);
    EditText(const EditText&);

    // destructor
    ~EditText();

    // set input focus on
    void setFocus();

    // get the maximum number of characters
    int  size();

    // copy text to string
    TCHAR* text(TCHAR*);

    // assignment operator
    EditText& operator = (const EditText&);

    // "get from" operator
    EditText& operator >> (TCHAR*);

    // "put to" operator
    EditText& operator << (TCHAR*);
    EditText& operator << (TCHAR);
    EditText& operator << (int);
    EditText& operator << (EditText& (*)(EditText&));

private:
    Window* win;
    int    idValue;
    POINT  upperLeft;
    int    heightValue;
    int    widthValue;
    DWORD  style;
    HWND   hwnd;
};



// --- Static Text Class -----------------------------------------------
//
//  The StaticText class implements fields of fixed text.
//----------------------------------------------------------------------
class StaticText
{
public:
    // constructors
    StaticText();
    StaticText(Window*, int, int, int, int,
        TextAlign = leftText, int = 0, const TCHAR* = 0);
    StaticText(const StaticText&);

    // destructor
    ~StaticText();

    // assignment operator
    StaticText& operator =  (const StaticText&);

    // equality operator
    int operator == (const StaticText&);

    // text output
    StaticText& operator << (TCHAR*);
    StaticText& operator << (TCHAR);
    StaticText& operator << (int);
    StaticText& operator << (StaticText& (*)(StaticText&));

protected:
    Window* win;
    POINT  upperLeft;
    int    heightValue;
    int    widthValue;
    DWORD  style;
    HWND   hwnd;
};



// --- List Box Class --------------------------------------------------
//
//  The ListBox class implements a scrollable list of strings.
//----------------------------------------------------------------------
class ListBox
{
public:
    // constructors
    ListBox();
    ListBox(Window*, int, int, int, int, int, int = 1);
    ListBox(const ListBox&);

    // destructor
    ~ListBox();

    // list manipulation
    void clear();
    const TCHAR* add(const TCHAR*);

    // assignment operator
    ListBox& operator =  (const ListBox&);

protected:
    Window* win;
    int    idValue;
    POINT  upperLeft;
    int    heightValue;
    int    widthValue;
    DWORD  style;
    HWND   hwnd;
};



//--- Templates --------------------------------------------------------

template <class T>
T maximum(T x, T y)
{
    return (x > y) ? x : y;
};

template <class T>
T minimum(T x, T y)
{
    return (x < y) ? x : y;
};


template <class T>
class WO_MANIP
{
public:
    WO_MANIP(Wostream& (*ff)(Wostream&, T), T ii)
        : f(ff), i(ii)
    { }

    friend Wostream& operator << (Wostream& wos, WO_MANIP m)
    {
        return (*m.f)(wos, m.i);
    }

private:
    T i;

    Wostream& (*f)(Wostream&, T);
};


template <class T>
class WO_MANIP2
{
public:
    WO_MANIP2(Wostream& (*ff)(Wostream&, T, T), T aa, T bb)
        : f(ff), a(aa), b(bb)
    { }

    friend Wostream& operator << (Wostream& wos, WO_MANIP2 m)
    {
        return (*m.f)(wos, m.a, m.b);
    }

private:
    T a;
    T b;

    Wostream& (*f)(Wostream&, T, T);
};



//--- Function Prototypes ----------------------------------------------

EditText& eraseText(EditText&);
StaticText& eraseText(StaticText&);

Wostream& dec(Wostream&);
Wostream& hex(Wostream&);
Wostream& oct(Wostream&);

Wostream& endl(Wostream&);
Wostream& eraseToEol(Wostream&);
Wostream& operator << (Wostream& wos, Wostream& (*)(Wostream&));

WO_MANIP<StreamBase> setbase(StreamBase);
WO_MANIP<Color>      setcolor(Color);
WO_MANIP2<int>       setpos(int, int);
WO_MANIP<int>        setprecision(int);
WO_MANIP<int>        setw(int);

void debugBox();
int  randomInt(int, int);



//--- External Variables -----------------------------------------------

//extern ostrstream dout;                 // debug output stream

#endif