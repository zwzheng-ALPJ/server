#ifndef LOGGER_H
#define LOGGER_H

#include <QDateTime>
#include <iostream>
#include <QDebug>


class logger {
public:
    ~logger();

    static logger& logging(int category=0);

    //重载 << 运算符
    // overload operator "<<" so it can be used just as cout or QDebug
    template<typename T>
    logger& operator<<(const T& data) {
        if(output_date)
        {
            QDateTime currentDateTime = QDateTime::currentDateTime();
            //std::cout<<"["<<currentDateTime.toString(Qt::ISODate).toStdString()<<"] "<<data;
            qDebug()<<"["<<currentDateTime.toString(Qt::ISODate)<<"] "<<data;
            output_date=false;
        }
        else
        {
            //std::cout<<data;
            qDebug()<<data;
        }

        return *this; //返回引用以支持链式调用 support chained calls
    }

private:
    bool output_date=true;
};


#endif // LOGGER_H
