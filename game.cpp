#include "game.h"
#include <QBitmap>
#include <QPixmap>
#include <QTimer>
#include <QPalette>
#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <time.h>

game::game(QWidget *parent)
    : QWidget(parent)
{
    window = 0;
    isStart = 2;
    setStartWindow();
}

game::~game()
{
    if(isStart == 1)
    {
        del_game();
        delete window;
    }
    else if(isStart == 0)
    {
        del_start();
        delete window;
    }
}

void game::creatmap()
{
    int b[MaxSizeX*MaxSizeY];
    int i, j, k;

    for(i = 0; i < MaxSizeX*MaxSizeY; i += 4)
        b[i] = b[i+1] = b[i+2] = b[i+3] = i/4+1;
    srand((unsigned)time(NULL));
    std::random_shuffle(b, b+MaxSizeX*MaxSizeY);
    memset(map, 0, sizeof(map));
    for(i = 1, k = 0; i <= MaxSizeX; i++)
        for(j = 1; i <= MaxSizeY; k++, j++)
            map[i][j] = b[k];

}

void game::recreate()
{
    std::cout<<"recreate"<<std::endl;
    int b[MaxSizeX*MaxSizeY];
    int i,j;
    int k=0;
    int temp;

    for(i=1;i<=MaxSizeX;i++){
        for(j=1;j<=MaxSizeY;j++){
            b[k]=map[i][j];
            k++;
        }
    }
    srand((unsigned)time(NULL));
    for(i=1;i<MaxSizeX*MaxSizeY;i++){
        if(b[i]==0)
            continue;
        j=rand()%(i+1);
        k=2;
        while(b[j]==0){
            j=rand()%(i+k);
            k++;
        }
        temp=b[i];
        b[i]=b[j];
        b[j]=temp;
    }
    k=0;
    for(i=1;i<=MaxSizeX;i++){
        for(j=1;j<=MaxSizeY;j++){
            map[i][j]=b[k];
            k++;
        }
    }
    for(i=0;i<MaxSizeX;i++){
        for(j=0;j<MaxSizeY;j++){
            if(map[i+1][j+1]!=0){
                delete image[i][j];
                delete signalMapper[i][j];
            }
        }
    }
    setimage();
    initialbucket();
    if(!judge())recreate();
}

void game::del_game()
{
    std::cout<<"game::~game()"<<std::endl;
    int i,j;
    for(i=0;i<MaxSizeX;i++){
        for(j=0;j<MaxSizeY;j++){
            if(map[i+1][j+1]!=0){
                delete image[i][j];
                delete signalMapper[i][j];
            }
        }
    }
    for(i=0;i<2*MaxSizeX+2*MaxSizeY+4;i++){
        delete block[i];
    }
    delete changebutton;
    delete tipbutton;
    if(isAI)delete aibutton;
    delete timer;

    delete gridlayout;
    delete vlayout;
    delete hlayout1;
}

void game::del_start()
{
    std::cout<<"game::~game()"<<std::endl;
    delete startbutton;
    delete gradebutton;
    delete hlayout1;
    delete hlayout2;
    delete hlayout3;
    delete vlayout;
}

void game::change(int num)
{
    std::cout<<"change"<<std::endl;
    int x=num/100+1;
    int y=num%100+1;
    double ftime;
    enum{first,second};
    static int vis=first;
    if(vis==first){
        x1=x;
        y1=y;
        vis=second;
        sound->play1();
        sound->stop1();
    }
    else if(vis==second){
        x2=x;
        y2=y;
        if(x1==x2&&y1==y2){
            sound->play1();
            sound->stop1();
        }
        else if(ok()){
            del(map[x1][y1],x1,y1);
            del(map[x1][y1],x2,y2);
            delete image[x1-1][y1-1];
            delete image[x2-1][y2-1];
            delete signalMapper[x1-1][y1-1];
            delete signalMapper[x2-1][y2-1];
            map[x1][y1]=0;
            map[x2][y2]=0;
            imageNum-=2;
            std::cout<<imageNum<<std::endl;
            if(imageNum==0){
                QMessageBox msg;
                timer->end();
                sound->stop();
                ftime=timer->count;
                //std::cout<<"time is:"<<eUser::user.Get_Name()<<std::endl;
                eUser::user.Game_End(eUser::user.qname,ftime);
                //std::cout<<"time is:"<<ftime<<std::endl;
                msg.setText(tr("You Win This Game, Congratulation"));
                msg.exec();
                del_game();
                delete window;
                setStartWindow();
                return;
            }
            if(!judge()){
                 QMessageBox msg;
                 msg.setText(tr("地图上无可连接的图标，\n系统将自动生成新地图"));
                 msg.exec();
                 recreate();
            }
        }
        else{
            sound->play1();
            sound->stop1();
        }
        vis=first;
    }
}

void game::setimage()
{
    QString str1[]=
    {
        ":/tubiao/Image/apashe.png",
        ":/tubiao/Image/bobo.png"
        ":/tubiao/Image/chaoyinfu.png"
        ":/tubiao/Image/chuanshanshu.png"
        ":/tubiao/Image/dacongya.png"
        ":/tubiao/Image/daidaishou.png"
        ":/tubiao/Image/duduya.png"
        ":/tubiao/Image/dujiaochong.png"
        ":/tubiao/Image/galagala.png"
        ":/tubiao/Image/haixingxing.png"
        ":/tubiao/Image/houguai.png"
        ":/tubiao/Image/jienigui.png"
        ":/tubiao/Image/jilidan.png"
        ":/tubiao/Image/kadigou.png"
        ":/tubiao/Image/kaixi.png"
        ":/tubiao/Image/kedaya.png"
        ":/tubiao/Image/labaya.png"
        ":/tubiao/Image/lapulasi.png"
        ":/tubiao/Image/lieque.png"
        ":/tubiao/Image/liuwei.png"
        ":/tubiao/Image/lvmaochong.png"
        ":/tubiao/Image/manaoshuimu.png"
        ":/tubiao/Image/maoqiu.png"
        ":/tubiao/Image/menghuan.png"
        ":/tubiao/Image/miaomiao.png"
        ":/tubiao/Image/miaowazhongzi.png"
        ":/tubiao/Image/nimilong.png"
        ":/tubiao/Image/mohaima.png"
        ":/tubiao/Image/niduolan.png"
        ":/tubiao/Image/niduolang.png"
        ":/tubiao/Image/pailasi.png"
        ":/tubiao/Image/pangding.png"
        ":/tubiao/Image/pikachu.png"
        ":/tubiao/Image/pipi.png"
        ":/tubiao/Image/wanli.png"
        ":/tubiao/Image/xaiohaishi.png"
    };
    QString str2[]=
    {
        ":/tubiao/Image/apashe.png",
        ":/tubiao/Image/bobo.png"
        ":/tubiao/Image/chaoyinfu.png"
        ":/tubiao/Image/chuanshanshu.png"
        ":/tubiao/Image/dacongya.png"
        ":/tubiao/Image/daidaishou.png"
        ":/tubiao/Image/duduya.png"
        ":/tubiao/Image/dujiaochong.png"
        ":/tubiao/Image/galagala.png"
        ":/tubiao/Image/haixingxing.png"
        ":/tubiao/Image/houguai.png"
        ":/tubiao/Image/jienigui.png"
        ":/tubiao/Image/jilidan.png"
        ":/tubiao/Image/kadigou.png"
        ":/tubiao/Image/kaixi.png"
        ":/tubiao/Image/kedaya.png"
        ":/tubiao/Image/labaya.png"
        ":/tubiao/Image/lapulasi.png"
        ":/tubiao/Image/lieque.png"
        ":/tubiao/Image/liuwei.png"
        ":/tubiao/Image/lvmaochong.png"
        ":/tubiao/Image/manaoshuimu.png"
        ":/tubiao/Image/maoqiu.png"
        ":/tubiao/Image/menghuan.png"
        ":/tubiao/Image/miaomiao.png"
        ":/tubiao/Image/miaowazhongzi.png"
        ":/tubiao/Image/nimilong.png"
        ":/tubiao/Image/mohaima.png"
        ":/tubiao/Image/niduolan.png"
        ":/tubiao/Image/niduolang.png"
        ":/tubiao/Image/pailasi.png"
        ":/tubiao/Image/pangding.png"
        ":/tubiao/Image/pikachu.png"
        ":/tubiao/Image/pipi.png"
        ":/tubiao/Image/wanli.png"
        ":/tubiao/Image/xaiohaishi.png"
    };
    QString str3[]=
    {
        ":/tubiao/Image/apashe.png",
        ":/tubiao/Image/bobo.png"
        ":/tubiao/Image/chaoyinfu.png"
        ":/tubiao/Image/chuanshanshu.png"
        ":/tubiao/Image/dacongya.png"
        ":/tubiao/Image/daidaishou.png"
        ":/tubiao/Image/duduya.png"
        ":/tubiao/Image/dujiaochong.png"
        ":/tubiao/Image/galagala.png"
        ":/tubiao/Image/haixingxing.png"
        ":/tubiao/Image/houguai.png"
        ":/tubiao/Image/jienigui.png"
        ":/tubiao/Image/jilidan.png"
        ":/tubiao/Image/kadigou.png"
        ":/tubiao/Image/kaixi.png"
        ":/tubiao/Image/kedaya.png"
        ":/tubiao/Image/labaya.png"
        ":/tubiao/Image/lapulasi.png"
        ":/tubiao/Image/lieque.png"
        ":/tubiao/Image/liuwei.png"
        ":/tubiao/Image/lvmaochong.png"
        ":/tubiao/Image/manaoshuimu.png"
        ":/tubiao/Image/maoqiu.png"
        ":/tubiao/Image/menghuan.png"
        ":/tubiao/Image/miaomiao.png"
        ":/tubiao/Image/miaowazhongzi.png"
        ":/tubiao/Image/nimilong.png"
        ":/tubiao/Image/mohaima.png"
        ":/tubiao/Image/niduolan.png"
        ":/tubiao/Image/niduolang.png"
        ":/tubiao/Image/pailasi.png"
        ":/tubiao/Image/pangding.png"
        ":/tubiao/Image/pikachu.png"
        ":/tubiao/Image/pipi.png"
        ":/tubiao/Image/wanli.png"
        ":/tubiao/Image/xaiohaishi.png"
    };

    for(int i=0;i<MaxSizeX;i++){
        for(int j=0;j<MaxSizeY;j++){
            int tmp=map[i+1][j+1];
            if(tmp!=0){
                signalMapper[i][j]= new QSignalMapper;
                image[i][j]=createButton(str1[tmp-1],str2[tmp-1],str3[tmp-1]);
                gridlayout->addWidget(image[i][j],i+1,j+1);
                connect(image[i][j],SIGNAL(clicked()),signalMapper[i][j],SLOT(map()));
                signalMapper[i][j]->setMapping(image[i][j],100*i+j);
                connect(signalMapper[i][j],SIGNAL(mapped(int)),this, SLOT(change(int)));
            }
        }
    }
}

void game::setStartWindow()
{
    isStart=0;
    window=new QWidget;
    window->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Background,QBrush(QPixmap(":/backgroud/Image/background_8.jpg")));
    window->setPalette(palette);

    startbutton=new QPushButton("开始游戏");
    gradebutton=new QPushButton("查看排名");
    debugbutton=new QPushButton("开始游戏（debug版）");
    quitbutton=new QPushButton("退出游戏");

    vlayout=new QVBoxLayout;
    hlayout1=new QHBoxLayout;
    hlayout2=new QHBoxLayout;
    hlayout3=new QHBoxLayout;
    hlayout4=new QHBoxLayout;

    hlayout1->addStretch();
    hlayout1->addWidget(startbutton);
    hlayout1->addStretch();

    hlayout2->addStretch();
    hlayout2->addWidget(gradebutton);
    hlayout2->addStretch();

    hlayout3->addStretch();
    hlayout3->addWidget(debugbutton);
    hlayout3->addStretch();

    hlayout4->addStretch();
    hlayout4->addWidget(quitbutton);
    hlayout4->addStretch();

    vlayout->addLayout(hlayout1);
    vlayout->addLayout(hlayout3);
    vlayout->addLayout(hlayout2);
    vlayout->addLayout(hlayout4);

    window->setLayout(vlayout);
    window->setFixedSize(600,450);
    window->show();

    connect(startbutton,SIGNAL(clicked()),this,SLOT(start_normal()));
    connect(debugbutton,SIGNAL(clicked()),this,SLOT(start_debug()));
    connect(gradebutton,SIGNAL(clicked()),this,SLOT(showGrade()));
    connect(quitbutton,SIGNAL(clicked()),this,SLOT(quitGame()));
}

void game::setblock()
{
    QString str(":/backgroud/Image/tubiaoback.jpg");
    int cnt=0;
    for(int i=0;i<=MaxSizeX+1;i++){
        block[cnt]=createButton(str);
        gridlayout->addWidget(block[cnt++],i,0);
    }
    for(int j=1;j<=MaxSizeY+1;j++){
        block[cnt]=createButton(str);
        gridlayout->addWidget(block[cnt++],0,j);
    }
    for(int i=1;i<=MaxSizeX+1;i++){
        block[cnt]=createButton(str);
        gridlayout->addWidget(block[cnt++],i,MaxSizeY+1);
    }
    for(int j=1;j<=MaxSizeY+1;j++){
        block[cnt]=createButton(str);
        gridlayout->addWidget(block[cnt++],MaxSizeX+1,j);
    }
}

void game::setlayout()
{
    QString str1("D:/game/image/map2.png");
    QString str2("D:/game/image/tip3.png");

    std::cout<<"setlayout"<<std::endl;
    window=new QWidget;
    window->setWindowTitle(tr("连连看"));
    window->resize(250,100);

    gridlayout=new QGridLayout;
    hlayout1=new QHBoxLayout;
    vlayout=new QVBoxLayout;

    changebutton=new QPushButton;
    QPixmap img1(str1),img2(str2);
    changebutton->setIcon(img1);
    changebutton->setIconSize(QSize(150,100));
    changebutton->setFlat(true);

    tipbutton=new QPushButton;
    tipbutton->setIcon(img2);
    tipbutton->setFlat(true);
    tipbutton->setIconSize(QSize(150,100));
    if(isAI)aibutton=new QPushButton(tr("人工智能"));
    else{
        aibutton=new QPushButton;
        aibutton->setFlat(true);
    }
    timer=new DisplayTime;

    vlayout->addWidget(timer);
    vlayout->addWidget(changebutton);
    vlayout->addWidget(tipbutton);
    vlayout->addWidget(aibutton);

    connect(tipbutton,SIGNAL(clicked()),this,SLOT(show()));
    connect(tipbutton,SIGNAL(clicked()),this,SLOT(changetext()));
    connect(changebutton,SIGNAL(clicked()),this,SLOT(recreate()));
    connect(changebutton,SIGNAL(clicked()),this,SLOT(changetext2()));
    if(isAI)connect(aibutton,SIGNAL(clicked()),this,SLOT(ai()));
    setblock();
    setimage();

    window->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Background,QBrush(QPixmap(":/backgroud/Image/backgroud_6.jpg")));
    window->setPalette(palette);
    hlayout1->addLayout(gridlayout);
    hlayout1->addLayout(vlayout);
    window->setLayout(hlayout1);
    window->show();
    judge();

}

void game::changetext()
{
    QString str[]={"D:/game/image/tip0.png",
                   "D:/game/image/tip1.png",
                   "D:/game/image/tip2.png"};
    QPixmap img0(str[0]),img1(str[1]),img2(str[2]);
    static int cnt=1;
    if(cnt==1){
        tipbutton->setIcon(img2);
    }
    else if(cnt==2){
        tipbutton->setIcon(img1);
    }
    else if(cnt==3){
        tipbutton->setIcon(img0);
        tipbutton->setEnabled(false);
    }
    cnt++;
}

void game::changetext2()
{
    QString str[]={"D:/game/image/map0.png",
                   "D:/game/image/map1.png"};
    QPixmap img0(str[0]),img1(str[1]);
    static int cnt=1;
    if(cnt==1){
        changebutton->setIcon(img1);
    }
    else if(cnt==2){
        changebutton->setIcon(img0);
        changebutton->setEnabled(false);
    }
    cnt++;
}

void game::start_normal()
{
    isAI=false;
    Enter *start=new Enter;
    start->show();
    connect(start->startButton,SIGNAL(clicked()),this,SLOT(startGame()));
}

void game::start_debug()
{
    isAI=true;
    Enter *start=new Enter;
    start->show();
    connect(start->startButton,SIGNAL(clicked()),this,SLOT(startGame()));
}

void game::startGame()
{
    isStart=1;
    delete startbutton;
    delete gradebutton;
    delete hlayout1;
    delete hlayout2;
    delete hlayout3;
    delete vlayout;
    delete window;
    isStart=true;
    imageNum=MaxSizeX*MaxSizeY;
    createmap();
    initialbucket();
    setlayout();
    sound->play();
}

void game::showGrade()
{
    Show *p=new Show();
    p->show();
}

void game::quitGame()
{
    isStart=3;
    del_start();
    delete window;
}

Button *game::createButton(QString &str1,QString &str2,QString &str3)
{
    Button *button=new Button;
    button->setGeometry(0,0,0,0);
    QPixmap img1,img2,img3;
    img1.load(str1);
    img2.load(str2);
    img3.load(str3);
    button->setFixedSize(img1.width(),img1.height());
    button->SetButtonIcons(img1,img2,img3);
    button->setIconSize(QSize(img1.width(),img1.height()));
    return button;
}


QPushButton *game::createButton(QString &str)
{
    QPushButton *button=new QPushButton;
    QPixmap img(str);
    button->setFixedSize(img.width(),img.height());
    button->setFlat(true);
    return button;
}

bool game::ok()
{
    std::cout<<"ok"<<std::endl;
    flag=false;
    count=0;
    if(map[x1][y1]!=map[x2][y2])return false;
    dfs(x1,y1,x2,y2,Null);
    if(flag){
        sound->play2();
        sound->stop2();
        return true;
    }
    return false;
}

void game::dfs(int x,int y,int x2,int y2,int type)
{
    if(flag)return;
    if(count>=3)return;
    if(x==MaxSizeX+2||x<0||y==MaxSizeY+2||y<0)return;
    if(x==x2&&y==y2){
        flag=true;
        return;
    }
    if(map[x][y]!=0&&type!=Null)return;
    if(type!=Up&&type!=Null){
        count++;
        dfs(x-1,y,x2,y2,Up);
        count--;
    }
    else{
        dfs(x-1,y,x2,y2,Up);
    }
    if(flag)return;
    if(type!=Down&&type!=Null){
        count++;
        dfs(x+1,y,x2,y2,Down);
        count--;
    }
    else{
        dfs(x+1,y,x2,y2,Down);
    }
    if(flag)return;
    if(type!=Left&&type!=Null){
        count++;
        dfs(x,y-1,x2,y2,Left);
        count--;
    }
    else{
        dfs(x,y-1,x2,y2,Left);
    }
    if(flag)return;
    if(type!=Right&&type!=Null){
        count++;
        dfs(x,y+1,x2,y2,Right);
        count--;
    }
    else{
        dfs(x,y+1,x2,y2,Right);
    }

    if(flag)return;
}

bool game::judge()
{
    std::cout<<"judge"<<std::endl;
    flag=false;
    static int cnt=1;
    for(int k=1;k<=imagenum;k++){
        for(int i=0;i<num[k]-1;i++){
            for(int j=i+1;j<num[k];j++){
                x3=bucket[k][i].x;
                y3=bucket[k][i].y;
                x4=bucket[k][j].x;
                y4=bucket[k][j].y;
                count=0;
                dfs(x3,y3,x4,y4,Null);
             //   std::cout<<x3<<" "<<y3<<" "<<x4<<" "<<y4<<" "<<std::endl;
                if(flag){
                    std::cout<<cnt++<<":yes,("<<x3<<","<<y3<<")("<<x4<<","<<y4<<")"<<std::endl;
                    return true;
                }
            }
        }
    }
    std::cout<<cnt++<<":false"<<std::endl;
    return false;
}

void game::del(int k,int x,int y)
{
    int i;
    for(i=0;i<num[k];i++){
        if(bucket[k][i].x==x&&bucket[k][i].y==y)break;
    }
    for(;i<num[k]-1;i++){
        bucket[k][i].x=bucket[k][i+1].x;
        bucket[k][i].y=bucket[k][i+1].y;
    }
    num[k]--;
}

void game::show()
{
    std::cout<<"show"<<std::endl;
    QString str1[]={"D:/game/image/1_3.jpg",
                   "D:/game/image/2_3.jpg",
                   "D:/game/image/3_3.jpg",
                   "D:/game/image/4_3.jpg",
                   "D:/game/image/5_3.jpg",
                   "D:/game/image/6_3.jpg",
                   "D:/game/image/7_3.jpg",
                   "D:/game/image/8_3.jpg",
                   "D:/game/image/9_3.jpg",
                   "D:/game/image/10_3.jpg",
                   "D:/game/image/11_3.jpg",
                   "D:/game/image/12_3.jpg",
                   "D:/game/image/13_3.jpg",
                   "D:/game/image/14_3.jpg",
                   "D:/game/image/15_3.jpg",
                   "D:/game/image/16_3.jpg",
                   "D:/game/image/17_3.jpg",
                   "D:/game/image/18_3.jpg",
                   "D:/game/image/19_3.jpg",
                   "D:/game/image/20_3.jpg",
                   "D:/game/image/21_3.jpg",
                   "D:/game/image/22_3.jpg",
                   "D:/game/image/23_3.jpg",
                   "D:/game/image/24_3.jpg",
                   "D:/game/image/25_3.jpg",
                   "D:/game/image/26_3.jpg",
                   "D:/game/image/27_3.jpg",
                   "D:/game/image/28_3.jpg",
                   "D:/game/image/29_3.jpg",
                   "D:/game/image/30_3.jpg",
                   "D:/game/image/31_3.jpg",
                   "D:/game/image/32_3.jpg",
                   "D:/game/image/33_3.jpg",
                   "D:/game/image/34_3.jpg",
                   "D:/game/image/35_3.jpg",
                   "D:/game/image/36_3.jpg"};
    QString str3[]={"D:/game/image/1_2.jpg",
                   "D:/game/image/2_2.jpg",
                   "D:/game/image/3_2.jpg",
                   "D:/game/image/4_2.jpg",
                   "D:/game/image/5_2.jpg",
                   "D:/game/image/6_2.jpg",
                   "D:/game/image/7_2.jpg",
                   "D:/game/image/8_2.jpg",
                   "D:/game/image/9_2.jpg",
                   "D:/game/image/10_2.jpg",
                   "D:/game/image/11_2.jpg",
                   "D:/game/image/12_2.jpg",
                   "D:/game/image/13_2.jpg",
                   "D:/game/image/14_2.jpg",
                   "D:/game/image/15_2.jpg",
                   "D:/game/image/16_2.jpg",
                   "D:/game/image/17_2.jpg",
                   "D:/game/image/18_2.jpg",
                   "D:/game/image/19_2.jpg",
                   "D:/game/image/20_2.jpg",
                   "D:/game/image/21_2.jpg",
                   "D:/game/image/22_2.jpg",
                   "D:/game/image/23_2.jpg",
                   "D:/game/image/24_2.jpg",
                   "D:/game/image/25_2.jpg",
                   "D:/game/image/26_2.jpg",
                   "D:/game/image/27_2.jpg",
                   "D:/game/image/28_2.jpg",
                   "D:/game/image/29_2.jpg",
                   "D:/game/image/30_2.jpg",
                   "D:/game/image/31_2.jpg",
                   "D:/game/image/32_2.jpg",
                   "D:/game/image/33_2.jpg",
                   "D:/game/image/34_2.jpg",
                   "D:/game/image/35_2.jpg",
                   "D:/game/image/36_2.jpg"};
    delete image[x3-1][y3-1];
    delete image[x4-1][y4-1];
    delete signalMapper[x3-1][y3-1];
    delete signalMapper[x4-1][y4-1];
    signalMapper[x3-1][y3-1]=new QSignalMapper;
    signalMapper[x4-1][y4-1]=new QSignalMapper;
    image[x3-1][y3-1]=createButton(str1[map[x3][y3]-1],str1[map[x3][y3]-1],str3[map[x3][y3]-1]);
    image[x4-1][y4-1]=createButton(str1[map[x4][y4]-1],str1[map[x4][y4]-1],str3[map[x4][y4]-1]);
    gridlayout->addWidget(image[x3-1][y3-1],x3,y3);
    gridlayout->addWidget(image[x4-1][y4-1],x4,y4);
    connect(image[x3-1][y3-1],SIGNAL(clicked()),signalMapper[x3-1][y3-1],SLOT(map()));
    connect(image[x4-1][y4-1],SIGNAL(clicked()),signalMapper[x4-1][y4-1],SLOT(map()));
    signalMapper[x3-1][y3-1]->setMapping(image[x3-1][y3-1],100*(x3-1)+y3-1);
    signalMapper[x4-1][y4-1]->setMapping(image[x4-1][y4-1],100*(x4-1)+y4-1);
    connect(signalMapper[x3-1][y3-1],SIGNAL(mapped(int)),this, SLOT(change(int)));
    connect(signalMapper[x4-1][y4-1],SIGNAL(mapped(int)),this, SLOT(change(int)));
}

void game::ai()
{
    change(100*(x3-1)+y3-1);
    change(100*(x4-1)+y4-1);
}




































