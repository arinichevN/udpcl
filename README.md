# udpcl
Client for Application Communication Protocol over UDP
Example:
echo -e '!@' | udpcl 192.168.1.2 49181 1
This command will send string: "!@\n\n" + 8_bit_crc to specified address and port, and wait for response for 1 second. 
