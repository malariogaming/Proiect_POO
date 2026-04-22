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
    if (!cladire) return;

    AddPassengerDialog dlg(this);
    dlg.setFloorLimits(ui.spinEtaje->value());

    if (dlg.exec() == QDialog::Accepted) {
        double weight = (double)dlg.getWeight();

        double maxCapacitateCladire = 0;
        auto& elevators = cladire->getElevators();

        for (const auto& e : elevators) {
            if (e->getMaxWeight() > maxCapacitateCladire) {
                maxCapacitateCladire = e->getMaxWeight();
            }
        }

        if (weight > maxCapacitateCladire) {
            QMessageBox::critical(this, "Eroare Capacitate", 
                QString("Niciun lift din cladire nu poate transporta aceasta greutate!\n"
                        "Greutate obiect: %1 kg\n"
                        "Capacitate maxima lift: %2 kg")
                .arg(weight).arg(maxCapacitateCladire));
            return;
        }

        int tip = dlg.getSelectedType();
        int start = dlg.getStartFloor();
        int dest = dlg.getDestFloor();
        QString infoPasager;
        QString emoji;

        std::shared_ptr<ITransportable> itemNou = nullptr;

        if (tip == 0) { // Standard
            std::string nume = dlg.getName();
            itemNou = std::make_shared<StandardPassenger>(nume, weight, start, dest);
            infoPasager = QString::fromStdString(nume);
            emoji = "👤";
        }
        else if (tip == 1) { // VIP
            std::string nume = dlg.getName();
            int priority = dlg.getPriority();
            itemNou = std::make_shared<VIPPassenger>(nume, weight, start, dest, priority);
            infoPasager = QString::fromStdString(nume) + " (VIP)";
            emoji = "🌟";
        }
        else if (tip == 2) { // Cargo
            bool fragile = dlg.isFragile();
            itemNou = std::make_shared<CargoBox>(weight, fragile, start, dest);
            infoPasager = "Cargo" + QString(fragile ? " [!]" : "");
            emoji = "📦";
        }

        if (itemNou) {
            cladire->getFloors()[start].addPassenger(itemNou);

            QLabel* pLabel = new QLabel(emoji + " " + infoPasager + " -> " + QString::number(dest));
            pLabel->setStyleSheet("color: #f1c40f; font-weight: bold; background-color: rgba(44, 62, 80, 200); "
                                  "border-radius: 5px; padding: 2px; border: 1px solid #f39c12;");

            if (!waitingAreas.isEmpty() && start < waitingAreas[0].size()) {
                waitingAreas[0][start]->addWidget(pLabel);
            }
            
            qDebug() << "Obiect adaugat cu succes la etajul" << start;
        }
    }
}

void MainWindow::on_btnGenerare_clicked() {
    int nrEtaje = ui.spinEtaje->value();
    int nrLifturi = ui.spinLifturi->value();

    ui.paginiPrincipale->setCurrentIndex(1);

    QPushButton* btnDialog = ui.paginiPrincipale->findChild<QPushButton*>("btnDeschideDialog");
    QPushButton* btnStart = ui.paginiPrincipale->findChild<QPushButton*>("btnStartSimulare");
    QPushButton* btnReset = ui.paginiPrincipale->findChild<QPushButton*>("btnReset");
    QPushButton* btnUrgenta = ui.paginiPrincipale->findChild<QPushButton*>("btnUrgenta");

    QLayoutItem* child;
    while ((child = ui.layoutSimulare->takeAt(0)) != nullptr) {
        QWidget* w = child->widget();
        if (w != nullptr && w != btnDialog && w != btnStart && w != btnReset && w != btnUrgenta) {
            delete w;
        }
        delete child;
    }

    if (cladire != nullptr) {
        delete cladire;
    }
    cladire = new Building(nrEtaje);
    
    db.connectToDB();

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

        std::shared_ptr<BaseElevator> liftLogic;

        if (tipLift == "Passenger") {
            liftLogic = std::make_shared<PassengerElevator>(i, 400.0, 0, nrEtaje - 1, 3);
        }
        else if (tipLift == "Freight") {
            liftLogic = std::make_shared<FreightElevator>(i, 1200.0, 0, nrEtaje - 1);
        }
        else { // Emergency
            liftLogic = std::make_shared<EmergencyElevator>(i, 600.0, 0, nrEtaje - 1);
        }

        *cladire += liftLogic;

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

    if (btnDialog && btnStart && btnReset && btnUrgenta) {
        btnDialog->setText("Adauga Pasager / Cargo");
        btnStart->setText("START SIMULARE");
        btnReset->setText("RESET SIMULARE");
        btnUrgenta->setText("Mod Urgenta");

        btnDialog->setStyleSheet(
            "QPushButton {"
            "   background-color: #2980b9;"
            "   color: white;"
            "   font-weight: bold;"
            "   border-radius: 5px;"
            "   min-height: 18px;"
            "}"
            "QPushButton:hover { background-color: #3498db; }"
        );

        btnStart->setStyleSheet(
            "QPushButton {"
            "   background-color: #27ae60;"
            "   color: white;"
            "   font-weight: bold;"
            "   border-radius: 5px;"
            "   min-height: 18px;"
            "}"
            "QPushButton:hover { background-color: #2ecc71; }"
            "QPushButton:disabled { background-color: #7f8c8d; }"
        );

        btnReset->setStyleSheet(
            "QPushButton {"
            "   background-color: #ff0000;"
            "   color: white;"
            "   font-weight: bold;"
            "   border-radius: 5px;"
            "   min-height: 18px;"
            "}"
            "QPushButton:hover { background-color: #ff7276; }"
            "QPushButton:disabled { background-color: #7f8c8d; }"
        );

        btnUrgenta->setStyleSheet(
            "QPushButton {"
            "   background-color: #ff0000;"
            "   color: white;"
            "   font-weight: bold;"
            "   border-radius: 5px;"
            "   min-height: 18px;"
            "}"
            "QPushButton:hover { background-color: #ff7276; }"
            "QPushButton:disabled { background-color: #7f8c8d; }"
        );

        ui.layoutSimulare->addWidget(btnDialog);
        ui.layoutSimulare->addWidget(btnStart);
        ui.layoutSimulare->addWidget(btnUrgenta);
        ui.layoutSimulare->addWidget(btnReset);

        ui.layoutSimulare->setAlignment(btnDialog, Qt::AlignCenter);
        ui.layoutSimulare->setAlignment(btnStart, Qt::AlignCenter);
        ui.layoutSimulare->setAlignment(btnUrgenta, Qt::AlignCenter);
        ui.layoutSimulare->setAlignment(btnReset, Qt::AlignCenter);
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

void MainWindow::on_btnStartSimulare_clicked() {
    if (!cladire) {
        QMessageBox::warning(this, "Eroare", "Genereaza mai întai cladirea!");
        return;
    }

    if (!timerSimulare) {
        timerSimulare = new QTimer(this);
        connect(timerSimulare, &QTimer::timeout, this, &MainWindow::actualizeazaInterfata);
    }

    timerSimulare->start(1000);

    ui.btnStartSimulare->setEnabled(false);
    ui.btnStartSimulare->setText("Simulare in curs");
    ui.btnGenerare->setEnabled(false);
}

void MainWindow::actualizeazaInterfata() {
    if (!cladire) return;

    int totalEtaje = ui.spinEtaje->value();
    auto& elevators = cladire->getElevators();

    for (auto& e : elevators) {
        int cf = e->getCurrentFloor();
        for (auto& item : e->getCargo()) {
            if (item->getDestination() == cf) {
                QLabel* lblSosit = new QLabel(QString("✅ %1 (Sosit!)").arg(QString::fromStdString(item->getName())));
                lblSosit->setStyleSheet("color: white; background-color: #c0392b; font-weight: bold; "
                    "border-radius: 5px; padding: 2px; border: 1px solid #e74c3c;");

                lblSosit->setProperty("status", "finalizat");

                if (cf < waitingAreas[0].size()) {
                    waitingAreas[0][cf]->addWidget(lblSosit);
                }
            }
        }
    }

    Scheduler::getInstance()->processLOOKAlgorithm(*cladire, db);

    QGridLayout* grid = qobject_cast<QGridLayout*>(ui.layoutSimulare->itemAt(0)->layout());
    for (int i = 0; i < elevators.size(); ++i) {
        int row = (totalEtaje - 1) - elevators[i]->getCurrentFloor();
        grid->addWidget(lifturiGrafice[i], row, i + 2, Qt::AlignCenter);
    }

    for (int j = 0; j < totalEtaje; ++j) {
        QLayout* layoutEtaj = waitingAreas[0][j];

        for (int i = layoutEtaj->count() - 1; i >= 0; --i) {
            QWidget* w = layoutEtaj->itemAt(i)->widget();
            if (w && w->property("status").toString() != "finalizat") {
                layoutEtaj->removeWidget(w);
                delete w;
            }
        }

        auto& coada = cladire->getFloors()[j].getWaitingQueue();
        for (const auto& p : coada) {

            QString iconita = QString::fromStdString(p->getIcon());
            QString nume = QString::fromStdString(p->getName());

            QLabel* lblAsteptare = new QLabel(QString("%1 %2 -> %3")
                .arg(iconita)
                .arg(nume)
                .arg(p->getDestination()));

            QString stil = "font-weight: bold; border-radius: 5px; padding: 2px; ";
            if (iconita == "🌟") {
                stil += "color: #f1c40f; background-color: rgba(44, 62, 80, 220); border: 2px solid gold;";
            }
            else if (iconita == "📦") {
                stil += "color: #ecf0f1; background-color: #d35400; border: 1px solid white;";
            }
            else {
                stil += "color: #f1c40f; background-color: rgba(44, 62, 80, 200); border: 1px solid #f39c12;";
            }

            lblAsteptare->setStyleSheet(stil);
            layoutEtaj->addWidget(lblAsteptare);
        }
    }

    bool activitate = false;
    for (int j = 0; j < totalEtaje; ++j) {
        if (!cladire->getFloors()[j].getWaitingQueue().empty()) activitate = true;
    }
    for (auto& e : elevators) {
        if (!e->getCargo().empty() || e->getStatus() != ElevatorStatus::IDLE) activitate = true;
    }

    if (!activitate) {
        timerSimulare->stop();
        ui.btnStartSimulare->setEnabled(true);
        ui.btnStartSimulare->setText("SIMULARE TERMINATA");
        ui.btnGenerare->setEnabled(true);
        QMessageBox::information(this, "Gata!", "Toata lumea a ajuns la etajul dorit.");
    }
}

void MainWindow::on_btnReset_clicked()
{
    if (timerSimulare) {
        timerSimulare->stop();
    }

    int totalEtaje = ui.spinEtaje->value();
    for (int j = 0; j < totalEtaje; ++j) {
        QLayout* layoutEtaj = waitingAreas[0][j];
        QLayoutItem* child;
        while ((child = layoutEtaj->takeAt(0)) != nullptr) {
            if (child->widget()) delete child->widget();
            delete child;
        }
    }

    auto& elevators = cladire->getElevators();
    QGridLayout* grid = qobject_cast<QGridLayout*>(ui.layoutSimulare->itemAt(0)->layout());

    for (int i = 0; i < elevators.size(); ++i) {
        elevators[i]->setCurrentFloor(0);
        elevators[i]->setStatus(ElevatorStatus::IDLE);
        elevators[i]->clearCargo();

        int rowParter = totalEtaje - 1;
        if (grid) {
            grid->addWidget(lifturiGrafice[i], rowParter, i + 2, Qt::AlignCenter);
        }
    }

    for (int j = 0; j < totalEtaje; ++j) {
        while (!cladire->getFloors()[j].getWaitingQueue().empty()) {
            cladire->getFloors()[j].removePassenger();
        }
    }

    ui.btnStartSimulare->setEnabled(true);
    ui.btnStartSimulare->setText("START SIMULARE");
    ui.btnStartSimulare->setStyleSheet("background-color: #27ae60; color: white; font-weight: bold;");
    ui.btnGenerare->setEnabled(true);
    
    ui.btnUrgenta->setStyleSheet("background-color: #ff0000; color: white; font-weight: bold;");
}

void MainWindow::on_btnUrgenta_clicked() {
    QPushButton* btnUrgenta = ui.paginiPrincipale->findChild<QPushButton*>("btnUrgenta");

    if (!cladire) return;

    regimUrgenta = !regimUrgenta;

    auto& elevators = cladire->getElevators();

    for (int i = 0; i < elevators.size(); ++i) {
        if (!std::dynamic_pointer_cast<EmergencyElevator>(elevators[i])) {
            if (regimUrgenta) {
                elevators[i]->setStatus(ElevatorStatus::OUT_OF_SERVICE);
                lifturiGrafice[i]->setStyleSheet("background-color: #2c3e50; border: 2px solid #1a1a1a; opacity: 0.5;");
            }
            else {
                elevators[i]->setStatus(ElevatorStatus::IDLE);
                if (std::dynamic_pointer_cast<FreightElevator>(elevators[i]))
                    lifturiGrafice[i]->setStyleSheet("background-color: #95a5a6; border: 2px solid #7f8c8d;");
                else
                    lifturiGrafice[i]->setStyleSheet("background-color: #3498db; border: 2px solid #2980b9;");
            }
        }
        else {
            if (regimUrgenta)
                lifturiGrafice[i]->setStyleSheet("background-color: #e74c3c; border: 3px solid #f1c40f;");
            else
                lifturiGrafice[i]->setStyleSheet("background-color: #e74c3c; border: 2px solid #c0392b;");
        }
    }

    if (regimUrgenta) {
        btnUrgenta->setText("⚠️ MOD URGENTA ACTIV");
        btnUrgenta->setStyleSheet("background-color: #c0392b; color: white; font-weight: bold; border: 2px solid white;");
    }
    else {
        btnUrgenta->setText("BUTON URGENTA");
        btnUrgenta->setStyleSheet("background-color: #7f8c8d; color: white;");
    }

    qDebug() << "Regim urgenta:" << (regimUrgenta ? "ACTIV" : "INACTIV");
}

void MainWindow::on_btnRandomizeaza_clicked() {
    // generam cladirea
    int nrEtaje = QRandomGenerator::global()->bounded(3, 6);   // 3, 4 sau 5
    int nrLifturi = QRandomGenerator::global()->bounded(3, 6); // 3, 4 sau 5

    ui.spinEtaje->setValue(nrEtaje);
    ui.spinLifturi->setValue(nrLifturi);

    // distribuim lifturile
    for (int i = 0; i < nrLifturi; ++i) {
        QComboBox* combo = qobject_cast<QComboBox*>(ui.tableLifturi->cellWidget(i, 1));
        if (combo) {
            if (i == nrLifturi - 1) {
                combo->setCurrentText("Emergency"); // Ultimul mereu urgenta
            }
            else if (i < nrLifturi / 2) {
                combo->setCurrentText("Passenger"); // Prima jumatate Pasageri
            }
            else {
                combo->setCurrentText("Freight");   // In mijloc marfa
            }
        }
    }

    on_btnGenerare_clicked();

    // generam obiecte
    int nrObiecte = QRandomGenerator::global()->bounded(3, 8);

    for (int i = 0; i < nrObiecte; ++i) {
        int tip = QRandomGenerator::global()->bounded(0, 3); // 0 (std), 1 (VIP), 2 (cargo)

        // etaje random
        int start = QRandomGenerator::global()->bounded(0, nrEtaje);
        int dest = QRandomGenerator::global()->bounded(0, nrEtaje);
        while (start == dest) {
            dest = QRandomGenerator::global()->bounded(0, nrEtaje);
        }

        double weight = 0;
        QString infoPasager;
        QString emoji;
        std::shared_ptr<ITransportable> itemNou = nullptr;

        if (tip == 0) { // Standard
            weight = QRandomGenerator::global()->bounded(75, 121); // 75 - 120 kg
            std::string nume = "Pasager_R" + std::to_string(i + 1);
            itemNou = std::make_shared<StandardPassenger>(nume, weight, start, dest);
            infoPasager = QString::fromStdString(nume);
            emoji = "👤";
        }
        else if (tip == 1) { // VIP
            weight = QRandomGenerator::global()->bounded(75, 121); // 75 - 120 kg
            std::string nume = "VIP_R" + std::to_string(i + 1);
            int priority = QRandomGenerator::global()->bounded(1, 6); // Prioritate 1-5
            itemNou = std::make_shared<VIPPassenger>(nume, weight, start, dest, priority);
            infoPasager = QString::fromStdString(nume) + " (VIP)";
            emoji = "🌟";
        }
        else if (tip == 2) { // Cargo
            weight = QRandomGenerator::global()->bounded(400, 801); // 400 - 800 kg
            bool fragile = (QRandomGenerator::global()->bounded(0, 2) == 1); // 50% șanse să fie fragil
            itemNou = std::make_shared<CargoBox>(weight, fragile, start, dest);
            infoPasager = "Cargo" + QString(fragile ? " [!]" : "");
            emoji = "📦";
        }

        if (itemNou) {
            cladire->getFloors()[start].addPassenger(itemNou);

            QLabel* pLabel = new QLabel(emoji + " " + infoPasager + " -> " + QString::number(dest));

            QString stil = "font-weight: bold; border-radius: 5px; padding: 2px; ";
            if (emoji == "🌟") {
                stil += "color: #f1c40f; background-color: rgba(44, 62, 80, 220); border: 2px solid gold;";
            }
            else if (emoji == "📦") {
                stil += "color: #ecf0f1; background-color: #d35400; border: 1px solid white;";
            }
            else {
                stil += "color: #f1c40f; background-color: rgba(44, 62, 80, 200); border: 1px solid #f39c12;";
            }

            pLabel->setStyleSheet(stil);

            if (!waitingAreas.isEmpty() && start < waitingAreas[0].size()) {
                waitingAreas[0][start]->addWidget(pLabel);
            }
        }
    }
}

void MainWindow::on_btnLoadFile_clicked() {
    // deschidem dialogul
    QString fileName = QFileDialog::getOpenFileName(this, "Incarca Scenariu JSON", "", "JSON Files (*.json);;All Files (*)");
    if (fileName.isEmpty()) return; // Utilizatorul a dat cancel

    // citim fisierul
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Eroare", "Nu am putut deschide fisierul!");
        return;
    }

    QString rawData = file.readAll();
    file.close();

    // citim din json
    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(rawData.toUtf8(), &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        QMessageBox::critical(this, "Eroare JSON", "Fisierul JSON este invalid:\n" + parseError.errorString());
        return;
    }

    QJsonObject rootObj = jsonDoc.object();

    // construim cladirea
    QJsonObject cladireObj = rootObj["cladire"].toObject();
    int nrEtaje = cladireObj["etaje"].toInt();
    QJsonArray lifturiArray = cladireObj["lifturi"].toArray();
    int nrLifturi = lifturiArray.size();

    // punem valorile in UI
    ui.spinEtaje->setValue(nrEtaje);
    ui.spinLifturi->setValue(nrLifturi);

    // selectam lifturile
    for (int i = 0; i < nrLifturi; ++i) {
        QComboBox* combo = qobject_cast<QComboBox*>(ui.tableLifturi->cellWidget(i, 1));
        if (combo) {
            QString tipLift = lifturiArray[i].toString();
            combo->setCurrentText(tipLift);
        }
    }

    on_btnGenerare_clicked();

    // adaugam obiectele si pasagerii
    QJsonArray obiecteArray = rootObj["obiecte"].toArray();

    for (int i = 0; i < obiecteArray.size(); ++i) {
        QJsonObject obj = obiecteArray[i].toObject();

        QString tipStr = obj["tip"].toString();
        int start = obj["start"].toInt();
        int dest = obj["dest"].toInt();
        double weight = obj["greutate"].toDouble();

        QString infoPasager;
        QString emoji;
        std::shared_ptr<ITransportable> itemNou = nullptr;

        if (tipStr == "Standard") {
            std::string nume = obj["nume"].toString().toStdString();
            itemNou = std::make_shared<StandardPassenger>(nume, weight, start, dest);
            infoPasager = QString::fromStdString(nume);
            emoji = "👤";
        }
        else if (tipStr == "VIP") {
            std::string nume = obj["nume"].toString().toStdString();
            int priority = obj["prioritate"].toInt();
            itemNou = std::make_shared<VIPPassenger>(nume, weight, start, dest, priority);
            infoPasager = QString::fromStdString(nume) + " (VIP)";
            emoji = "🌟";
        }
        else if (tipStr == "Cargo") {
            bool fragile = obj["fragil"].toBool();
            itemNou = std::make_shared<CargoBox>(weight, fragile, start, dest);
            infoPasager = "Cargo" + QString(fragile ? " [!]" : "");
            emoji = "📦";
        }

        if (itemNou) {
            cladire->getFloors()[start].addPassenger(itemNou);

            QLabel* pLabel = new QLabel(emoji + " " + infoPasager + " -> " + QString::number(dest));
            QString stil = "font-weight: bold; border-radius: 5px; padding: 2px; ";

            if (emoji == "🌟") {
                stil += "color: #f1c40f; background-color: rgba(44, 62, 80, 220); border: 2px solid gold;";
            }
            else if (emoji == "📦") {
                stil += "color: #ecf0f1; background-color: #d35400; border: 1px solid white;";
            }
            else {
                stil += "color: #f1c40f; background-color: rgba(44, 62, 80, 200); border: 1px solid #f39c12;";
            }

            pLabel->setStyleSheet(stil);

            if (!waitingAreas.isEmpty() && start < waitingAreas[0].size()) {
                waitingAreas[0][start]->addWidget(pLabel);
            }
        }
    }

    QMessageBox::information(this, "Succes", "Scenariul a fost incarcat cu succes!");
}