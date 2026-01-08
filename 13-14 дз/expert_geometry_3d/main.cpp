#include "dx12_raytracing.hpp"
#include "geometry3d.hpp"
#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QLineEdit>
#include <QMenuBar>
#include <QToolBar>
#include <iostream>
#include <QWindow>
#include <QAction>
#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <locale>
#include <string>
#include <thread
#include <chrono>
#include <QDebug>

using namespace Geometry3D;

class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
            setupUI();
            initializeGeometry();
            setupRendering();
            startRendering();
        }

    private slots:
        void updateGeometry() {
            try {
                double x1 = vertex1XEdit->text().toDouble();
                double y1 = vertex1YEdit->text().toDouble();
                double z1 = vertex1ZEdit->text().toDouble();
                double x2 = vertex2XEdit->text().toDouble();
                double y2 = vertex2YEdit->text().toDouble();
                double z2 = vertex2ZEdit->text().toDouble();
                double x3 = vertex3XEdit->text().toDouble();
                double y3 = vertex3YEdit->text().toDouble();
                double z3 = vertex3ZEdit->text().toDouble();

                box = AdvancedBox<HeapStorage, StrictValidation, JSONSerialization>(
                    Point<double,3>(x1, y1, z1),
                    Point<double,3>(x2, y2, z2),
                    Point<double,3>(x3, y3, z3)
                );

                updateUI();
                dx12Renderer->UpdateGeometry(box);
            } catch (const std::exception& e) {
                std::cerr << "Ошибка обновления геометрии: " << e.what() << std::endl;
            }
        }

        void resetGeometry() {
            vertex1XEdit->setText("0");
            vertex1YEdit->setText("0");
            vertex1ZEdit->setText("0");
            vertex2XEdit->setText("3");
            vertex2YEdit->setText("0");
            vertex2ZEdit->setText("0");
            vertex3XEdit->setText("0");
            vertex3YEdit->setText("4");
            vertex3ZEdit->setText("0");
            updateGeometry();
        }

        void showAbout() {
            QMessageBox::about
            (
                this, 
                "О программе", 
                "3D Геометрия - экспертный уровень\n\nПриложение для визуализации и расчета 3D геометрических фигур с использованием DirectX 12 и Qt."
            );
        }

        void updateUI() {
            volumeLabel->setText(QString("Объем: %1").arg(box.volume()));
            surfaceAreaLabel->setText(QString("Площадь поверхности: %1").arg(box.surface_area()));
            auto centroid = box.centroid_3d();
            centroidLabel->setText(QString("Центр масс: (%1, %2, %3)").arg(centroid[0]).arg(centroid[1]).arg(centroid[2]));
            fpsLabel->setText(QString("FPS: %1").arg(dx12Renderer->GetFPS()));
        }

    private:
        void setupUI() {
            setWindowTitle("3D Геометрия - Треугольник");
            resize(1000, 700);
            QMenuBar *menuBar = this->menuBar();
            QMenu *fileMenu = menuBar->addMenu("Файл");
            QAction *exitAction = fileMenu->addAction("Выход");
            connect(exitAction, &QAction::triggered, this, &QWidget::close);
            QMenu *viewMenu = menuBar->addMenu("Вид");
            QAction *toggleToolbarAction = viewMenu->addAction("Показать панель инструментов");
            toggleToolbarAction->setCheckable(true);
            toggleToolbarAction->setChecked(true);
            QMenu *helpMenu = menuBar->addMenu("Помощь");
            QAction *aboutAction = helpMenu->addAction("О программе");
            connect(aboutAction, &QAction::triggered, this, &MainWindow::showAbout);
            toolBar = addToolBar("Главная");
            QAction *updateAction = toolBar->addAction("Обновить");
            connect(updateAction, &QAction::triggered, this, &MainWindow::updateGeometry);
            QAction *resetAction = toolBar->addAction("Сброс");
            connect(resetAction, &QAction::triggered, this, &MainWindow::resetGeometry);
            connect(toggleToolbarAction, &QAction::toggled, toolBar, &QToolBar::setVisible);
            QWidget *centralWidget = new QWidget;
            setCentralWidget(centralWidget);
            QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
            QHBoxLayout *inputLayout = new QHBoxLayout();
            inputLayout->addWidget(new QLabel("Вершина 1:"));
            vertex1XEdit = new QLineEdit("0");
            vertex1YEdit = new QLineEdit("0");
            vertex1ZEdit = new QLineEdit("0");
            inputLayout->addWidget(vertex1XEdit);
            inputLayout->addWidget(vertex1YEdit);
            inputLayout->addWidget(vertex1ZEdit);
            inputLayout->addWidget(new QLabel("Вершина 2:"));
            vertex2XEdit = new QLineEdit("3");
            vertex2YEdit = new QLineEdit("0");
            vertex2ZEdit = new QLineEdit("0");
            inputLayout->addWidget(vertex2XEdit);
            inputLayout->addWidget(vertex2YEdit);
            inputLayout->addWidget(vertex2ZEdit);
            inputLayout->addWidget(new QLabel("Вершина 3:"));
            vertex3XEdit = new QLineEdit("0");
            vertex3YEdit = new QLineEdit("4");
            vertex3ZEdit = new QLineEdit("0");
            inputLayout->addWidget(vertex3XEdit);
            inputLayout->addWidget(vertex3YEdit);
            inputLayout->addWidget(vertex3ZEdit);
            QPushButton *updateButton = new QPushButton("Обновить");

            connect(
                updateButton, 
                &QPushButton::clicked, 
                this, 
                &MainWindow::updateGeometry
            );
            inputLayout->addWidget(updateButton);

            mainLayout->addLayout(inputLayout);

            QHBoxLayout *infoLayout = new QHBoxLayout();
            volumeLabel = new QLabel("Объем: 0");
            surfaceAreaLabel = new QLabel("Площадь поверхности: 0");
            centroidLabel = new QLabel("Центр масс: (0,0,0)");
            fpsLabel = new QLabel("FPS: 0");
            infoLayout->addWidget(volumeLabel);
            infoLayout->addWidget(surfaceAreaLabel);
            infoLayout->addWidget(centroidLabel);
            infoLayout->addWidget(fpsLabel);
            mainLayout->addLayout(infoLayout);

            renderWindow = new QWindow();
            renderWindow->setGeometry(0, 0, 600, 400);
            QWidget *renderWidget = QWidget::createWindowContainer(renderWindow);
            mainLayout->addWidget(renderWidget);

            QTimer *timer = new QTimer(this);

            connect(
                timer, 
                &QTimer::timeout, 
                this, 
                &MainWindow::updateUI
            );

            timer->start(100);
        }

        void initializeGeometry() {
            box = AdvancedBox<HeapStorage, StrictValidation, JSONSerialization>(
                Point<double,3>(0,0,0),
                Point<double,3>(3,0,0),
                Point<double,3>(0,4,0)
            );
        }

        void setupRendering() {
            dx12Renderer = new DX12RayTracing(
                (
                    HWND
                )renderWindow->winId(), 
                600, 
                400
            );
            dx12Renderer->Initialize();
            dx12Renderer->UpdateGeometry(box);
        }

        void startRendering() {
            renderTimer = new QTimer(this);
            connect(
                renderTimer, 
                &QTimer::timeout, 
                [this]() {
                    dx12Renderer->Render();
                }
            );
            renderTimer->start(16);
        }

        AdvancedBox<HeapStorage, StrictValidation, JSONSerialization> box;
        DX12RayTracing *dx12Renderer;
        QWindow *renderWindow;
        QTimer *renderTimer;
        QLineEdit *vertex1XEdit, *vertex1YEdit, *vertex1ZEdit;
        QLineEdit *vertex2XEdit, *vertex2YEdit, *vertex2ZEdit;
        QLineEdit *vertex3XEdit, *vertex3YEdit, *vertex3ZEdit;
        QLabel *volumeLabel, *surfaceAreaLabel, *centroidLabel, *fpsLabel;
        QToolBar *toolBar;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    std::cout << "Qt version: " << qVersion() << std::endl;
    qDebug() << "Platform name:" << QGuiApplication::platformName();
    std::setlocale(LC_ALL, ".65001");
    try {
        std::cout << "=== Система геометрии экспертного уровня (3D) ===\n" << std::endl;
        
        AdvancedBox<HeapStorage, 
        StrictValidation, 
        JSONSerialization> box(
            Point<double,3>(0,0,0), 
            Point<double,3>(3,0,0), 
            Point<double,3>(0,4,0)
        );

        std::cout << "1. Продвинутый треугольник:" << std::endl;

        std::cout << "   Объем: " << box.volume() << std::endl;
        std::cout << "   Площадь поверхности: " << box.surface_area() << std::endl;
        std::cout << "   Центр масс: " << box.centroid_3d() << std::endl;
        std::cout << "   Количество обращений: " << box.access_count() << std::endl;
        std::cout << "   В формате JSON: " << box.serialize() << std::endl;

        MainWindow window;
        window.show();
        
        std::cout << "\n2. Генератор точек внутри треугольника:" << std::endl;

        auto points = box.generate_points(2.0);
        size_t count = 0;
        for (auto&& point : points) {
            if (count++ < 5) {
                std::cout << "   Точка " << count << ": " << point << std::endl;
            }
        }
        
        std::cout << "\n3. Точки на границе треугольника:" << std::endl;

        size_t point_count = 0;
        for (auto&& point : box.get_boundary_points(8)) {
            if (point_count++ < 8) {
                std::cout << "   Граничная точка " << point_count << ": " << point << std::endl;
            }
        }
        
        std::cout << "\n4. Параллельный расчет объема: " << box.parallel_volume() << std::endl;

        std::cout << "\n5. Асинхронный расчет объема:" << std::endl;

        auto future = box.async_volume();
        std::cout << "   Результат асинхронного расчета: " << future.get() << std::endl;

        std::cout << "\n6. Сопоставление с образцом для 3D:" << std::endl;

        AdvancedBox<HeapStorage, StrictValidation, JSONSerialization>::ShapeVariant3D shape = box;
        std::cout << "   " << AdvancedBox<HeapStorage, StrictValidation, JSONSerialization>::match_shape_3d(shape) << std::endl;
        std::cout << "\n7. Тест производительности для расчета объема:" << std::endl;
        auto benchmark_func = [&box]() {
            volatile double result = box.volume();
            (void)result;
        };
        auto time = Benchmark::measure(benchmark_func, 100000);
        std::cout << "   Среднее время расчета объема: " << time << " мс" << std::endl;

        std::cout << "\n8. Экспертное управление параллельными вычислениями для 3D:" << std::endl;

        ThreadManager thread_manager;
        thread_manager.execute_sequentially([&box](int id) {
            double factor = 1.0 + id * 0.1;
            box.scale(factor);
        }, 4, "Масштабирование 3D");
        
        std::cout << "   Итоговый объем после масштабирования: " << box.volume() << std::endl;
        
        thread_manager.execute_with_barrier(
            [](int id, Barrier& barrier) {
                std::this_thread::sleep_for(
                    std::chrono::milliseconds(
                        100 * (id + 1)
                    )
                );
                
                barrier.arrive_and_wait();
            }, 4, "Синхронизация 3D"
        );
        
        std::cout << "\n9. Демонстрация обработки исключений для 3D:" << std::endl;
        
        try {
            AdvancedBox<HeapStorage, 
            StrictValidation, 
            JSONSerialization> invalid_box(
                Point<double,3>(0,0,0), 
                Point<double,3>(0,-5,0), 
                Point<double,3>(0,0,2)
            );
        } catch (const std::exception& e) {
            std::cout << "   Обнаружена ошибка: " << e.what() << std::endl;
        }
        
        std::cout << "\n10. Инспекция свойств треугольника:" << std::endl;

        box.visit_members([](const std::string& name, const auto& value) {
            std::cout << "   Visiting member: " << name << ", type: " << typeid(value).name() << std::endl;
            if (name == "vertices") {
                std::cout << "   Вершины: ";
                for (const auto& p : value) std::cout << p << " ";
                std::cout << std::endl;
            } else if (name == "indices") {
                std::cout << "   Индексы: ";
                for (auto idx : value) std::cout << idx << " ";
                std::cout << std::endl;
            }
        });
        
        std::cout << "\n11. Демонстрация стирания типов для 3D:" << std::endl;

        ShapeFactory::AnyShape3D any_shape_3d(box);
        std::cout << "   Объем (через стирание типов): " << any_shape_3d.volume() << std::endl;
        std::cout << "   Площадь поверхности (через стирание типов): " << any_shape_3d.surface_area() << std::endl;
        
        std::cout << "\n12. Работа с точками в 3D:" << std::endl;

        Point<double, 3> p1(1.0, 2.0, 3.0);
        Point<double, 3> p2(4.0, 5.0, 6.0);
        std::cout << "   Точка 1: " << p1 << std::endl;
        std::cout << "   Точка 2: " << p2 << std::endl;
        std::cout << "   Расстояние между точками: " << p1.distance(p2) << std::endl;

        std::cout << "\n13. 3D трансформации и визуализация:" << std::endl;

        std::cout << "   Исходный треугольник:" << std::endl;
        std::cout << "   Исходная площадь поверхности: " << box.surface_area() << std::endl;

        std::cout << "   После вращения вокруг оси X на 45°:" << std::endl;
        std::cout << "   Новая площадь поверхности: " << box.surface_area() << std::endl;
        std::cout << "   Новый объем: " << box.volume() << std::endl;

        Point<double, 2> proj = box.project_2d();
        std::cout << "   2D проекция центра: " << proj << std::endl;

        std::cout << "\n14. Дополнительные 3D расчеты:" << std::endl;

        AdvancedBox<HeapStorage, 
        StrictValidation, 
        JSONSerialization> box2(
            Point<double,3>(0,0,0), 
            Point<double,3>(2,0,0), 
            Point<double,3>(0,3,0)
        );

        std::cout << "   Второй треугольник:" << std::endl;
        std::cout << "   Объем: " << box2.volume() << ", Площадь поверхности: " << box2.surface_area() << std::endl;
        std::cout << "   Центр масс: " << box2.centroid_3d() << std::endl;
        std::cout << "   Сериализация: " << box2.serialize() << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "\nКритическая ошибка: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "\n=== Программа успешно завершена ===" << std::endl;
    return app.exec();
}

#include "main.moc"