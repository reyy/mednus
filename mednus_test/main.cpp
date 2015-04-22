#include <QTest>
#include <QApplication>

#include "tst_mednusquiztester.h"
#include "tst_mednusnetworktester.h"

int main(int argc, char *argv[]) {

    QApplication *app = new QApplication(argc, argv);

    MedNUSQuizTester test1;
    QTest::qExec(&test1, argc, argv);

    MedNUSNetworkTester test2;
    QTest::qExec(&test2, argc, argv);

    delete app;

    return 0;
}
