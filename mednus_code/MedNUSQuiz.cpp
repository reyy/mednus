#include "MedNUSQuiz.h"

MedNUSQuizQuestion::MedNUSQuizQuestion(QWidget *parent, QGroupBox *box)
{
    //box->addWidget(new QLabel("This is a question that has to be answered."));

    _optionButtonGroup = new QButtonGroup(box);
    _optionButtonGroup->addButton(new QRadioButton("option 1", parent));
    _optionButtonGroup->addButton(new QRadioButton("option 2", parent));
    _optionButtonGroup->addButton(new QRadioButton("option 3", parent));
    _optionButtonGroup->addButton(new QRadioButton("option 4", parent));
}

MedNUSQuizQuestion::~MedNUSQuizQuestion()
{
    delete _option1;
}

MedNUSQuiz::MedNUSQuiz(QString filename, QWidget *parent) :
    QWidget(parent)
{
    _content = new QGroupBox(this);
    _layout = new QVBoxLayout(this);

    // Add questions here
    QButtonGroup *temp;
    QRadioButton *tempButton;
    QLabel *tempLabel;

    temp = new QButtonGroup(_content);
    tempLabel = new QLabel("Q1. There are _____ cranial bones and _____ facial bones in the adult skull.");
    tempLabel->setStyleSheet("QLabel { color : white; }");
    _layout->addWidget(tempLabel);
    tempButton = new QRadioButton("A) 6; 10");
    tempButton->setStyleSheet("QRadioButton { color : white; }");
    temp->addButton(tempButton, 1);
    _layout->addWidget(tempButton);
    tempButton = new QRadioButton("B) 8; 14");
    tempButton->setStyleSheet("QRadioButton { color : white; }");
    temp->addButton(tempButton, 2);
    _layout->addWidget(tempButton);
    tempButton = new QRadioButton("C) 12; 12");
    tempButton->setStyleSheet("QRadioButton { color : white; }");
    temp->addButton(tempButton, 3);
    _layout->addWidget(tempButton);
    tempButton = new QRadioButton("D) 5; 9");
    tempButton->setStyleSheet("QRadioButton { color : white; }");
    temp->addButton(tempButton, 4);
    _layout->addWidget(tempButton);
    _questionOptionGroup.append(temp);
    _correctAnswerList.append(3);

    temp = new QButtonGroup(_content);
    tempLabel = new QLabel("Q2. Which of these is not a normal function of the skeleton?");
    tempLabel->setStyleSheet("QLabel { color : white; }");
    _layout->addWidget(tempLabel);
    tempButton = new QRadioButton("A) mineral storage");
    tempButton->setStyleSheet("QRadioButton { color : white; }");
    temp->addButton(tempButton, 1);
    _layout->addWidget(tempButton);
    tempButton = new QRadioButton("B) protection of underlying tissues");
    tempButton->setStyleSheet("QRadioButton { color : white; }");
    temp->addButton(tempButton, 2);
    _layout->addWidget(tempButton);
    tempButton = new QRadioButton("C) storing hemopoietic tissues");
    tempButton->setStyleSheet("QRadioButton { color : white; }");
    temp->addButton(tempButton, 3);
    _layout->addWidget(tempButton);
    tempButton = new QRadioButton("D) All of these are functions of the skeleton.");
    tempButton->setStyleSheet("QRadioButton { color : white; }");
    temp->addButton(tempButton, 4);
    _layout->addWidget(tempButton);
    _questionOptionGroup.append(temp);
    _correctAnswerList.append(1);

    temp = new QButtonGroup(_content);
    tempLabel = new QLabel("Q3. Choose the surface feature that represents a depression in a bone.");
    tempLabel->setStyleSheet("QLabel { color : white; }");
    _layout->addWidget(tempLabel);
    tempButton = new QRadioButton("A) fossa");
    tempButton->setStyleSheet("QRadioButton { color : white; }");
    temp->addButton(tempButton, 1);
    _layout->addWidget(tempButton);
    tempButton = new QRadioButton("B) process");
    tempButton->setStyleSheet("QRadioButton { color : white; }");
    temp->addButton(tempButton, 2);
    _layout->addWidget(tempButton);
    tempButton = new QRadioButton("C) facet");
    tempButton->setStyleSheet("QRadioButton { color : white; }");
    temp->addButton(tempButton, 3);
    _layout->addWidget(tempButton);
    tempButton = new QRadioButton("D) condyle");
    tempButton->setStyleSheet("QRadioButton { color : white; }");
    temp->addButton(tempButton, 4);
    _layout->addWidget(tempButton);
    _questionOptionGroup.append(temp);
    _correctAnswerList.append(1);

    temp = new QButtonGroup(_content);
    tempLabel = new QLabel("Q4. The medullary cavity of a long bone is lined with a thin layer of connective tissue called the");
    tempLabel->setStyleSheet("QLabel { color : white; }");
    _layout->addWidget(tempLabel);
    tempButton = new QRadioButton("A) diaphysis");
    tempButton->setStyleSheet("QRadioButton { color : white; }");
    temp->addButton(tempButton, 1);
    _layout->addWidget(tempButton);
    tempButton = new QRadioButton("B) endosteum");
    tempButton->setStyleSheet("QRadioButton { color : white; }");
    temp->addButton(tempButton, 2);
    _layout->addWidget(tempButton);
    tempButton = new QRadioButton("C) periosteum");
    tempButton->setStyleSheet("QRadioButton { color : white; }");
    temp->addButton(tempButton, 3);
    _layout->addWidget(tempButton);
    tempButton = new QRadioButton("D) epiphysis");
    tempButton->setStyleSheet("QRadioButton { color : white; }");
    temp->addButton(tempButton, 4);
    _layout->addWidget(tempButton);
    _questionOptionGroup.append(temp);
    _correctAnswerList.append(1);

    temp = new QButtonGroup(_content);
    tempLabel = new QLabel("Q5. _____ are the type of bone cell that tears down bone during the building and remodeling process.");
    tempLabel->setStyleSheet("QLabel { color : white; }");
    _layout->addWidget(tempLabel);
    tempButton = new QRadioButton("A) osteocytes");
    tempButton->setStyleSheet("QRadioButton { color : white; }");
    temp->addButton(tempButton, 1);
    _layout->addWidget(tempButton);
    tempButton = new QRadioButton("B) osteoblasts");
    tempButton->setStyleSheet("QRadioButton { color : white; }");
    temp->addButton(tempButton, 2);
    _layout->addWidget(tempButton);
    tempButton = new QRadioButton("C) osteoclasts");
    tempButton->setStyleSheet("QRadioButton { color : white; }");
    temp->addButton(tempButton, 3);
    _layout->addWidget(tempButton);
    tempButton = new QRadioButton("D) bone lining cells");
    tempButton->setStyleSheet("QRadioButton { color : white; }");
    temp->addButton(tempButton, 4);
    _layout->addWidget(tempButton);
    _questionOptionGroup.append(temp);
    _correctAnswerList.append(1);

    /*
    temp = new QButtonGroup(_content);
    tempLabel = new QLabel("Q6. During bone formation, as the periosteum calcifies, it gives rise to a thin plate of compact bone called the _____.");
    tempLabel->setStyleSheet("QLabel { color : white; }");
    _layout->addWidget(tempLabel);
    tempButton = new QRadioButton("A) periosteal bud");
    tempButton->setStyleSheet("QRadioButton { color : white; }");
    temp->addButton(tempButton, 1);
    _layout->addWidget(tempButton);
    tempButton = new QRadioButton("B) periosteal bone collar");
    tempButton->setStyleSheet("QRadioButton { color : white; }");
    temp->addButton(tempButton, 2);
    _layout->addWidget(tempButton);
    tempButton = new QRadioButton("C) primary ossification center");
    tempButton->setStyleSheet("QRadioButton { color : white; }");
    temp->addButton(tempButton, 3);
    _layout->addWidget(tempButton);
    tempButton = new QRadioButton("D) epiphyseal plate");
    tempButton->setStyleSheet("QRadioButton { color : white; }");
    temp->addButton(tempButton, 4);
    _layout->addWidget(tempButton);
    _questionOptionGroup.append(temp);
    _correctAnswerList.append(1);

    temp = new QButtonGroup(_content);
    tempLabel = new QLabel("Q7. Which pair of fontanels are located on the front sides of the infant skull?");
    tempLabel->setStyleSheet("QLabel { color : white; }");
    _layout->addWidget(tempLabel);
    tempButton = new QRadioButton("A) posterolateral fontanels");
    tempButton->setStyleSheet("QRadioButton { color : white; }");
    temp->addButton(tempButton, 1);
    _layout->addWidget(tempButton);
    tempButton = new QRadioButton("B) anterior fontanel");
    tempButton->setStyleSheet("QRadioButton { color : white; }");
    temp->addButton(tempButton, 2);
    _layout->addWidget(tempButton);
    tempButton = new QRadioButton("C) posterior fontanel");
    tempButton->setStyleSheet("QRadioButton { color : white; }");
    temp->addButton(tempButton, 3);
    _layout->addWidget(tempButton);
    tempButton = new QRadioButton("D) anterolateral fontanels");
    tempButton->setStyleSheet("QRadioButton { color : white; }");
    temp->addButton(tempButton, 4);
    _layout->addWidget(tempButton);
    _questionOptionGroup.append(temp);
    _correctAnswerList.append(1);

    temp = new QButtonGroup(_content);
    tempLabel = new QLabel("Q8. The foramen magnum is located in the _____ bone of the skull.");
    tempLabel->setStyleSheet("QLabel { color : white; }");
    _layout->addWidget(tempLabel);
    tempButton = new QRadioButton("A) frontal");
    tempButton->setStyleSheet("QRadioButton { color : white; }");
    temp->addButton(tempButton, 1);
    _layout->addWidget(tempButton);
    tempButton = new QRadioButton("B) parietal");
    tempButton->setStyleSheet("QRadioButton { color : white; }");
    temp->addButton(tempButton, 2);
    _layout->addWidget(tempButton);
    tempButton = new QRadioButton("C) occipital");
    tempButton->setStyleSheet("QRadioButton { color : white; }");
    temp->addButton(tempButton, 3);
    _layout->addWidget(tempButton);
    tempButton = new QRadioButton("D) temporal");
    tempButton->setStyleSheet("QRadioButton { color : white; }");
    temp->addButton(tempButton, 4);
    _layout->addWidget(tempButton);
    _questionOptionGroup.append(temp);
    _correctAnswerList.append(1);

    temp = new QButtonGroup(_content);
    tempLabel = new QLabel("Q9. Which of these features is not present in the sphenoid bone?");
    tempLabel->setStyleSheet("QLabel { color : white; }");
    _layout->addWidget(tempLabel);
    tempButton = new QRadioButton("A) crista galli");
    tempButton->setStyleSheet("QRadioButton { color : white; }");
    temp->addButton(tempButton, 1);
    _layout->addWidget(tempButton);
    tempButton = new QRadioButton("B) sella turcica");
    tempButton->setStyleSheet("QRadioButton { color : white; }");
    temp->addButton(tempButton, 2);
    _layout->addWidget(tempButton);
    tempButton = new QRadioButton("C) pterygoid processes");
    tempButton->setStyleSheet("QRadioButton { color : white; }");
    temp->addButton(tempButton, 3);
    _layout->addWidget(tempButton);
    tempButton = new QRadioButton("D) foramen rotundum");
    tempButton->setStyleSheet("QRadioButton { color : white; }");
    temp->addButton(tempButton, 4);
    _layout->addWidget(tempButton);
    tempButton = new QRadioButton("E) foramen ovale");
    tempButton->setStyleSheet("QRadioButton { color : white; }");
    temp->addButton(tempButton, 5);
    _layout->addWidget(tempButton);
    _questionOptionGroup.append(temp);
    _correctAnswerList.append(1);

    temp = new QButtonGroup(_content);
    tempLabel = new QLabel("Q10. Besides the maxilla, the hard palate is also composed of the _____.");
    tempLabel->setStyleSheet("QLabel { color : white; }");
    _layout->addWidget(tempLabel);
    tempButton = new QRadioButton("A) ethmoid bone");
    tempButton->setStyleSheet("QRadioButton { color : white; }");
    temp->addButton(tempButton, 1);
    _layout->addWidget(tempButton);
    tempButton = new QRadioButton("B) palatine bones");
    tempButton->setStyleSheet("QRadioButton { color : white; }");
    temp->addButton(tempButton, 2);
    _layout->addWidget(tempButton);
    tempButton = new QRadioButton("C) zygomatic bones");
    tempButton->setStyleSheet("QRadioButton { color : white; }");
    temp->addButton(tempButton, 3);
    _layout->addWidget(tempButton);
    tempButton = new QRadioButton("D) nasal bone");
    tempButton->setStyleSheet("QRadioButton { color : white; }");
    temp->addButton(tempButton, 4);
    _layout->addWidget(tempButton);
    _questionOptionGroup.append(temp);
    _correctAnswerList.append(1);

    temp = new QButtonGroup(_content);
    tempLabel = new QLabel("Q11. What two bones, or portions of bones, make up the nasal septum?");
    tempLabel->setStyleSheet("QLabel { color : white; }");
    _layout->addWidget(tempLabel);
    tempButton = new QRadioButton("A) perpendicular plate of the ethmoid; vomer");
    tempButton->setStyleSheet("QRadioButton { color : white; }");
    temp->addButton(tempButton, 1);
    _layout->addWidget(tempButton);
    tempButton = new QRadioButton("B) nasal; vomer");
    tempButton->setStyleSheet("QRadioButton { color : white; }");
    temp->addButton(tempButton, 2);
    _layout->addWidget(tempButton);
    tempButton = new QRadioButton("C) maxilla; sphemoid");
    tempButton->setStyleSheet("QRadioButton { color : white; }");
    temp->addButton(tempButton, 3);
    _layout->addWidget(tempButton);
    tempButton = new QRadioButton("D) inferior nasal concha; perpendicular plate of the ethomoid");
    tempButton->setStyleSheet("QRadioButton { color : white; }");
    temp->addButton(tempButton, 4);
    _layout->addWidget(tempButton);
    _questionOptionGroup.append(temp);
    _correctAnswerList.append(1);

    temp = new QButtonGroup(_content);
    tempLabel = new QLabel("Q12. Cervical vertebrae differ from lumbar vertebrae in all of these aspects except");
    tempLabel->setStyleSheet("QLabel { color : white; }");
    _layout->addWidget(tempLabel);
    tempButton = new QRadioButton("A) size");
    tempButton->setStyleSheet("QRadioButton { color : white; }");
    temp->addButton(tempButton, 1);
    _layout->addWidget(tempButton);
    tempButton = new QRadioButton("B) bifid spinous process");
    tempButton->setStyleSheet("QRadioButton { color : white; }");
    temp->addButton(tempButton, 2);
    _layout->addWidget(tempButton);
    tempButton = new QRadioButton("C) presence of transverse foramina");
    tempButton->setStyleSheet("QRadioButton { color : white; }");
    temp->addButton(tempButton, 3);
    _layout->addWidget(tempButton);
    tempButton = new QRadioButton("D) presence of transverse processes");
    tempButton->setStyleSheet("QRadioButton { color : white; }");
    temp->addButton(tempButton, 4);
    _layout->addWidget(tempButton);
    _questionOptionGroup.append(temp);
    _correctAnswerList.append(1);

    */

    _markButton = new QPushButton("Check Answers");
    _markButton->setGeometry(QRect(QPoint(100, 100), QSize(200, 50)));
    _markButton->setStyleSheet("QPushButton { color : white; }");
    connect(_markButton, SIGNAL(released()), this, SLOT(markQuiz()));
    _layout->addWidget(_markButton);

    _content->setLayout(_layout);
}

MedNUSQuiz::~MedNUSQuiz()
{
    delete _layout;
    delete _content;

    //_questionList->clear();
    //delete _questionList;
    //_questionList = NULL;
}

void MedNUSQuiz::markQuiz()
{
    _score = 0;
    // go to each button group and see
    // what is the correct answer.
    // add up the score and report to the user.
    for (int i = 0; i < 2; i++)
    {
        QTextStream(stdout) << "Question " << i+1 << ": ";
        //QTextStream(stdout) << _questionOptionGroup[i]->checkedId() << endl;
        if (_questionOptionGroup[i]->checkedId() == _correctAnswerList[i])
        {
            _score++;
            QTextStream(stdout) << "CORRECT\n";
        }
        else
        {
            QTextStream(stdout) << "WRONG\n";
        }
    }

    QTextStream(stdout) << "SCORE: " << _score << endl;

    _scoreMsgBox = new QMessageBox();
    QString text = "SCORE: ";
    text += QString::number(_score);
    _scoreMsgBox->setText(text);
    _scoreMsgBox->exec();
}
