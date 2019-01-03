#ifndef _BURSTRECEIVER_H_
#define _BURSTRECEIVER_H_

#include <string.h>
#include <omnetpp.h>

#include "inet/common/INETDefs.h"
#include "inet/transportlayer/contract/udp/UdpSocket.h"
#include "inet/networklayer/common/L3AddressResolver.h"

#include "BurstPacket_m.h"

class BurstReceiver : public omnetpp::cSimpleModule
{
  inet::UdpSocket socket;

    ~BurstReceiver();

    int numReceived_;
    int recvBytes_;

    bool mInit_;

    omnetpp::simsignal_t burstRcvdPkt_;
    omnetpp::simsignal_t burstPktDelay_;

  protected:

    virtual int numInitStages() const override { return inet::NUM_INIT_STAGES; }
    void initialize(int stage) override;
    void handleMessage(omnetpp::cMessage *msg) override;
};

#endif

