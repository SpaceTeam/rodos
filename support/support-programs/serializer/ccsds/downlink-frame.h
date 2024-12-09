
#pragma once

#include "rodos.h"
#include "ccsds/ccsds-envelop.h"


/* Sergio Montenegro
 * Nov 2016
 */


namespace CCSDS {

/*
 * DownlinkFrame: abstraction and simple use of ccsds DownlinkEnvelop
 * ist a TransferFrame, with a single Sourcepackage
 */


/* how to use:
 *   DownlinkFrame myTm;
 *   char* dest  = myTm.beginNewSpTF();
 *   myTm.setVals(321, 1,2,3,4); // (spaceCraftId, apid, serv, subserv, vcid)
 *   serialize_your_data_to(dest);
 *   DownlinkTransferFrame* tf = myTm.commit();
 *   publish_or_send(tf);
 *
 * shortcut:
 *   char* dest  = myTm.beginNewSpTF(1,2,3,4); // (apid, serv, subserv, vcid)
 *   serialize_your_data_to(dest);
 *   ...
 */

struct DownlinkFrame {

    DownlinkEnvelop downlinkEnvelop;

    uint8_t* beginNewFrame() {
        downlinkEnvelop.beginNewTF();
        uint16_t len = downlinkEnvelop.beginNewSP();
        downlinkEnvelop.lenOfCurrentUserData = len;
        return downlinkEnvelop.userDataBuf;
    }

   uint8_t* beginNewFrame(uint16_t spaceCraftId, uint8_t apid, uint8_t serv, uint8_t subserv, uint8_t vcid) {
        beginNewFrame();
        setVals(spaceCraftId, apid, serv, subserv, vcid);
        return downlinkEnvelop.userDataBuf;
    }


    DownlinkTransferFrame* commit() {
        //downlinkEnvelop.lenOfCurrentUserData = MAX_USER_DATA_LEN; // allready set in beginNewFrame()
        downlinkEnvelop.commitSP();
        downlinkEnvelop.commitTF();
        return downlinkEnvelop.getTfPtr();
    }

    void setVcid(uint8_t vcid) {
        downlinkEnvelop.tfHeader.virtualChanId        = vcid;
    }

    void setVals(uint16_t spaceCraftId, uint16_t apid, uint8_t serv, uint8_t subserv, uint8_t vcid) {
        downlinkEnvelop.initDefaultTFHeaderAndTrailer(spaceCraftId);
        downlinkEnvelop.initDefaultSPHeader();

        downlinkEnvelop.spHeader.applicationId     = apid;
        downlinkEnvelop.spHeader.service           = serv;
        downlinkEnvelop.spHeader.subservice        = subserv;

        downlinkEnvelop.tfHeader.virtualChanId     = vcid;
    }

};
}
