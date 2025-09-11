#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QTimer>
#include <QRandomGenerator>

//#pragma execution_character_set("utf-8")
MainWindow::MainWindow(QWidget* parent) : FramelessMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->initMainWindowUI();
    this->roundPeogreInit();
    this->pieInit();
    this->lineInit();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initMainWindowUI()
{
    //设置标题栏控件

    this->showMaximized();
    this->setTitleBar(ui->labTitle);
    //关联信号
    connect(this, SIGNAL(titleDblClick()), this, SLOT(titleDblClick()));
    connect(this, SIGNAL(windowStateChange(bool)), this, SLOT(windowStateChange(bool)));
    //设置样式表
    QStringList list;
    list << "#titleBar{background-color:#09233e;}";
    //    list << "#titleBar{border-top-left-radius:4px;border-top-right-radius:4px;}";
    list << "#widgetMain{border:1px solid #041d48;background-image: url(:/images/bg.png);}";
    //list << "#widgetMain{border-bottom-left-radius:8px;border-bottom-right-radius:8px;}";
    this->setStyleSheet(list.join(""));
    int font_id = QFontDatabase::addApplicationFont(":/Fonts/iconfont.ttf");
    QString font_name = QFontDatabase::applicationFontFamilies(font_id).at(0);
    iconfont = QFont(font_name);
    iconfont.setPixelSize(16);
    //最小化
    ui->btnMenu_Min->setFont(iconfont);
    ui->btnMenu_Min->setText(QChar(0xe729));
    //最大化
    ui->btnMenu_Max->setFont(iconfont);
    ui->btnMenu_Max->setText(QChar(0xe720));
    //关闭
    ui->btnMenu_Close->setFont(iconfont);
    ui->btnMenu_Close->setText(QChar(0xe610));
}

void MainWindow::roundPeogreInit()
{
    for (int i = 0; i < 6; i++)
    {
        rpb[i] = new RoundProgressBar();
        rpb[i]->setOutterBarWidth(20);
        rpb[i]->setInnerBarWidth(15);
        rpb[i]->setOutterColor(QColor(255, 255, 255, 150));
        rpb[i]->setPrecision(2);
        rpb[i]->setInnerColor(QColor(255, 0, 0), QColor(0, 255, 0));
        rpb[i]->setValue(0); //设置默认值为0
        rpb[i]->setControlFlags(RoundProgressBar::all);
    }
    ui->vlayout_1->addWidget(rpb[0]);
    ui->vlayout_2->addWidget(rpb[1]);
    ui->vlayout_3->addWidget(rpb[2]);
    ui->vlayout_4->addWidget(rpb[3]);
    ui->vlayout_5->addWidget(rpb[4]);
    ui->vlayout_6->addWidget(rpb[5]);
    QTimer* timer = new QTimer();
    connect(timer, &QTimer::timeout, this, [this]
    {
        //key的单位是秒
        x_time = QDateTime::currentDateTime().toMSecsSinceEpoch() / 1000.0;
        //这里的8，是指横坐标时间宽度为8秒，如果想要横坐标显示更多的时间
        //就把8调整为比较大到值，比如要显示60秒，那就改成60。
        for (int i = 0; i < lineSty.size(); i++)
        {
            lineSty[i]->xAxis->setRange(x_time + 0.25, 8, Qt::AlignRight); //设定x轴的范围
            lineSty[i]->replot();
        }

        static double constant = 60;
        for (int i = 0; i < 6; ++i)
        {
            // 生成 1~5 的随机整数（闭区间）
            int temp = QRandomGenerator::global()->bounded(1, 6);

            // 生成 0~4 的随机整数（闭区间）
            int n = QRandomGenerator::global()->bounded(5);
            double fn = n / (temp * 1.0); //产生10以内的随机浮点数，精度为小数点后1位
            rpb[i]->setValue(constant + fn);
            lineSty[i]->graph(0)->addData(x_time, constant + fn); //添加数据1到曲线1
        }
    });
    timer->start(1000);
}

void MainWindow::pieInit()
{
    //饼状图
    QPieSeries* my_pieSeries = new QPieSeries();
    //中间圆与大圆的比例
    my_pieSeries->setHoleSize(0.35);
    QVector<QString> distri_name = {"正常运行", "湿度预警", "运行错误", "未知状态"};
    //扇形区域颜色
    QPieSlice* pie_distri[4];
    QVector<QColor> distri_color = {
        QColor(13, 222, 121), QColor(255, 157, 77), QColor(216, 30, 6), QColor(255, 255, 255)
    };
    //扇形数据
    QVector<int> distri_data = {50, 26, 3, 4};
    for (int i = 0; i < 4; i++)
    {
        pie_distri[i] = new QPieSlice();
        pie_distri[i]->setValue(distri_data[i]); //扇形占整个圆的百分比
        pie_distri[i]->setLabel(distri_name[i]);
        pie_distri[i]->setLabelVisible();
        pie_distri[i]->setColor(distri_color[i]);
        pie_distri[i]->setLabelColor(distri_color[i]);
        pie_distri[i]->setBorderColor(distri_color[i]);
        pie_distri[i]->setLabelFont(QFont("黑体", 16));
        my_pieSeries->append(pie_distri[i]);
    }
    // 图表视图
    QChart* chart = new QChart();
    // 设置标题颜色
    chart->setTitleBrush(QColor("#ffffff"));
    chart->setTitleFont(QFont("微软雅黑", 20)); //设置标题字体
    chart->setTitle("各车间运行状态统计图");
    chart->setBackgroundVisible(false);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->legend()->setBackgroundVisible(false);
    chart->legend()->setFont(QFont("黑体", 14)); // 图例字体
    chart->legend()->setBrush(QColor("#ffffff"));
    // 设置下图标颜色
    chart->legend()->setLabelColor(QColor("#ffffff"));
    chart->addSeries(my_pieSeries);

    QChartView* chartView = new QChartView();
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setChart(chart);
    ui->gridLayout_2->addWidget(chartView);
}

void MainWindow::lineInit()
{
    QVector<QString> line_name = {"车间1", "车间2", "车间3", "车间4", "车间5", "车间6"};
    lineSty.append(ui->plot_1);
    lineSty.append(ui->plot_2);
    lineSty.append(ui->plot_3);
    lineSty.append(ui->plot_4);
    lineSty.append(ui->plot_5);
    lineSty.append(ui->plot_6);
    for (int i = 0; i < lineSty.size(); i++)
    {
        QCPTextElement* textelement = new QCPTextElement(lineSty[i], line_name[i], QFont("sans", 17, QFont::Bold));
        textelement->setTextColor(QColor(15, 167, 234));
        titel.append(textelement);
    }
    for (int i = 0; i < lineSty.size(); i++)
    {
        QSharedPointer<QCPAxisTickerDateTime> dateTick(new QCPAxisTickerDateTime);
        dateTick->setDateTimeFormat("HH:mm:ss");
        lineSty[i]->xAxis->setTicker(dateTick);
        lineSty[i]->plotLayout()->insertRow(0);
        lineSty[i]->plotLayout()->addElement(0, 0, titel[i]);
        lineSty[i]->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
            QCP::iSelectLegend | QCP::iSelectPlottables); //允许鼠标点击拖动
        lineSty[i]->setBackground(QColor(61, 85, 122));
        lineSty[i]->legend->setVisible(true);
        lineSty[i]->legend->setBrush(QColor(255, 255, 255, 150)); //legend背景色设为白色但背景透明，允许图像在legend区域可见

        lineSty[i]->xAxis->setLabel("时间/s");                     //XY轴的名字
        lineSty[i]->xAxis->setLabelColor(QColor(255, 255, 255)); //XY轴的名字
        lineSty[i]->xAxis->setBasePen(QColor(255, 255, 255));    //x轴颜色
        lineSty[i]->xAxis->setTickPen(QColor(255, 255, 255));    //设置x坐标线上 大 分格线颜色
        lineSty[i]->xAxis->setSubTickPen(QColor(255, 255, 255)); //设置x坐标线上小分格线颜色
        lineSty[i]->xAxis->setTickLabelColor(QColor(255, 255, 255));

        lineSty[i]->yAxis->setLabel("湿度/°C");
        lineSty[i]->yAxis->setLabelColor(QColor(255, 255, 255)); //XY轴的名字
        lineSty[i]->yAxis->setBasePen(QColor(255, 255, 255));    //设置y坐标线颜色
        lineSty[i]->yAxis->setTickPen(QColor(255, 255, 255));    //设置x坐标线上 大 分格线颜色
        lineSty[i]->yAxis->setSubTickPen(QColor(255, 255, 255)); //设置x坐标线上小分格线颜色
        lineSty[i]->yAxis->setTickLabelColor(QColor(255, 255, 255));

        lineSty[i]->addGraph();

        //数据线样式设计
        lineSty[i]->graph(0)->setPen(QColor(27, 254, 51));
        lineSty[i]->graph(0)->setName("湿度");

        lineSty[i]->xAxis->setRange(0, 40);
        lineSty[i]->yAxis->setRange(0, 140);
    }
}

void MainWindow::drawLine()
{
}

void MainWindow::titleDblClick()
{
    on_btnMenu_Max_clicked();
}

void MainWindow::windowStateChange(bool max)
{
    Q_UNUSED(max);
    //    ui->btnMenu_Max->setText(max ? "还原" : "最大");
}

void MainWindow::on_btnMenu_Min_clicked()
{
#ifdef Q_OS_MACOS
    this->setWindowFlags(this->windowFlags() & ~Qt::FramelessWindowHint);
#endif
    this->showMinimized();
}

void MainWindow::on_btnMenu_Max_clicked()
{
    if (this->isMaximized())
    {
        this->showNormal();
    }
    else
    {
        this->showMaximized();
    }
}

void MainWindow::on_btnMenu_Close_clicked()
{
    this->close();
}


void MainWindow::on_btn_line_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::on_btn__clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}
