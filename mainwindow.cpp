#include "mainwindow.h"

mainwindow::mainwindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    setWindowTitle("Draw word:)");
    QWidget* MyWidget = new QWidget(this);
    MyWidget->setLayout(ui.gridLayout);
    setCentralWidget(MyWidget);
    resize(390, 300);

    NW_config = ReadDataNetWork("Config.txt");
    NW.Init(NW_config);
    NW.ReadWeights();
    QWidget* WProg = new QWidget();
    QGridLayout* phbxLayout = new QGridLayout();
    ph = new QProgressBar * [26];
    lbl = new QLabel * [26];
    char w = 97;
    for (int i = 0; i < 26; i++) {
        ph[i] = new QProgressBar();
        lbl[i] = new QLabel();
        QString h = w;
        lbl[i]->setText(h.toUpper());
        phbxLayout->addWidget(lbl[i], i, 0);
        phbxLayout->addWidget(ph[i], i, 1);
        w++;
    }
    WProg->setLayout(phbxLayout);
    WProg->show();
    //ui.lcdNumber->display("");
}
void mainwindow::clearLCD() {
    //ui.lcdNumber->display("");
}
void mainwindow::ReadTest(double* input, int input_n) {
    QFile mFile("test.txt");
    mFile.open(QIODevice::ReadOnly);
    QTextStream fin(&mFile);
    if (mFile.isOpen()) {
        for (int j = 0; j < input_n; j++)
            fin >> input[j];
    }
    else QMessageBox::information(0, "Error", "Error reading the file");
    mFile.close();
}
void mainwindow::StartGuess() {
    double* input = new double[NW_config.size[0]];
    ReadTest(input, NW_config.size[0]);
    NW.SetInput(input);
    double* words = NW.ForwardFeed();
    double w = NW.SearchMaxIndex(words);
    char word = (int)w;
    word += 97;
    QString h = word;
    ui.label->setText(h.toUpper());
    for (int i = 0; i < 26; i++) {
        ph[i]->setValue(words[i] * 100);
    }
}
data_NetWork mainwindow::ReadDataNetWork(string path) {
    data_NetWork data{};
    ifstream fin;
    fin.open(path);
    if (!fin.is_open()) {
        cout << "Error reading the file " << path << endl;
        throw runtime_error("rror reading the file\n");
    }
    else
        cout << path << " loading...\n";
    string tmp;
    int n;
    while (!fin.eof()) {
        fin >> tmp;
        if (tmp == "NetWork") {
            fin >> n;
            data.L = n;
            data.size = new int[n];
            for (int i = 0; i < n; i++) {
                fin >> data.size[i];
            }
        }
    }
    fin.close();
    return data;
}