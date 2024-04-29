Implementation Overview:

	For my implementation the first thing I do is register the signals the user will need to receive from the server, for synchronization and to mark an 
inbound (to user) message. This avoids any race conditions involving the user not having the handler set up when the server relays a message. Then I fork 
3 child processes and set a local variable to mark if they are the parent to 0(not the parent), and a local variable for its assigned child number. Next I 
create 2 arrays (3 by 2), to store my pipes(parent to child, and child to parent). For the children I close every pipe except the childs write pipe for child 
to parent, and every pipe except the childs read pipe for parent to child. In the parent I close all the read ends for parent to child and close all the write 
ends for child to parent. After this I put the child processes into an infinite loop to wait for a signal from the parent process that it is ready to read, 
the parent process calls the server_handler function. After the user processes break out of the infinite loop they each then call the user_handler function.

****Please note the use of the SA_RESTART flag when initializing my sigaction structs handles the case of interrupted read/write, This flag makes the default
behavior when interrupted by a signal to restart the system call.******

Functions:

	new_msg: This handles all signals and increments the proper global/atomic variable when it receives a signal. Note each user function uses the same 
global variable, msg_num_ser, to keep track of how many pending messages from the server there are (signal: SIGRTMIN + 3). 

	user_handler: In the user_handler function all the messages are defined and then are broadcast to the server depending on which user calls the function.
The user then closes its write pipe and sends an additional signal to the server. After that it enters an infinite loop and when it receives a message it 
parses the message contents, and the user that sent it, and outputs it to the screen wrapped in a different output message (message received, instead of 
message sent). After it reads/writes a message it decrements the count of msg_num_ser. Once it reads the final message from the server it closes its read
 pipe and exits the function.

	server_handler: In the server_handler function it registers the new_msg signal handler with the signals for user input messages (SIGRTMIN + 0, +1, +2),
then sends a signal to each user process that it is ready to receive messages. Then it enters an infinite loop and waits to receive messages. Once it receives 
a message it reads it from the correct input pipe and then writes it to the corresponding pipe of the message receiver. If it reads 0 then it sets the 
corresponding flag for the sending user to 0 to mark that the user has closed its write pipe and finished sending messages, and closes the read pipe from
 the user. Once it closes all its read pipes it then closes all its write pipes and notifies the user with a signal so the user will close their read ends 
and exit the process. Once it notifies all users it then waits and reaps all 3 child processes before exiting.
