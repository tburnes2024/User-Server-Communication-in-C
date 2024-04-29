#include <stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include <time.h>
#include <unistd.h> 
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>

//sig user 0 is 34(SIGRTMIN + 0), user 1 is 35, user 2 is 36,parent is SIGRTMIN + 3 (denoted by sender)
volatile sig_atomic_t msg_num_usr0 = 0;
volatile sig_atomic_t msg_num_usr1 = 0;
volatile sig_atomic_t msg_num_usr2 = 0;
int msg_num_ser = 0;     //parent to child signal

//sync global variable, signal for this will be 38 (sigrtmin + 4)
int sync_user_to_server = 0;


//handles all new messages to any process
// PARAMETER: signum, to determine which user sent the message and the message purpose
void new_msg(int signum){
    int user_no = signum - SIGRTMIN;
    
    //depending on signal origin increment proper global/atomic variable
    if(user_no == 0){
        msg_num_usr0++;
    }else if(user_no == 1){
        msg_num_usr1++;
    }else if(user_no == 2){
        msg_num_usr2++;
    }else if(user_no == 3){
        msg_num_ser++;
    }else if(user_no == 4){
        sync_user_to_server++;   
    }else{
        perror("received invalid signal");
        exit(0);
    }
    
    return;
}

//handle user functions, I/O
//PARAMETER: takes the read and write pipes of the user process, also take in the childnum assigned to process at creation
void user_handler(int child_to_parent[3][2], int parent_to_child[3][2], int child ){
    //number of bytes write/read
    int nw;
    int nr;
    //running status of server 1:open, 0:closed
    int server = 1;
    //empty sigval struct for sigqueue calls
    union sigval val;
    val.sival_int = 0;
    //buffer is input array for read pipes, pad is array of null chars used to pad messages to 128 bytes
    char buffer[128];
    char pad[128];
    //user messages
    char user0_msg1[] = "user_1_sent_message_to_user_1:<All men are mortal.>";
    char user0_msg2[] = "user_1_sent_message_to_user_2:<Socrates is a man.>";
    char user0_msg3[] = "user_1_sent_message_to_user_3:<Therefore, Socrates is mortal.>";
    char user1_msg1[] = "user_2_sent_message_to_user_1:<We know what we are,>";
    char user1_msg2[] = "user_2_sent_message_to_user_3:<But know not what we may be.>";
    char user2_msg1[] = "user_3_sent_message_to_user_1:<We wish you a prosperous Year of the Dragon!>";
     
    //initialize array to pad messages
    for(int i = 0; i < 128; i++){
        pad[i] = '\0';
    }

    if(child == 0){
        if ((nw = write(child_to_parent[child][1], user0_msg1, sizeof(user0_msg1)-1)) == -1){
            perror("child(0) failed to write to pipe");
            exit(1);
        }
        if ((nw = write(child_to_parent[child][1], pad, 128-nw)) == -1){
            perror("child(0) failed to write to pipe");
            exit(1);
        }
        if(printf("%s\n", user0_msg1) == -1){
            perror("printf failed to output properly");
            exit(1);
        }
        if(sigqueue(getppid(), SIGRTMIN + child, val) == -1){
            perror("sigqueue signal failed to send");
            exit(1);
        }
        if ((nw = write(child_to_parent[child][1], user0_msg2, sizeof(user0_msg2) -1)) == -1){
            perror("child(0) failed to write to pipe");
            exit(1);
        }
        if ((nw = write(child_to_parent[child][1], pad, 128-nw)) == -1){
            perror("child(0) failed to write to pipe");
            exit(1);
        }
        if(printf("%s\n", user0_msg2) == -1){
            perror("printf failed to output properly");
            exit(1);
        }
        if(sigqueue(getppid(), SIGRTMIN + child, val) == -1){
            perror("sigqueue signal failed to send");
            exit(1);
        }
        if ((nw = write(child_to_parent[child][1], user0_msg3, sizeof(user0_msg3) -1)) == -1){
            perror("child(0) failed to write to pipe");
            exit(1);
        }
        if ((nw = write(child_to_parent[child][1], pad, 128-nw)) == -1){
            perror("child(0) failed to write to pipe");
            exit(1);
        }
        if(printf("%s\n", user0_msg3) == -1){
            perror("printf failed to output properly");
            exit(1);
        }
        if(sigqueue(getppid(), SIGRTMIN + child, val) == -1){
            perror("sigqueue signal failed to send");
            exit(1);
        }
    }else if(child == 1){
        if ((nw = write(child_to_parent[child][1], user1_msg1, sizeof(user1_msg1)-1)) == -1){
            perror("child(1) failed to write to pipe");
            exit(1);
        }
        if ((nw = write(child_to_parent[child][1], pad, 128-nw)) == -1){
            perror("child(1) failed to write to pipe");
            exit(1);
        }
        if(printf("%s\n", user1_msg1) == -1){
            perror("printf failed to output properly");
            exit(1);
        }
        if(sigqueue(getppid(), SIGRTMIN + child, val) == -1){
            perror("sigqueue signal failed to send");
            exit(1);
        }
        if ((nw = write(child_to_parent[child][1], user1_msg2, sizeof(user1_msg2) -1)) == -1){
            perror("child(1) failed to write to pipe");
            exit(1);
        }
        if ((nw = write(child_to_parent[child][1], pad, 128-nw)) == -1){
            perror("child(1) failed to write to pipe");
            exit(1);
        }
        if(printf("%s\n", user1_msg2) == -1){
            perror("printf failed to output properly");
            exit(1);
        }
        if(sigqueue(getppid(), SIGRTMIN + child, val) == -1){
            perror("sigqueue signal failed to send");
            exit(1);
        }
    }else if(child == 2){
        if ((nw = write(child_to_parent[child][1], user2_msg1, sizeof(user2_msg1)-1)) == -1){
            perror("child(2) failed to write to pipe");
            exit(1);
        }
        if ((nw = write(child_to_parent[child][1], pad, 128-nw)) == -1){
            perror("child(2) failed to write to pipe");
            exit(1);
        }
        if(printf("%s\n", user2_msg1) == -1){
            perror("printf failed to output properly");
            exit(1);
        }
        if(sigqueue(getppid(), SIGRTMIN + child, val) == -1){
            perror("sigqueue signal failed to send");
            exit(1);
        }
    }
    
    //signal parent that child is done writing, close write pipe
    if(close(child_to_parent[child][1]) == -1){
        perror("close pipe failed");
        exit(1);
    }
    if(sigqueue(getppid(), SIGRTMIN + child, val) == -1){
        perror("sigqueue signal failed to send");
        exit(1);
    }
    
    //receive messages from server -server = 0 server off, server on by default
        while(server){
            if(msg_num_ser > 0){
                
                if ((nr = read(parent_to_child[child][0], buffer, 128)) == -1){
                    perror("child failed to read msg from parent");
                    exit(1);
                }
                
                if(nr == 0){
                    server = 0;
                    if(close(parent_to_child[child][0]) == -1){
                        perror("close pipe failed");
                        exit(1);
                    }
                }else{
                    //this outputs the generic string user x received msg from y, then reads the portion of 
                    //the message between the <> and then adds a final > to end of output
                    if(printf("user_%c_received_message_from_user_%c:<", buffer[28], buffer[5]) == -1){
                        perror("printf failed to output properly");
                        exit(1);
                    }
                    for(int i = 31; ; i++){
                        if(buffer[i] == '>'){break;}
                        if(printf("%c", buffer[i]) == -1){
                            perror("printf failed to output properly");
                            exit(1);
                        }
                    }
                    if(printf(">\n") == -1){
                        perror("printf failed to output properly");
                        exit(1);
                    }
                }
                msg_num_ser--;
            }
        }
    
    return;
}


//server message handler function
//PARAMETER: takes in read and write pipes for the parent, also takes the array of child process pids for sending signals
void server_handler(int read_from[3][2],int write_to[3][2], pid_t child[3]){
    struct sigaction sa;
    sa.sa_handler = new_msg;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    
    //variables to mark the status of user processes (running or terminated)
    int user0 = 1;
    int user1 = 1;
    int user2 = 1;
    //number write/read
    int nw;
    int nr;
    //char array for getting input to server
    char read_to[128];
    //empty struct, needed as input for sigqueue function
    union sigval val;
    val.sival_int = 0;
    
    //register user input signal handlers
    for(int i = 0; i < 3; i++){
        if(sigaction(SIGRTMIN + i, &sa, NULL) == -1){
            perror("Error calling sigaction(server_handler)");
            exit(1);
        }
    }
    
    //signal user that parent is ready to receive messages
    for(int i = 0; i < 3; i++){
        if(sigqueue(child[i], SIGRTMIN + 4, val) == -1){
            perror("sigqueue signal failed to send");
            exit(1);
        }
    }
    
    //infinite loop and wait to receive a message
    while(user0 || user1 || user2){ 
    
        if(msg_num_usr0 > 0){
        //Read a message from the pipe user0_to_server, redirect message to intended user
          
            if ((nr = read(read_from[0][0], read_to, 128)) == -1){
                perror("parent failed to read from pipe(user0)");
                exit(1);
            }
            
            //if 0 bytes were read indicating EOF mark that userK has closed write pipe
            //else handle inbound message and redirect to correct user
            if(nr == 0){
                user0 = 0;
                if(close(read_from[0][0]) == -1){
                    perror("pipe close failed");
                    exit(1);
                }
            }else{
                if(read_to[28] == '1'){
                    if ((nw = write(write_to[0][1],read_to, 128)) == -1){
                        perror("parent failed to write to pipe(user0)");
                        exit(1);
                    }
                    if(sigqueue(child[0], SIGRTMIN + 3, val) == -1){
                        perror("sigqueue signal failed to send");
                        exit(1);
                    }
                }else if(read_to[28] == '2'){
                    if ((nw = write(write_to[1][1],read_to, 128)) == -1){
                        perror("parent failed to write to pipe(user0)");
                        exit(1);
                    }
                    if(sigqueue(child[1], SIGRTMIN + 3, val) == -1){
                        perror("sigqueue signal failed to send");
                        exit(1);
                    }
                }else if(read_to[28] == '3'){
                    if ((nw = write(write_to[2][1],read_to, 128)) == -1){
                        perror("parent failed to write to pipe(user0)");
                        exit(1);
                    }
                    if(sigqueue(child[2], SIGRTMIN + 3, val) == -1){
                        perror("sigqueue signal failed to send");
                        exit(1);
                    }
                }
            }
            msg_num_usr0--;
          
        }
        if(msg_num_usr1 > 0){
          //Read a message from the pipe user1_to_server, redirect message to intended user
          
            if ((nr = read(read_from[1][0], read_to, 128)) == -1){
                perror("parent failed to read from pipe(user1)");
                exit(1);
            }
            
            //if 0 bytes were read indicating EOF mark that userK has closed write pipe
            //else handle inbound message and redirect to correct user
            if(nr == 0){
                user1 = 0;
                if(close(read_from[1][0]) == -1){
                    perror("pipe close failed");
                    exit(1);
                }
            }else{
                if(read_to[28] == '1'){
                    if ((nw = write(write_to[0][1],read_to, 128)) == -1){
                        perror("parent failed to write to pipe(user1)");
                        exit(1);
                    }
                    if(sigqueue(child[0], SIGRTMIN + 3, val) == -1){
                        perror("sigqueue signal failed to send");
                        exit(1);
                    }
                }else if(read_to[28] == '2'){
                    if ((nw = write(write_to[1][1],read_to, 128)) == -1){
                        perror("parent failed to write to pipe(user1)");
                        exit(1);
                    }
                    if(sigqueue(child[1], SIGRTMIN + 3, val) == -1){
                        perror("sigqueue signal failed to send");
                        exit(1);
                    }
                }else if(read_to[28] == '3'){
                    if ((nw = write(write_to[2][1],read_to, 128)) == -1){
                        perror("parent failed to write to pipe(user0)");
                        exit(1);
                    }
                    if(sigqueue(child[2], SIGRTMIN + 3, val) == -1){
                        perror("sigqueue signal failed to send");
                        exit(1);
                    }
                }
            }
            msg_num_usr1--;
          
        }
        if(msg_num_usr2 > 0){
          //Read a message from the pipe user2_to_server, redirect message to intended user
          
            if ((nr = read(read_from[2][0], read_to, 128)) == -1){
                perror("parent failed to read from pipe(user2)");
                exit(1);
            }
            
            //if 0 bytes were read indicating EOF mark that userK has closed write pipe
            //else handle inbound message and redirect to correct user
            if(nr == 0){
                user2 = 0;
                if(close(read_from[2][0]) == -1){
                    perror("pipe close failed");
                    exit(1);
                }
            }else{
                if(read_to[28] == '1'){
                    if ((nw = write(write_to[0][1],read_to, 128)) == -1){
                        perror("parent failed to write to pipe(user1)");
                        exit(1);
                    }
                    if(sigqueue(child[0], SIGRTMIN + 3, val) == -1){
                        perror("sigqueue signal failed to send");
                        exit(1);
                    }
                }else if(read_to[28] == '2'){
                    if ((nw = write(write_to[1][1],read_to, 128)) == -1){
                        perror("parent failed to write to pipe(user1)");
                        exit(1);
                    }
                    if(sigqueue(child[1], SIGRTMIN + 3, val) == -1){
                        perror("sigqueue signal failed to send");
                        exit(1);
                    }
                }else if(read_to[28] == '3'){
                    if ((nw = write(write_to[2][1],read_to, 128)) == -1){
                        perror("parent failed to write to pipe(user0)");
                        exit(1);
                    }
                    if(sigqueue(child[2], SIGRTMIN + 3, val) == -1){
                        perror("sigqueue signal failed to send");
                        exit(1);
                    }
                }
            }
            msg_num_usr2--;
        }
    }
    
    //close write pipes
    for(int i = 0; i < 3; i++){
        if(close(write_to[i][1]) == -1){
            perror("close pipe failed");
            exit(1);
        }
        if(sigqueue(child[i], SIGRTMIN + 3, val) == -1){
            perror("sigqueue signal failed to send");
            exit(1);
        }
    }
    
    //reap user processes
    for(int i = 0; i < 3; i++){
        wait(NULL);
    }
    
    return;
}

int main(){
    
    //this is for synchronizing the users, initialize and set up signal handler and signals
    struct sigaction sa;
    sa.sa_handler = new_msg;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    union sigval val;
    val.sival_int = 0;
    
    //register signals for user (sync, and inbound msg, 3 and 4)
    for(int i = 0; i < 2; i++){
        if(sigaction(SIGRTMIN + 3 + i, &sa, NULL) == -1){
            perror("Error calling sigaction(main)");
            exit(1);
        }
    }
    
    //array to hold child process pids
    pid_t child[3];
    //child -> parent
    int child_parent[3][2];
    //parent -> child
    int parent_child[3][2];
    int childnum = -1;
    int is_parent = 1;


    //allocate pipes child -> parent and parent -> child
    for(int i = 0; i < 3; i++){
        if(pipe(child_parent[i]) == -1){
            perror("pipe could not be created");
            exit(1);
        }
        if(pipe(parent_child[i]) == -1){
            perror("pipe could not be created");
            exit(1);
        }
    }

    //fork 3 children
    for(int i = 0; i < 3; i++){
        if((child[i] = fork()) == -1){
            perror("fork sys call failed");
            exit(1);
        }
        if(child[i] == 0){
            childnum = i;
            is_parent = 0; 
            break;
        }
    }

    //close pipes
    if(is_parent){                                            //close parent pipes that need to be closed
        //close all parent to child reads
        for(int i = 0; i < 3; i++){
            if(close(parent_child[i][0]) == -1){
                perror("close pipe failed");
                exit(1);
            }
        }
        //close all child to parent writes
        for(int i = 0; i < 3; i++){
            if(close(child_parent[i][1]) == -1){
                perror("close pipe failed");
                exit(1);
            }
        }
        
    }else if(childnum < 3 && childnum >= 0 && !is_parent){         //close child pipes that need to be closed
        //close everything except child write pipe
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 2; j++){
                if(!(i == childnum && j == 1)){
                    if(close(child_parent[i][j]) == -1){
                        perror("close pipe failed");
                        exit(1);
                    }
                }
            }
        }

        //close everything except child read pipe
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 2; j++){
                if(!(i == childnum && j == 0)){
                    if(close(parent_child[i][j]) == -1){
                        perror("close pipe failed");
                        exit(1);
                    }
                }
            }
        }
    }
    
    //if not parent, wait for signal to synchronize processes for writing/reading, if parent call server_handler
    if(is_parent){
        server_handler(child_parent, parent_child, child);
    }else{
        while(1){
            if(sync_user_to_server){break;}
        }
    }
    
    //for all children, call user_handler function to handle communication
    if(!is_parent){
        user_handler(child_parent, parent_child, childnum);
    }

    exit(0);
}
