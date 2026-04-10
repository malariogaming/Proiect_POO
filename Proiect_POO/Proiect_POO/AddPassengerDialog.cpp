#include "AddPassengerDialog.h"

AddPassengerDialog::AddPassengerDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

    connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(ui.buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

AddPassengerDialog::~AddPassengerDialog()
{
}

void AddPassengerDialog::setFloorLimits(int maxFloors) {
    ui.spinStart->setMinimum(0);
    ui.spinDest->setMinimum(0);

    ui.spinStart->setMaximum(maxFloors - 1);
    ui.spinDest->setMaximum(maxFloors - 1);
}

void AddPassengerDialog::on_comboType_currentIndexChanged(int index) {
    ui.specificStack->setCurrentIndex(index);
}

int AddPassengerDialog::getSelectedType() const {
    return ui.comboType->currentIndex();
}

std::string AddPassengerDialog::getName() const {
    int type = getSelectedType();
    if (type == 0) return ui.lineNameStandard->text().toStdString();
    if (type == 1) return ui.lineNameVIP->text().toStdString();
    return "Cargo Unit";
}

int AddPassengerDialog::getWeight() const {
    return ui.spinWeight->value();
}

int AddPassengerDialog::getStartFloor() const {
    return ui.spinStart->value();
}

int AddPassengerDialog::getDestFloor() const {
    return ui.spinDest->value();
}

int AddPassengerDialog::getPriority() const {
    return (getSelectedType() == 1) ? ui.spinPriority->value() : 0;
}

bool AddPassengerDialog::isFragile() const {
    return (getSelectedType() == 2) && ui.checkFragile->isChecked();
}

void AddPassengerDialog::accept() {
    if (ui.spinStart->value() == ui.spinDest->value()) {
        QMessageBox::warning(this, "Eroare", "Etajul de start nu poate fi acelasi cu destinatia!");
        return;
    }
    QDialog::accept();
}