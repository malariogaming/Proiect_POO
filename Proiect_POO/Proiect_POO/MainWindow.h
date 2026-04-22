#pragma once

#include <QMainWindow>
#include <QFrame>
#include <QVBoxLayout>
#include <QVector>
#include <QHBoxLayout>
#include <QLabel>
#include <QFileDialog>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include "ui_MainWindow.h"
#include "AddPassengerDialog.h"

#include "Building.h"
#include "Scheduler.h"
#include "DatabaseManager.h"
#include <QTimer>
#include <QRandomGenerator>

#include "PassengerElevator.h"
#include "FreightElevator.h"
#include "EmergencyElevator.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private slots:
	void on_btnGenerare_clicked();
	void on_btnDeschideDialog_clicked();
	void on_spinLifturi_valueChanged(int nr);
	void on_btnStartSimulare_clicked();
	void actualizeazaInterfata();
	void on_btnReset_clicked();
	void on_btnUrgenta_clicked();
	void on_btnRandomizeaza_clicked();
	void on_btnLoadFile_clicked();

private:
	Ui::MainWindowClass ui;
	QVector<QFrame*> lifturiGrafice;

	QVector<QVector<QBoxLayout*>> waitingAreas;

	Building* cladire = nullptr;
	DatabaseManager db;
	QTimer* timerSimulare = nullptr;

	bool regimUrgenta = false;
};