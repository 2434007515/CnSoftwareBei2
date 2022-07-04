//
// Created by 光球层上的黑子 on 2022/7/2.
//

#ifndef CNSOFTBEI_MESSAGE_H
#define CNSOFTBEI_MESSAGE_H
#include <cstring>
#include <string>
#include <iostream>
using std::string;
class message
{
private:
    static const int MAX_HEAD_LENGTH = 9;
    static const int MAX_BODY_LENGTH = 32;
    char head[MAX_HEAD_LENGTH + 1];
    char body[MAX_BODY_LENGTH + 1];
    char sendMsg[MAX_HEAD_LENGTH + MAX_BODY_LENGTH + 1];

public:
    enum headEnum
    {
        connect,
        load,
        start,
        part,
        finish
    };
    message(headEnum head, const char *body)
    {
        this->setHead(head);
        this->setBody(body);
    };
    message(const char *s)
    {
        for (int i = 0; i < MAX_HEAD_LENGTH; i++)
        {
            if (s[i] == ' ')
            {
                sendMsg[i] = ' ';
                head[i] = '\0';
            }
            else
                sendMsg[i] = head[i] = s[i];
        }
        for (int i = 0; i < MAX_BODY_LENGTH; i++)
        {
            sendMsg[i + MAX_HEAD_LENGTH] = body[i] = s[i + MAX_HEAD_LENGTH];
            if (s[i + MAX_HEAD_LENGTH] == '\0')
                break;
        }
        // std::cout << "s:" << s << " sizeof(s):" << strlen(s) << std::endl;
        // std::cout << "msg构造:" << sendMsg << " sizeof(msg):" << strlen(sendMsg) << std::endl;
    };
    const char *getHead() const
    {
        return head;
    }
    const char *getBody() const
    {
        return body;
    }
    const char *getSendMsg() const
    {
        return sendMsg;
    }
    int setBody(const char *s)
    {
        strcpy(body, s);
        strcpy(sendMsg + MAX_HEAD_LENGTH, s);
        return 0;
    }
    void setHead(headEnum e)
    {
        memset(head, 0, sizeof head);
        switch (e)
        {
        case load:
            strcpy(head, "load");
            break;
        case start:
            strcpy(head, "start");
            break;
        case part:
            strcpy(head, "part");
            break;
        case connect:
            strcpy(head, "connect");
            break;
        case finish:
            strcpy(head, "finish");
            break;
        }
        strcpy(sendMsg, head);
        for (int i = 0; i < MAX_HEAD_LENGTH; i++)
        {
            if (sendMsg[i] == '\0')
                sendMsg[i] = ' ';
        }
    }
};
#endif // CNSOFTBEI_MESSAGE_H
