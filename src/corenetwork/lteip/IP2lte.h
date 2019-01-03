//
//                           SimuLTE
//
// This file is part of a software released under the license included in file
// "license.pdf". This license can be also found at http://www.ltesimulator.com/
// The above file and the present reference are part of the software itself,
// and cannot be removed from it.
//

#ifndef __SIMULTE_IP2LTE_H_
#define __SIMULTE_IP2LTE_H_

#include <omnetpp.h>
#include "common/LteControlInfo.h"
#include "inet/common/packet/ChunkQueue.h"
#include "inet/networklayer/ipv4/Ipv4Header_m.h"
#include "stack/handoverManager/LteHandoverManager.h"
#include "corenetwork/binder/LteBinder.h"

class LteHandoverManager;

// a sort of five-tuple with only two elements (a two-tuple...), src and dst addresses
typedef std::pair<inet::Ipv4Address, inet::Ipv4Address> AddressPair;

/**
 *
 */
class IP2lte : public omnetpp::cSimpleModule
{
public:
    using IpChunk = inet::Ptr<const inet::Ipv4Header>;

private:
    omnetpp::cGate *stackGateOut_;       // gate connecting IP2lte module to LTE stack
    omnetpp::cGate *ipGateOut_;          // gate connecting IP2lte module to network layer
    LteNodeType nodeType_;      // node type: can be ENODEB, UE

    // datagram sequence numbers (one for each flow)
    // TODO move numbering to PDCP
    std::map<AddressPair, unsigned int> seqNums_;

    // obsolete with the above map
    unsigned int seqNum_;       // datagram sequence number (RLC fragmentation needs it)

    // reference to the binder
    LteBinder* binder_;

    // MAC node id of this node
    MacNodeId nodeId_;

    /*
     * Handover support
     */

    // manager for the handover
    LteHandoverManager* hoManager_;
    // store the pair <ue,target_enb> for temporary forwarding of data during handover
    std::map<MacNodeId, MacNodeId> hoForwarding_;
    // store the UEs for temporary holding of data received over X2 during handover
    std::set<MacNodeId> hoHolding_;

    typedef std::list<inet::Packet*> IpDatagramQueue;
    std::map<MacNodeId, IpDatagramQueue> hoFromX2_;
    std::map<MacNodeId, IpDatagramQueue> hoFromIp_;

    bool ueHold_;
    IpDatagramQueue ueHoldFromIp_;

    /**
     * Handle packets from transport layer and forward them to the stack
     */
    void fromIpUe(IpChunk datagram);

    /**
     * Manage packets received from Lte Stack
     * and forward them to transport layer.
     */
    void toIpUe(IpChunk datagram);
    /**
     * Forward packets to the LTE stack
     */
    void toStackUe(IpChunk datagram);

    void fromIpEnb(inet::Ipv4Header * datagram);
    void toIpEnb(omnetpp::cMessage * msg);
    void toStackEnb(inet::Ipv4Header* datagram);

    /**
     * utility: set nodeType_ field
     *
     * @param s string containing the node type ("enodeb", "ue")
     */
    void setNodeType(std::string s);

    /**
     * utility: print LteStackControlInfo fields
     *
     * @param ci LteStackControlInfo object
     */
    void printControlInfo(FlowControlInfo* ci);
    void registerInterface();
    void registerMulticastGroups();
  protected:
    virtual void initialize(int stage) override;
    virtual int numInitStages() const override { return inet::NUM_INIT_STAGES; }
    virtual void handleMessage(omnetpp::cMessage *msg) override;
    virtual void finish() override;
  public:

    /*
     * Handover management at the eNB side
     */
    void triggerHandoverSource(MacNodeId ueId, MacNodeId targetEnb);
    void triggerHandoverTarget(MacNodeId ueId, MacNodeId sourceEnb);
    void sendTunneledPacketOnHandover(inet::Ipv4Header* datagram, MacNodeId targetEnb);
    void receiveTunneledPacketOnHandover(inet::Ipv4Header* datagram, MacNodeId sourceEnb);
    void signalHandoverCompleteSource(MacNodeId ueId, MacNodeId targetEnb);
    void signalHandoverCompleteTarget(MacNodeId ueId, MacNodeId sourceEnb);

    /*
     * Handover management at the UE side
     */
    void triggerHandoverUe();
    void signalHandoverCompleteUe();

    virtual ~IP2lte();
};

#endif
