#include "mainwindow.h"
#include "hashsetopen.h"
#include "hashsetclosed.h"
#include "dijkstra.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("BISA4110 - Data Structures");
    resize(900, 700);

    setStyleSheet(
        "QWidget { background-color: #000000; color: #ffffff; font-family: 'Segoe UI'; font-size: 14px; }"
        "QPushButton { background-color: #000000; color: #ffffff; border: 2px solid #ffffff; border-radius: 4px; padding: 10px 20px; font-weight: bold; }"
        "QPushButton:hover { background-color: #ffffff; color: #000000; }"
        "QLineEdit { background-color: #111111; color: #ffffff; border: 2px solid #555555; padding: 8px; }"
        "QTextEdit { background-color: #0a0a0a; color: #ffffff; border: 2px solid #ffffff; font-family: 'Consolas'; }"
        );

    stack = new QStackedWidget(this);
    stack->addWidget(buildMenuPage());
    stack->addWidget(buildOpenPage());
    stack->addWidget(buildClosedPage());
    stack->addWidget(buildDijkstraHardPage());
    stack->addWidget(buildDijkstraFilePage());
    stack->addWidget(buildHelpPage());
    setCentralWidget(stack);

    showMenu();
}

MainWindow::~MainWindow() {}

void MainWindow::showMenu() { stack->setCurrentIndex(0); }
void MainWindow::showOpenPage() { stack->setCurrentIndex(1); }
void MainWindow::showClosedPage() { stack->setCurrentIndex(2); }
void MainWindow::showDijkstraHardPage() { stack->setCurrentIndex(3); }
void MainWindow::showDijkstraFilePage() { stack->setCurrentIndex(4); }
void MainWindow::showHelpPage() { stack->setCurrentIndex(5); }

QPushButton* createBackButton(MainWindow* mw) {
    QPushButton* btn = new QPushButton("<- Back");
    QObject::connect(btn, &QPushButton::clicked, mw, &MainWindow::showMenu);
    return btn;
}

QWidget* MainWindow::buildMenuPage() {
    QWidget* page = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(15);

    QLabel* title = new QLabel("BISA4110\nDATA STRUCTURES GROUP ASSIGNMENT 2026");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 28px; font-weight: bold; margin-bottom: 20px;");
    layout->addWidget(title);

    auto addBtn = [&](QString text, auto slot) {
        QPushButton* b = new QPushButton(text);
        b->setMinimumSize(400, 50);
        b->setCursor(Qt::PointingHandCursor);
        connect(b, &QPushButton::clicked, this, slot);
        layout->addWidget(b, 0, Qt::AlignCenter);
    };

    addBtn("1. HashSetOpen (Open Hashing)", &MainWindow::showOpenPage);
    addBtn("2. HashSetClosed (Closed Hashing)", &MainWindow::showClosedPage);
    addBtn("3. Dijkstra (Hard-coded Graph)", &MainWindow::showDijkstraHardPage);
    addBtn("4. Dijkstra (Load Graph from File)", &MainWindow::showDijkstraFilePage);
    addBtn("5. Show graph.txt Format", &MainWindow::showHelpPage);

    QPushButton* exitBtn = new QPushButton("0. Exit");
    exitBtn->setMinimumSize(400, 50);
    connect(exitBtn, &QPushButton::clicked, this, &QWidget::close);
    layout->addWidget(exitBtn, 0, Qt::AlignCenter);

    return page;
}

QWidget* MainWindow::buildOpenPage() {
    QWidget* page = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(page);
    QHBoxLayout* topBar = new QHBoxLayout;
    topBar->addWidget(new QLabel("<h3>Open Hashing</h3>"));
    topBar->addStretch();
    QPushButton* runBtn = new QPushButton("Run Demo");
    connect(runBtn, &QPushButton::clicked, this, &MainWindow::runOpenDemo);
    topBar->addWidget(runBtn);
    topBar->addWidget(createBackButton(this));
    layout->addLayout(topBar);

    openOutput = new QTextEdit; openOutput->setReadOnly(true);
    layout->addWidget(openOutput);
    return page;
}

void MainWindow::runOpenDemo() {
    QString log;
    try {
        HashSetOpen set(7);
        set.insert(10, "Mamosa"); set.insert(15, "Thabo"); set.insert(20, "Mpho");
        set.insert(17, "Lerato"); set.insert(24, "Khotso");
        log += "Initial table:\n" + set.display() + "\n";
        log += QString("Size: %1\n\n").arg(set.size());

        std::string val;
        if (set.get(10, val)) log += QString("Get 10: %1\n").arg(QString::fromStdString(val));
        log += QString("Contains 99: %1\n").arg(set.contains(99) ? "yes" : "no");
        log += QString("Remove 17: %1\n").arg(set.remove(17) ? "success" : "fail");
        log += QString("Contains 17 after removal: %1\n\n").arg(set.contains(17) ? "yes" : "no");

        log += "Final table:\n" + set.display();
    } catch (const std::exception& e) { log += e.what(); }
    openOutput->setPlainText(log);
}

QWidget* MainWindow::buildClosedPage() {
    QWidget* page = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(page);
    QHBoxLayout* topBar = new QHBoxLayout;
    topBar->addWidget(new QLabel("<h3>Closed Hashing</h3>"));
    topBar->addStretch();
    QPushButton* runBtn = new QPushButton("Run Demo");
    connect(runBtn, &QPushButton::clicked, this, &MainWindow::runClosedDemo);
    topBar->addWidget(runBtn);
    topBar->addWidget(createBackButton(this));
    layout->addLayout(topBar);

    closedOutput = new QTextEdit; closedOutput->setReadOnly(true);
    layout->addWidget(closedOutput);
    return page;
}

void MainWindow::runClosedDemo() {
    QString log;
    try {
        HashSetClosed set(7);
        log += QString("Initial capacity: %1\n\n").arg(set.getCapacity());
        set.insert(10, "Mamosa"); set.insert(17, "Lerato");
        set.insert(24, "Khotso"); set.insert(15, "Thabo");
        log += "Initial table:\n" + set.display() + "\n";

        set.remove(10);
        log += "After removing 10:\n" + set.display() + "\n";

        log += "--- Triggering Rehash ---\n";
        set.insert(30, "Palesa"); set.insert(40, "Neo");
        set.insert(50, "Teboho"); set.insert(60, "Nthabiseng");

        for (const auto& msg : set.rehashLog()) log += msg + "\n";
        log += "\nFinal table:\n" + set.display();
    } catch (const std::exception& e) { log += e.what(); }
    closedOutput->setPlainText(log);
}

QWidget* MainWindow::buildDijkstraHardPage() {
    QWidget* page = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(page);
    QHBoxLayout* topBar = new QHBoxLayout;
    topBar->addWidget(new QLabel("<h3>Dijkstra (Hard-coded)</h3>"));
    topBar->addStretch();
    QPushButton* runBtn = new QPushButton("Run Demo");
    connect(runBtn, &QPushButton::clicked, this, &MainWindow::runDijkstraHard);
    topBar->addWidget(runBtn);
    topBar->addWidget(createBackButton(this));
    layout->addLayout(topBar);

    hardOutput = new QTextEdit; hardOutput->setReadOnly(true);
    layout->addWidget(hardOutput);
    return page;
}

void MainWindow::runDijkstraHard() {
    QString log;
    std::unordered_map<std::string, std::vector<Edge>> graph;
    graph["A"] = { Edge("B", 3), Edge("C", 1) };
    graph["B"] = { Edge("D", 2) };
    graph["C"] = { Edge("D", 4) };
    graph["D"] = {};

    DijkstraResult res = runDijkstra(graph, "A");
    if (!res.valid) { hardOutput->setPlainText(res.errorMessage); return; }

    log += "Shortest distances from A:\n";
    for (std::string v : {"A", "B", "C", "D"}) {
        int d = res.distances.at(v);
        log += QString("  %1: %2\n").arg(QString::fromStdString(v))
                   .arg(d == INT_MAX ? "unreachable" : QString::number(d));
    }
    log += "\nPath to D: " + printPath(reconstructPath(res.previous, "A", "D"));
    hardOutput->setPlainText(log);
}

QWidget* MainWindow::buildDijkstraFilePage() {
    QWidget* page = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(page);
    QHBoxLayout* topBar = new QHBoxLayout;
    topBar->addWidget(new QLabel("<h3>Dijkstra from File</h3>"));
    topBar->addStretch();
    topBar->addWidget(createBackButton(this));
    layout->addLayout(topBar);

    QHBoxLayout* fileRow = new QHBoxLayout;
    filePathEdit = new QLineEdit; filePathEdit->setPlaceholderText("Select graph.txt...");
    QPushButton* browseBtn = new QPushButton("Browse...");
    connect(browseBtn, &QPushButton::clicked, this, &MainWindow::browseFile);
    fileRow->addWidget(filePathEdit); fileRow->addWidget(browseBtn);
    layout->addLayout(fileRow);

    QHBoxLayout* actionRow = new QHBoxLayout;
    targetEdit = new QLineEdit; targetEdit->setPlaceholderText("Target (e.g. D)");
    QPushButton* runBtn = new QPushButton("Compute");
    connect(runBtn, &QPushButton::clicked, this, &MainWindow::runDijkstraFile);
    actionRow->addWidget(targetEdit); actionRow->addWidget(runBtn);
    layout->addLayout(actionRow);

    fileOutput = new QTextEdit; fileOutput->setReadOnly(true);
    layout->addWidget(fileOutput);
    return page;
}

void MainWindow::browseFile() {
    QString file = QFileDialog::getOpenFileName(this, "Select Graph File", "", "Text Files (*.txt)");
    if (!file.isEmpty()) filePathEdit->setText(file);
}

void MainWindow::runDijkstraFile() {
    QFile file(filePathEdit->text());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        fileOutput->setPlainText("Could not open file.");
        return;
    }
    QTextStream in(&file);
    std::unordered_map<std::string, std::vector<Edge>> graph;
    std::string source = in.readLine().trimmed().toStdString();
    int edges = in.readLine().toInt();

    for (int i = 0; i < edges; i++) {
        QString line = in.readLine();
        QStringList parts = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        if (parts.size() == 3) {
            graph[parts[0].toStdString()].push_back(Edge(parts[1].toStdString(), parts[2].toInt()));
            if (graph.find(parts[1].toStdString()) == graph.end()) graph[parts[1].toStdString()] = {};
        }
    }
    if (graph.find(source) == graph.end()) graph[source] = {};

    DijkstraResult res = runDijkstra(graph, source);
    QString log = QString("Source: %1\n\n").arg(QString::fromStdString(source));

    if (!res.valid) { fileOutput->setPlainText(log + res.errorMessage); return; }

    for (const auto& p : res.distances) {
        log += QString("%1: %2\n").arg(QString::fromStdString(p.first))
        .arg(p.second == INT_MAX ? "unreachable" : QString::number(p.second));
    }

    QString target = targetEdit->text().trimmed();
    if (!target.isEmpty() && res.distances.count(target.toStdString())) {
        log += "\nPath: " + printPath(reconstructPath(res.previous, source, target.toStdString()));
    }
    fileOutput->setPlainText(log);
}

QWidget* MainWindow::buildHelpPage() {
    QWidget* page = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(page);
    QHBoxLayout* topBar = new QHBoxLayout;
    topBar->addWidget(new QLabel("<h3>graph.txt Format</h3>"));
    topBar->addStretch();
    topBar->addWidget(createBackButton(this));
    layout->addLayout(topBar);

    QTextEdit* helpText = new QTextEdit;
    helpText->setReadOnly(true);
    helpText->setPlainText("Line 1: Source\nLine 2: Number of edges\nNext lines: from to weight\n\nA\n4\nA B 3\nA C 1\nB D 2\nC D 4");
    layout->addWidget(helpText);
    return page;
}