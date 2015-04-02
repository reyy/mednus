#ifndef MEDNUSQUIZSTYLESHEET_H
#define MEDNUSQUIZSTYLESHEET_H

#define RADIO_BUTTON_STYLESHEET "QRadioButton { margin-left:10px;margin-right:10px;margin-top:2px;margin-bottom:2px;} QRadioButton::indicator{margin-right:10px;}QRadioButton::indicator::unchecked{image:url(:/images/check_default.png)} QRadioButton::indicator::unchecked:pressed{image:url(:/images/check_default.png)} QRadioButton::indicator::unchecked:hover{image:url(:/images/check_default_hover.png)} QRadioButton::indicator::checked{image:url(:/images/check_filled.png)} QRadioButton::indicator::checked:pressed{image:url(:/images/check_filled.png)} QRadioButton::indicator::checked:hover{image:url(:/images/check_filled_hover.png)}"
#define TEACHER_COMMENT_STYLESHEET "QLabel{color:#97d5ee;margin-top:5px;margin-bottom:5px;}"
#define MARK_BUTTON_STYLESHEET "QPushButton {margin-top:20px;background:rgba(0,0,0,255);border: 5px solid #e5a539;padding:10px;} QPushButton::pressed {background:rgba(30,30,30,255);}"
#define EDIT_BUTTON_STYLESHEET "QPushButton {margin-top:20px;background:rgba(0,0,0,255);border: 5px solid #e5a539;padding:10px;} QPushButton::pressed {background:rgba(30,30,30,255);}"
#define VIEW_QUIZ_BUTTON_STYLESHEET "QPushButton {margin-top:20px;background:rgba(0,0,0,255);border: 5px solid #e5a539;padding:10px;} QPushButton::pressed {background:rgba(30,30,30,255);}"
#define INSTRUCTION_TEXT_LABEL_STYLESHEET "QLabel {color:#adbfc6;}"
#define SCORE_TEXT_LABEL_STYLESHEET "QLabel{color:#e5a539;margin-top:10px;margin-bottom:10px;padding:5px;background:rgba(0,0,0,100);border-top-left-radius: 30px;border-bottom-right-radius: 30px;}"
#define DUMMY_SPACE_STYLESHEET "QLabel {color:#FFFFFF;}"
#define TIMER_LABEL_STYLESHEET "QLabel {color:rgba(229,164,57);margin-top:10px}"
#define TIMED_QUIZ_WARNING_STYLESHEET "QLabel{color:#e5a539;margin-top:10px;margin-bottom:10px;}"
#define ATTEMPT_LABEL_STYLESHEET "QLabel{color:#e5a539;margin-top:10px;margin-bottom:10px;}"
#define START_QUIZ_BUTTON_STYLESHEET "QPushButton {margin-top:5px;background:rgba(0,0,0,255);border: 5px solid #e5a539;padding:10px;} QPushButton::pressed {background:rgba(30,30,30,255);}"
#define WARNING_LABEL_STYLESHEET "QLabel { color:#e5a539;margin-top:20px;margin-left:10px;margin-right:10px;}"

#define QUESTION_ANSWERED_NOTICE_STYLESHEET "QLabel{margin-top:8px;margin-bottom:4px;padding:10px;background:rgba(0,0,0,50);border-top-left-radius: 16px;border-bottom-right-radius: 16px;}"
#define QUESTION_UNANSWERED_NOTICE_STYLESHEET "QLabel{margin-top:8px;margin-bottom:4px;padding:10px;background:rgba(229,165,57,255);border-top-left-radius: 16px;border-bottom-right-radius: 16px;}"

// Quiz radio button highlights
#define RADIO_BUTTON_HIGHLIGHT_GREEN "QRadioButton::indicator {width: 0px;height: 13px; } QRadioButton {font: bold;color: rgba(50, 255, 50, 255);}"
#define RADIO_BUTTON_HIGHLIGHT_RED "QRadioButton::indicator {width: 0px;height: 13px; } QRadioButton {font: bold;color: rgba(255, 0, 0, 255);}"
#define RADIO_BUTTON_HIGHLIGHT_GREY "QRadioButton::indicator {width: 0px;height: 13px; } QRadioButton {color: rgba(128, 128, 128, 255);}"

#endif // MEDNUSQUIZSTYLESHEET_H
