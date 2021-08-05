/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/core-module.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/simple-udp-application-module.h"

using namespace ns3;

int main (int argc, char *argv[])
{
  CommandLine cmd;
  cmd.Parse (argc, argv);
  
  NodeContainer nodes;
  nodes.Create (2);

  PointToPointHelper p2p;
  p2p.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  p2p.SetChannelAttribute ("Delay", StringValue ("200ms"));

  NetDeviceContainer p2pDevs;
  p2pDevs = p2p.Install (nodes);
  p2p.EnableAsciiAll("simple-udp");

  InternetStackHelper stack;
  stack.Install (nodes);

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer ifaces;
  ifaces = address.Assign (p2pDevs);
  
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
  Packet::EnablePrinting (); 

  //Create a UDP applications
  Ptr <SimpleUdpApplication> sender = CreateObject <SimpleUdpApplication> ();
  //Set the start & stop times
  sender->SetStartTime (Seconds(0));
  sender->SetStopTime (Seconds (10));
  //install one application at node 0, and the other at node 1
  nodes.Get(0)->AddApplication (sender);

  // Create a UDP receiver application
  Ptr <SimpleUdpApplication> receiver = CreateObject <SimpleUdpApplication> ();
  nodes.Get(1)->AddApplication (receiver);
  
  //This is the IP address of node 1
  Ipv4Address dest_ip ("10.1.1.2");
  //Schedule an event to send a packet of size 400 using udp targeting IP of node 0, and port 9
  Ptr <Packet> packet = Create <Packet> (400);
 
  Simulator::Schedule (Seconds (1.1), &SimpleUdpApplication::SendPacket, sender, packet, dest_ip, 9);
    
  LogComponentEnable ("SimpleUdpApplication", LOG_LEVEL_INFO);

  Simulator::Stop (Seconds (10));
  Simulator::Run ();
  Simulator::Destroy ();

}
