#include <iostream>
#include "ns3/log.h"
#include "ns3/udp-socket.h"
#include "ns3/simulator.h"
#include "ns3/csma-net-device.h"
#include "ns3/ethernet-header.h"
#include "ns3/arp-header.h"
#include "ns3/ipv4-header.h"
#include "ns3/udp-header.h"
#include "simple-udp-application.h"
#include "timestamp-tag.h"

#define PURPLE_CODE "\033[95m"
#define CYAN_CODE "\033[96m"
#define TEAL_CODE "\033[36m"
#define BLUE_CODE "\033[94m"
#define GREEN_CODE "\033[32m"
#define YELLOW_CODE "\033[33m"
#define LIGHT_YELLOW_CODE "\033[93m"
#define RED_CODE "\033[91m"
#define BOLD_CODE "\033[1m"
#define END_CODE "\033[0m"

namespace ns3
{
  NS_LOG_COMPONENT_DEFINE("SimpleUdpApplication");
  NS_OBJECT_ENSURE_REGISTERED(SimpleUdpApplication);

  TypeId
  SimpleUdpApplication::GetTypeId()
  {
    static TypeId tid = TypeId("ns3::SimpleUdpApplication")
                            .AddConstructor<SimpleUdpApplication>()
                            .SetParent<Application>();
    return tid;
  }

  TypeId
  SimpleUdpApplication::GetInstanceTypeId() const
  {
    return SimpleUdpApplication::GetTypeId();
  }

  SimpleUdpApplication::SimpleUdpApplication()
  {
    m_port = 9;
  }
  SimpleUdpApplication::~SimpleUdpApplication() 
  {
  }
  void SimpleUdpApplication::SetupReceiveSocket(Ptr<Socket> socket, uint16_t port)
  {
    InetSocketAddress local = InetSocketAddress(Ipv4Address::GetAny(), port);
    if (socket->Bind(local) == -1)
    {
      NS_FATAL_ERROR("Failed to bind socket");
    }
  }
  void SimpleUdpApplication::StartApplication()
  {
    //Receive sockets
    TypeId tid = TypeId::LookupByName("ns3::UdpSocketFactory");
    m_recv_socket = Socket::CreateSocket(GetNode(), tid);

    SetupReceiveSocket(m_recv_socket, m_port);

    m_recv_socket->SetRecvCallback(MakeCallback(&SimpleUdpApplication::HandleRead, this));
    //Send Socket
    m_send_socket = Socket::CreateSocket(GetNode(), tid);
  }

  void SimpleUdpApplication::HandleRead(Ptr<Socket> socket)
  {
    // NS_LOG_FUNCTION(this << socket);
    Ptr<Packet> packet;
    Address from;
    Address localAddress;
    while ((packet = socket->RecvFrom(from)))
    {
      TimestampTag txTag;   
      NS_LOG_INFO(TEAL_CODE
                  << "\n"
                  << "HandleRead : Received a Packet of size: " << packet->GetSize() 
                  << " at time " << Now().GetSeconds() 
                  << END_CODE);
      if (packet->PeekPacketTag (txTag))
        {
          double tx = txTag.GetSeconds ();
          NS_LOG_INFO(TEAL_CODE << "The delay is: " << Simulator::Now().GetSeconds() - tx << END_CODE);
        }
    }
  }

  void SimpleUdpApplication::SendPacket(Ptr<Packet> packet, Ipv4Address destination, uint16_t port)
  {
    // NS_LOG_FUNCTION (this << packet << destination << port);
    NS_LOG_INFO(BLUE_CODE
                << "Send a packet of Size:" << packet->GetSize () << "\n"
                << " at time " << Now().GetSeconds ()
                << "\n"
                << " to " << "IP Address: " << destination << " port: " << port
                << END_CODE);
    TimestampTag txTag;
    txTag.SetTimestamp(Simulator::Now());
    packet->AddPacketTag(txTag);
    m_send_socket->Connect(InetSocketAddress(Ipv4Address::ConvertFrom(destination), port));
    m_send_socket->Send(packet);
  }

} // namespace ns3
