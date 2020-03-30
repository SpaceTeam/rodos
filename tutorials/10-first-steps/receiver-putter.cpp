#include "rodos.h"
#include "topics.h"

static Application receiverName("ReciverPutter", 1100);

class JustPrint : public Putter {

    /// Implements the complete generic interface of putter
    bool putGeneric(const long topicId, unsigned int msgLen, const void* msg, [[gnu::unused]] const NetMsgInfo& netMsgInfo) {
        PRINTF("ReciverPutter -  msgLen: %d data: %ld topicId: %ld\n", msgLen, *(const long*)msg, topicId);
        return true;
    }

} justPrint;


static Subscriber nameNotImportant01(counter1, justPrint, "justprint01");
static Subscriber nameNotImportant02(counter2, justPrint, "justprint02");
