#include "commandprotocol.h"
#include <QStringConverter>


commandProtocol::commandProtocol(void){
    frameType = _INVALID_FRAME;
    params.clear();
    cmd = "";
    id = -1;
}

commandProtocol::commandProtocol(QByteArray* data, QStringConverter::Encoding formatpar)
{
    int i,init;
    QString param;
    QChar terminator;  
    int len = 0;
    int frame_len = 0;

    // Init of the protocol decoding flags
    frameType = _INVALID_FRAME;
    this->format = formatpar;
    this->id = -1;
    params.clear();
    cmd = "";

    // No data are available
    if(data == nullptr) return;

    // Decodes the data based on the Encoding format
    auto toUtf = QStringDecoder(format);
    QString frame = toUtf(*data);

    // Inizio sequenza: carattere '<'
    for(i=0; i<frame.size(); i++)
    {
        // Elimina gli spazi vuoti
        if(frame.at(i)=='<') break;
    }

    // No initiator matched
    if( i == frame.size()) return;
    init = i; // Init frame index

    // Find the ID or the 'A' character field of the command frame
    param.clear();
    bool isAcknowledge = false;
    bool isStatus = false;
    while(++i < frame.size())
    {

        // Verify if it is the Acknowledge frame format
        if(frame.at(i)=='A'){
            isAcknowledge = true;
            break;
        }
        if(frame.at(i)=='S'){
            isStatus = true;
            break;
        }

        if(frame.at(i)==' ') continue; // No spaces
        if((frame.at(i) > '9') || (frame.at(i) < '0')) return; // Only valid digits or A character


        while(i < frame.size() )
        {
            if(frame.at(i) == ' ') break;
            if((frame.at(i) > '9') || (frame.at(i) < '0')) return;
            param.append(frame.at(i));
            i++;
        }
        break;
    }
    if( i == frame.size()) return;

    // In the standard message format the ID is assigned here
    if((!isAcknowledge) && (!isStatus)) id=param.toInt();

    // Find the frame message lenght
    param.clear();
    while( ++i < frame.size())
    {        
        if(frame.at(i) == ' ') continue; // No spaces
        if((frame.at(i) > '9') || (frame.at(i) < '0')) return; // Only valid digits

        while( i < frame.size())
        {
            if(frame.at(i)==' ') break;
            if((frame.at(i) > '9') || (frame.at(i) < '0')) return;
            param.append(frame.at(i));
            i++;
        }
        break;
    }
    if(i==frame.size()) return;
    len = param.toInt();

    // Find the command field between the   % -- %
    while(++i<frame.size())
    {
        if(frame.at(i)=='%') break;
        if(frame.at(i)!=' ') return;
    }
    if(i==frame.size()) return;

    param.clear();
    while(++i<frame.size())
    {
        if(frame.at(i)==' ') continue; // No spaces
        if(frame.at(i)=='%') return;   // End command limiter
        if(frame.at(i)=='>') return;   // End command frame

        while(i<frame.size())
        {
            if(frame.at(i)==' ') break;
            if(frame.at(i)=='%') break;
            if(frame.at(i)=='>') return;
            param.append(frame.at(i));
            i++;
        }
        break;
    }

    if(i==frame.size()) return;

    // Assignes the ID(ACK frame) or Command (Command frame)
    cmd = param;

    i--;

    // Find the parameters
    while(++i<frame.size())
    {
        if(frame.at(i)=='%') break;
        if(frame.at(i)==' ') continue;
        if(frame.at(i)=='>') return;
        param.clear();

        // Cerca gli apici per determinare una stringa unica
        if(frame.at(i) == '"')
        {
            terminator='"';
            i++;
        }else  terminator = ' ';

        while(i<frame.size())
        {
            if(frame.at(i)==terminator) break;
            if(frame.at(i)=='%') break;
            if(frame.at(i)=='>')
            {
                params.clear();
                return;
            }

            param.append(frame.at(i));
            i++;

        }


        params.append(param);

        if(terminator==' ') i--;
    }
    if(i==frame.size())
    {
        params.clear();
        return;
    }

    // Verifica fine sequenza: simbolo '>'
    while(++i<frame.size())
    {
        if(frame.at(i)!=' ') break;
    }
    if(frame.at(i)!='>')
    {
        params.clear();
        return;
    }

    // Verifica lunghezza totale
    frame_len = i-init+1;
    if(frame_len !=len)
    {
        params.clear();
        return;
    }

    // Evaluates the frame type
    if(isAcknowledge){
        if(params.size() < 1) frameType = _INVALID_FRAME;
        else{
            frameType = _ACK_FRAME;
            id = cmd.toInt();
            code = params[0].toInt();
        }
    }else if(isStatus){
        frameType = _STATUS_FRAME;
        id = 0;
        code = 0;
    }else if(cmd == "OK"){
        if(params.size() < 1) frameType = _INVALID_FRAME;
        else{
            code = params[0].toInt();
            if(code == 0) frameType = _OK_FRAME;
            else frameType = _DELAYED_FRAME;
        }
    }else if(cmd == "NOK"){
        if(params.size() < 1) frameType = _INVALID_FRAME;
        else{
            frameType = _NOK_FRAME;
            code = params[0].toInt();
        }
    }else if(cmd == "NA") frameType = _NA_FRAME;
    else frameType =_CMD_FRAME;
    return;

}


QByteArray commandProtocol::formatCommand(QStringConverter::Encoding format, int id, QString cmd, QList<QString>* params)
{
    QString command_frame,ID_frame,LEN_frame,message_frame;
    int i;

    // Command frame creation
    command_frame.clear();
    command_frame.append("%");
    command_frame.append(cmd);
    command_frame.append(" ");

    if( (params != nullptr) && (params->count()))
    {
        for(i=0; i<params->count(); i++)
        {
            command_frame.append(params->at(i));
            command_frame.append(" ");
        }
    }
    command_frame.append(" %>");

    // Create the ID frame
    ID_frame = QString("<%1 ").arg(id);

    // Create the lenght frame
    int len = ID_frame.size() + command_frame.size() +  5;
    message_frame = ID_frame + QString("%1 ").arg(len,4,10,QChar('0')) + command_frame;

    // Encode the string using the current format
    auto fromUtf16 = QStringEncoder(format);
    return fromUtf16(message_frame);

}

QByteArray commandProtocol::formatDelayCompletedCommand(QStringConverter::Encoding format, int id, int code, QList<QString>* params)
{
    QString frame;
    QString A_frame = "<A ";
    QString command_frame = QString("% %2 %3 ").arg(id).arg(code);

    if( (params != nullptr) && (params->count()))
    {
        for(int i=0; i<params->count(); i++)
        {
            command_frame.append(params->at(i));
            command_frame.append(" ");
        }
    }
    command_frame.append(" %>");

    int len = A_frame.size() + command_frame.size() +  5;
    QString message_frame = A_frame + QString("%1 ").arg(len,4,10,QChar('0')) + command_frame;

    // Encode the string using the current format
    auto fromUtf = QStringEncoder(format);
    return  fromUtf(message_frame);
}

QByteArray commandProtocol::formatStatusCommand(QStringConverter::Encoding format, QString cmd, QList<QString>* params)
{
    QString frame;
    QString A_frame = "<S ";
    QString command_frame = "% " + cmd + " " ;

    if( (params != nullptr) && (params->count()))
    {
        for(int i=0; i<params->count(); i++)
        {
            command_frame.append(params->at(i));
            command_frame.append(" ");
        }
    }
    command_frame.append(" %>");

    int len = A_frame.size() + command_frame.size() +  5;
    QString message_frame = A_frame + QString("%1 ").arg(len,4,10,QChar('0')) + command_frame;

    // Encode the string using the current format
    auto fromUtf = QStringEncoder(format);
    return  fromUtf(message_frame);
}



