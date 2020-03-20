#include "rodos.h"
#include "topics.h"

static Application  receiverName("commandReceiver", 1116);

struct CommandReceiver : public Subscriber {
    CommandReceiver() : Subscriber(command, "CommandReceiver") { }
    long put(const long, const long, const void* msg, const NetMsgInfo&) {
        UserCmd* cmd = (UserCmd*)msg;
        PRINTF("command  %d bytes: %s\n", cmd->cmdLen, cmd->cmdLine);
        return 1;
    }
} commandReceiver;

