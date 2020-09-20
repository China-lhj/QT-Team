#ifndef GAME_H
#define GAME_H

#include "button.h"
#include "class.h"
#include "displaytime.h"
#include "enter.h"
#include "player.h"
#include "show.h"
#include <QWidget>
#include <QLayout>
#include <QPushButton>
#include <QSignalMapper>
#include <QMessageBox>
#include <iostream>

class game : public QWidget
{
    Q_OBJECT

private:
    enum
    {
        Null, Right, Left, Up, Down
    };
    enum
    {
        MaxSizeX = 10, MaxSizeY = 14
    };
    enum
    {
        imagenum = 36
    };
    int x1, y1, x2, y2, x3, y3, x4, y4;
    int map[MaxSizeX+2][MaxSizeY+2];
    int count;
    int num[imagenum+1];
    int isStart;
    bool flag;
    bool isAI;
    struct
    {
        int x;
        int y;
    }bucket[imagenum+1][5];

    QGridLayout *gridlayout;
    QHBoxLayout *hlayout1, *hlayout2, *hlayout3, *hlayout4;
    QVBoxLayout *vlayout;
    QSignalMapper *signalMapper[MaxSizeX][MaxSizeY];
    QPushButton *block[2*MaxSizeX+2*MaxSizeY+4];
    Button *image[MaxSizeX][MaxSizeY];
    QWidget *window;        // 主窗口
    QPushButton *changebutton;
    QPushButton *tipbutton;
    QPushButton *aibutton;
    Displaytime *timer;

    QPushButton *startbutton;
    QPushButton *gradebutton;
    QPushButton *debugbutton;
    QPushButton *quitbutton;

    Player *sound;

    void creatmap();        //创建地图
    void setlayout();
    void setimage();
    void setblock();
    inline void initialbucket()
    {
        memset(num, 0, sizeof(num));
        memset(bucket, 0, sizeof(bucket));
        for(int i = 1; i <= MaxSizeX; i++)
            for(int j = 1; j <= MaxSizeY; j++)
                if(map[i][j] != 0)
                {
                    bucket[map[i][j]][num[map[i][j]]].x = i;
                    bucket[map[i][j]][num[map[i][j]]++].y = y;
                }
    }
    bool ok();
    bool judge();
    void dfs(int x, int y, int x1, int y1, int type);   //搜索函数
    void del(int k, int x, int y);

    Button *creatButton(QString &str1, QString &str2, QString &str3);
    QPushButton *creatButton(QString &str);

    void del_start();
    void del_game();

public:
    game(QWidget *parent = 0);
    virtual ~game();
    int imageNum;

private slots:
    void change(int num);   //接收图片的坐标并作出判断
    void show();
    void recreat();
    void changetext();
    void changetext2();
    void setStartWindow();
    void ai();
    void start_nomal();
    void start_debug();
    void showGrade();       //显示成绩
    void startGame();       //开始游戏
    void quitGame();        //退出游戏
};

#endif // GAME_H



















