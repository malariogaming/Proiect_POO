#pragma once

#include <QDialog>
#include <QMessageBox>
#include "ui_AddPassengerDialog.h"

class AddPassengerDialog : public QDialog
{
	Q_OBJECT

public:
	AddPassengerDialog(QWidget *parent = nullptr);
	~AddPassengerDialog();

	void setFloorLimits(int maxFloors);

	int getSelectedType() const; // 0 = standard, 1 = vip, 2 = cargo
	std::string getName() const;
	int getWeight() const;
	int getStartFloor() const;
	int getDestFloor() const;
	int getPriority() const;    // folosit doar pentru vip
	bool isFragile() const;     // folosit doar pentru cargo

private slots:
	void on_comboType_currentIndexChanged(int index);
	void accept();

private:
	Ui::AddPassengerDialog ui;
};