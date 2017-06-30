#include "caculator.h"
#include "ui_caculator.h"

#include <QPushButton>
#include <QDebug>
#include <QtMath>

Caculator::Caculator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Caculator)
{
    ui->setupUi(this);

    //数字点击事件绑定
    connect(ui->btn0,SIGNAL(clicked()),
            this,SLOT(numClickedSlot()));
    connect(ui->btn1,SIGNAL(clicked()),
            this,SLOT(numClickedSlot()));
    connect(ui->btn2,SIGNAL(clicked()),
            this,SLOT(numClickedSlot()));
    connect(ui->btn3,SIGNAL(clicked()),
            this,SLOT(numClickedSlot()));
    connect(ui->btn4,SIGNAL(clicked()),
            this,SLOT(numClickedSlot()));
    connect(ui->btn5,SIGNAL(clicked()),
            this,SLOT(numClickedSlot()));
    connect(ui->btn6,SIGNAL(clicked()),
            this,SLOT(numClickedSlot()));
    connect(ui->btn7,SIGNAL(clicked()),
            this,SLOT(numClickedSlot()));
    connect(ui->btn8,SIGNAL(clicked()),
            this,SLOT(numClickedSlot()));
    connect(ui->btn9,SIGNAL(clicked()),
            this,SLOT(numClickedSlot()));

    //操作符'+' '-' '*' '/'点击事件绑定
    connect(ui->opAddBtn,SIGNAL(clicked()),
            this,SLOT(opClickedSlot()));
    connect(ui->opSubBtn,SIGNAL(clicked()),
            this,SLOT(opClickedSlot()));
    connect(ui->opMulBtn,SIGNAL(clicked()),
            this,SLOT(opClickedSlot()));
    connect(ui->opDivBtn,SIGNAL(clicked()),
            this,SLOT(opClickedSlot()));

    //'='点击事件绑定
    connect(ui->eqlBtn,SIGNAL(clicked()),
            this,SLOT(eqlClickedSlot()));

    //'.'事件
    connect(ui->opPointBtn,SIGNAL(clicked()),
            this,SLOT(numClickedSlot()));

    //clear事件绑定
    connect(ui->clearBtn,SIGNAL(clicked()),
            this,SLOT(clearClickedSlot()));
    connect(ui->clearAllBtn,SIGNAL(clicked()),
            this,SLOT(clearAllClickedSlot()));

    //退格事件绑定
    connect(ui->delBtn,SIGNAL(clicked(bool)),
            this,SLOT(delClickedSlot()));

    //单目运算符操作实现
    connect(ui->sqrtBtn,SIGNAL(clicked(bool)),
            this,SLOT(sqrtClickedSlot()));
    connect(ui->squareBtn,SIGNAL(clicked(bool)),
            this,SLOT(squareClickedSlot()));
    connect(ui->rcpBtn,SIGNAL(clicked(bool)),
            this,SLOT(rcpClickedSlot()));
    connect(ui->negativeBtn,SIGNAL(clicked(bool)),
            this,SLOT(negativeClickedSlot()));


}

Caculator::~Caculator()
{
    delete ui;
}

/*数字处理*/
void Caculator::numClickedSlot(void)
{
    isEqlClicked=false;
    QPushButton* btn = (QPushButton*)sender();

    //设置下层显示框，显示结果
    if(isOpClicked){
        ui->screenLdt->setText(btn->text());
    }else{
        ui->screenLdt->setText(ui->screenLdt->text()+btn->text());
    }
    //设置上层显示框，实时显示表达式
    ui->memLdt->setText(ui->memLdt->text()+btn->text());
    //标识此时最后一个字符非运算符
    isOpClicked=false;
}

/* 运算符处理  +-*/
void Caculator::opClickedSlot(void)
{
    isEqlClicked=false;
    //运算符未被使用
    if(!isOpClicked){
        if(ui->screenLdt->text().contains("-")){
            express+=ui->screenLdt->text().mid(1);
        }
        else{
            express+=ui->screenLdt->text();
        }
        caculateSlot();
        isOpClicked=true;   //操作符已按
    }
}
/* 运算符处理 =  */
void Caculator::eqlClickedSlot(void)
{
    if(isOpClicked||isEqlClicked){

    }
    else{
        if(ui->screenLdt->text().contains("-")){
            express=express+ui->screenLdt->text().mid(1);
        }
        else{
            express+=ui->screenLdt->text();
        }
        caculateSlot();
        isOpClicked=false;   //操作符未按

        //等于号被点击,清空表达式、记忆框
        if(ui->screenLdt->text().contains("-")){
            express="neg";
        }else{
            express="";
        }
        QString hisStr = ui->memLdt->text()+"="+ui->screenLdt->text()+"\t\n"+ui->hisTdt->toPlainText();
        ui->hisTdt->setPlainText(hisStr);
        ui->memLdt->setText(ui->screenLdt->text());
        isEqlClicked=true;
    }
}

/* 取正负 */
void Caculator::negativeClickedSlot(void)
{
    QString screenStr = ui->screenLdt->text();
    QString memStr = ui->memLdt->text();
    QString resStr=tr("%1").arg(screenStr.toDouble()*(-1));
    express+="neg";
    ui->memLdt->setText(ui->memLdt->text().left(memStr.length()-screenStr.length())+" (-"+screenStr+")");
    ui->screenLdt->setText(resStr);
}

/* 求平方根 */
void Caculator::sqrtClickedSlot(void)
{
    QString screenStr = ui->screenLdt->text();
    QString memStr = ui->memLdt->text();
    QString resStr=tr("%1").arg(sqrt(screenStr.toDouble()));
    ui->memLdt->setText(ui->memLdt->text().left(memStr.length()-screenStr.length())+"√"+screenStr);
    ui->screenLdt->setText(resStr);

}

/* 求平方 实现方法:对数字进行运算，将结果写回文本框 */
void Caculator::squareClickedSlot(void){
    QString screenStr = ui->screenLdt->text();
    QString memStr = ui->memLdt->text();
    QString resStr=tr("%1").arg(screenStr.toDouble()*screenStr.toDouble());
    ui->memLdt->setText(ui->memLdt->text()+"^2");
    ui->screenLdt->setText(resStr);
}

/*求三次方*/
void Caculator::threeClickedSlot(void)
{
    QString screenStr = ui->screenLdt->text();
    QString memStr = ui->memLdt->text();
    QString resStr=tr("%1").arg(screenStr.toDouble()*screenStr.toDouble()*screenStr.toDouble());
    ui->memLdt->setText(ui->memLdt->text()+"^3");
    ui->screenLdt->setText(resStr);
}

/*求倒数*/
void Caculator::rcpClickedSlot(void)
{
    QString screenStr = ui->screenLdt->text();
    QString memStr = ui->memLdt->text();
    QString resStr=tr("%1").arg(1.0/screenStr.toDouble());
    ui->memLdt->setText(ui->memLdt->text().left(memStr.length()-screenStr.length())+"1/"+screenStr);
    ui->screenLdt->setText(resStr);
}




/* 点 处理 */
void Caculator::pointClickedSlot(void)
{
    if(isOpClicked){

    }else{
        QPushButton* btn = (QPushButton*)sender();

        //设置下层显示框，显示结果
        ui->screenLdt->setText(ui->screenLdt->text()+btn->text());
        //设置上层显示框，实时显示表达式
        ui->memLdt->setText(ui->memLdt->text()+btn->text());
        //标识此时最后一个字符非运算符
        isOpClicked=true;
    }
}

/* 清除历史记录文本框 */
void Caculator::clearClickedSlot(void)
{
    ui->hisTdt->clear();
}
/* 清除所有文本框 */
void Caculator::clearAllClickedSlot(void)
{
    ui->screenLdt->clear();
    ui->memLdt->clear();
    express="";
}
/* 退格处理 */
void Caculator::delClickedSlot(void)
{
    QString memStr=ui->memLdt->text();                //获取记录框字符串长度
    if(memStr.right(1)=="+"||memStr.right(1)=="-"||memStr.right(1)=="*"||memStr.right(1)=="/"){

    }else{
        memStr=memStr.left(memStr.length()-1);            //截取除最后一个字符的字符串
        ui->memLdt->setText(memStr);                      //将截取后的字符显示

        QString screenStr=ui->screenLdt->text();          //获取字符串长度
        screenStr=screenStr.left(screenStr.length()-1);   //截取除最后一个字符的字符串
        ui->screenLdt->setText(screenStr);                //将截取后的字符显示
    }
}

/* 负号被按下 */
void Caculator::minusClickedSlot(void)
{

}

/*计算表达式*/
void Caculator::caculateSlot(void){

    QPushButton* btn = (QPushButton*)sender();

    if(express.indexOf('+')!=-1){
        int loa=express.indexOf('+');
        int firNum=0;
        int secNum=0;
        if(express.left(loa).contains("neg")){
            firNum=express.left(loa).mid(3).toDouble()*(-1);
        }else {
            firNum=express.left(loa).toDouble();
        }
        if(express.mid(loa+1).contains("neg")){
            secNum=express.mid(loa+1).mid(3).toDouble()*(-1);
        }else{
            secNum=express.mid(loa+1).toDouble();
        }
        ui->screenLdt->setText(QString::number(firNum+secNum));
        express=ui->screenLdt->text();
    }else if(express.indexOf('-')!=-1){
        int loa=express.indexOf('-');
        int firNum=0;
        int secNum=0;
        if(express.left(loa).contains("neg")){
            firNum=express.left(loa).mid(3).toDouble()*(-1);
        }else {
            firNum=express.left(loa).toDouble();
        }
        if(express.mid(loa+1).contains("neg")){
            secNum=express.mid(loa+1).mid(3).toDouble()*(-1);
        }else{
            secNum=express.mid(loa+1).toDouble();
        }
        ui->screenLdt->setText(tr("%1").arg(firNum-secNum));
        express=ui->screenLdt->text();
    }else if(express.indexOf('*')!=-1){
        int loa=express.indexOf('*');
        int firNum=0;
        int secNum=0;
        if(express.left(loa).contains("neg")){
            firNum=express.left(loa).mid(3).toDouble()*(-1.0);
        }else {
            firNum=express.left(loa).toDouble();
        }
        if(express.mid(loa+1).contains("neg")){
            secNum=express.mid(loa+1).mid(3).toDouble()*(-1.0);
        }else{
            secNum=express.mid(loa+1).toDouble();
        }
        ui->screenLdt->setText(tr("%1").arg(firNum*secNum));
        express=ui->screenLdt->text();
    }else if(express.indexOf('/')!=-1){
        int loa=express.indexOf('/');
        int firNum=0;
        int secNum=0;
        if(express.left(loa).contains("neg")){
            firNum=express.left(loa).mid(3).toDouble()*(-1.0);
        }else {
            firNum=express.left(loa).toDouble();
        }
        if(express.mid(loa+1).contains("neg")){
            secNum=express.mid(loa+1).mid(3).toDouble()*(-1.0);
        }else{
            secNum=express.mid(loa+1).toDouble();
        }
        ui->screenLdt->setText(tr("%1").arg(firNum/secNum));
        express=ui->screenLdt->text();
    }else{

    }
    QString tmp=btn->text();
    if (tmp=="="){
        tmp="";
    }
    ui->memLdt->setText(ui->memLdt->text()+tmp);
    express+=tmp;

}

