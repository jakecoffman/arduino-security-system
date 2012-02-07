/*************************************************************
Arduino Alarm by jake@coffshire.com
*************************************************************/

#include <SPI.h>
#include <Ethernet.h>

// Arduino network information
byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 }; 
EthernetClient client;
char smtpServer[] = "smtp.charter.net";
char google[] = "www.google.com";

// Call to initialize the communication with the email server
void setupComm()
{
  Serial.println("Trying to connect");
  playNote('a', 250);
  if (!Ethernet.begin(mac))
  {
    playNote('g', 250);
    Serial.println("Failed to DHCP");
    // no point in carrying on, so do nothing forevermore:
    while(true);
  }
  playNote('b', 250);
  delay(1000);
  
  // print your local IP address:
  Serial.print("My IP address: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print("."); 
  }
  Serial.println();
  
  if(email("Arduino is up!"))
  {
    playNote('c', 250);
  }
  else
  {
    playNote('g', 250);
    delay(2000);
  }
}
    
// Call to send an email. 
bool email(char* text)
{
  bool success = false;
  Serial.println("Sending email...");

  if(client.connect(google, 80))
  {
    Serial.println("Got google");
    client.stop();
  }
  else
  {
    Serial.println("Not google");
  }

  if (client.connect(smtpServer, 25)) 
  {
    Serial.println("connected");
    delay(100);
    client.println("HELO arduino"); 
    for(int i=0; i<999; ++i)
    {
      if(client.read() > 0)
        break;
    } 
    Serial.println("responded");
    // Put your "from" email address here
    client.println("MAIL FROM: arduino@coffshire.com"); 
    for(int i=0; i<999; ++i)
    {
      if(client.read() > 0)
        break;
    } 
    Serial.println("mail from'd");
    // Put where you are sending it here
    client.println("RCPT TO: jake@coffshire.com");
    for(int i=0; i<999; ++i)
    {
      if(client.read() > 0)
        break;
    } 
    Serial.println("rcpt");
    client.println("DATA"); 
    for(int i=0; i<999; ++i)
    {
      if(client.read() > 0)
        break;
    } 

    client.println("From: arduino@coffshire.com");
    client.println("To: jake.coffman@gmail.com");  
    client.println("Subject: From your arduino"); 

    client.println(text); 
    client.println("."); 

    client.println("QUIT"); 
    for(int i=0; i<999; ++i)
    {
      if(client.read() > 0)
        break;
    } 
    Serial.println("datad");
    success = true;
    client.println();

  } 
  else 
  {
    Serial.println("Failed");
    client.println("QUIT"); //attempt to cleanup the connection
  }
  client.stop();
  return success;
}
