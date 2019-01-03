//
//

#ifndef _BURSTSENDER_H_
#define _BURSTSENDER_H_

#include <string.h>
#include <omnetpp.h>

#include "inet/common/INETDefs.h"
#include "inet/transportlayer/contract/udp/UdpSocket.h"
#include "inet/networklayer/common/L3AddressResolver.h"

#include "BurstPacket_m.h"

class BurstSender : public omnetpp::cSimpleModule
{
  inet::UdpSocket socket;
    //has the sender been initialized?
    bool initialized_;

    // timers
    omnetpp::cMessage* selfBurst_;
    omnetpp::cMessage* selfPacket_;

    //sender
    int idBurst_;
    int idFrame_;

    int burstSize_;
    int size_;
    omnetpp::simtime_t startTime_;
    omnetpp::simtime_t interBurstTime_;
    omnetpp::simtime_t intraBurstTime_;

    omnetpp::simsignal_t burstSentPkt_;
    // ----------------------------

    omnetpp::cMessage *selfSender_;
    omnetpp::cMessage *initTraffic_;

    omnetpp::simtime_t timestamp_;
    int localPort_;
    int destPort_;
    inet::L3Address destAddress_;

    void initTraffic();
    void sendBurst();
    void sendPacket();

  public:
    ~BurstSender();
    BurstSender();

  protected:

    virtual int numInitStages() const override { return inet::NUM_INIT_STAGES; }
    void initialize(int stage) override;
    void handleMessage(omnetpp::cMessage *msg) override;
};

#endif

