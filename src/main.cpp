#include <QApplication>
#include<QWidget>
#include<QVBoxLayout>
#include<QHBoxLayout>
#include<QLabel>
#include<QSpinBox>
#include<QGridLayout>
#include<QLineEdit>
#include<vector>

int main(int argc, char *argv[])
	{
	 QApplication app(argc, argv);

	QWidget window;
	window.setWindowTitle("Matrix Calculator v1.0");
	window.resize(800, 500);
//----------------------------------------------------------------------------------------------------------------
	QVBoxLayout *mainlayout = new QVBoxLayout(&window);

	QLabel *title = new QLabel("Welcome to MATRIX CALCULATOR ");

	mainlayout->addWidget(title);

	//Matrix Order Control--------------------------

	QHBoxLayout *sizeLayout = new QHBoxLayout;

	QLabel *rowsLabel = new QLabel("Matrix Order:");

	QSpinBox *rowsBox = new QSpinBox;
	rowsBox->setRange(1, 10);
	rowsBox->setValue(2);

	QLabel *xLabel = new QLabel("x");

	QSpinBox *columnsBox = new QSpinBox;
	columnsBox->setRange(1, 10);
	columnsBox->setValue(2);

	sizeLayout->addWidget(rowsLabel);
	sizeLayout->addWidget(rowsBox);
	sizeLayout->addWidget(xLabel);
	sizeLayout->addWidget(columnsBox);

	mainlayout->addLayout(sizeLayout);

	QLabel *matrixLabel = new QLabel("Matrix A");
	mainlayout->addWidget(matrixLabel);

	QGridLayout *matrixGrid = new QGridLayout;
	mainlayout->addLayout(matrixGrid);


	//Store The Input Boxes---------------------------
	std::vector<QLineEdit*>cells;

	//Create matrix Grid------------------------------
	auto createMatrix =[&]()
	{
	//remove old cells---------
	for (QLineEdit *cell:cells)
	{ cell->deleteLater(); }
	cells.clear();

	int rows = rowsBox->value();
	int columns = columnsBox->value();

	//Create new cells-----------
	for (int i =0 ;i<rows;i++)
	{ for (int j=0;j<columns;j++)
		{ 
		QLineEdit *cell = new QLineEdit;
		cell->setFixedSize(70,40);
		cell->setPlaceholderText("0");

		matrixGrid->addWidget(cell,i ,j);
	cells.push_back(cell);
		}
	}
	};

	//Create initial matrix--------
	createMatrix();

	// Recreate matrix when size changes-------
	QObject::connect(rowsBox,
		QOverload<int>::of(&QSpinBox::valueChanged),
		createMatrix);

	QObject::connect(columnsBox,
		QOverload<int>::of(&QSpinBox::valueChanged),
		createMatrix);



//---------------------------------------------------------------------------------------------
	window.show();


	return app.exec();

	}
