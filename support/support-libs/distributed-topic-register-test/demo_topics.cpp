
#include "rodos.h"

Topic<long> counter0(1100, "counter0");
Topic<long> counter1(1101, "counter1");
Topic<long> counter2(1102, "counter2");
Topic<long> counter3(1103, "counter3");
Topic<long> counter4(1104, "counter4");
Topic<long> counter5(1105, "counter5");
Topic<long> counter6(1106, "counter6");
Topic<long> counter7(1107, "counter7");
Topic<long> counter8(1108, "counter8");
Topic<long> counter9(1109, "counter9");


//__________________________________________________


void nopReceier([[gnu::unused]] long &val) { }

SubscriberReceiver<long> nameNotImportant11(counter1, nopReceier, "rec11");
SubscriberReceiver<long> nameNotImportant12(counter1, nopReceier, "rec12");
SubscriberReceiver<long> nameNotImportant13(counter1, nopReceier, "rec13");
SubscriberReceiver<long> nameNotImportant14(counter1, nopReceier, "rec14");

SubscriberReceiver<long> nameNotImportant21(counter2, nopReceier, "rec21");
SubscriberReceiver<long> nameNotImportant22(counter2, nopReceier, "rec32");

SubscriberReceiver<long> nameNotImportant31(counter3, nopReceier, "rec51");
SubscriberReceiver<long> nameNotImportant32(counter3, nopReceier, "rec52");
SubscriberReceiver<long> nameNotImportant33(counter3, nopReceier, "rec53");

SubscriberReceiver<long> nameNotImportant4(counter4, nopReceier, "rec53");
SubscriberReceiver<long> nameNotImportant5(counter5, nopReceier, "rec53");
SubscriberReceiver<long> nameNotImportant6(counter6, nopReceier, "rec53");
SubscriberReceiver<long> nameNotImportant7(counter7, nopReceier, "rec53");
SubscriberReceiver<long> nameNotImportant8(counter8, nopReceier, "rec53");
SubscriberReceiver<long> nameNotImportant9(counter9, nopReceier, "rec53");

