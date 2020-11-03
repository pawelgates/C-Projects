### REAL TIME SYSTEMS ### Created by PAVEL HEYETS ###

This is TCP Multiple Client Server that simulating CHECK IN / CHECK OUT system for any work place from different offices using Fork(). 

Server has a list of registered users: "USERS.TXT".
All the actions on the server being recorded to the LOG file: "LOG.TXT" (It will be created automatically).
If the client is CHECKED IN, server will not allow to client to CHECK IN twice.


When Client is connected, he has to put his ID number and then to type IN or OUT. If the ID is incorrect, server reply it. If it is correct, server will wish him "Good day" by his name from the system.
When Client is OUT, Server counts the time that client was IN.

*****

If we run the server with argument "time" ("./server time"), server will create TXT Time report for each client that was in. 



