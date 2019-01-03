#include "apps/burst/BurstReceiver.h"
#include "inet/common/packet/chunk/cPacketChunk.h"

Define_Module(BurstReceiver);

using namespace omnetpp;

BurstReceiver::~BurstReceiver()
{

}

void BurstReceiver::initialize(int stage)
{
    cSimpleModule::initialize(stage);

    if (stage == inet::INITSTAGE_LOCAL)
    {
        mInit_ = true;

        numReceived_ = 0;

        recvBytes_ = 0;

        burstRcvdPkt_ = registerSignal("burstRcvdPkt");
        burstPktDelay_ = registerSignal("burstPktDelay");
    }
    else if (stage == inet::INITSTAGE_APPLICATION_LAYER)
    {
        int port = par("localPort");
        EV << "BurstReceiver::initialize - binding to port: local:" << port << endl;
        if (port != -1)
        {
            socket.setOutputGate(gate("udpOut"));
            socket.bind(port);
        }
    }
}

void BurstReceiver::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage())
        return;

    auto packet = check_and_cast<inet::Packet*>(msg);
    auto chunk = packet->popAtFront<inet::cPacketChunk>();
    auto pPacket = check_and_cast<BurstPacket*>(chunk->getPacket());

    if (pPacket == 0)
        throw cRuntimeError("BurstReceiver::handleMessage - FATAL! Error when casting to Cbr packet");

    numReceived_++;

    simtime_t delay = simTime()-pPacket->getTimestamp();
    EV << "BurstReceiver::handleMessage - Packet received: FRAME[" << pPacket->getMsgId() << "] with delay["<< delay << "]" << endl;

    emit(burstPktDelay_, delay);
    emit(burstRcvdPkt_, (long)pPacket->getMsgId());

    delete msg;
}


