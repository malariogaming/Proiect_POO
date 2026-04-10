#include "MainWindow.h"
#include "StandardPassenger.h"
#include "VIPPassenger.h"
#include "CargoBox.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
    ui.tableLifturi->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

MainWindow::~MainWindow()
{}

void MainWindow::on_btnDeschideDialog_clicked() {
    AddPassengerDialog dlg(this);
    dlg.setFloorLimits(ui.spinEtaje->value());

    if (dlg.exec() == QDialog::Accepted) {
        int tip = dlg.getSelectedType();
        int weight = dlg.getWeight();
        int start = dlg.getStartFloor();
        int dest = dlg.getDestFloor();

        QString infoPasager;
        QString emoji;

        if (tip == 0) { // Standard
            std::string nume = dlg.getName();
            // transportabil = new StandardPassenger(nume, weight, start, dest);
            infoPasager = QString::fromStdString(nume);
            emoji = "👤";
            qDebug() << "Creat Pasager Standard:" << infoPasager;
        }
        else if (tip == 1) { // VIP
            std::string nume = dlg.getName();
            int priority = dlg.getPriority();
            // transportabil = new VIPPassenger(nume, weight, start, dest, priority);
            infoPasager = QString::fromStdString(nume) + " (VIP)";
            emoji = "🌟";
            qDebug() << "Creat Pasager VIP:" << infoPasager << "Prioritate:" << priority;
        }
        else if (tip == 2) { // Cargo
            bool fragile = dlg.isFragile();
            // transportabil = new CargoBox(weight, fragile, start, dest);
            infoPasager = "Cargo" + QString(fragile ? " [!]" : "");
            emoji = "📦";
            qDebug() << "Creat Cargo Box. Fragil:" << (fragile ? "DA" : "NU");
        }


        QLabel* pLabel = new QLabel(emoji + " " + infoPasager + " -> " + QString::number(dest));
        pLabel->setStyleSheet("color: #f1c40f; font-weight: bold; background-color: rgba(44, 62, 80, 200); "
            "border-radius: 5px; padding: 2px; border: 1px solid #f39c12;");

        int nrEtaje = ui.spinEtaje->value();
        int row = (nrEtaje - 1) - start;

        int startt = dlg.getStartFloor();
        if (!waitingAreas.isEmpty() && startt < waitingAreas[0].size()) {
            waitingAreas[0][startt]->addWidget(pLabel);
        }
    }
}

void MainWindow::on_btnGenerare_clicked() {
    int nrEtaje = ui.spinEtaje->value();
    int nrLifturi = ui.spinLifturi->value();

    ui.paginiPrincipale->setCurrentIndex(1);

    QPushButton* btnDialog = ui.paginiPrincipale->findChild<QPushButton*>("btnDeschideDialog");

    QLayoutItem* child;
    while ((child = ui.layoutSimulare->takeAt(0)) != nullptr) {
        QWidget* w = child->widget();
        if (w != nullptr && w != btnDialog) {
            delete w;
        }
        delete child;
    }

    lifturiGrafice.clear();
    waitingAreas.clear();

    QGridLayout* buildingGrid = new QGridLayout();
    buildingGrid->setSpacing(0);
    buildingGrid->setContentsMargins(10, 10, 10, 10);

    for (int j = 0; j < nrEtaje; ++j) {
        int row = (nrEtaje - 1) - j;
        waitingAreas.append(QVector<QBoxLayout*>());

        QString cellStyle = "border-bottom: 1px solid #555; background-color: #222;";

        QLabel* floorLabel = new QLabel(QString("Etaj %1").arg(j));
        floorLabel->setFixedSize(70, 50);
        floorLabel->setStyleSheet(cellStyle + "color: white; border-right: 1px solid #555;");
        floorLabel->setAlignment(Qt::AlignCenter);
        buildingGrid->addWidget(floorLabel, row, 0);

        QWidget* waitingWidget = new QWidget();
        waitingWidget->setStyleSheet(cellStyle);
        QHBoxLayout* hBox = new QHBoxLayout(waitingWidget);

        hBox->setSpacing(8); 
        hBox->setContentsMargins(10, 5, 10, 5);
        hBox->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

        buildingGrid->addWidget(waitingWidget, row, 1);

        if (waitingAreas.size() > 0) waitingAreas[0].append(hBox);
    }

    for (int i = 0; i < nrLifturi; ++i) {

        for (int j = 0; j < nrEtaje; ++j) {
            int row = (nrEtaje - 1) - j;
            QFrame* shaftSegment = new QFrame();

            shaftSegment->setStyleSheet("border-bottom: 1px solid #555; border-left: 1px solid #444; border-right: 1px solid #444; background-color: #1a1a1a;");
            shaftSegment->setFixedWidth(60);
            buildingGrid->addWidget(shaftSegment, row, i + 2);
        }

        QComboBox* combo = qobject_cast<QComboBox*>(ui.tableLifturi->cellWidget(i, 1));
        QString tipLift = (combo) ? combo->currentText() : "Passenger";

        QString stil;
        if (tipLift == "Passenger") stil = "background-color: #3498db; border: 2px solid #2980b9; border-radius: 4px;";
        else if (tipLift == "Freight") stil = "background-color: #95a5a6; border: 2px solid #7f8c8d; border-radius: 2px;";
        else stil = "background-color: #e74c3c; border: 2px solid #c0392b; border-radius: 4px;";

        QFrame* elevatorCabin = new QFrame();
        elevatorCabin->setFixedSize(40, 40);
        elevatorCabin->setStyleSheet(stil);

        buildingGrid->addWidget(elevatorCabin, nrEtaje - 1, i + 2, Qt::AlignCenter);

        lifturiGrafice.append(elevatorCabin);
    }

    ui.layoutSimulare->addLayout(buildingGrid);
    ui.layoutSimulare->addStretch();

    if (btnDialog) {
        ui.layoutSimulare->addWidget(btnDialog);
        ui.layoutSimulare->setAlignment(btnDialog, Qt::AlignCenter);
    }
}

void MainWindow::on_spinLifturi_valueChanged(int nr) {
    ui.tableLifturi->setRowCount(nr);

    for (int i = 0; i < nr; i++) {
        ui.tableLifturi->setItem(i, 0, new QTableWidgetItem("Lift " + QString::number(i + 1)));

        QComboBox* combo = new QComboBox();
        combo->addItems({ "Passenger", "Freight", "Emergency" });

        ui.tableLifturi->setCellWidget(i, 1, combo);
    }
}