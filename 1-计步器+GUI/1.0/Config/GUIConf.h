/*
*********************************************************************************************************
*                                                uC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              礐/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
File        : GUIConf.h
Purpose     : Configures abilities, fonts etc.
----------------------------------------------------------------------
*/


#ifndef GUICONF_H
#define GUICONF_H

#define GUI_OS                    (0)  /* 支持操作系统，当用到ucos时再打开，Compile with multitasking support */
#define GUI_SUPPORT_TOUCH         (1)  /* 触摸屏的支持，这里先关闭Support a touch screen (req. win-manager) */
#define GUI_SUPPORT_UNICODE       (0)  /* 建议先关闭，以后用到汉字库时再打开Support mixed ASCII/UNICODE strings */

#define GUI_DEFAULT_FONT          &GUI_Font8x16
#define GUI_ALLOC_SIZE            12500  /* Size of dynamic memory ... For WM and memory devices*/
//#define GUI_ALLOC_SIZE          5000  /* Size of dynamic memory ... For WM and memory devices*/

/*********************************************************************
*
*         Configuration of available packages
*/

#define GUI_WINSUPPORT            1  /* 窗口功能支持，Window manager package available */
#define GUI_SUPPORT_MEMDEV        1  /* 内存管理，建议关闭Memory devices available */
#define GUI_SUPPORT_AA            1  /* 抗锯齿功能，打开后可以提高显示效果，建议先关闭Anti aliasing available */

#endif  /* Avoid multiple inclusion */

						   

