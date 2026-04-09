#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

MainWindow::~MainWindow()
{}

void MainWindow::on_btnGenerare_clicked() {
    int nrEtaje = ui.spinEtaje->value();
    int nrLifturi = ui.spinLifturi->value();

    ui.paginiPrincipale->setCurrentIndex(1);

    // Curățăm layout-ul vechi
    QLayoutItem* child;
    while ((child = ui.layoutSimulare->takeAt(0)) != nullptr) {
        if (child->widget()) delete child->widget();
        else if (child->layout()) {
            // Curățare recursivă dacă e nevoie
        }
        delete child;
    }

    QHBoxLayout* mainBuildingLayout = new QHBoxLayout();

    for (int i = 0; i < nrLifturi; ++i) {
        QGridLayout* shaftLayout = new QGridLayout();
        shaftLayout->setSpacing(5);

        for (int j = 0; j < nrEtaje; ++j) {
            int row = (nrEtaje - 1) - j;

            QLabel* floorLabel = new QLabel(QString("Etaj %1").arg(j));
            floorLabel->setStyleSheet("border: 1px solid gray; background-color: #333; color: white;");
            floorLabel->setAlignment(Qt::AlignCenter);
            floorLabel->setFixedSize(60, 40);

            shaftLayout->addWidget(floorLabel, row, 0);
        }

        QFrame* elevatorCabin = new QFrame();
        elevatorCabin->setFixedSize(40, 40);
        elevatorCabin->setStyleSheet("background-color: blue; border: 2px solid cyan;");

        shaftLayout->addWidget(elevatorCabin, nrEtaje - 1, 1);

        mainBuildingLayout->addLayout(shaftLayout);
        lifturiGrafice.append(elevatorCabin);
    }

    ui.layoutSimulare->addLayout(mainBuildingLayout);
}