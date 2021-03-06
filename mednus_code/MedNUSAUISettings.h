#ifndef MEDNUSAUISETTINGS_H
#define MEDNUSAUISETTINGS_H

#define LESSONPANEL_WIDTH 300
#define LESSONPANEL_WIDTH_L 500
#define LESSONPANEL_BORDER 5
#define LESSONPANEL_BORDERICON 15
#define LESSONPANEL_HEIGHT 176
#define LESSONPANEL_CLICKHEIGHT 64
#define LESSONPANEL_CONTRACTED_CLICKHEIGHT 24
#define LESSONPACKAGEITEM_HEIGHT 24
#define LESSONPACKAGEBUTTON_OFFSET 80
#define TOPBAR_HEIGHT 40
#define SIDEBAR_OFFSET 14
#define LOGO_WIDTH 160
#define LOGO_LIMIT 4

#define VIDEO_BORDER 10
#define VIDEO_SEP_LENGTH 7
#define VIDEO_TIME_LENGTH 50
#define VIDEO_ICON_SIZE 24

#define VIDEO_INDEX 0
#define PDF_INDEX 1
#define MESH_INDEX 2

#define TEXT_QUIZ_INTRO "Select an answer for every question. Unanswered questions will prevent quiz from being submitted. Click on the Submit button at the bottom of the page to have your answers graded."
#define TEXT_QUIZ_WARNING "Not all questions attempted, please check through and submit again."
#define TEXT_QUIZ_TIMED_QUIZ_WARNING "Time Given: "

#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))

#define SAFE_DELETE(a) {if( (a) != NULL ) delete (a); (a) = NULL;}

enum interfaceMode {
    NONE,
    STUDENT,
    LECTURER
};

enum fileType {
    GENERIC,
    PDF,
    VIDEO,
    MODEL,
    QUIZ,
    IMAGE
};

#endif // MEDNUSAUISETTINGS_H
