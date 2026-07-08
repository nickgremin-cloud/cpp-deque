#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <algorithm>
#include <iterator>
#include <random>
#include <functional>
#include "algo.h"

static std::deque<std::string> tea {
    "Чай Лунцзин",
    "Эрл Грей",
    "Сенча",
    "Пуэр",
    "Дарджилинг",
    "Ассам",
    "Матча",
    "Ганпаудер",
    "Оолонг",
    "Лапсанг Сушонг"
};

static std::deque<std::string> cakes {
    "Красный бархат",
    "Наполеон",
    "Медовик",
    "Тирамису",
    "Прага",
    "Чизкейк",
    "Захер",
    "Эстерхази",
    "Морковный торт",
    "Чёрный лес",
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , random_gen_(std::random_device{}())
{
    ui->setupUi(this);

    connect(ui->list_widget, &QListWidget::currentRowChanged, this, &MainWindow::on_list_widget_currentRowChanged);

    ApplyModel();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::SetRandomGen(const std::mt19937& random_gen) {
    random_gen_ = random_gen;
}

void MainWindow::ApplyModel() {
    long old_distance = 0;
    if (!vector_model_.items.empty() && vector_model_.iterator != vector_model_.items.end()) {
        old_distance = std::distance(vector_model_.items.begin(), vector_model_.iterator);
    }
    
    ui->list_widget->clear();
    
    for (size_t i = 0; i < vector_model_.items.size(); ++i) {
        QString itemText = QString::number(i) + ": " + QString::fromStdString(vector_model_.items[i]);
        ui->list_widget->addItem(itemText);
    }
    
    ui->list_widget->addItem("end");
    
    if (vector_model_.items.empty()) {
        vector_model_.iterator = vector_model_.items.begin();
    } else if (old_distance >= 0 && static_cast<size_t>(old_distance) < vector_model_.items.size()) {
        auto it = vector_model_.items.begin();
        std::advance(it, old_distance);
        vector_model_.iterator = it;
    } else if (old_distance >= static_cast<long>(vector_model_.items.size())) {
        vector_model_.iterator = vector_model_.items.end();
    } else {
        vector_model_.iterator = vector_model_.items.begin();
    }
    
    ApplyIterator();
    
    bool isEmpty = vector_model_.items.empty();
    ui->btn_pop_back->setEnabled(!isEmpty);
    ui->btn_pop_front->setEnabled(!isEmpty);
    
    ui->txt_size->setText(QString::number(vector_model_.items.size()));
}

void MainWindow::ApplyIterator() {
    auto distance = std::distance(vector_model_.items.begin(), vector_model_.iterator);
    
    if (vector_model_.iterator != vector_model_.items.end()) {
        ui->list_widget->setCurrentRow(static_cast<int>(distance));
        
        bool hasItems = !vector_model_.items.empty();
        bool notAtEnd = vector_model_.iterator != vector_model_.items.end();
        ui->btn_edit->setEnabled(hasItems && notAtEnd);
        ui->btn_erase->setEnabled(notAtEnd);
        ui->btn_next->setEnabled(notAtEnd);
        
        if (hasItems && notAtEnd) {
            ui->txt_elem_content->setText(QString::fromStdString(*vector_model_.iterator));
        } else {
            ui->txt_elem_content->clear();
        }
    } else {
        ui->list_widget->setCurrentRow(static_cast<int>(vector_model_.items.size()));
        
        ui->btn_edit->setEnabled(false);
        ui->btn_erase->setEnabled(false);
        ui->btn_next->setEnabled(false);
        
        ui->txt_elem_content->clear();
    }
    
    ui->btn_prev->setEnabled(vector_model_.iterator != vector_model_.items.begin());
}

void MainWindow::on_btn_push_front_clicked() {
    QString content = ui->txt_elem_content->text();
    vector_model_.items.push_front(content.toStdString());
    vector_model_.iterator = vector_model_.items.begin();
    ApplyModel();
}

void MainWindow::on_btn_push_back_clicked() {
    QString content = ui->txt_elem_content->text();
    vector_model_.items.push_back(content.toStdString());
    vector_model_.iterator = vector_model_.items.begin();
    ApplyModel();
}

void MainWindow::on_btn_pop_front_clicked() {
    if (!vector_model_.items.empty()) {
        vector_model_.items.pop_front();
        vector_model_.iterator = vector_model_.items.begin();
        ApplyModel();
    }
}

void MainWindow::on_btn_pop_back_clicked() {
    if (!vector_model_.items.empty()) {
        vector_model_.items.pop_back();
        vector_model_.iterator = vector_model_.items.begin();
        ApplyModel();
    }
}

void MainWindow::on_btn_clear_clicked() {
    vector_model_.items.clear();
    vector_model_.iterator = vector_model_.items.begin();
    ApplyModel();
}

void MainWindow::on_btn_tea_clicked() {
    vector_model_.items = tea;
    vector_model_.iterator = vector_model_.items.begin();
    ApplyModel();
}

void MainWindow::on_btn_cakes_clicked() {
    vector_model_.items = cakes;
    vector_model_.iterator = vector_model_.items.begin();
    ApplyModel();
}

void MainWindow::on_btn_edit_clicked() {
    if (vector_model_.iterator != vector_model_.items.end() && !vector_model_.items.empty()) {
        QString content = ui->txt_elem_content->text();
        *vector_model_.iterator = content.toStdString();
        ApplyModel();
    }
}

void MainWindow::on_btn_erase_clicked() {
    if (vector_model_.iterator != vector_model_.items.end() && !vector_model_.items.empty()) {
        vector_model_.iterator = vector_model_.items.erase(vector_model_.iterator);
        vector_model_.iterator = vector_model_.items.begin();
        ApplyModel();
    }
}

void MainWindow::on_btn_insert_clicked() {
    QString content = ui->txt_elem_content->text();
    vector_model_.iterator = vector_model_.items.insert(vector_model_.iterator, content.toStdString());
    vector_model_.iterator = vector_model_.items.begin();
    ApplyModel();
}

void MainWindow::on_btn_prev_clicked() {
    if (vector_model_.iterator != vector_model_.items.begin()) {
        --vector_model_.iterator;
        ApplyIterator();
    }
}

void MainWindow::on_btn_next_clicked() {
    if (vector_model_.iterator != vector_model_.items.end()) {
        ++vector_model_.iterator;
        ApplyIterator();
    }
}

void MainWindow::on_btn_begin_clicked() {
    vector_model_.iterator = vector_model_.items.begin();
    ApplyIterator();
}

void MainWindow::on_btn_end_clicked() {
    vector_model_.iterator = vector_model_.items.end();
    ApplyIterator();
}

void MainWindow::on_btn_resize_clicked() {
    bool ok;
    int newSize = ui->txt_size->text().toInt(&ok);
    if (ok) {
        newSize = std::min(newSize, 1000);
        vector_model_.items.resize(newSize);
        vector_model_.iterator = vector_model_.items.begin();
        ApplyModel();
    }
}

void MainWindow::on_btn_find_clicked() {
    QString content = ui->txt_elem_content->text();
    auto it = std::find(vector_model_.items.begin(), vector_model_.items.end(), content.toStdString());
    vector_model_.iterator = it;
    ApplyIterator();
}

void MainWindow::on_btn_count_clicked() {
    QString content = ui->le_count->text();
    int count_result = std::count(vector_model_.items.begin(), vector_model_.items.end(), content.toStdString());
    ui->lbl_count->setText(QString::number(count_result));
}

void MainWindow::on_btn_min_element_clicked() {
    if (!vector_model_.items.empty()) {
        auto it = std::min_element(vector_model_.items.begin(), vector_model_.items.end());
        vector_model_.iterator = it;
    } else {
        vector_model_.iterator = vector_model_.items.end();
    }
    ApplyIterator();
}

void MainWindow::on_btn_max_element_clicked() {
    if (!vector_model_.items.empty()) {
        auto it = std::max_element(vector_model_.items.begin(), vector_model_.items.end());
        vector_model_.iterator = it;
    } else {
        vector_model_.iterator = vector_model_.items.end();
    }
    ApplyIterator();
}

void MainWindow::on_btn_merge_sort_clicked() {
    vector_model_.items = MergeSort(vector_model_.items, std::less<std::string>());
    ApplyModel();
}

void MainWindow::on_btn_merge_sOrT_clicked() {
    vector_model_.items = MergeSort(vector_model_.items, 
                                   [](const std::string& left, const std::string& right) {
                                       return QString::compare(QString::fromStdString(left), 
                                                              QString::fromStdString(right), 
                                                              Qt::CaseInsensitive) < 0;
                                   });
    ApplyModel();
}

void MainWindow::on_btn_shuffle_clicked() {
    std::shuffle(vector_model_.items.begin(), vector_model_.items.end(), random_gen_);
    ApplyModel();
}

void MainWindow::on_btn_unique_clicked() {
    if (std::is_sorted(vector_model_.items.begin(), vector_model_.items.end())) {
        auto it = std::unique(vector_model_.items.begin(), vector_model_.items.end());
        vector_model_.items.erase(it, vector_model_.items.end());
        vector_model_.iterator = vector_model_.items.begin();
    }
    ApplyModel();
}

void MainWindow::on_btn_reverse_clicked() {
    std::reverse(vector_model_.items.begin(), vector_model_.items.end());
    ApplyModel();
}

void MainWindow::on_btn_lower_bound_clicked() {
    if (std::is_sorted(vector_model_.items.begin(), vector_model_.items.end())) {
        QString content = ui->txt_elem_content->text();
        auto it = std::lower_bound(vector_model_.items.begin(), vector_model_.items.end(), content.toStdString());
        vector_model_.iterator = it;
        ApplyIterator();
    }
}

void MainWindow::on_btn_upper_bound_clicked() {
    if (std::is_sorted(vector_model_.items.begin(), vector_model_.items.end())) {
        QString content = ui->txt_elem_content->text();
        auto it = std::upper_bound(vector_model_.items.begin(), vector_model_.items.end(), content.toStdString());
        vector_model_.iterator = it;
        ApplyIterator();
    }
}

void MainWindow::on_list_widget_currentRowChanged(int currentRow) {
    if (currentRow < 0) {
        currentRow = 0;
    }
    
    if (currentRow >= 0 && static_cast<size_t>(currentRow) < vector_model_.items.size()) {
        auto it = vector_model_.items.begin();
        std::advance(it, currentRow);
        vector_model_.iterator = it;
    } else if (currentRow >= static_cast<int>(vector_model_.items.size())) {
        vector_model_.iterator = vector_model_.items.end();
    }
    ApplyIterator();
}