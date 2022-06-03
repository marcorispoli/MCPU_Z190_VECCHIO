#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>

#define FIRST_STATUS_REGISTER_INDEX 4
#define FIRST_DATA_REGISTER_INDEX 1
#define FIRST_PARAM_REGISTER_INDEX 1



class dataregT
{
public:

    explicit dataregT(uchar d0, uchar d1, uchar d2, uchar d3){
        data[0] = d0;
        data[1] = d1;
        data[2] = d2;
        data[3] = d3;
    }

    explicit dataregT(void){
        data[0] = 0;
        data[1] = 0;
        data[2] = 0;
        data[3] = 0;
    }

    void clear(void){
        data[0] = 0;
        data[1] = 0;
        data[2] = 0;
        data[3] = 0;
    }
public:
    uchar data[4];
};



class deviceClass: public QObject
{
    Q_OBJECT

public:
    explicit deviceClass(uint ID, uchar revMaj, uchar revMin, uchar revSub, QObject *parent = nullptr);

    typedef uchar _dataregT[4];


    // Protocol command execution
    virtual bool readRevision(dataregT* data)
    {
        if(!data) return false;
        *data = revisionreg;
        return true;
    }

    virtual bool readSystem(dataregT* data)
    {
        if(!data) return false;
        *data = systemreg;
        return true;
    }
    virtual bool readError(dataregT* data){
        if(!data) return false;
        *data = errorreg;
        errorreg.clear();
        return true;
    }
    virtual bool readCommand(dataregT* data){
        if(!data) return false;
        *data = commandreg;
        return true;
    }

    virtual bool readStatus(uchar index, dataregT* data){
        index = index - FIRST_STATUS_REGISTER_INDEX; // Remove the reserved register address space
        if(index >= statusRegisterList.size()) return false;
        *data = statusRegisterList.at(index);
        return true;
    }

    virtual bool writeParam(uchar index, dataregT data){
        index = index - FIRST_PARAM_REGISTER_INDEX;
        if(index >= paramRegisterList.size()) return false;
        paramRegisterList[index] = data;
        return true;
    }
    virtual bool writeData(uchar index, dataregT data){
        index = index - FIRST_DATA_REGISTER_INDEX;
        if(index >= dataRegisterList.size()) return false;
        dataRegisterList[index] = data;
        return true;
    }

    virtual bool execCommand(uchar cmd, dataregT* data){
        return false;
    }

public:

    dataregT revisionreg;
    dataregT systemreg;
    dataregT errorreg;
    dataregT commandreg;

    QList<dataregT> statusRegisterList;
    QList<dataregT> dataRegisterList;
    QList<dataregT> paramRegisterList;


private:
    uint ID;
};

#endif // DEVICE_H
