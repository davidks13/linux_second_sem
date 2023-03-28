#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <pwd.h>
#include <cstring>


void sigusr1_handler(int sig)
{
    pid_t sender_pid = getpid();
    pid_t parent_pid = getppid();
    uid_t sender_uid = getuid();
    struct passwd *sender_pwd = getpwuid(sender_uid);
    user_regs_struct regs;
    ptrace(PTRACE_GETREGS, parent_pid, 0, &regs);

    std::cout << "Received a SIGUSR1 signal from process " << sender_pid <<
                " executed by " << sender_uid << " (" << sender_uid << ")." << std::endl;
    std::cout << "State of the context: ";
    std::cout  << "RIP: " << std::hex << regs.rip << ", ";
    std::cout  << "RAX: " << std::hex << regs.rax << ", ";
    std::cout  << "RBX: " << std::hex << regs.rbx << "." << std::endl;
}


int main()
{
    pid_t pid = getpid();
    std::cout << "My PID is: " << pid << std::endl;

    struct sigaction sa;
    sa.sa_handler = sigusr1_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        std::cerr << strerror(errno);
        return 1;
    }

    while (true) {
        sleep(10);
    }

    return 0;
}

