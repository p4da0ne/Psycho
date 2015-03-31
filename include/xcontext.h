/******  XCONTEXT.H **********  Dishlenko S.G.  ***** 29/11/02 ****
*                                                                  *
*              Copyright (c) PANORAMA Group 1991-2001              *
*                      All Rights Reserved                         *
*                                                                  *
********************************************************************
*                                                                  *
*                FOR      XWindow                                  *
*                                                                  *
********************************************************************
*                                                                  *
*            ОПИСАНИЕ ТИПОВ ПЕРЕМЕННЫХ                             *
*                                                                  *
*******************************************************************/

#ifndef XCONTEXT_H           // 29/11/02
#define XCONTEXT_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

typedef struct XCONTEXT  // Аналог Windows HDC для XWindow
{
  Display * xcDisplay;   // Связь с X-сервером
  Window    xcWindow;    // Идентификатор окна
  GC        xcContext;   // Графический контекст окна
  DRAWPOINT xcPoint;     // Расположение отображаемой области в окне
                         // (левый верхний угол в пикселах)
}
  XCONTEXT;

#define XHDC       XCONTEXT*

#endif  // XCONTEXT_H

