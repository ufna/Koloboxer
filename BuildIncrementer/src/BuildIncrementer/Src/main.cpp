#include <QChar>
#include <QCoreApplication>
#include <QFile>
#include <QString>
#include <QTextStream>

#define ERR_NO_ARGUMENTS 1
#define ERR_FILE_NOT_READABLE 2
#define ERR_FILE_NOT_WRITEABLE 3

#define ARR_SIZE 4

int main(int argc, char *argv[])
{
	if( argc == 1 )
		return ERR_NO_ARGUMENTS;

    QCoreApplication a(argc, argv);

	QFile file( argv[1] );
	QString str;
	QChar *data;
	int pos, k;
	QString substr[ARR_SIZE] = { 
		"FILEVERSION", 
		"\"FileVersion\",",
		"PRODUCTVERSION", 
		"\"ProductVersion\",", 
		 };
	

	if (!file.open(QFile::ReadOnly | QFile::Text))
		return ERR_FILE_NOT_READABLE;

	QTextStream stream(&file);
	str = stream.readAll();

	file.close();

	// FILEVERSION
	pos = str.indexOf(substr[0], 0, Qt::CaseSensitive);
	pos += substr[0].length();
	data = str.data();

	k = 0;
	while(k!= 2){
		if(str.at(pos) == ',')
			k++;
		pos++;
	};

	for(k=1; k <= pos; k++)
		data++;

	k = 0;
	while(str.at(pos+k) != ',')
		k++;
	
	str.replace(pos+1, k, QString("%1").arg( QString(data, k).toInt() + 1) );

	// OTHER
	for( int i=1; i<ARR_SIZE; i++ ) 
	{
		pos = str.indexOf(substr[i], 0, Qt::CaseSensitive);
		pos += substr[i].length();
		data = str.data();

		k = 0;
		while(k!= 2){
			if(str.at(pos) == ',')
				k++;
			pos++;
		}

		for(k=1; k <= pos; k++)
			data++;

		k = 0;
		while(str.at(pos+k) != ',')
			k++;

		str.replace(pos+1, k, QString("%1").arg( QString(data, k).toInt() + 1) );
	}

	if (!file.open(QFile::WriteOnly | QFile::Text))
		return ERR_FILE_NOT_WRITEABLE;

	stream << str;
	stream.flush();

	exit(0);

    return a.exec();
}
