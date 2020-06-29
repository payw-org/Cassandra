#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFileDialog"
#include <fstream>
#include <iostream>
//#include "unistd.h"

#include "BOP_manager.h"

std::string location;
QList<QTableWidgetItem *> searchList;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(1);
    ui->stackedTable->setCurrentIndex(0);

    const QColor *shadowColor = new QColor(0, 0, 0, 80);

    //int numOfButton = 12;
    QGraphicsDropShadowEffect* effect[12];
    for (int i=0; i<12; i++) {
        effect[i] = new QGraphicsDropShadowEffect();
        effect[i]->setBlurRadius(100);
        effect[i]->setXOffset(0);
        effect[i]->setYOffset(15);
        effect[i]->setColor(*shadowColor);
    }
    ui->startButton->setGraphicsEffect(effect[0]);
    ui->about->setGraphicsEffect(effect[1]);
    ui->goBack->setGraphicsEffect(effect[2]);
    ui->goBackFromAbout->setGraphicsEffect(effect[3]);
    ui->selectFile->setGraphicsEffect(effect[4]);
    ui->goBackFromScore->setGraphicsEffect(effect[5]);
    ui->BOPButton->setGraphicsEffect(effect[6]);
    ui->buttonDone->setGraphicsEffect(effect[7]);
    ui->buttonGoBackFromCreateNewMovie->setGraphicsEffect(effect[8]);
    ui->buttonComplete->setGraphicsEffect(effect[9]);
    ui->buttonAddMoreMovies->setGraphicsEffect(effect[10]);
    ui->timeSchedulingButton->setGraphicsEffect(effect[11]);

    // remove textedit focus
    ui->searchActor->setAttribute(Qt::WA_MacShowFocusRect, 0);
    ui->searchDirector->setAttribute(Qt::WA_MacShowFocusRect, 0);
    ui->lineEditActors->setAttribute(Qt::WA_MacShowFocusRect, 0);
    ui->lineEditDirectors->setAttribute(Qt::WA_MacShowFocusRect, 0);
    ui->lineEditMovieTitle->setAttribute(Qt::WA_MacShowFocusRect, 0);
    ui->lineEditRunningTime->setAttribute(Qt::WA_MacShowFocusRect, 0);

    // table size
    ui->actorTable->setColumnWidth(0, ui->actorTable->width()/2+50);
    ui->actorTable->setColumnWidth(1, ui->actorTable->width()/2-50);
    ui->directorTable->setColumnWidth(0, ui->actorTable->width()/2+50);
    ui->directorTable->setColumnWidth(1, ui->actorTable->width()/2-50);

    ui->movieScoreTable->setColumnWidth(0, ui->movieScoreTable->width()/6+10);
    ui->movieScoreTable->setColumnWidth(1, ui->movieScoreTable->width()/6+10);
    ui->movieScoreTable->setColumnWidth(2, ui->movieScoreTable->width()/6+100);
    ui->movieScoreTable->setColumnWidth(3, ui->movieScoreTable->width()/6-50);
    ui->movieScoreTable->setColumnWidth(4, ui->movieScoreTable->width()/6-50);
    ui->movieScoreTable->setColumnWidth(5, ui->movieScoreTable->width()/6-20);

    // running time gets only number
    ui->lineEditRunningTime->setValidator( new QIntValidator(0, 1000, this));

    // set font
    QFontDatabase::addApplicationFont("://NanumSquareRegular.ttf");
    QFontDatabase::addApplicationFont("://NanumSquareBold-Bold.ttf");
    QFontDatabase::addApplicationFont("://NanumSquareExtraBold-ExtBd.ttf");
    //QMessageBox::information(NULL, "Message", QString::number(id));
    QFont nanum;
    nanum.setFamily("NanumSquare");
    QFont nanumBold;
    nanumBold.setFamily("NanumSquareBold");
    QFont nanumExtra;
    nanumExtra.setFamily("NanumSquareExtraBold");
    QApplication::setFont(nanumBold);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_selectFile_clicked()
{    
    actorsDirectorsScore();
}

void MainWindow::actorsDirectorsScore()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Movie Data"), QDir::homePath(), tr("CSV Files (*.csv)"));
    if (!fileName.isEmpty()) {

        std::cout << "File selected";
        location = fileName.toUtf8().constData();

        manager = new Manager(location);
        manager->calcBOP();

        const char * temp_char;
        int countActor = 0, countDirector = 0, j=0;
        stringstream temp_str;

        unordered_map<string, Actor> actorMap = manager->getActorMap();
        unordered_map<string, Actor>::iterator iterActor;
        unordered_map<string, Director> directorMap = manager->getDirectorMap();
        unordered_map<string, Director>::iterator iterDirector;

        for (iterActor = actorMap.begin(); iterActor != actorMap.end(); iterActor++)
        {
            countActor++;
        }
        for (iterDirector = directorMap.begin(); iterDirector != directorMap.end(); iterDirector++)
        {
            countDirector++;
        }

        // set row count
        ui->actorTable->setRowCount(countActor);

        for (iterActor = actorMap.begin(); iterActor != actorMap.end(); iterActor++)
        {
            temp_char = iterActor->second.getName().c_str();
            QTableWidgetItem *actorItem = new QTableWidgetItem(tr(temp_char));
            actorItem->setTextAlignment(Qt::AlignCenter);
            ui->actorTable->setItem(j, 0, actorItem);

            temp_str << (iterActor->second.getScore());
            temp_char = temp_str.str().c_str();
            temp_str.str("");
            QTableWidgetItem *scoreActorItem = new QTableWidgetItem(tr(temp_char));
            scoreActorItem->setTextAlignment(Qt::AlignCenter);
            ui->actorTable->setItem(j, 1, scoreActorItem);

            j++;
        }

        j=0;

        // set row count
        ui->directorTable->setRowCount(countDirector);

        for (iterDirector = directorMap.begin(); iterDirector != directorMap.end(); iterDirector++)
        {
            temp_char = iterDirector->second.getName().c_str();
            QTableWidgetItem *directorItem = new QTableWidgetItem(tr(temp_char));
            directorItem->setTextAlignment(Qt::AlignCenter);
            ui->directorTable->setItem(j, 0, directorItem);

            temp_str << (iterDirector->second.getScore());
            temp_char = temp_str.str().c_str();
            temp_str.str("");
            QTableWidgetItem *scoreDirectorItem = new QTableWidgetItem(tr(temp_char));
            scoreDirectorItem->setTextAlignment(Qt::AlignCenter);
            ui->directorTable->setItem(j, 1, scoreDirectorItem);

            j++;
        }

        ui->stackedWidget->setCurrentIndex(3);
    }
}

void MainWindow::on_searchActor_textChanged(const QString &arg1)
{
    searchList = ui->actorTable->findItems(ui->searchActor->text(), Qt::MatchContains);
    if (ui->searchActor->text().isEmpty()){
        for (int i=0; i<ui->actorTable->rowCount();i++){
            ui->actorTable->showRow(i);
        }
    }
    else {
        for (int i=0; i<ui->actorTable->rowCount();i++){
            ui->actorTable->hideRow(i);
        }
        for (int i=0; i<searchList.size(); i++){
            ui->actorTable->showRow(searchList.at(i)->row());
        }
    }
//    for (int i = 0; i < ui->actorTable->rowCount(); i++) {
//        if (i==search.at)
//        ui->actorTable->showRow(search.at(i)->row());
//        ui->actorTable->showro;
//        search.at(i)->row()
//    }
}

void MainWindow::on_searchDirector_textChanged(const QString &arg1)
{
    searchList = ui->directorTable->findItems(ui->searchDirector->text(), Qt::MatchContains);
    if (ui->searchDirector->text().isEmpty()){
        for (int i=0; i<ui->directorTable->rowCount();i++){
            ui->directorTable->showRow(i);
        }
    }
    else {
        for (int i=0; i<ui->directorTable->rowCount();i++){
            ui->directorTable->hideRow(i);
        }
        for (int i=0; i<searchList.size(); i++){
            ui->directorTable->showRow(searchList.at(i)->row());
        }
    }
}

void MainWindow::on_BOPButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}

void MainWindow::on_buttonGoBackFromCreateNewMovie_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_buttonDone_clicked()
{
//    manager->addMovie("형","권수경","조정석/디오/박신혜/김강현/지대한",
//                      "110","코미디","12세","11","23");
//    manager->addMovie("라라랜드","다미엔 차젤레","라이언 고슬링/엠마 스톤/J.K. 시몬스/존 레전드/소노야 미즈노/로즈마리 드윗/제이슨 푸치스/메건 페이/제시카 로테/마일즈 앤더슨/트레버 리자우어/칼리 헤르난데스",
//                      "127","멜로/로맨스","12세","12","7");
//    manager->addMovie("신비한 동물사전","데이빗 예이츠","에디 레드메인/콜린 파렐/캐서린 워터스턴/앨리슨 수돌/댄 포글러/사만다 모튼/젠 머레이/페이스 우드-블라크로브/에즈라 밀러/론 펄먼/존 보이트/젬마 찬/카르멘 에조고/크리스틴 마자노/데니스 코라시카우/조쉬 카우더리",
//                      "132","판타지","12세","11","16");
//    manager->addMovie("판도라","박정우","김남길/김영애/문정희/정진영/이경영/김대명/김주현/강신일/유승목/주진모/송영창/김영웅",
//                      "136","스릴러","12세","12","7");
//    manager->addMovie("미씽: 사라진 여자","이언희","엄지원/공효진/김희원/박해준/서하늬/이성욱/고준/장원영/김진구/김지훈/체리쉬 라미레즈/길해연/김선영/이용이/서은아",
//                      "100","미스터리","15세","11","30");
//    manager->addMovie("당신, 거기 있어줄래요", "홍지영","김윤석/변요한/채서진/김상호/김호정/안세하/박혜수",
//                      "111","드라마","12세","12","14");
//    manager->addMovie("원피스 필름 골드","미야모토 히로아키","타나카 마유미/나카이 카즈야/오카무라 아케미/야마구치 캇페이/히라타 히로아키/오오타니 이쿠에/야마구치 유리코/야오 카즈키/야마지 카즈히로/쵸",
//                      "120","애니메이션","12세","12","8");
//    manager->addMovie("목숨 건 연애","송민규","하지원/천전명/진백림/오정세/윤소희/송민정/이다인/김원해/정해균",
//                      "103","멜로/로맨스","15세","12","14");
//    manager->addMovie("아기배달부 스토크","니콜라스 스톨러/더그 스윗랜드","앤디 샘버그/켈시 그래머/제니퍼 애니스톤/타이 버렐/케이티 크라운/키건 마이클 키/조던 필레/안톤 스타크먼/대니 트레조/스티븐 크레이머 글리크먼",
//                      "87","애니메이션","전체관","12","7");
//    ui->stackedWidget->setCurrentIndex(5);


    if (ui->lineEditActors->text().isEmpty()
        || ui->lineEditDirectors->text().isEmpty()
        || ui->lineEditMovieTitle->text().isEmpty()
        || ui->lineEditRunningTime->text().isEmpty()) { }
    else {
        // work
        manager->addMovie(ui->lineEditMovieTitle->text().toUtf8().constData(),
                          ui->lineEditDirectors->text().toUtf8().constData(),
                          ui->lineEditActors->text().toUtf8().constData(),
                          ui->lineEditRunningTime->text().toUtf8().constData(),
                          getGenre(), getAgeGroup(), ui->releaseMonth->text().toUtf8().constData(),
                          ui->releaseDay->text().toUtf8().constData());

        ui->stackedWidget->setCurrentIndex(5);
    }
}

string MainWindow::getGenre()
{
    if (ui->radioAction->isChecked()) return "액션";
    else if (ui->radioAdventure->isChecked()) return "어드벤처";
    else if (ui->radioAnimation->isChecked()) return "애니메이션";
    else if (ui->radioComedy->isChecked()) return "코미디";
    else if (ui->radioCrime->isChecked()) return "범죄";
    else if (ui->radioDocumentary->isChecked()) return "다큐멘터리";
    else if (ui->radioDrama->isChecked()) return "드라마";
    else if (ui->radioFantasy->isChecked()) return "판타지";
    else if (ui->radioFamily->isChecked()) return "가족";
    else if (ui->radioHorror->isChecked()) return "공포(호러)";
    else if (ui->radioHistorical->isChecked()) return "사극";
    else if (ui->radioMystery->isChecked()) return "미스터리";
    else if (ui->radioMusical->isChecked()) return "뮤지컬";
    else if (ui->radioRomance->isChecked()) return "멜로/로맨스";
    else if (ui->radioSF->isChecked()) return "SF";
    else if (ui->radioThriller->isChecked()) return "스릴러";
    else if (ui->radioWar->isChecked()) return "전쟁";
    else if (ui->radioWestern->isChecked()) return "서부극(웨스턴)";
}

string MainWindow::getAgeGroup()
{
    if (ui->radioAll->isChecked()) return "전체관";
    else if (ui->radio12->isChecked()) return "12세";
    else if (ui->radio15->isChecked()) return "15세";
    else if (ui->radio18->isChecked()) return "18세";
    else if (ui->radioAdult->isChecked()) return "청소년";
}

void MainWindow::on_buttonAddMoreMovies_clicked()
{
    ui->lineEditActors->clear();
    ui->lineEditDirectors->clear();
    ui->lineEditMovieTitle->clear();
    ui->lineEditRunningTime->clear();
    ui->lineEditMovieTitle->setFocus();
    ui->radioAction->setChecked(true);
    ui->radioAll->setChecked(true);
    ui->stackedWidget->setCurrentIndex(4);
}

void MainWindow::on_actionOpen_triggered()
{
    actorsDirectorsScore();
}

void MainWindow::on_buttonComplete_clicked()
{
    vector<string>::iterator stringIter;
    vector<Movie>::iterator movieIter;
    vector<Movie> movieList = manager->getNewMovieList();
    int i=0;

    for(movieIter = movieList.begin(); movieIter != movieList.end(); movieIter++) {
        i++;
    }

    ui->movieScoreTable->setRowCount(i);
    QTableWidgetItem *title[i];
    QTableWidgetItem *director[i];
    QTableWidgetItem *actor[i];
    QTableWidgetItem *genreItem[i];
    QTableWidgetItem *agegroupItem[i];
    QTableWidgetItem *specItem[i];

    i=0;

    for(movieIter = movieList.begin(); movieIter != movieList.end(); movieIter++) {
        int runningTime = movieIter->getRunningTime();

        string spectator = to_string(movieIter->getNumOfSpectator());

        string actors,directors;
        string name = movieIter->getName();
        string genre = movieIter->getGenre();
        string ageGroup = movieIter->getAgeGroup();

        vector<string> vectorActor = movieIter->getActors();
        vector<string> vectorDirector = movieIter->getDirectors();

        for(stringIter = vectorActor.begin(); stringIter != vectorActor.end(); stringIter++) {
            actors += *stringIter;
            actors += ", ";
        }

        actors.erase(actors.length()-2, 2);

        for(stringIter = vectorDirector.begin(); stringIter != vectorDirector.end(); stringIter++) {
            directors += *stringIter;
            directors += ", ";
        }

        directors.erase(directors.length()-2, 2);


        title[i] = new QTableWidgetItem(tr(name.c_str()));
        title[i]->setTextAlignment(Qt::AlignCenter);
        ui->movieScoreTable->setItem(i, 0, title[i]);

        director[i] = new QTableWidgetItem(tr(directors.c_str()));
        director[i]->setTextAlignment(Qt::AlignCenter);
        ui->movieScoreTable->setItem(i, 1, director[i]);

        actor[i] = new QTableWidgetItem(tr(actors.c_str()));
        actor[i]->setTextAlignment(Qt::AlignCenter);
        ui->movieScoreTable->setItem(i, 2, actor[i]);

        genreItem[i] = new QTableWidgetItem(tr(genre.c_str()));
        genreItem[i]->setTextAlignment(Qt::AlignCenter);
        ui->movieScoreTable->setItem(i, 3, genreItem[i]);

        agegroupItem[i] = new QTableWidgetItem(tr(ageGroup.c_str()));
        agegroupItem[i]->setTextAlignment(Qt::AlignCenter);
        ui->movieScoreTable->setItem(i, 4, agegroupItem[i]);

        specItem[i] = new QTableWidgetItem(tr(spectator.c_str()));
        specItem[i]->setTextAlignment(Qt::AlignCenter);
        ui->movieScoreTable->setItem(i, 5, specItem[i]);

        //QTableWidgetItem *item3 = new QTableWidgetItem(tr());
        //ui->movieScoreTable->setItem(i, 3, item3);
        i++;
    }

    ui->stackedWidget->setCurrentIndex(6);
}

void MainWindow::on_goBack_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_about_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_goBackFromAbout_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_goBackFromScore_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_testButton_clicked()
{
    QLabel *label = new QLabel();
    label->setMinimumHeight(50);
    label->setMaximumHeight(50);
    label->setStyleSheet("background-color: #e74c3c;color: white;font-size: 10pt;border:3px solid #e74c3c;"
                         "border-radius:5px;");
    label->setText("닥터 스트레인지");
    label->setAlignment(Qt::AlignCenter);

    QLabel *label2 = new QLabel();
    label2->setMinimumHeight(50);
    label2->setMaximumHeight(50);
    label2->setStyleSheet("background-color: #3498db;color: white;font-size: 10pt;");
    label2->setText("Doctor Strange");
    label2->setAlignment(Qt::AlignCenter);

    QLabel *label3 = new QLabel();
    label3->setMinimumHeight(50);
    label3->setMaximumHeight(50);
    label3->setStyleSheet("background-color: #2ecc71;color: white;font-size: 10pt;");
    label3->setText("Doctor Strange");
    label3->setAlignment(Qt::AlignCenter);

    ui->day1Screen1->addWidget(label);
    ui->day1Screen2->addWidget(label2);
    ui->day1Screen3->addWidget(label3);
}

void MainWindow::on_pushButton_clicked()
{
    //ui->textRect->setMinimumHeight(50);
    //ui->textRect->setMaximumHeight(50);
//    QLabel *label = new QLabel(this);
//    label->setMinimumHeight(40);
//    label->setMaximumHeight(40);
//    label->setStyleSheet("background-color: red;");
    //ui->testLayout->addWidget(label);
    ui->day1Screen1->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding));
}

void MainWindow::on_timeSchedulingButton_clicked()
{
    manager->calcTimeScheduling(12, 10, 7);

    int blank;
    int movieTime,moviePixel;

    QBoxLayout *qScreen[42];
    qScreen[0] = ui->day1Screen1;
    qScreen[1] = ui->day1Screen2;
    qScreen[2] = ui->day1Screen3;
    qScreen[3] = ui->day1Screen4;
    qScreen[4] = ui->day1Screen5;
    qScreen[5] = ui->day1Screen6;

    qScreen[6] = ui->day2Screen1;
    qScreen[7] = ui->day2Screen2;
    qScreen[8] = ui->day2Screen3;
    qScreen[9] = ui->day2Screen4;
    qScreen[10] = ui->day2Screen5;
    qScreen[11] = ui->day2Screen6;

    qScreen[12] = ui->day3Screen1;
    qScreen[13] = ui->day3Screen2;
    qScreen[14] = ui->day3Screen3;
    qScreen[15] = ui->day3Screen4;
    qScreen[16] = ui->day3Screen5;
    qScreen[17] = ui->day3Screen6;

    qScreen[18] = ui->day4Screen1;
    qScreen[19] = ui->day4Screen2;
    qScreen[20] = ui->day4Screen3;
    qScreen[21] = ui->day4Screen4;
    qScreen[22] = ui->day4Screen5;
    qScreen[23] = ui->day4Screen6;

    qScreen[24] = ui->day5Screen1;
    qScreen[25] = ui->day5Screen2;
    qScreen[26] = ui->day5Screen3;
    qScreen[27] = ui->day5Screen4;
    qScreen[28] = ui->day5Screen5;
    qScreen[29] = ui->day5Screen6;

    qScreen[30] = ui->day6Screen1;
    qScreen[31] = ui->day6Screen2;
    qScreen[32] = ui->day6Screen3;
    qScreen[33] = ui->day6Screen4;
    qScreen[34] = ui->day6Screen5;
    qScreen[35] = ui->day6Screen6;

    qScreen[36] = ui->day7Screen1;
    qScreen[37] = ui->day7Screen2;
    qScreen[38] = ui->day7Screen3;
    qScreen[39] = ui->day7Screen4;
    qScreen[40] = ui->day7Screen5;
    qScreen[41] = ui->day7Screen6;



    queue<vector<Screen>> timeTable = manager->getTimeTable();
    int order = 0;
    while(!timeTable.empty()){
        vector<Screen> screenList = timeTable.front();
        timeTable.pop();

        for(Screen screen : screenList){
            QLabel *qBlank = new QLabel(this);
            blank = (((float)(screen.getBeginTime()) - 9.0 * 60.0) / (60.0 * 18.0)) * 575.0;
            qBlank->setMinimumHeight(blank);
            qBlank->setMaximumHeight(blank);
            qBlank->setStyleSheet("background-color: transparent;");
            qScreen[order]->addWidget(qBlank);
            queue<Movie> mQueue = screen.getMovieList();
            while(!mQueue.empty()){
                Movie movie = mQueue.front();
                mQueue.pop();

                movieTime = movie.getRunningTime();
                moviePixel = ((float)movieTime / (60.0 * 18.0)) * 575.0;
                QLabel *qMovie = new QLabel(this);
                qMovie->setMinimumHeight(moviePixel);
                qMovie->setMaximumHeight(moviePixel);
                qMovie->setStyleSheet("font-size:11px;background-color:#34495e;color:white;border-bottom:1px solid white;");
                qMovie->setAlignment(Qt::AlignCenter);
                qMovie->setText(QString::fromStdString( movie.getName() ));
                qScreen[order]->addWidget(qMovie);
            }
            qScreen[order]->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding));
            order++;
        }
    }

    ui->stackedWidget->setCurrentIndex(7);
}

void MainWindow::on_goToNextDay_clicked()
{
    int index = ui->stackedTable->currentIndex();
    index++;
    if (index==7)
        index=0;

    int dayNum = ui->dayNumber->text().toInt();
    dayNum++;
    if (dayNum==8)
        dayNum=1;

    ui->stackedTable->setCurrentIndex(index);
    ui->dayNumber->setText(QString::number(dayNum));
}

void MainWindow::on_goToPreviousDay_clicked()
{
    int index = ui->stackedTable->currentIndex();
    index--;
    if (index==-1)
        index=6;

    int dayNum = ui->dayNumber->text().toInt();
    dayNum--;
    if (dayNum==0)
        dayNum=7;

    ui->stackedTable->setCurrentIndex(index);
    ui->dayNumber->setText(QString::number(dayNum));
}
