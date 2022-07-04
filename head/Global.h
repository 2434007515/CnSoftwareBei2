#ifndef CNSOFTBEI_GLOBAL_H
#define CNSOFTBEI_GLOBAL_H

#include <string>
#include "client.h"
#include "message.h"

class Global
{
private:
    static int finish_cnt[5];
    static bool is_finished;
    static int index;
    static int n;

public:
    static void init(int a, int b)
    {
        index = a;
        n = b;
        is_finished = false;
    }
    static void set_finished()
    {
    }
    static void sendFinish(client &sender, string cnt = " 0000")
    {
        cnt[index]++;
        sender.send(message(message::headEnum::finish, cnt.c_str()));
    }
};

#endif // CNSOFTBEI_GLOBAL_H