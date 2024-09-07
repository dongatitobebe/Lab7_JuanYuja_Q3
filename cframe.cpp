#include "cframe.h"
#include "ui_cframe.h"
#include <fstream>
#include <QString>
#include <QMessageBox>
#include "funciones.h"

cframe::cframe(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::cframe)
{
    ui->setupUi(this);

}

Funciones fun;

cframe::~cframe()
{
    delete ui;
}


// boton salir

void cframe::on_bton_Salir_clicked()
{
    exit(0);
}


void cframe::on_pushButton_clicked()
{
    fun.reporteEstudiantes(ui->teEstudiantes);
}


void cframe::on_pushButton_7_clicked()
{


    //s
}


void cframe::on_pushButton_4_clicked()
{

    fun.modificarProducto(ui->sbcodigo->value(),ui->sbstock->value());
}


void cframe::on_pushButton_2_clicked()
{
    fun.productoBinario(ui->teProducto);
}


void cframe::on_pushButton_3_clicked()
{
    fun.leerProducto(ui->teProducto);
}


void cframe::on_pushButton_5_clicked()
{
    fun.generarCSV();
    fun.leerCSV(ui->teCSV);
}


void cframe::on_pushButton_6_clicked()
{
    fun.generarReporteCSV();
}

