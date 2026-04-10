#pragma once

#include <QMainWindow>
#include <QFrame>
#include <QVBoxLayout>
#include <QVector>
#include <QHBoxLayout>
#include <QLabel>
#include "ui_MainWindow.h"
#include "AddPassengerDialog.h"

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

private:
	Ui::MainWindowClass ui;
	QVector<QFrame*> lifturiGrafice;

	QVector<QVector<QBoxLayout*>> waitingAreas;
};