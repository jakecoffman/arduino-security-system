/*************************************************************
Arduino Alarm by jake@coffshire.com
*************************************************************/

#include <Ethernet.h>
#include "Dhcp.h"
#include "Dns.h"
#include <string.h>

// Dummy mac address
byte mac[] = { 0x01, 0x02, 0x02, 0x32, 0x02, 0x02 }; 
// This is the IP address of the SMTP server
byte emailserver[] = { 209,225,8,224 };
// This is where the DNS server IP will be stored
byte dnsserver[6];

Client emailclient;
Client webclient;

// Helper to print IP addresses
void printArray(Print *output, char* delimeter, byte* data, int len, int base)
{
  char buf[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  
  for(int i = 0; i < len; i++)
  {
    if(i != 0)
      output->print(delimeter);
      
    output->print(itoa(data[i], buf, base));
  }
  
  output->println();
}

// Call to initialize the communication with the email server
void setupComm()
{
  if (Dhcp.beginWithDHCP(mac))
  {    
    Dhcp.getDnsServerIp(dnsserver);
    printArray(&Serial, ".", dnsserver, 4, 10);
    
    emailclient.init(emailserver, 25);
  }
  else
  {
    // beep in errors
    play(ERRORCHIME);
  }
}
    
// Call to send an email. 
bool email(char* text)
{
  bool success = false;
  Serial.println("Sending email...");

  if (emailclient.connect()) 
  {
    Serial.println("connected");

    emailclient.println("HELO arduino"); 
    for(int i=0; i<999; ++i)
                {
                  if(emailclient.read() > 0)
                    break;
                } 

                // Put your "from" email address here
    emailclient.println("MAIL FROM: arduino@coffshire.com"); 
    for(int i=0; i<999; ++i)
    {
      if(emailclient.read() > 0)
        break;
    } 

    // Put where you are sending it here
    emailclient.println("RCPT TO: jake@coffshire.com");
    for(int i=0; i<999; ++i)
    {
      if(emailclient.read() > 0)
        break;
    } 

    emailclient.println("DATA"); 
    for(int i=0; i<999; ++i)
    {
      if(emailclient.read() > 0)
        break;
    } 

    emailclient.println("From: arduino@coffshire.com");
    emailclient.println("To: jake@coffshire.com");  
    emailclient.println("Subject: From your arduino"); 

    emailclient.println(text); 
    emailclient.println("."); 

    emailclient.println("QUIT"); 
    for(int i=0; i<999; ++i)
    {
      if(emailclient.read() > 0)
        break;
    } 

    success = true;
    emailclient.println();

  } 
  else 
  {
    emailclient.println("QUIT"); //attempt to cleanup the connection
  }
  emailclient.stop();
  return success;
}
