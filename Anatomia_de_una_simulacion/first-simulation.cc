#include "ns3/core-module.h"            //modulo de ns3
#include "ns3/network-module.h"         //modul de red
#include "ns3/internet-module.h"        //Modulo de internet
#include "ns3/point-to-point-module.h"  //Modulo de punto a punto (especial para esta practica)
#include "ns3/applications-module.h"    //Mdulo de aplicaciones ns3

// Topology
//          10.1.1.0
//  n0  ----------------   n1
//       point-to-point

using namespace ns3;    //indicar el namespace de ns3

NS_LOG_COMPONENT_DEFINE("FirstSimulation");      //DEFINIR COMPONENTE DE LA BITÁCORA


int main(int argc, char** argv)                 //Declaracion de la funcion principal
{
    //Mostrar infomacion Relevante de las aplicaciones UDP del server y del cliente
    LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);    
    LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);


    //Creacion de nodos
    NodeContainer nodes;                         //indicacion de los nodos
    nodes.Create(2);

    //Creacion y modificacion del Canal
    PointToPointHelper pointToPoint;             //Creacion de la tecnologia P2P
    pointToPoint.SetDeviceAttribute("DataRate", StringValue("5Mbps"));                //indicacion de atributos
    pointToPoint.SetChannelAttribute("Delay", StringValue("2ms"));                      //Configuracion del canal

    //Creacion de las interfaces de Red
    NetDeviceContainer devices;                   //Creaccion de interfaces de Red
    devices = pointToPoint.Install(nodes);        //Simulacion de conexión fisica con las interfaces de red

    //Instalacion de stacks de protocolos TCP/IP a los nodos
    InternetStackHelper stack;          //Pila de protocolos
    stack.Install(nodes);           //Indicamos que todos nodos tienen los portocolos de TCP/IP

    //Configuracions de las direcciones IP
    Ipv4AddressHelper address;            //Indicamos las dirrecciones IP que tendran los nodos
    address.SetBase("10.1.1.0", "255.255.255.0");      //Indicamos la direccion IP y la mascara de la red

    Ipv4InterfaceContainer interfaces = address.Assign(devices);   //Creamos el contenedor que tendra todas las interfaces de red y configura las IP de cada nodo

    //Instalacion en el nodo Server
    UdpEchoServerHelper echoServer(3000);     //Creamos las aplicaciones UDP echo Server

    //Englobamos todas las aplicaciones creadas en un AplicationContainer para decirles cuando iniciar y cuando finalizar
    ApplicationContainer serverApps = echoServer.Install(nodes.Get(1));   
    serverApps.Start(Seconds(1.0));              //Indicamos en que momento va a iniciar el ServerApp
    serverApps.Stop(Seconds(10.0));              //Indicamos en que momento va a finalizar el ServerApp

    //Instalacion en el nodo Cliente
    UdpEchoClientHelper echoClient(interfaces.GetAddress(1), 3000);     //Creacion de la aplicacion UDP echo Cliente
    echoClient.SetAttribute("MaxPackets", UintegerValue(1));            //Modificaciones de los atributos del Cliente
    echoClient.SetAttribute("Interval", TimeValue(Seconds(1.0)));        //Modificaciones de los atributos del Cliente
    echoClient.SetAttribute("PacketSize", UintegerValue(1024));         //Modificaciones de los atributos del Cliente

    //Englobamos todas las aplicaciones creadas en un AplicationContainer para decirles cuando iniciar y cuando finalizar
    ApplicationContainer clientApps = echoClient.Install(nodes.Get(0));   
    clientApps.Start(Seconds(2.0));              //Indicamos en que momento va a iniciar el clientApp
    clientApps.Stop(Seconds(10.0));              //Indicamos en que momento va a finalizar el clientApp


    Simulator::Run();           //Ejecucion de los eventos 
    Simulator::Destroy();       //Destruccion de la instancia del simulador

    return 0;       //Retornamos un 0 porque el main asi lo pide

}




