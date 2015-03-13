**New - updated for Arduino software version 1.0. No longer has external dependencies!**

Simple homemade Arduino based alarm system. It emails and texts me when me door opens! It consists of:

  * Arduino
  * Ethernet Shield
  * Piezo Buzzer
  * Blue LED
  * Two long strands of 22 gauge wire

The system is not connected to a computer. Instead, it connected directly to the internet with an Ethernet shield. I get emails or texts when:

  * the door opens
  * the door has been kept open for more than 15 seconds (so if the door sensor falls off or something, I know.)
  * the system reboots, so if I lose power I will know.

Uses a DHCP library to get an IP address. SMTP server is hard coded (as is the email address to send texts/emails to).

Usage video: http://youtu.be/lfOxgK1-5HM