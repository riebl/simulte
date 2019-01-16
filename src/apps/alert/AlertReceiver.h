//
//                           SimuLTE
//
// This file is part of a software released under the license included in file
// "license.pdf". This license can be also found at http://www.ltesimulator.com/
// The above file and the present reference are part of the software itself,
// and cannot be removed from it.
//

#ifndef _LTE_AlertReceiver_H_
#define _LTE_AlertReceiver_H_

#include <string.h>
#include <omnetpp.h>

#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/transportlayer/contract/udp/UdpSocket.h"
#include "apps/alert/AlertPacket_m.h"

class AlertReceiver : public omnetpp::cSimpleModule
{
    inet::UdpSocket socket;

    omnetpp::simsignal_t alertDelay_;
    omnetpp::simsignal_t alertRcvdMsg_;

  protected:

    virtual int numInitStages() const { return inet::NUM_INIT_STAGES; }
    void initialize(int stage);
    void handleMessage(omnetpp::cMessage *msg);
};

#endif

