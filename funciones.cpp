#include "funciones.h"

Funciones::Funciones() {}

void Funciones::reporteEstudiantes(QTextEdit *texto)
{
    // Obtener el contenido del QTextEdit
    QString contenido = texto->toPlainText();

    // Crear un archivo para guardar la lista de estudiantes y calificaciones
    QFile archivo("lista_estudiantes.txt");
    if (archivo.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&archivo);
        out << contenido;  // Guardar tal cual el contenido
        archivo.close();

        // Mostrar mensaje de éxito al guardar lista de estudiantes
        QMessageBox::information(nullptr, "Éxito", "La lista de estudiantes se guardó correctamente en 'lista_estudiantes.txt'.");
    } else {
        // Mostrar mensaje de error si no se pudo abrir el archivo
        QMessageBox::warning(nullptr, "Error", "No se pudo abrir el archivo 'lista_estudiantes.txt' para escritura.");
        return;
    }

    // Crear un nuevo archivo para el reporte
    QFile archivo_reporte("reporte_estudiantes.txt");
    if (archivo_reporte.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&archivo_reporte);

        // Dividir el contenido en líneas
        QStringList lineas = contenido.split('\n');

        for (const QString& linea : lineas) {
            // Dividir cada línea en nombre y calificación (separados por un espacio)
            QStringList datos = linea.split(' ', Qt::SkipEmptyParts);

            if (datos.size() == 2) {
                QString nombre = datos[0];
                int calificacion = datos[1].toInt();

                // Verificar si aprobó o reprobó
                if (calificacion < 60) {
                    out << nombre << " reprobó con calificación de " << calificacion << "\n";
                } else {
                    out << nombre << " aprobó con calificación de " << calificacion << "\n";
                }
            }
        }

        archivo_reporte.close();

        // Mostrar mensaje de éxito al guardar el reporte
        QMessageBox::information(nullptr, "Éxito", "El reporte se generó correctamente en 'reporte_estudiantes.txt'.");
    } else {
        // Mostrar mensaje de error si no se pudo abrir el archivo
        QMessageBox::warning(nullptr, "Error", "No se pudo abrir el archivo 'reporte_estudiantes.txt' para escritura.");
    }
}

void Funciones::productoBinario(QTextEdit *texto)
{
 //  // Abrir el archivo binario en modo de escritura (crea el archivo si no existe)
    // Abrir el archivo binario en modo de escritura (crea el archivo si no existe)
    QFile archivo("productos.bin");
    if (!archivo.open(QIODevice::WriteOnly)) {
        // Mostrar mensaje de error si no se puede abrir el archivo
        QMessageBox::warning(nullptr, "Error", "No se pudo abrir el archivo 'productos.bin' para escritura.");
        return;
    }

    QDataStream out(&archivo);
    out.setVersion(QDataStream::Qt_5_15);

    // Obtener el texto del QTextEdit
    QString contenido = texto->toPlainText();
    QStringList lineas = contenido.split("\n", Qt::SkipEmptyParts);  // Dividimos el texto por líneas

    // Expresión regular para verificar el formato: codigo - nombre - precio - stock
    QRegularExpression regex("^\\s*\\d+\\s*-\\s*\\w+\\s*-\\s*\\d+\\.?\\d*\\s*-\\s*\\d+\\s*$");

    // Verificar cada línea para asegurar que siga el formato correcto
    foreach (QString linea, lineas) {
        // Verificar si la línea cumple con el formato correcto
        if (!regex.match(linea).hasMatch()) {
            // Si la línea no tiene el formato correcto, mostrar advertencia y saltar esa línea
            QMessageBox::warning(nullptr, "Formato incorrecto", "Una de las líneas no tiene el formato correcto. Debe tener: codigo - nombre - precio - stock.");
            return;
        }

        // Dividimos la línea por el carácter '-'
        QStringList partes = linea.split("-", Qt::SkipEmptyParts);

        // Eliminar espacios extra alrededor de los datos
        QString codigo = partes[0].trimmed();
        QString nombre = partes[1].trimmed();
        bool okPrecio, okStock;
        float precio = partes[2].trimmed().toFloat(&okPrecio);
        int stock = partes[3].trimmed().toInt(&okStock);

        // Verificar que la conversión fue exitosa
        if (!okPrecio || !okStock) {
            QMessageBox::warning(nullptr, "Error", "Error en la conversión de precio o stock en una de las líneas.");
            continue;
        }

        // Escribir los datos en el archivo binario
        out << codigo << nombre << precio << stock;
    }

    // Cerrar el archivo
    archivo.close();

    // Mostrar mensaje de éxito
    QMessageBox::information(nullptr, "Éxito", "Los productos fueron guardados correctamente en el archivo binario.");

}

void Funciones::modificarProducto(int code, int stock)
{
    // Abrir el archivo binario para leer/escribir
    QFile archivo("productos.bin");
    if (!archivo.open(QIODevice::ReadWrite)) {
        // Mostrar mensaje de error si no se pudo abrir el archivo
        QMessageBox::warning(nullptr, "Error", "No se pudo abrir el archivo 'productos.bin' para lectura/escritura.");
        return;
    }

    QDataStream inOut(&archivo);
    inOut.setVersion(QDataStream::Qt_5_15);

    // Variables para almacenar los datos de cada producto
    QString codigo;
    QString nombre;
    float precio;
    int stockExistente;  // Renombrado para evitar sombra con el parámetro

    // Buscar el producto por código
    bool productoEncontrado = false;

    while (!inOut.atEnd()) {
        // Obtener la posición actual para volver a escribir si encontramos el producto
        qint64 posicion = archivo.pos();

        // Leer los datos del archivo
        inOut >> codigo >> nombre >> precio >> stockExistente;  // Usamos stockExistente

        // Si el código coincide con el que buscamos
        if (codigo.toInt() == code) {
            productoEncontrado = true;

            // Mover el puntero del archivo de nuevo a la posición inicial de este producto
            archivo.seek(posicion);

            // Escribir los mismos datos pero con el nuevo valor de stock
            inOut << codigo << nombre << precio << stock;  // Usamos stock (el parámetro de la función)

            QMessageBox::information(nullptr, "Éxito", "El stock del producto con código " + QString::number(code) + " ha sido modificado.");
            break;
        }
    }

    // Si no se encontró el producto
    if (!productoEncontrado) {
        QMessageBox::warning(nullptr, "Error", "No se encontró un producto con el código " + QString::number(code) + ".");
    }

    // Cerrar el archivo
    archivo.close();
}

void Funciones::leerProducto(QTextEdit *texto)
{
    // Abrir el archivo binario en modo lectura
    QFile archivo("productos.bin");
    if (!archivo.open(QIODevice::ReadOnly)) {
        // Mostrar mensaje de error si no se pudo abrir el archivo
        QMessageBox::warning(nullptr, "Error", "No se pudo abrir el archivo 'productos.bin' para lectura.");
        return;
    }

    QDataStream in(&archivo);
    in.setVersion(QDataStream::Qt_5_15);

    // Limpiar el QTextEdit antes de mostrar los nuevos datos
    texto->clear();

    // Variables para almacenar los datos de cada producto
    QString codigo;
    QString nombre;
    float precio;
    int stock;

    // Leer el archivo binario
    while (!in.atEnd()) {
        // Leer los datos del archivo
        in >> codigo >> nombre >> precio >> stock;

        // Formatear los datos en una cadena
        QString producto = codigo + " " + nombre + " " + QString::number(precio, 'f', 2) + " " + QString::number(stock);

        // Agregar el producto al QTextEdit
        texto->append(producto);
    }

    // Cerrar el archivo después de leer
    archivo.close();

    // Mostrar mensaje de éxito
    QMessageBox::information(nullptr, "Éxito", "Los productos se han leído correctamente.");
}

void Funciones::generarCSV()
{
    std::ofstream archivo("carros.csv");
    if (!archivo) {
        QMessageBox::warning(nullptr,"Alerta", "No se pudo crear el archivo 'carros.csv'");
        return;
    }

    // Escribir los encabezados de las columnas
    archivo << "Marca,Modelo,Año,Tamaño de Motor (L),Precio (USD),Prima (USD)" << std::endl;

    // Escribir 15 listados de carros
    archivo << "Toyota,Corolla,2021,1.8,20000,1500" << std::endl;
    archivo << "Honda,Civic,2020,2.0,22000,1600" << std::endl;
    archivo << "Ford,F-150,2019,3.5,35000,1800" << std::endl;
    archivo << "Chevrolet,Malibu,2021,2.5,24000,1700" << std::endl;
    archivo << "BMW,Serie 3,2022,2.0,42000,2500" << std::endl;
    archivo << "Mercedes-Benz,Clase C,2021,2.0,43000,2600" << std::endl;
    archivo << "Audi,A4,2020,2.0,41000,2400" << std::endl;
    archivo << "Nissan,Altima,2019,2.5,23000,1600" << std::endl;
    archivo << "Hyundai,Elantra,2021,1.6,19000,1400" << std::endl;
    archivo << "Kia,Sportage,2020,2.4,25000,1700" << std::endl;
    archivo << "Mazda,CX-5,2022,2.5,28000,1800" << std::endl;
    archivo << "Tesla,Model 3,2021,Electric,45000,3000" << std::endl;
    archivo << "Volkswagen,Jetta,2020,1.4,21000,1500" << std::endl;
    archivo << "Subaru,Impreza,2019,2.0,20000,1450" << std::endl;
    archivo << "Jeep,Wrangler,2022,3.6,40000,2800" << std::endl;

    // Cerrar el archivo
    archivo.close();

    QMessageBox::information(nullptr, "Aviso", "Archivo CSV generado exitosamente.");
}

void Funciones::leerCSV(QTextEdit* texto)
{
    // Abre el archivo CSV
    QFile archivo("carros.csv");
    if (!archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(nullptr, "Error", "No se pudo abrir el archivo 'carros.csv'.");
        return;
    }

    // Crea un QTextStream para leer el contenido del archivo
    QTextStream in(&archivo);
    QString contenido;

    // Lee el contenido del archivo línea por línea
    while (!in.atEnd()) {
        QString linea = in.readLine();
        contenido.append(linea + "\n");
    }

    // Cierra el archivo
    archivo.close();

    // Muestra el contenido en el QTextEdit
    texto->setPlainText(contenido);
}

void Funciones::generarReporteCSV()
{
    QFile archivo("carros.csv");
    if (!archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(nullptr, "Error", "No se pudo abrir el archivo 'carros.csv'.");
        return;
    }

    QTextStream in(&archivo);
    QString linea;
    QStringList headers;

    // Variables para calcular estadísticas
    int totalAutos = 0;
    double totalValor = 0.0;
    double totalPrima = 0.0;
    double valorMinimo = std::numeric_limits<double>::max();
    double valorMaximo = std::numeric_limits<double>::min();
    QString autoMasAccesible;
    QString autoMasCaro;

    // Leer el encabezado
    if (!in.atEnd()) {
        linea = in.readLine();
        headers = linea.split(',');
    }

    // Leer los datos y calcular estadísticas
    while (!in.atEnd()) {
        linea = in.readLine();
        QStringList datos = linea.split(',');
        if (datos.size() == 6) {
            QString marca = datos[0];
            QString modelo = datos[1];
            QString anio = datos[2];
            QString tamanoMotor = datos[3];
            double precio = datos[4].toDouble();
            double prima = datos[5].toDouble();

            // Calcular estadísticas
            totalAutos++;
            totalValor += precio;
            totalPrima += prima;

            if (precio < valorMinimo) {
                valorMinimo = precio;
                autoMasAccesible = marca + " " + modelo;
            }
            if (precio > valorMaximo) {
                valorMaximo = precio;
                autoMasCaro = marca + " " + modelo;
            }
        }
    }

    archivo.close();

    // Calcular promedios
    double promedioValor = (totalAutos > 0) ? totalValor / totalAutos : 0;
    double promedioPrima = (totalAutos > 0) ? totalPrima / totalAutos : 0;

    // Crear el archivo de reporte
    QFile reporte("reporte_estadisticas.txt");
    if (!reporte.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(nullptr, "Error", "No se pudo crear el archivo 'reporte_estadisticas.txt'.");
        return;
    }

    QTextStream out(&reporte);

    // Escribir el reporte
    out << "Reporte de Estadísticas de Autos\n";
    out << "--------------------------------\n";
    out << "Total de Autos: " << totalAutos << "\n";
    out << "Auto Más Accesible: " << autoMasAccesible << " (Precio: " << valorMinimo << " USD)\n";
    out << "Auto Más Caro: " << autoMasCaro << " (Precio: " << valorMaximo << " USD)\n";
    out << "Promedio del Valor de los Autos: " << promedioValor << " USD\n";
    out << "Promedio de la Prima: " << promedioPrima << " USD\n";

    reporte.close();

    QMessageBox::information(nullptr, "Aviso","Archivo 'reporte_estadisticas.txt' generado exitosamente.");
}

