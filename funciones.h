#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <QTextEdit>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QMessageBox>
#include <QDataStream>
#include <QString>
#include <QRegularExpression>
#include <fstream>



class Funciones
{
public:
    Funciones();
    void reporteEstudiantes(QTextEdit* texto);
    void productoBinario(QTextEdit* texto);
    void modificarProducto(int code, int stock);
    void leerProducto(QTextEdit* texto);
    void generarCSV();
    void leerCSV(QTextEdit* texto);
    void generarReporteCSV();

};

#endif // FUNCIONES_H
